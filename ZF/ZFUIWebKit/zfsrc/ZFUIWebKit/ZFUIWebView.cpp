#include "ZFUIWebView.h"
#include "protocol/ZFProtocolZFUIWebView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIWebView)

// ============================================================
// ZFUIWebView
ZFOBJECT_REGISTER(ZFUIWebView)

ZFEVENT_REGISTER(ZFUIWebView, WebLoadStateOnUpdate)

ZFMETHOD_DEFINE_1(ZFUIWebView, void, loadUrl
        , ZFMP_IN(const zfchar *, url)
        ) {
    ZFPROTOCOL_ACCESS(ZFUIWebView)->loadUrl(this, url);
}
ZFMETHOD_DEFINE_2(ZFUIWebView, void, loadHtml
        , ZFMP_IN(const zfchar *, html)
        , ZFMP_IN_OPT(const zfchar *, baseUrl, zfnull)
        ) {
    ZFPROTOCOL_ACCESS(ZFUIWebView)->loadHtml(this, html, baseUrl);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, reload) {
    ZFPROTOCOL_ACCESS(ZFUIWebView)->reload(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, loadStop) {
    ZFPROTOCOL_ACCESS(ZFUIWebView)->loadStop(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, goBack) {
    ZFPROTOCOL_ACCESS(ZFUIWebView)->goBack(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, goForward) {
    ZFPROTOCOL_ACCESS(ZFUIWebView)->goForward(this);
}

ZFMETHOD_DEFINE_0(ZFUIWebView, zfbool, loading) {
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->loading(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, zfbool, goBackAvailable) {
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->goBackAvailable(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, zfbool, goForwardAvailable) {
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->goForwardAvailable(this);
}

void ZFUIWebView::objectOnInit(void) {
    zfsuper::objectOnInit();

    ZFCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD NativeImplViewDestroy {
    public:
        static void action(
                ZF_IN zfanyT<ZFUIView> const &view
                , ZF_IN void *nativeImplView
                ) {
            ZFPROTOCOL_ACCESS(ZFUIWebView)->nativeWebViewDestroy(view, nativeImplView);
        }
    };
    zfbool nativeImplViewRequireVirtualIndex = zftrue;
    void *nativeImplView = ZFPROTOCOL_ACCESS(ZFUIWebView)->nativeWebViewCreate(this, nativeImplViewRequireVirtualIndex);
    this->nativeImplView(
        nativeImplView,
        NativeImplViewDestroy::action,
        nativeImplViewRequireVirtualIndex);
}
void ZFUIWebView::objectOnDealloc(void) {
    zfsuper::objectOnDealloc();
}
void ZFUIWebView::objectOnDeallocPrepare(void) {
    this->loadStop();
    zfsuper::objectOnDeallocPrepare();
}

ZF_NAMESPACE_GLOBAL_END

