/**
 * @file ZFWebSocket.h
 * @brief web socket client
 */

#ifndef _ZFI_ZFWebSocket_h_
#define _ZFI_ZFWebSocket_h_

#include "ZFNetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFWebSocketPrivate;
// ============================================================
/**
 * @brief web socket client
 */
zfclass ZFLIB_ZFNet ZFWebSocket : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFWebSocket, ZFObject)

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * @note may or may not fired in background thread, unless #eventOnMainThread is set
     */
    ZFEVENT(OnOpen)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 would be a #v_ZFResultType indicates normal close or fail,
     * param1 would be a #v_zfstring shows reason hint
     * @note may or may not fired in background thread, unless #eventOnMainThread is set
     */
    ZFEVENT(OnClose)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 would be a #v_zfstring contains data,
     * the data should not be cached for further use
     * @note may or may not fired in background thread, unless #eventOnMainThread is set
     */
    ZFEVENT(OnRecv)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 would be a #v_zfstring contains binary data,
     * the data should not be cached for further use
     * @note may or may not fired in background thread, unless #eventOnMainThread is set
     */
    ZFEVENT(OnRecvBin)

public:
    /**
     * @brief whether notify event on main thread, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, eventOnMainThread, zftrue)

public:
    /**
     * @brief open web socket connection
     */
    ZFMETHOD_DECLARE_1(void, open
            , ZFMP_IN(const zfstring &, url)
            )
    /**
     * @brief close the web socket
     */
    ZFMETHOD_DECLARE_0(void, close)

    /**
     * @brief true if #open successfully
     */
    ZFMETHOD_DECLARE_0(zfbool, valid)

    // ============================================================
public:
    /**
     * @brief send data
     */
    ZFMETHOD_DECLARE_1(void, send
            , ZFMP_IN(const zfstring &, data)
            )
    /**
     * @brief send data
     */
    ZFMETHOD_DECLARE_2(void, send
            , ZFMP_IN(const void *, data)
            , ZFMP_IN(zfindex, size)
            )

    /**
     * @brief send binary data
     */
    ZFMETHOD_DECLARE_1(void, sendBin
            , ZFMP_IN(const zfstring &, data)
            )
    /**
     * @brief send binary data
     */
    ZFMETHOD_DECLARE_2(void, sendBin
            , ZFMP_IN(const void *, data)
            , ZFMP_IN(zfindex, size)
            )

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFWebSocketPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFWebSocket_h_

