/**
 * @file ZFTcp.h
 * @brief tcp operation
 */

#ifndef _ZFI_ZFTcp_h_
#define _ZFI_ZFTcp_h_

#include "ZFNetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTcpPrivate;
// ============================================================
/**
 * @brief tcp request util
 */
zfclass ZFLIB_ZFNet ZFTcp : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFTcp, ZFObject)

public:
    /**
     * @brief open a socket with specified host and port
     *
     * if host is null or empty,
     * start local server that listen on the port,
     * use #accept to start accept client connection\n
     * \n
     * return a socket connection token that can be used by #send and #recv,
     * or null if error occurred
     *
     * note:
     * -  the owner tcp object would be retained until #close called
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_2(void *, open,
                       ZFMP_IN(const zfchar *, host),
                       ZFMP_IN(zfuint, port))
    /**
     * @brief close the socket, see #open
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_1(void, close,
                       ZFMP_IN(void *, socket))

    /**
     * @brief current opened port, or null if not opened
     *
     * return "localhost" for server
     */
    ZFMETHOD_DECLARE_0(const zfchar *, host)
    /**
     * @brief current opened port, or 0 if not opened
     */
    ZFMETHOD_DECLARE_0(zfuint, port)

public:
    /**
     * @brief start accept client connection
     *
     * return a socket connection token that can be used by #send and #recv,
     * or null if error occurred or closed
     *
     * note:
     * -  this method would block current thread until receive client connection or #close called
     */
    ZFMETHOD_DECLARE_0(void *, accept)

    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_3(zfbool, send,
                       ZFMP_IN(void *, socket),
                       ZFMP_IN(const void *, data),
                       ZFMP_IN(zfindex, size))
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_3(zfbool, send,
                       ZFMP_IN(void *, socket),
                       ZFMP_IN(const zfchar *, data),
                       ZFMP_IN_OPT(zfindex, size, zfindexMax()))
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_2(zfbool, send,
                       ZFMP_IN(void *, socket),
                       ZFMP_IN(const ZFBuffer &, data))
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_2(zfbool, send,
                       ZFMP_IN(void *, socket),
                       ZFMP_IN(const ZFInput &, input))

    /**
     * @brief recv packet
     *
     * return length of received bytes
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     * -  call #close to stop recv
     */
    ZFMETHOD_DECLARE_3(zfindex, recv,
                       ZFMP_IN(void *, socket),
                       ZFMP_IN_OUT(ZFBuffer &, data),
                       ZFMP_IN(zfindex, maxSize))
    /**
     * @brief recv packet
     *
     * return length of received bytes
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     * -  call #close to stop recv
     */
    ZFMETHOD_DECLARE_3(zfindex, recv,
                       ZFMP_IN(void *, socket),
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(zfindex, maxSize))

    /**
     * @brief native tcp impl
     */
    ZFMETHOD_DECLARE_0(void *, nativeTcp)

public:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFTcpPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTcp_h_

