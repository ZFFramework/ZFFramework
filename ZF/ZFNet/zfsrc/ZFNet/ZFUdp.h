/**
 * @file ZFUdp.h
 * @brief udp operation
 */

#ifndef _ZFI_ZFUdp_h_
#define _ZFI_ZFUdp_h_

#include "ZFNetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUdpAddrPrivate;
/**
 * @brief udp packet
 */
zfclassLikePOD ZFLIB_ZFNet ZFUdpAddr {
public:
    /** @cond ZFPrivateDoc */
    ZFUdpAddr(void);
    ZFUdpAddr(ZF_IN const ZFUdpAddr &ref);
    ~ZFUdpAddr(void);
    ZFUdpAddr &operator = (ZF_IN const ZFUdpAddr &ref);
    inline operator zfbool (void) const {
        return this->valid();
    }
    /** @endcond */

public:
    /** @brief whether the addr is valid */
    zfbool valid(void) const;
    /** @brief clear the resolved addr */
    void clear(void);

    /** @brief get remote info */
    zfbool remoteInfoT(ZF_IN_OUT zfstring &remoteAddr) const;
    /** @brief get remote info */
    zfstring remoteInfo(void) const;

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    void _ZFP_addr(ZF_IN void *addr);
    void *_ZFP_addr(void) const;
private:
    _ZFP_ZFUdpAddrPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFNet, ZFUdpAddr, ZFUdpAddr)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFNet, ZFUdpAddr, ZFUdpAddr)
ZFOUTPUT_TYPE(ZFUdpAddr, {v.objectInfoT(s);})

zfclassFwd _ZFP_ZFUdpPrivate;
// ============================================================
/**
 * @brief udp request util
 */
zfclass ZFLIB_ZFNet ZFUdp : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUdp, ZFObject)

public:
    /**
     * @brief open a socket ready for #send or #recv
     *
     * call this method with non zero port to bind to specified port,
     * or with zero port to let system to decide\n
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_1(zfbool, open
            , ZFMP_IN_OPT(zfuint, port, 0)
            )
    /**
     * @brief close the socket, see #open
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_0(void, close)

    /**
     * @brief true if #open successfully
     */
    ZFMETHOD_DECLARE_0(zfbool, valid)

    /**
     * @brief current opened port, or 0 if not opened or opened with 0 port
     */
    ZFMETHOD_DECLARE_0(zfuint, port)

public:
    /**
     * @brief resolve host for #send
     */
    ZFMETHOD_DECLARE_1(ZFUdpAddr, addrResolve
            , ZFMP_IN(const zfstring &, host)
            )

    // ============================================================
public:
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_3(zfbool, send
            , ZFMP_IN(const ZFUdpAddr &, addr)
            , ZFMP_IN(zfuint, port)
            , ZFMP_IN(const zfstring &, data)
            )
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_4(zfbool, send
            , ZFMP_IN(const ZFUdpAddr &, addr)
            , ZFMP_IN(zfuint, port)
            , ZFMP_IN(const void *, data)
            , ZFMP_IN(zfindex, size)
            )
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_3(zfbool, send
            , ZFMP_IN(const ZFUdpAddr &, addr)
            , ZFMP_IN(zfuint, port)
            , ZFMP_IN(const ZFInput &, input)
            )

    // ============================================================
public:
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_3(zfbool, send
            , ZFMP_IN(const zfstring &, host)
            , ZFMP_IN(zfuint, port)
            , ZFMP_IN(const zfstring &, data)
            )
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_4(zfbool, send
            , ZFMP_IN(const zfstring &, host)
            , ZFMP_IN(zfuint, port)
            , ZFMP_IN(const void *, data)
            , ZFMP_IN(zfindex, size)
            )
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_3(zfbool, send
            , ZFMP_IN(const zfstring &, host)
            , ZFMP_IN(zfuint, port)
            , ZFMP_IN(const ZFInput &, input)
            )

    // ============================================================
public:
    /**
     * @brief recv packet until timeout
     *
     * you may call #close to stop recv
     *
     * timeout:
     * -  `<0` : block current thread until anything received
     * -  `0` : do not block current thread, return 0 if nothing to receive
     * -  `>0` : block current thread, until anything received, or reach timeout
     */
    ZFMETHOD_DECLARE_3(zfstring, recv
            , ZFMP_OUT_OPT(ZFUdpAddr *, addr, zfnull)
            , ZFMP_OUT_OPT(zfuint *, port, zfnull)
            , ZFMP_IN_OPT(zftimet, timeout, -1)
            )

public:
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUdpPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUdp_h_

