/**
 * @file ZFTcp.h
 * @brief tcp operation
 */

#ifndef _ZFI_ZFTcp_h_
#define _ZFI_ZFTcp_h_

#include "ZFNetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief type of the tcp socket
 */
ZFENUM_BEGIN(ZFLIB_ZFNet, ZFTcpType)
    ZFENUM_VALUE(Invalid) /**< @brief invalid */
    ZFENUM_VALUE(Server) /**< @brief opened as server */
    ZFENUM_VALUE(Client) /**< @brief opened as client */
    ZFENUM_VALUE(ServerAccept) /**< @brief opened by server accept */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Invalid)
    ZFENUM_VALUE_REGISTER(Server)
    ZFENUM_VALUE_REGISTER(Client)
    ZFENUM_VALUE_REGISTER(ServerAccept)
ZFENUM_END(ZFLIB_ZFNet, ZFTcpType)
ZFENUM_REG(ZFLIB_ZFNet, ZFTcpType)

zfclassFwd _ZFP_ZFTcpPrivate;
// ============================================================
/**
 * @brief tcp request util
 */
zfclass ZFLIB_ZFNet ZFTcp : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFTcp, ZFObject)

public:
    /**
     * @brief open a socket with specified host and port
     *
     * -  if host is null or empty,
     *   start local server that listen on the port,
     *   use #accept to start accept client connection,
     *   and use #send or #recv on the tcp object returned by #accept
     * -  if host is not null or empty,
     *   try to connect to remote host,
     *   use #send or #recv to communicate with remote host
     */
    ZFMETHOD_DECLARE_2(zfbool, open
            , ZFMP_IN(const zfstring &, host)
            , ZFMP_IN(zfuint, port)
            )
    /**
     * @brief close the socket, see #open
     *
     * close would be called automatically when owner tcp object deallocated
     */
    ZFMETHOD_DECLARE_0(void, close)

    /**
     * @brief type of this tcp object
     */
    ZFMETHOD_DECLARE_0(ZFTcpType, type)

    /**
     * @brief true if #open or #accept successfully
     */
    ZFMETHOD_DECLARE_0(zfbool, valid)

    /**
     * @brief current opened port, or null if not opened
     *
     * return "localhost" for server (null host with #open)
     */
    ZFMETHOD_DECLARE_0(const zfstring &, host)
    /**
     * @brief current opened port, or 0 if not opened
     */
    ZFMETHOD_DECLARE_0(zfuint, port)

    /**
     * @brief get remote addr, always fail if current #type is #v_ZFTcpType::e_Invalid or #v_ZFTcpType::e_Server
     */
    ZFMETHOD_DECLARE_2(zfbool, remoteInfo
            , ZFMP_OUT(zfstring &, remoteAddr)
            , ZFMP_OUT(zfuint &, remotePort)
            )

public:
    /**
     * @brief start accept client connection
     *
     * return a new tcp object that can #send and #recv to communicate with client,
     * or null if error or no client connection
     *
     * note:
     * -  this method may or may not block current thread (depends on impl),
     *   recommended to put it in thread and loop with sleep
     */
    ZFMETHOD_DECLARE_0(zfautoT<ZFTcp>, accept)

    // ============================================================
public:
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_1(zfbool, send
            , ZFMP_IN(const zfstring &, data)
            )
    /**
     * @brief send packet
     *
     * note:
     * -  this method would block current thread until done,
     *   call in new thread if necessary
     */
    ZFMETHOD_DECLARE_2(zfbool, send
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
    ZFMETHOD_DECLARE_1(zfbool, send
            , ZFMP_IN(const ZFInput &, input)
            )

    // ============================================================
public:
    /**
     * @brief recv packet until timeout
     *
     * return length of received bytes,
     * you may call #close to stop recv
     *
     * timeout:
     * -  `<0` : block current thread until anything received
     * -  `0` : do not block current thread, return 0 if nothing to receive
     * -  `>0` : block current thread, until anything received, or reach timeout
     *
     * note: received data would be appended to buffer
     */
    ZFMETHOD_DECLARE_3(zfindex, recv
            , ZFMP_IN_OUT(zfstring &, data)
            , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
            , ZFMP_IN_OPT(zftimet, timeout, -1)
            )
    /**
     * @brief recv packet until timeout
     *
     * return length of received bytes,
     * you may call #close to stop recv
     *
     * timeout:
     * -  `<0` : block current thread until anything received
     * -  `0` : do not block current thread, return 0 if nothing to receive
     * -  `>0` : block current thread, until anything received, or reach timeout
     *
     * note: received data would be appended to output
     */
    ZFMETHOD_DECLARE_3(zfindex, recv
            , ZFMP_IN_OUT(const ZFOutput &, output)
            , ZFMP_IN_OPT(zfindex, maxSize, zfindexMax())
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
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFTcpPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTcp_h_

