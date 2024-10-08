#include "ZFUIWebJSBridge.h"
#include "protocol/ZFProtocolZFUIWebJSBridge.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIWebJSBridgeSendData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeSendData, ZFJson, send)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeSendData, ZFJson, response)

ZFOBJECT_REGISTER(ZFUIWebJSBridgeRecvData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeRecvData, ZFJson, recv)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeRecvData, ZFJson, response)

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

ZFEVENT_REGISTER(ZFUIWebJSBridge, WebMessageBeforeSend)
ZFEVENT_REGISTER(ZFUIWebJSBridge, WebMessageAfterSend)
ZFEVENT_REGISTER(ZFUIWebJSBridge, WebMessageBeforeRecv)
ZFEVENT_REGISTER(ZFUIWebJSBridge, WebMessageAfterRecv)

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

ZFMETHOD_DEFINE_1(ZFUIWebJSBridge, ZFJson, send
        , ZFMP_IN_OUT(ZFJson &, send)
        ) {
    zfobj<ZFUIWebJSBridgeSendData> dataSend;
    dataSend->send = send;
    this->webMessageBeforeSend(dataSend);
    dataSend->response = ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->send(this, send);
    this->webMessageAfterSend(dataSend);
    return dataSend->response;
}
ZFJson ZFUIWebJSBridge::_ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(ZF_IN_OUT ZFJson &recv) {
    zfobj<ZFUIWebJSBridgeRecvData> dataRecv;
    dataRecv->recv = recv;
    this->webMessageBeforeRecv(dataRecv);
    this->webMessageAfterRecv(dataRecv);
    return dataRecv->response;
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

