/**
 * @file ZFProtocolZFUIWebView.h
 * @brief protocol for ZFUIWebView
 */

#ifndef _ZFI_ZFProtocolZFUIWebView_h_
#define _ZFI_ZFProtocolZFUIWebView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIWebKit/ZFUIWebView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUIWebView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIWebKit, ZFUIWebView)
public:
    /**
     * @brief create native web view
     */
    virtual void *nativeWebViewCreate(
            ZF_IN ZFUIWebView *webView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) zfpurevirtual;
    /**
     * @brief destroy native web view
     */
    virtual void nativeWebViewDestroy(
            ZF_IN ZFUIWebView *webView
            , ZF_IN void *nativeWebView
            ) zfpurevirtual;

    /**
     * @brief see #ZFUIWebView::loadUrl
     */
    virtual void loadUrl(
            ZF_IN ZFUIWebView *webView
            , ZF_IN const zfchar *url
            ) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::loadHtml
     */
    virtual void loadHtml(
            ZF_IN ZFUIWebView *webView
            , ZF_IN const zfchar *html
            , ZF_IN_OPT const zfchar *baseUrl = zfnull
            ) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::reload
     */
    virtual void reload(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::loadStop
     */
    virtual void loadStop(ZF_IN ZFUIWebView *webView) zfpurevirtual;

    /**
     * @brief see #ZFUIWebView::goBack
     */
    virtual void goBack(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::goForward
     */
    virtual void goForward(ZF_IN ZFUIWebView *webView) zfpurevirtual;

    /**
     * @brief see #ZFUIWebView::loading
     */
    virtual zfbool loading(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::goBackAvailable
     */
    virtual zfbool goBackAvailable(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::goForwardAvailable
     */
    virtual zfbool goForwardAvailable(ZF_IN ZFUIWebView *webView) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when web load start or stop
     *   (both success or fail)
     */
    zffinal void notifyWebLoadStateOnUpdate(ZF_IN ZFUIWebView *webView) {
        webView->_ZFP_ZFUIWebView_notifyWebLoadStateOnUpdate();
    }
ZFPROTOCOL_INTERFACE_END(ZFUIWebView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIWebView_h_

