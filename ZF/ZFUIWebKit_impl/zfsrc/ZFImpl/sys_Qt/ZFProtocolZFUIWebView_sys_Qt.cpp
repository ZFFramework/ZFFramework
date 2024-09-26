#include "ZFImpl_sys_Qt_ZFUIWebKit_impl.h"
#include "ZFUIWebKit/protocol/ZFProtocolZFUIWebView.h"

#if ZF_ENV_sys_Qt && (defined(QT_WEBKITWIDGETS_LIB) || defined(QT_WEBENGINEWIDGETS_LIB))

#ifndef QT_WEBKITWIDGETS_LIB
    #include <QWebEngineView>
    #include <QWebEngineHistory>
    #define _ZFP_ZFUIWebViewImpl_sys_Qt_QWebViewBase QWebEngineView
#else
    #include <QWebView>
    #include <QWebHistory>
    #define _ZFP_ZFUIWebViewImpl_sys_Qt_QWebViewBase QWebView
#endif

class _ZFP_ZFUIWebViewImpl_sys_Qt_View : public _ZFP_ZFUIWebViewImpl_sys_Qt_QWebViewBase {
    Q_OBJECT

public:
    ZFUIWebView *_ZFP_ownerZFUIWebView;
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIWebView) *_ZFP_impl;
    zfbool _ZFP_loading;

public:
    _ZFP_ZFUIWebViewImpl_sys_Qt_View(
            ZF_IN ZFUIWebView *ownerZFUIWebView
            , ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIWebView) *impl
            )
    : _ZFP_ownerZFUIWebView(ownerZFUIWebView)
    , _ZFP_impl(impl)
    , _ZFP_loading(zffalse)
    {
        this->connect(this, SIGNAL(loadStarted()), this, SLOT(_ZFP_loadOnStart()));
        this->connect(this, SIGNAL(loadFinished(bool)), this, SLOT(_ZFP_loadOnStop(bool)));
    }
    ~_ZFP_ZFUIWebViewImpl_sys_Qt_View(void) {
        this->disconnect();
    }

public slots:
    void _ZFP_loadOnStart(void) {
        _ZFP_loading = zftrue;
        _ZFP_impl->notifyWebLoadStateOnUpdate(_ZFP_ownerZFUIWebView);
    }
    void _ZFP_loadOnStop(bool success) {
        _ZFP_loading = zffalse;
        _ZFP_impl->notifyWebLoadStateOnUpdate(_ZFP_ownerZFUIWebView);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIWebViewImpl_sys_Qt, ZFUIWebView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:UIWebView")
public:
    virtual void *nativeWebViewCreate(
            ZF_IN ZFUIWebView *webView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        return new _ZFP_ZFUIWebViewImpl_sys_Qt_View(webView, this);
    }
    virtual void nativeWebViewDestroy(
            ZF_IN ZFUIWebView *webView
            , ZF_IN void *nativeWebView
            ) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebViewTmp = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)nativeWebView;
        nativeWebViewTmp->deleteLater();
    }

    virtual void loadUrl(
            ZF_IN ZFUIWebView *webView
            , ZF_IN const zfchar *url
            ) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        nativeWebView->load(QUrl(QString::fromUtf8(url)));
        nativeWebView->show();
    }
    virtual void loadHtml(
            ZF_IN ZFUIWebView *webView
            , ZF_IN const zfchar *html
            , ZF_IN_OPT const zfchar *baseUrl = zfnull
            ) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        nativeWebView->setHtml(QString::fromUtf8(html),
                               (baseUrl == zfnull)
                               ? QUrl()
                               : QUrl(QString::fromUtf8(baseUrl))
                               );
        nativeWebView->show();
    }
    virtual void reload(ZF_IN ZFUIWebView *webView) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        nativeWebView->reload();
    }
    virtual void loadStop(ZF_IN ZFUIWebView *webView) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        nativeWebView->stop();
    }

    virtual void goBack(ZF_IN ZFUIWebView *webView) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        nativeWebView->back();
    }
    virtual void goForward(ZF_IN ZFUIWebView *webView) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        nativeWebView->forward();
    }

    virtual zfbool loading(ZF_IN ZFUIWebView *webView) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        return nativeWebView->_ZFP_loading;
    }
    virtual zfbool goBackAvailable(ZF_IN ZFUIWebView *webView) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        return nativeWebView->history()->canGoBack();
    }
    virtual zfbool goForwardAvailable(ZF_IN ZFUIWebView *webView) {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = (_ZFP_ZFUIWebViewImpl_sys_Qt_View *)webView->nativeImplView();
        return nativeWebView->history()->canGoForward();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIWebViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIWebView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

