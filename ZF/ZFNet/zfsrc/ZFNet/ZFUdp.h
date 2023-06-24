/**
 * @file ZFUdp.h
 * @brief udp operation
 */

#ifndef _ZFI_ZFUdp_h_
#define _ZFI_ZFUdp_h_

#include "ZFNetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief udp packet
 */
zfclass ZFLIB_ZFNet ZFUdpPacket : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUdpPacket, ZFObject)
    ZFALLOC_CACHE_RELEASE({
        cache->host.removeAll();
        cache->port = 0;
        cache->data.bufferSize(0);
    })

public:
    /** @brief host of the packet */
    zfstring host;
    /** @brief port of the packet */
    zfuint port;
    /** @brief data of the packet */
    ZFBuffer data;

public:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
};

zfclassFwd _ZFP_ZFUdpPrivate;
// ============================================================
/**
 * @brief udp request util
 */
zfclass ZFLIB_ZFNet ZFUdp : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUdp, ZFObject)

public:
    /**
     * @brief open a socket with specified port,
     *   or 0 to open a random port
     *
     * note:
     * -  the owner udp object would be retained until #close called
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_1(zfbool, open,
                       ZFMP_IN_OPT(zfuint, port, 0))
    /**
     * @brief close the socket, see #open
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_0(void, close)

    /**
     * @brief current opened port, or 0 if not opened
     */
    ZFMETHOD_DECLARE_0(zfuint, port)

public:
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_1(zfbool, send,
                       ZFMP_IN(ZFUdpPacket *, packet))

    /**
     * @brief recv packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     * -  call #close to stop recv
     */
    ZFMETHOD_DECLARE_0(zfautoObjectT<ZFUdpPacket *>, recv)

    /**
     * @brief native udp impl
     */
    ZFMETHOD_DECLARE_0(void *, nativeUdp)

public:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);

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

