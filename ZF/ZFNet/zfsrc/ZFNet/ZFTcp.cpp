#include "ZFTcp.h"
#include "protocol/ZFProtocolZFTcp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFTcpType)

// ============================================================
zfclassNotPOD _ZFP_ZFTcpPrivate {
public:
    void *nativeSocket;
    zfstring host;
    zfuint port;
    ZFTcpType type;
public:
    _ZFP_ZFTcpPrivate(void)
    : nativeSocket(zfnull)
    , host()
    , port(0)
    , type(ZFTcpType::e_Invalid)
    {
    }
};

void ZFTcp::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    ret += "tcp:";
    zfstringAppend(ret, "%s:%s", this->host(), this->port());
}

void ZFTcp::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTcpPrivate);
}
void ZFTcp::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFTcp::objectOnDeallocPrepare(void) {
    this->close();
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_2(ZFTcp, zfbool, open
            , ZFMP_IN(const zfstring &, host)
            , ZFMP_IN(zfuint, port)
            ) {
    if(d->type == ZFTcpType::e_ServerAccept) {
        ZFLog("calling open() on tcp returned by accept()");
        return zffalse;
    }

    this->close();
    if(!host) {
        d->host = "localhost";
        d->type = ZFTcpType::e_Server;
    }
    else {
        d->host = host;
        d->type = ZFTcpType::e_Client;
    }
    d->port = port;
    d->nativeSocket = zfnull;
    d->nativeSocket = ZFPROTOCOL_ACCESS(ZFTcp)->open(this, host, port);
    if(d->nativeSocket == zfnull) {
        d->host.removeAll();
        d->port = 0;
        d->nativeSocket = zfnull;
        d->type = ZFTcpType::e_Invalid;
        return zffalse;
    }
    return zftrue;
}
ZFMETHOD_DEFINE_0(ZFTcp, void, close) {
    if(d->type != ZFTcpType::e_Invalid) {
        ZFPROTOCOL_ACCESS(ZFTcp)->close(this, d->nativeSocket);
        d->nativeSocket = zfnull;
        d->host.removeAll();
        d->port = 0;
        d->type = ZFTcpType::e_Invalid;
    }
}

ZFMETHOD_DEFINE_0(ZFTcp, ZFTcpType, type) {
    return d->type;
}

ZFMETHOD_DEFINE_0(ZFTcp, zfbool, valid) {
    return d->type != ZFTcpType::e_Invalid;
}
ZFMETHOD_DEFINE_0(ZFTcp, const zfstring &, host) {
    return d->host;
}
ZFMETHOD_DEFINE_0(ZFTcp, zfuint, port) {
    return d->port;
}

ZFMETHOD_DEFINE_2(ZFTcp, zfbool, remoteInfo
            , ZFMP_OUT(zfstring &, remoteAddr)
            , ZFMP_OUT(zfuint &, remotePort)
            ) {
    switch(d->type) {
        case ZFTcpType::e_Invalid:
        case ZFTcpType::e_Server:
            return zffalse;
        default:
            return ZFPROTOCOL_ACCESS(ZFTcp)->remoteInfo(this, d->nativeSocket, remoteAddr, remotePort);
    }
}

ZFMETHOD_DEFINE_0(ZFTcp, zfautoT<ZFTcp>, accept) {
    switch(d->type) {
        case ZFTcpType::e_Invalid:
            ZFLog("calling accept() before open() successfully");
            return zfnull;
        case ZFTcpType::e_Server:
            break;
        default:
            ZFLog("calling accept() on non server type socket");
            return zfnull;
    }

    void *acceptSocket = ZFPROTOCOL_ACCESS(ZFTcp)->accept(this, d->nativeSocket);
    if(acceptSocket == zfnull) {
        return zfnull;
    }
    zfobj<ZFTcp> ret;
    ret->d->nativeSocket = acceptSocket;
    ret->d->host = d->host;
    ret->d->port = d->port;
    ret->d->type = ZFTcpType::e_ServerAccept;
    return ret;
}

ZFMETHOD_DEFINE_1(ZFTcp, zfbool, send
            , ZFMP_IN(const zfstring &, data)
            ) {
    return this->send(data.buffer(), data.length());
}
ZFMETHOD_DEFINE_2(ZFTcp, zfbool, send
            , ZFMP_IN(const void *, data)
            , ZFMP_IN(zfindex, size)
            ) {
    switch(d->type) {
        case ZFTcpType::e_Invalid:
            ZFLog("calling send() on invalid socket");
            return zffalse;
        case ZFTcpType::e_Server:
            ZFLog("calling send() on server socket");
            return zffalse;
        default:
            break;
    }
    return ZFPROTOCOL_ACCESS(ZFTcp)->send(this, d->nativeSocket, data, size);
}
ZFMETHOD_DEFINE_1(ZFTcp, zfbool, send
            , ZFMP_IN(const ZFInput &, input)
            ) {
    zfstring buf;
    ZFInputRead(buf, input);
    return this->send(buf.buffer(), buf.length());
}
ZFMETHOD_DEFINE_3(ZFTcp, zfindex, recv
            , ZFMP_IN_OUT(zfstring &, data)
            , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
            , ZFMP_IN_OPT(zftimet, timeout, -1)
            ) {
    switch(d->type) {
        case ZFTcpType::e_Invalid:
            ZFLog("calling recv() before open() successfully");
            return 0;
        case ZFTcpType::e_Server:
            ZFLog("calling recv() on server type socket");
            return 0;
        default:
            break;
    }
    if(maxSize == zfindexMax()) {
        maxSize = 4096;
    }
    data.capacity(data.length() + maxSize);
    zfindex recvSize = ZFPROTOCOL_ACCESS(ZFTcp)->recv(this, d->nativeSocket, data.zfunsafe_buffer() + data.length(), maxSize, timeout);
    data.zfunsafe_length(data.length() + recvSize);
    data.zfunsafe_buffer()[data.length()] = '\0';
    return recvSize;
}
ZFMETHOD_DEFINE_3(ZFTcp, zfindex, recv
            , ZFMP_IN_OUT(const ZFOutput &, output)
            , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
            , ZFMP_IN_OPT(zftimet, timeout, -1)
            ) {
    switch(d->type) {
        case ZFTcpType::e_Invalid:
            ZFLog("calling recv() before open() successfully");
            return 0;
        case ZFTcpType::e_Server:
            ZFLog("calling recv() on server type socket");
            return 0;
        default:
            break;
    }
    if(maxSize == zfindexMax()) {
        maxSize = 4096;
    }

    zfbyte buf[1024];
    zfindex sizeRead = 0;
    do {
        zfindex sizeToRead = zfmMin((zfindex)sizeof(buf), maxSize - sizeRead);
        zfindex size = ZFPROTOCOL_ACCESS(ZFTcp)->recv(this, d->nativeSocket, buf, sizeToRead, timeout);
        size = output.execute(buf, size);
        sizeRead += size;
        if(size < sizeToRead || sizeRead >= maxSize) {
            break;
        }
    } while(zftrue);
    return sizeRead;
}

ZF_NAMESPACE_GLOBAL_END

