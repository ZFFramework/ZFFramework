/**
 * @file ZFUIWebJSBridge.h
 * @brief web JS bridge
 */

#ifndef _ZFI_ZFUIWebJSBridge_h_
#define _ZFI_ZFUIWebJSBridge_h_

#include "ZFUIWebView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief store necessary data for #ZFUIWebJSBridge::webMessageBeforeSend
 */
zfclass ZFLIB_ZFUIWebKit ZFUIWebJSBridgeSendData : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIWebJSBridgeSendData, ZFObject)

public:
    /**
     * @brief message sent to web
     */
    ZFJson send;
    /**
     * @brief message response from web
     */
    ZFJson response;
};

// ============================================================
/**
 * @brief store necessary data for #ZFUIWebJSBridge::webMessageBeforeRecv
 */
zfclass ZFLIB_ZFUIWebKit ZFUIWebJSBridgeRecvData : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIWebJSBridgeRecvData, ZFObject)

public:
    /**
     * @brief message received from web
     */
    ZFJson recv;
    /**
     * @brief message response to web
     */
    ZFJson response;
};

// ============================================================
zfclassFwd _ZFP_ZFUIWebJSBridgePrivate;
/**
 * @brief web JS bridge
 */
zfclass ZFLIB_ZFUIWebKit ZFUIWebJSBridge : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIWebJSBridge, ZFObject)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeSendData
     */
    ZFEVENT(WebMessageBeforeSend)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeSendData
     */
    ZFEVENT(WebMessageAfterSend)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeRecvData
     */
    ZFEVENT(WebMessageBeforeRecv)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeRecvData
     */
    ZFEVENT(WebMessageAfterRecv)

public:
    /**
     * @brief access the bridge via the webView
     *
     * return old one if exist or create new one if not exist\n
     * the bridge object would be stored as an object tag to it's owner webView
     */
    ZFMETHOD_DECLARE_STATIC_1(ZFUIWebJSBridge *, instanceForWebView
            , ZFMP_IN(ZFUIWebView *, webView)
            )

public:
    /**
     * @brief owner of this JS bridge
     */
    ZFMETHOD_DECLARE_0(ZFUIWebView *, webView)

public:
    /**
     * @brief send message to web
     *
     * note: the message to send can be modified by observing #EventWebMessageBeforeSend
     */
    ZFMETHOD_DECLARE_1(ZFJson, send
            , ZFMP_IN_OUT(ZFJson &, send)
            )
    zffinal ZFJson _ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(ZF_IN_OUT ZFJson &recv);

    ZFOBJECT_PRIVATE_ALLOC("can only be created by ZFUIWebJSBridge::instanceForWebView")
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    /** @brief see #EventWebMessageBeforeSend */
    virtual inline void webMessageBeforeSend(ZF_IN ZFUIWebJSBridgeSendData *dataSend) {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageBeforeSend(), dataSend);
    }
    /** @brief see #EventWebMessageAfterSend */
    virtual inline void webMessageAfterSend(ZF_IN ZFUIWebJSBridgeSendData *dataSend) {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageAfterSend(), dataSend);
    }
    /** @brief see #EventWebMessageBeforeRecv */
    virtual inline void webMessageBeforeRecv(ZF_IN ZFUIWebJSBridgeRecvData *dataRecv) {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageBeforeRecv(), dataRecv);
    }
    /** @brief see #EventWebMessageAfterRecv */
    virtual inline void webMessageAfterRecv(ZF_IN ZFUIWebJSBridgeRecvData *dataRecv) {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageAfterRecv(), dataRecv);
    }

private:
    _ZFP_ZFUIWebJSBridgePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWebJSBridge_h_

