/**
 * @file ZFUIWebJSBridge.h
 * @brief web JS bridge
 */

#ifndef _ZFI_ZFUIWebJSBridge_h_
#define _ZFI_ZFUIWebJSBridge_h_

#include "ZFUIWebView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

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
     * called when send message to web\n
     * param0 is a #v_ZFJson
     */
    ZFEVENT(OnSend)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #v_ZFJson
     */
    ZFEVENT(OnRecv)

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
     * note: the message to send can be modified by observing #E_OnSend
     */
    ZFMETHOD_DECLARE_1(void, send
            , ZFMP_IN_OUT(ZFJson &, data)
            )
    zffinal void _ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(ZF_IN_OUT ZFJson &data) {
        this->onRecv(data);
    }

    ZFOBJECT_PRIVATE_ALLOC("can only be created by ZFUIWebJSBridge::instanceForWebView")
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    /** @brief see #E_OnSend */
    virtual inline void onSend(ZF_IN_OUT ZFJson &data) {
        this->observerNotify(ZFUIWebJSBridge::E_OnSend(), zfobj<v_ZFJson>(data));
    }
    /** @brief see #E_OnRecv */
    virtual inline void onRecv(ZF_IN_OUT ZFJson &data) {
        this->observerNotify(ZFUIWebJSBridge::E_OnRecv(), zfobj<v_ZFJson>(data));
    }

private:
    _ZFP_ZFUIWebJSBridgePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWebJSBridge_h_

