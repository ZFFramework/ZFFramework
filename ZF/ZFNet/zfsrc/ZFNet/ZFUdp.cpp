#include "ZFUdp.h"
#include "protocol/ZFProtocolZFUdp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUdpAddrPrivate {
public:
    zfuint refCount;
    void *hostAddr;

public:
    _ZFP_ZFUdpAddrPrivate(void)
    : refCount(1)
    , hostAddr(zfnull)
    {
    }
    ~_ZFP_ZFUdpAddrPrivate(void) {
        if(this->hostAddr) {
            ZFPROTOCOL_ACCESS(ZFUdp)->hostRelease(this->hostAddr);
        }
    }
};

ZFUdpAddr::ZFUdpAddr(void)
: d(zfnull)
{
}
ZFUdpAddr::ZFUdpAddr(ZF_IN const ZFUdpAddr &ref)
: d(ref.d)
{
    if(d != zfnull) {
        ++(d->refCount);
    }
}
ZFUdpAddr::~ZFUdpAddr(void) {
    if(d != zfnull) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
    }
}
ZFUdpAddr &ZFUdpAddr::operator = (ZF_IN const ZFUdpAddr &ref) {
    _ZFP_ZFUdpAddrPrivate *dTmp = d;
    if(ref.d != zfnull) {
        ++(ref.d->refCount);
    }
    d = ref.d;
    if(dTmp != zfnull) {
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfdelete(d);
        }
    }
    return *this;
}

void ZFUdpAddr::_ZFP_hostAddr(ZF_IN void *hostAddr) {
    void *hostAddrOld = zfnull;

    if(d == zfnull) {
        if(hostAddr != zfnull) {
            d = zfnew(_ZFP_ZFUdpAddrPrivate);
            d->hostAddr = hostAddr;
        }
    }
    else {
        hostAddrOld = d->hostAddr;
        d->hostAddr = hostAddr;
    }

    if(hostAddrOld != zfnull) {
        ZFPROTOCOL_ACCESS(ZFUdp)->hostRelease(hostAddrOld);
    }
}
void *ZFUdpAddr::_ZFP_hostAddr(void) const {
    return d != zfnull ? d->hostAddr : zfnull;
}

zfbool ZFUdpAddr::valid(void) const {
    return d != zfnull && d->hostAddr != zfnull;
}

void ZFUdpAddr::clear(void) {
    if(d != zfnull && d->hostAddr != zfnull) {
        void *hostAddrOld = d->hostAddr;
        d->hostAddr = zfnull;
        ZFPROTOCOL_ACCESS(ZFUdp)->hostRelease(hostAddrOld);
    }
}

zfbool ZFUdpAddr::remoteInfo(
        ZF_OUT zfstring &remoteAddr
        , ZF_OUT zfuint &remotePort
        ) const {
    if(d == zfnull || d->hostAddr == zfnull) {
        return zffalse;
    }
    else {
        return ZFPROTOCOL_ACCESS(ZFUdp)->remoteInfo(d->hostAddr, remoteAddr, remotePort);
    }
}

ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFUdpAddr, ZFUdpAddr)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUdpAddr, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUdpAddr, void, clear)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFUdpAddr, zfbool, remoteInfo
        , ZFMP_OUT(zfstring &, remoteAddr)
        , ZFMP_OUT(zfuint &, remotePort)
        )

void ZFUdpAddr::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstring remoteAddr;
    zfuint remotePort;
    if(this->remoteInfo(remoteAddr, remotePort)) {
        zfstringAppend(ret, "udp:%s:%s", remoteAddr, remotePort);
    }
    else {
        ret += "udp:null";
    }
}

// ============================================================
zfclassNotPOD _ZFP_ZFUdpPrivate {
public:
    void *nativeSocket;
    zfuint port;
};

void ZFUdp::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    ret += "udp:";
    zfstringAppend(ret, "%s", this->port());
}

void ZFUdp::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUdpPrivate);
    d->nativeSocket = zfnull;
    d->port = 0;
}
void ZFUdp::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_1(ZFUdp, zfbool, open
        , ZFMP_IN_OPT(zfuint, port, 0)
        ) {
    this->close();
    d->port = port;
    d->nativeSocket = ZFPROTOCOL_ACCESS(ZFUdp)->open(this, d->port);
    if(d->nativeSocket == zfnull) {
        d->port = 0;
        return zffalse;
    }
    if(port != 0) {
        ZFCoreAssert(d->port != 0);
    }
    return zftrue;
}
ZFMETHOD_DEFINE_0(ZFUdp, void, close) {
    if(d->nativeSocket != zfnull) {
        ZFPROTOCOL_ACCESS(ZFUdp)->close(this, d->nativeSocket);
        d->nativeSocket = zfnull;
        d->port = 0;
    }
}
ZFMETHOD_DEFINE_0(ZFUdp, zfbool, valid) {
    return d->nativeSocket != zfnull;
}
ZFMETHOD_DEFINE_0(ZFUdp, zfuint, port) {
    return d->port;
}

