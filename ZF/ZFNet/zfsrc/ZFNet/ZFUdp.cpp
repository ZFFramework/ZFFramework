#include "ZFUdp.h"
#include "protocol/ZFProtocolZFUdp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUdpAddrPrivate {
public:
    zfuint refCount;
    void *addr;

public:
    _ZFP_ZFUdpAddrPrivate(void)
    : refCount(1)
    , addr(zfnull)
    {
    }
    ~_ZFP_ZFUdpAddrPrivate(void) {
        if(this->addr) {
            ZFPROTOCOL_ACCESS(ZFUdp)->addrRelease(this->addr);
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

void ZFUdpAddr::_ZFP_addr(ZF_IN void *addr) {
    void *addrOld = zfnull;

    if(d == zfnull) {
        if(addr != zfnull) {
            d = zfnew(_ZFP_ZFUdpAddrPrivate);
            d->addr = addr;
        }
    }
    else {
        addrOld = d->addr;
        d->addr = addr;
    }

    if(addrOld != zfnull) {
        ZFPROTOCOL_ACCESS(ZFUdp)->addrRelease(addrOld);
    }
}
void *ZFUdpAddr::_ZFP_addr(void) const {
    return d != zfnull ? d->addr : zfnull;
}

zfbool ZFUdpAddr::valid(void) const {
    return d != zfnull && d->addr != zfnull;
}

void ZFUdpAddr::clear(void) {
    if(d != zfnull && d->addr != zfnull) {
        void *addrOld = d->addr;
        d->addr = zfnull;
        ZFPROTOCOL_ACCESS(ZFUdp)->addrRelease(addrOld);
    }
}

zfbool ZFUdpAddr::remoteInfoT(ZF_IN_OUT zfstring &remoteAddr) const {
    if(d == zfnull || d->addr == zfnull) {
        return zffalse;
    }
    else {
        return ZFPROTOCOL_ACCESS(ZFUdp)->remoteInfo(d->addr, remoteAddr);
    }
}
zfstring ZFUdpAddr::remoteInfo(void) const {
    zfstring ret;
    this->remoteInfoT(ret);
    return ret;
}

ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFUdpAddr, ZFUdpAddr)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUdpAddr, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUdpAddr, void, clear)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUdpAddr, zfbool, remoteInfoT
        , ZFMP_OUT(zfstring &, remoteAddr)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUdpAddr, zfstring, remoteInfo)

void ZFUdpAddr::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstring remoteAddr;
    if(this->remoteInfoT(remoteAddr)) {
        zfstringAppend(ret, "UdpAddr:%s", remoteAddr);
    }
    else {
        ret += "UdpAddr:null";
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

ZFMETHOD_DEFINE_1(ZFUdp, ZFUdpAddr, addrResolve
        , ZFMP_IN(const zfstring &, host)
        ) {
    void *addr = ZFPROTOCOL_ACCESS(ZFUdp)->addrResolve(host);
    ZFUdpAddr ret;
    if(addr != zfnull) {
        ret._ZFP_addr(addr);
    }
    return ret;
}

// ============================================================
ZFMETHOD_DEFINE_3(ZFUdp, zfbool, send
        , ZFMP_IN(const ZFUdpAddr &, addr)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const zfstring &, data)
        ) {
    return this->send(addr, port, data.buffer(), data.length());
}
ZFMETHOD_DEFINE_4(ZFUdp, zfbool, send
        , ZFMP_IN(const ZFUdpAddr &, addr)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const void *, data)
        , ZFMP_IN(zfindex, size)
        ) {
    return addr.valid()
        && size != zfindexMax()
        && ZFPROTOCOL_ACCESS(ZFUdp)->send(this, d->nativeSocket, addr._ZFP_addr(), port, data, size);
}
ZFMETHOD_DEFINE_3(ZFUdp, zfbool, send
        , ZFMP_IN(const ZFUdpAddr &, addr)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const ZFInput &, input)
        ) {
    if(addr.valid()) {
        zfstring buf;
        if(ZFInputRead(buf, input) == zfindexMax()) {
            return zffalse;
        }
        return this->send(addr, port, buf.buffer(), buf.length());
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
    return this->send(this->addrResolve(host), port, data);
}
ZFMETHOD_DEFINE_4(ZFUdp, zfbool, send
        , ZFMP_IN(const zfstring &, host)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const void *, data)
        , ZFMP_IN(zfindex, size)
        ) {
    return this->send(this->addrResolve(host), port, data, size);
}
ZFMETHOD_DEFINE_3(ZFUdp, zfbool, send
        , ZFMP_IN(const zfstring &, host)
        , ZFMP_IN(zfuint, port)
        , ZFMP_IN(const ZFInput &, input)
        ) {
    return this->send(this->addrResolve(host), port, input);
}

// ============================================================
ZFMETHOD_DEFINE_3(ZFUdp, zfstring, recv
        , ZFMP_OUT_OPT(ZFUdpAddr *, addr, zfnull)
        , ZFMP_OUT_OPT(zfuint *, port, zfnull)
        , ZFMP_IN_OPT(zftimet, timeout, -1)
        ) {
    if(!this->valid()) {
        ZFLog("calling recv() before open() successfully");
        return 0;
    }
    void *addrImpl = zfnull;
    zfuint portTmp = 0;
    zfstring ret = ZFPROTOCOL_ACCESS(ZFUdp)->recv(
            this
            , d->nativeSocket
            , addrImpl
            , portTmp
            , timeout
            );
    if(addr) {
        addr->_ZFP_addr(addrImpl);
    }
    if(port) {
        *port = portTmp;
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

