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
    , type(v_ZFTcpType::e_Invalid)
    {
    }
};

void ZFTcp::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    switch (this->type()) {
        case v_ZFTcpType::e_Server:
            zfstringAppend(ret, "TcpServer:%s", this->port());
            break;
        case v_ZFTcpType::e_Client:
            zfstringAppend(ret, "TcpClient:%s:%s", this->host(), this->port());
            break;
        case v_ZFTcpType::e_ServerAccept:
            ret += "TcpRecv";
            break;
        case v_ZFTcpType::e_Invalid:
        default:
            break;
    }
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

ZFMETHOD_DEFINE_3(ZFTcp, zfbool, open
            , ZFMP_IN(const zfstring &, host)
            , ZFMP_IN(zfuint, port)
            , ZFMP_IN_OPT(zftimet, timeout, 2000)
            ) {
    if(d->type == v_ZFTcpType::e_ServerAccept) {
        ZFLog("calling open() on tcp returned by accept()");
        return zffalse;
    }

    this->close();
    if(!host) {
        d->host = zfnull;
        d->type = v_ZFTcpType::e_Server;
    }
    else {
        d->host = host;
        d->type = v_ZFTcpType::e_Client;
    }
    d->port = port;
    d->nativeSocket = zfnull;
    d->nativeSocket = ZFPROTOCOL_ACCESS(ZFTcp)->open(this, host, port, timeout);
    if(d->nativeSocket == zfnull) {
        d->host = zfnull;
        d->port = 0;
        d->nativeSocket = zfnull;
        d->type = v_ZFTcpType::e_Invalid;
        return zffalse;
    }
    return zftrue;
}
ZFMETHOD_DEFINE_0(ZFTcp, void, close) {
    if(d->type != v_ZFTcpType::e_Invalid) {
        ZFPROTOCOL_ACCESS(ZFTcp)->close(this, d->nativeSocket);
        d->nativeSocket = zfnull;
        d->host = zfnull;
        d->port = 0;
        d->type = v_ZFTcpType::e_Invalid;
    }
}

ZFMETHOD_DEFINE_0(ZFTcp, ZFTcpType, type) {
    return d->type;
}

ZFMETHOD_DEFINE_0(ZFTcp, zfbool, valid) {
    return d->type != v_ZFTcpType::e_Invalid;
}
ZFMETHOD_DEFINE_0(ZFTcp, const zfstring &, host) {
    return d->host;
}
ZFMETHOD_DEFINE_0(ZFTcp, zfuint, port) {
    return d->port;
}

ZFMETHOD_DEFINE_1(ZFTcp, zfbool, remoteInfoT
            , ZFMP_IN_OUT(zfstring &, remoteAddr)
            ) {
    switch(d->type) {
        case v_ZFTcpType::e_Invalid:
        case v_ZFTcpType::e_Server:
            return zffalse;
        default:
            return ZFPROTOCOL_ACCESS(ZFTcp)->remoteInfo(this, d->nativeSocket, remoteAddr);
    }
}
ZFMETHOD_DEFINE_0(ZFTcp, zfstring, remoteInfo) {
    zfstring ret;
    this->remoteInfoT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFTcp, zfautoT<ZFTcp>, accept
        , ZFMP_IN_OPT(zftimet, timeout, -1)
        ) {
    switch(d->type) {
        case v_ZFTcpType::e_Invalid:
            ZFLog("calling accept() before open() successfully");
            return zfnull;
        case v_ZFTcpType::e_Server:
            break;
        default:
            ZFLog("calling accept() on non server type socket");
            return zfnull;
    }

    void *acceptSocket = ZFPROTOCOL_ACCESS(ZFTcp)->accept(this, d->nativeSocket, timeout);
    if(acceptSocket == zfnull) {
        return zfnull;
    }
    zfobj<ZFTcp> ret;
    ret->d->nativeSocket = acceptSocket;
    ret->d->type = v_ZFTcpType::e_ServerAccept;
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
        case v_ZFTcpType::e_Invalid:
            ZFLog("calling send() on invalid socket");
            return zffalse;
        case v_ZFTcpType::e_Server:
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
    if(ZFInputRead(buf, input) == zfindexMax()) {
        return zffalse;
    }
    return this->send(buf.buffer(), buf.length());
}
ZFMETHOD_DEFINE_3(ZFTcp, zfindex, recv
            , ZFMP_IN_OUT(zfstring &, data)
            , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
            , ZFMP_IN_OPT(zftimet, timeout, -1)
            ) {
    switch(d->type) {
        case v_ZFTcpType::e_Invalid:
            ZFLog("calling recv() before open() successfully");
            return 0;
        case v_ZFTcpType::e_Server:
            ZFLog("calling recv() on server type socket");
            return 0;
        default:
            break;
    }

    const zfindex blockSize = 1024;
    data.capacity(data.length() + blockSize);
    zfindex sizeRead = 0;
    do {
        zfindex sizeToRead = zfmMin(blockSize, maxSize - sizeRead);
        zfindex size = ZFPROTOCOL_ACCESS(ZFTcp)->recv(this, d->nativeSocket, data.zfunsafe_buffer() + data.length(), sizeToRead, timeout);
        if(size == zfindexMax()) {
            data.zfunsafe_buffer()[data.length()] = '\0';
            return zfindexMax();
        }
        data.zfunsafe_length(data.length() + size);
        data.zfunsafe_buffer()[data.length()] = '\0';
        sizeRead += size;
        if(size < sizeToRead || sizeRead >= maxSize) {
            break;
        }
    } while(zftrue);
    return sizeRead;
}
ZFMETHOD_DEFINE_3(ZFTcp, zfindex, recv
            , ZFMP_IN_OUT(const ZFOutput &, output)
            , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
            , ZFMP_IN_OPT(zftimet, timeout, -1)
            ) {
    switch(d->type) {
        case v_ZFTcpType::e_Invalid:
            ZFLog("calling recv() before open() successfully");
            return 0;
        case v_ZFTcpType::e_Server:
            ZFLog("calling recv() on server type socket");
            return 0;
        default:
            break;
    }

    zfbyte buf[1024];
    zfindex sizeRead = 0;
    do {
        zfindex sizeToRead = zfmMin((zfindex)sizeof(buf), maxSize - sizeRead);
        zfindex size = ZFPROTOCOL_ACCESS(ZFTcp)->recv(this, d->nativeSocket, buf, sizeToRead, timeout);
        if(size == zfindexMax()) {
            return zfindexMax();
        }
        size = output.execute(buf, size);
        sizeRead += size;
        if(size < sizeToRead || sizeRead >= maxSize) {
            break;
        }
    } while(zftrue);
    return sizeRead;
}

ZF_NAMESPACE_GLOBAL_END