ZFMETHOD_DEFINE_2(ZFUdp, ZFUdpAddr, hostResolve
        , ZFMP_IN(const zfstring &, host)
        , ZFMP_IN(zfuint, port)
        ) {
    void *hostAddr = ZFPROTOCOL_ACCESS(ZFUdp)->hostResolve(host, port);
    ZFUdpAddr ret;
    if(hostAddr != zfnull) {
        ret._ZFP_hostAddr(hostAddr);
    }
    return ret;
}

// ============================================================
ZFMETHOD_DEFINE_2(ZFUdp, zfbool, send
        , ZFMP_IN(const ZFUdpAddr &, hostAddr)
        , ZFMP_IN(const zfstring &, data)
        ) {
    return this->send(hostAddr, data.buffer(), data.length());
}
ZFMETHOD_DEFINE_3(ZFUdp, zfbool, send
        , ZFMP_IN(const ZFUdpAddr &, hostAddr)
        , ZFMP_IN(const void *, data)
        , ZFMP_IN(zfindex, size)
        ) {
    return hostAddr.valid()
        && size != zfindexMax()
        && ZFPROTOCOL_ACCESS(ZFUdp)->send(this, d->nativeSocket, hostAddr._ZFP_hostAddr(), data, size);
}
ZFMETHOD_DEFINE_2(ZFUdp, zfbool, send
        , ZFMP_IN(const ZFUdpAddr &, hostAddr)
        , ZFMP_IN(const ZFInput &, input)
        ) {
    if(hostAddr.valid()) {
        zfstring buf;
        ZFInputRead(buf, input);
        return this->send(hostAddr, buf.buffer(), buf.length());
    }
    else {
        return zffalse;
    }
}

// ============================================================
ZFMETHOD_DEFINE_3(ZFUdp, zfbool, send
        , ZFMP_IN(const zfstring &, host)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const zfstring &, data)
        ) {
    return this->send(this->hostResolve(host, port), data);
}
ZFMETHOD_DEFINE_4(ZFUdp, zfbool, send
        , ZFMP_IN(const zfstring &, host)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const void *, data)
        , ZFMP_IN(zfindex, size)
        ) {
    return this->send(this->hostResolve(host, port), data, size);
}
ZFMETHOD_DEFINE_3(ZFUdp, zfbool, send
        , ZFMP_IN(const zfstring &, host)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const ZFInput &, input)
        ) {
    return this->send(this->hostResolve(host, port), input);
}

// ============================================================
ZFMETHOD_DEFINE_4(ZFUdp, zfindex, recv
        , ZFMP_OUT(ZFUdpAddr &, hostAddr)
        , ZFMP_IN_OUT(zfstring &, data)
        , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
        , ZFMP_IN_OPT(zftimet, timeout, -1)
        ) {
    if(!this->valid()) {
        ZFLog("calling recv() before open() successfully");
        return 0;
    }
    if(maxSize == zfindexMax()) {
        maxSize = 4096;
    }
    data.capacity(data.length() + maxSize);
    void *hostAddrImpl = zfnull;
    zfindex recvSize = ZFPROTOCOL_ACCESS(ZFUdp)->recv(
        this,
        d->nativeSocket,
        hostAddrImpl,
        data.zfunsafe_buffer() + data.length(),
        maxSize,
        timeout);
    data.zfunsafe_length(data.length() + recvSize);
    data.zfunsafe_buffer()[data.length()] = '\0';
    hostAddr._ZFP_hostAddr(hostAddrImpl);
    return recvSize;
}
ZFMETHOD_DEFINE_4(ZFUdp, zfindex, recv
        , ZFMP_OUT(ZFUdpAddr &, hostAddr)
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
        , ZFMP_IN_OPT(zftimet, timeout, -1)
        ) {
    if(!this->valid()) {
        ZFLog("calling recv() before open() successfully");
        return 0;
    }
    if(maxSize == zfindexMax()) {
        maxSize = 4096;
    }

    zfbyte buf[4096];
    zfindex sizeRead = 0;
    do {
        zfindex sizeToRead = zfmMin((zfindex)sizeof(buf), maxSize - sizeRead);
        void *hostAddrImpl = zfnull;
        zfindex size = ZFPROTOCOL_ACCESS(ZFUdp)->recv(this, d->nativeSocket, hostAddrImpl, buf, sizeToRead, timeout);
        if(hostAddrImpl != zfnull) {
            hostAddr._ZFP_hostAddr(hostAddrImpl);
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

