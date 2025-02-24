#include "ZFUIWebJSBridge.h"
#include "protocol/ZFProtocolZFUIWebJSBridge.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIWebJSBridgePrivate {
public:
    void *nativeWebJSBridge;
    ZFUIWebView *webView;

public:
    _ZFP_ZFUIWebJSBridgePrivate(void)
    : nativeWebJSBridge(zfnull)
    , webView(zfnull)
    {
    }
};

// ============================================================
// ZFUIWebJSBridge
ZFOBJECT_REGISTER(ZFUIWebJSBridge)

ZFEVENT_REGISTER(ZFUIWebJSBridge, OnSend)
ZFEVENT_REGISTER(ZFUIWebJSBridge, OnRecv)

#define _ZFP_ZFUIWebJSBridge_tagKey "_ZFP_ZFUIWebJSBridge_tagKey"
ZFMETHOD_DEFINE_1(ZFUIWebJSBridge, ZFUIWebJSBridge *, instanceForWebView
        , ZFMP_IN(ZFUIWebView *, webView)
        ) {
    ZFUIWebJSBridge *ret = webView->objectTag(_ZFP_ZFUIWebJSBridge_tagKey);
    if(ret == zfnull) {
        zfauto tmp = ZFUIWebJSBridge::ClassData()->newInstance();
        ret = tmp;
        ret->d->webView = webView;
        webView->objectTag(_ZFP_ZFUIWebJSBridge_tagKey, ret);
    }
    return ret;
}

ZFMETHOD_DEFINE_0(ZFUIWebJSBridge, ZFUIWebView *, webView) {
    return d->webView;
}

ZFMETHOD_DEFINE_1(ZFUIWebJSBridge, void, send
        , ZFMP_IN_OUT(ZFJson &, data)
        ) {
    this->onSend(data);
    ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->send(this, data);
}

void ZFUIWebJSBridge::objectOnInit(void) {
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIWebJSBridgePrivate);
    d->nativeWebJSBridge = ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->nativeWebJSBridgeCreate(this);
}
void ZFUIWebJSBridge::objectOnDealloc(void) {
    ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->nativeWebJSBridgeDestroy(this, d->nativeWebJSBridge);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

