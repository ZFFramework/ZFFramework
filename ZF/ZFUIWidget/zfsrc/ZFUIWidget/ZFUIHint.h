/**
 * @file ZFUIHint.h
 * @brief a quick UI hint to show short messages
 */

#ifndef _ZFI_ZFUIHint_h_
#define _ZFI_ZFUIHint_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief default hint duration, 1500 by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIWidget, zftimet, ZFUIHintDuration)
/**
 * @brief default #ZFUIWindow::windowLevel for #ZFUIHint, 2000 by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIWidget, zfint, ZFUIHintWindowLevel)

// ============================================================
zfclassFwd _ZFP_ZFUIHintPrivate;
/**
 * @brief a quick UI hint to show short messages
 *
 * @note can only be created or show or hide in main thread
 */
zfclass ZFLIB_ZFUIWidget ZFUIHint : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIHint, ZFObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to init the hint object,
     * typically used for one-time init for each hint object,
     * to update app-wide default setting
     */
    ZFEVENT(HintOnInit)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to update the hint object before show (each time)
     */
    ZFEVENT(HintOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when the hint showed
     */
    ZFEVENT(HintOnShow)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when the hint hide
     */
    ZFEVENT(HintOnHide)

public:
    /**
     * @brief get hint list in specified #ZFUIRootWindow
     */
    ZFMETHOD_DECLARE_STATIC_1(ZFCoreArray<zfautoT<ZFUIHint> >, hintList
            , ZFMP_IN_OPT(ZFUIRootWindow *, rootWindow, zfnull)
            )

public:
    /**
     * @brief the window to hold the #content
     *
     * by default, the window would:
     * -  have #v_ZFUIWindowLevel::e_ZFFrameworkFgHighest as #ZFUIWindow::windowLevel
     * -  have #ZFUIView::viewUIEnableTree been disabled
     * -  have #ZFUIGlobalStyle::itemSizeText as #ZFUIView::viewSizeMin
     * -  have #ZFUIWindow::layoutParam as:
     *   -  #ZFUISizeParamWrapWrap as #ZFUILayoutParam::sizeParam
     *   -  #v_ZFUIAlign::e_Center as #ZFUILayoutParam::align
     *   -  #ZFUIGlobalStyle::itemMargin as #ZFUILayoutParam::margin
     *
     * you should update the layout param of the hint window instead of the hint view,
     * and the hint view would be layouted according the window's layout param
     */
    ZFMETHOD_DECLARE_0(ZFUIWindow *, window)

public:
    /**
     * @brief the actual hint view
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIView>, content)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIView>, content)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIView>, content)

    /**
     * @brief hint duration in miliseconds, 0 to use #ZFUIHintDuration, 0 by default
     *
     * @note show and hide animation won't be included in #duration
     */
    ZFPROPERTY_ASSIGN(zftimet, duration)
    /** @brief util method to #duration */
    ZFMETHOD_DECLARE_0(zftimet, durationFixed)

    /**
     * @brief the animation to show the hint, null by default
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, aniShow)
    /**
     * @brief the animation to hide the hint, null by default
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, aniHide)

    /**
     * @brief whether automatically fix frame according to #ZFUIOnScreenKeyboardAutoResizeStart,
     *   true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, windowAutoResize, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, windowAutoResize)

public:
    /**
     * @brief show the hint with animation, do nothing if showing
     *
     * if there's another hint showing,
     * this one would be wait until all previous hint hide\n
     * the hint object would be retained when show,
     * and would be released automatically after hide
     */
    ZFMETHOD_DECLARE_0(void, show)
    /**
     * @brief hide the hint with animation,
     *   do nothing if not showing,
     *   cancel show task if delaying
     */
    ZFMETHOD_DECLARE_0(void, hide)
    /**
     * @brief whether the hint is showing or delaying, see #delaying
     */
    ZFMETHOD_DECLARE_0(zfbool, showing)
    /**
     * @brief whether the hint is delaying, see #showing
     */
    ZFMETHOD_DECLARE_0(zfbool, delaying)
    /**
     * @brief return the animation if animating, or null if not
     */
    ZFMETHOD_DECLARE_0(ZFAnimation *, started)

protected:
    /** @brief see #ZFUIHint::E_HintOnInit */
    virtual inline void hintOnInit(void) {
        this->observerNotify(ZFUIHint::E_HintOnInit());
    }
    /** @brief see #ZFUIHint::E_HintOnUpdate */
    virtual inline void hintOnUpdate(void) {
        this->observerNotify(ZFUIHint::E_HintOnUpdate());
    }
    /** @brief see #ZFUIHint::E_HintOnShow */
    virtual inline void hintOnShow(void) {
        this->observerNotify(ZFUIHint::E_HintOnShow());
    }
    /** @brief see #ZFUIHint::E_HintOnHide */
    virtual inline void hintOnHide(void) {
        this->observerNotify(ZFUIHint::E_HintOnHide());
    }

protected:
    zfoverride
    virtual inline void objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
        zfsuper::objectInfoImplAppend(ret);
        if(this->content() != zfnull) {
            ret += ", content: ";
            this->content()->objectInfoT(ret);
        }
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUIHintPrivate *d;
    friend zfclassFwd _ZFP_ZFUIHintPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIHint_h_

