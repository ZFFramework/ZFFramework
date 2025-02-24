/**
 * @file ZFUIWebView.h
 * @brief web view
 */

#ifndef _ZFI_ZFUIWebView_h_
#define _ZFI_ZFUIWebView_h_

#include "ZFUIWebKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIWebView
/**
 * @brief web view
 */
zfclass ZFLIB_ZFUIWebKit ZFUIWebView : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIWebView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIWebView)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when load start or stop,
     * new state can be checked by #loading
     */
    ZFEVENT(WebLoadStateOnUpdate)

public:
    // ============================================================
    // properties
    ZFPROPERTY_ON_INIT_INLINE(zfbool, focusable) {
        propertyValue = zftrue;
    }

public:
    /**
     * @brief load from url
     */
    ZFMETHOD_DECLARE_1(void, loadUrl
            , ZFMP_IN(const zfchar *, url)
            )
    /**
     * @brief load from html string
     */
    ZFMETHOD_DECLARE_2(void, loadHtml
            , ZFMP_IN(const zfchar *, html)
            , ZFMP_IN_OPT(const zfchar *, baseUrl, zfnull)
            )
    /**
     * @brief reload current web data
     */
    ZFMETHOD_DECLARE_0(void, reload)
    /**
     * @brief stop loading
     */
    ZFMETHOD_DECLARE_0(void, loadStop)

    /**
     * @brief go back, see #goBackAvailable
     */
    ZFMETHOD_DECLARE_0(void, goBack)
    /**
     * @brief go forward, see #goForwardAvailable
     */
    ZFMETHOD_DECLARE_0(void, goForward)

public:
    /**
     * @brief true if web is loading
     */
    ZFMETHOD_DECLARE_0(zfbool, loading)

    /**
     * @brief whether the web can go back, see #goBack
     */
    ZFMETHOD_DECLARE_0(zfbool, goBackAvailable)
    /**
     * @brief whether the web can go forward, see #goForward
     */
    ZFMETHOD_DECLARE_0(zfbool, goForwardAvailable)

public:
    zffinal void _ZFP_ZFUIWebView_notifyWebLoadStateOnUpdate(void) {
        this->loadStateOnUpdate();
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    /** @brief see #E_WebLoadStateOnUpdate */
    virtual inline void loadStateOnUpdate(void) {
        this->observerNotify(ZFUIWebView::E_WebLoadStateOnUpdate());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWebView_h_

