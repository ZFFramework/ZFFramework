/**
 * @file ZFUISysWindow.h
 * @brief root native window
 */

#ifndef _ZFI_ZFUISysWindow_h_
#define _ZFI_ZFUISysWindow_h_

#include "ZFUIRootView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFUISysWindow::mainWindow has attached,
 * either by access #ZFUISysWindow::mainWindow,
 * or by #ZFUISysWindow::mainWindowRegister
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, SysWindowMainWindowOnAttach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfclassFwd ZFUISysWindowEmbedImpl;
zfclassFwd _ZFP_ZFUISysWindowPrivate;
/**
 * @brief root native window
 *
 * use #mainWindow to access app's main window,
 * use #modalWindowShow to show a new window,
 * use #nativeWindowEmbed to attach to native impl,
 * never create #ZFUISysWindow's instance manually
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUISysWindow : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUISysWindow, ZFObject)

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window created,
     * see #nativeWindowIsCreated for more info
     */
    ZFEVENT(SysWindowOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window about to destroy
     */
    ZFEVENT(SysWindowOnDestroy)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window become active
     */
    ZFEVENT(SysWindowOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window enter background
     */
    ZFEVENT(SysWindowOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window rotated,
     * new orientation can be accessed by #sysWindowOrientation
     */
    ZFEVENT(SysWindowOnRotate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when #sysWindowMargin changed,
     * param0 is a #v_ZFUIMargin that holds the old window margin
     */
    ZFEVENT(SysWindowMarginOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when #ZFUIKeyEvent fired to the window,
     * and none of it's view handled the event by #ZFUIEvent::eventResolved\n
     * param0 is the #ZFUIKeyEvent
     */
    ZFEVENT(SysWindowOnKeyEvent)

public:
    /**
     * @brief used to embed #ZFUISysWindow to existing UI framework
     *
     * #ZFUISysWindow would create internal native window by default,
     * however, you may create your own native window and attach #ZFUISysWindow to it\n
     * to achieve this feature,
     * you must supply custom embed impl by #ZFUISysWindowEmbedImpl,
     * call this method to get an attached #ZFUISysWindow,
     * and manually manage the life cycle of the returned #ZFUISysWindow\n
     * \n
     * the embedImpl would be retained until the created window destroy,
     * so that you only need to take care of the created window,
     * to explicitly clear all the contents,
     * simply use `window->nativeWindowEmbedImplDestroy()`\n
     * \n
     * see #ZFUISysWindowEmbedImpl for all the impl that you need to implement
     */
    static zfautoT<ZFUISysWindow> nativeWindowEmbed(ZF_IN ZFUISysWindowEmbedImpl *embedImpl);

    /**
     * @brief see #nativeWindowEmbed
     */
    virtual ZFUISysWindowEmbedImpl *nativeWindowEmbedImpl(void);

    /**
     * @brief util to destroy the window created by #nativeWindowEmbed
     *
     * equal to #ZFUISysWindowEmbedImpl::notifyOnPause
     * + #ZFUISysWindowEmbedImpl::notifyOnDestroy
     */
    ZFMETHOD_DECLARE_0(void, nativeWindowEmbedImplDestroy)

    /**
     * @brief default impl to attach window to native view
     *
     * note, after attach, you must destroy the window by #nativeWindowEmbedImplDestroy
     * before destroy the nativeParent\n
     * the nativeParent must support add one or more child which fill itself
     */
    ZFMETHOD_DECLARE_STATIC_1(zfautoT<ZFUISysWindow>, nativeWindowEmbedNativeView
            , ZFMP_IN(void *, nativeParent)
            )

    /**
     * @brief util to attach window to native view,
     *   and register a getter method with specified name
     *
     * usage:
     * @code
     *   zfauto sysWindow = ZFUISysWindow::nativeWindowEmbedNativeView(nativeParent, "MyWindow");
     *   if(sysWindow != zfnull) {
     *       zfobj<ZFUIWindow> window(ZFInvoke("MyWindow"));
     *       ...
     *   }
     * @endcode
     */
    ZFMETHOD_DECLARE_STATIC_2(zfautoT<ZFUISysWindow>, nativeWindowEmbedNativeView
            , ZFMP_IN(void *, nativeParent)
            , ZFMP_IN(const zfstring &, sysWindowName)
            )
    /**
     * @brief remove window attached by #nativeWindowEmbedNativeView
     */
    ZFMETHOD_DECLARE_STATIC_1(void, nativeWindowEmbedNativeViewCleanup
            , ZFMP_IN(const zfstring &, sysWindowName)
            )

public:
    /**
     * @brief manually register main window, must be called before accessing #mainWindow
     *
     * this method is useful if you want to embed whole ZFFramework to existing UI framework,
     * to create a custom #ZFUISysWindow, see #nativeWindowEmbed\n
     * @note you must manually manage the life cycle of the registered #ZFUISysWindow
     * @note once registered, it can not be changed back to original internal window,
     *   until #ZFFrameworkCleanup
     * @note it's safe to unregister and register a new window,
     *   but you must ensure you won't access the children of the window after you do so
     */
    ZFMETHOD_DECLARE_STATIC_1(void, mainWindowRegister
            , ZFMP_IN(ZFUISysWindow *, window)
            )
    /**
     * @brief get application's main window
     *
     * usually, use only one window is recommended\n
     * the main window would be created and attached automatically
     * when first time accessed (directly or implicitly by showing a #ZFUIWindow),
     * and can not be changed after creation\n
     * to embed ZFFramework to native framework,
     * you may use #mainWindowRegister
     */
    ZFMETHOD_DECLARE_STATIC_0(zfanyT<ZFUISysWindow>, mainWindow)
    /**
     * @brief whether #mainWindow has attached
     */
    ZFMETHOD_DECLARE_STATIC_0(zfbool, mainWindowAttached)

    /**
     * @brief default window that #ZFUIWindow would attach to, set null to use #mainWindow
     */
    ZFMETHOD_DECLARE_STATIC_1(void, keyWindow
            , ZFMP_IN(ZFUISysWindow *, window)
            )
    /**
     * @brief see #keyWindow, return #mainWindow if not set
     */
    ZFMETHOD_DECLARE_STATIC_0(zfanyT<ZFUISysWindow>, keyWindow)

public:
    /**
     * @brief native window's size
     *
     * to change window's size or layout,
     * use #sysWindowLayoutParam\n
     * to observe window size change,
     * attach #ZFUIView::E_ViewLayoutOnLayoutFinish listener to #rootView
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, sysWindowSize)

public:
    /**
     * @brief native window margin according to impl
     *
     * the margin usually used for mobile phones,
     * whose edge contains virtual button or front camera\n
     * by default, #ZFUIWindow would automatically exclude the margin,
     * you may change it by #ZFUIWindow::sysWindowMarginShouldApply
     */
    ZFMETHOD_DECLARE_0(const ZFUIMargin &, sysWindowMargin)
    void _ZFP_ZFUISysWindow_sysWindowMargin(ZF_IN const ZFUIMargin &sysWindowMargin);
protected:
    /**
     * @brief see #E_SysWindowMarginOnUpdate
     */
    virtual void sysWindowMarginOnUpdate(ZF_IN const ZFUIMargin &sysWindowMarginOld);

    ZFOBJECT_PRIVATE_ALLOC("can only be created by ZFUISysWindow::modalWindowShow")
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief access the internal native window
     *
     * note, this value would be valid only after
     * #E_SysWindowOnCreate\n
     * note, this value can be null if window created by #nativeWindowEmbed
     */
    ZFMETHOD_DECLARE_0(void *, nativeWindow)

public:
    /**
     * @brief true if sys window created
     *
     * @note for some implementation,
     *   creating a ZFUISysWindow may or may not create native window immediately,
     *   you may use this method as well as #E_SysWindowOnCreate to see when did window created,
     *   typical usage:
     *   @code
     *       ZFUISysWindow *newWindow = ZFUISysWindow::mainWindow()->modalWindowShow();
     *       if(!newWindow->nativeWindowIsCreated()) {
     *           ZFListener observer = ... {
     *               initWindow();
     *           };
     *           newWindow->observerAdd(ZFUISysWindow::E_SysWindowOnCreate(), observer);
     *       }
     *       else {
     *           initWindow();
     *       }
     *   @endcode
     */
    ZFMETHOD_DECLARE_0(zfbool, nativeWindowIsCreated)
    /**
     * @brief true if sys window resumed
     */
    ZFMETHOD_DECLARE_0(zfbool, nativeWindowIsResumed)

public:
    /**
     * @brief get current orientation
     */
    ZFMETHOD_DECLARE_0(ZFUIOrientation, sysWindowOrientation)
    /**
     * @brief set supported orientation, orientation may or may not change immediately
     */
    ZFMETHOD_DECLARE_1(void, sysWindowOrientationFlags
            , ZFMP_IN(const ZFUIOrientationFlags &, sysWindowOrientationFlags)
            )
    /**
     * @brief get supported orientation
     */
    ZFMETHOD_DECLARE_0(const ZFUIOrientationFlags &, sysWindowOrientationFlags)

    /**
     * @brief whether prefer fullscreen
     */
    ZFPROPERTY_ASSIGN(zfbool, preferFullscreen, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, preferFullscreen)

public:
    /**
     * @brief show a new modal window,
     *   may or may not show immediately,
     *   may have different behavior on different implementations
     *
     * use only if necessary\n
     * it's recommended that app have only one ZFUISysWindow,
     * use #ZFUIWindow if necessary
     */
    ZFMETHOD_DECLARE_0(zfautoT<ZFUISysWindow>, modalWindowShow)
    /**
     * @brief finish and destroy self as a modal window
     */
    ZFMETHOD_DECLARE_0(void, modalWindowFinish)
    /**
     * @brief get self's current showing modal window
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUISysWindow>, modalWindowShowing)
    /**
     * @brief get self's owner if self is a showing modal window
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUISysWindow>, modalWindowOwner)

public:
    /**
     * @brief get window's layout param, fill parent by default
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUILayoutParam>, sysWindowLayoutParam)

    /**
     * @brief access the root view of the window
     *
     * the root view is ensured #ZFUIRootView type,
     * and you must not add it to another view
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIRootView>, rootView)

public:
    zffinal ZFUIRect _ZFP_ZFUISysWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect);
    zffinal void _ZFP_ZFUISysWindow_onCreate(ZF_IN void *nativeWindow);
    zffinal void _ZFP_ZFUISysWindow_onDestroy(void);
    zffinal void _ZFP_ZFUISysWindow_onResume(void);
    zffinal void _ZFP_ZFUISysWindow_onPause(void);
    zffinal void _ZFP_ZFUISysWindow_onRotate(void);
    zffinal void _ZFP_ZFUISysWindow_sysWindowLayoutUpdate(void);
    zffinal void _ZFP_ZFUISysWindow_keyEvent(ZF_IN ZFUIKeyEvent *event) {
        this->observerNotify(ZFUISysWindow::E_SysWindowOnKeyEvent(), event);
    }

private:
    _ZFP_ZFUISysWindowPrivate *d;
};

// ============================================================
/**
 * @brief impl for #ZFUISysWindow::nativeWindowEmbed
 */
zfinterface ZFLIB_ZFUIKit ZFUISysWindowEmbedImpl : zfextend ZFInterface {
    ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(ZFUISysWindowEmbedImpl, ZFInterface)

public:
    zfanyT<ZFUISysWindow> _ZFP_ownerZFUISysWindow;
    /**
     * @brief the attached #ZFUISysWindow
     */
    inline zfanyT<ZFUISysWindow> const &ownerZFUISysWindow(void) {
        return this->_ZFP_ownerZFUISysWindow;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFUISysWindowEmbedImpl(void) : _ZFP_ownerZFUISysWindow(zfnull) {}
    /** @endcond */

    // ============================================================
public:
    /**
     * @brief called when owner #ZFUISysWindow is about to destroy
     */
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow) {
    }

    /**
     * @brief called to add root view to native window container
     *
     * if nativeParentView was specified,
     * it would be used to copy some settings to the window's root view
     */
    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) zfpurevirtual;
    /**
     * @brief called to remove root view to native window container
     */
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow) zfpurevirtual;

    /**
     * @brief see #ZFUISysWindow::modalWindowShow
     */
    virtual zfautoT<ZFUISysWindow> modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner) zfpurevirtual;
    /**
     * @brief see #ZFUISysWindow::modalWindowFinish
     */
    virtual void modalWindowFinish(
            ZF_IN ZFUISysWindow *sysWindowOwner
            , ZF_IN ZFUISysWindow *sysWindowToFinish
            ) zfpurevirtual;

    /**
     * @brief called to update suggested window layout param,
     *   fill with no margin by default
     */
    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow) {
    }
    /**
     * @brief called when window layout param changed
     */
    virtual void sysWindowLayoutParamOnUpdate(ZF_IN ZFUISysWindow *sysWindow) {
    }

    /**
     * @brief see #ZFUISysWindow::sysWindowOrientation
     */
    virtual ZFUIOrientation sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow) {
        return v_ZFUIOrientation::e_Top;
    }
    /**
     * @brief see #ZFUISysWindow::sysWindowOrientationFlags,
     *  impl should have #v_ZFUIOrientation::e_Top as init value
     */
    virtual void sysWindowOrientationFlags(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) {
    }

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must call this method to measure window's frame,
     *   and layout window using the result frame
     *
     * if you are embedding ZFUISysWindow to native view with custom layout logic,
     * you may skip this method
     */
    zffinal ZFUIRect notifyMeasureWindow(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN const ZFUIRect &rootRefRect
            , ZF_IN const ZFUIMargin &sysWindowMargin
            ) {
        sysWindow->_ZFP_ZFUISysWindow_sysWindowMargin(sysWindowMargin);
        return sysWindow->_ZFP_ZFUISysWindow_measureWindow(rootRefRect);
    }
    /**
     * @brief implementation must call this to notify after window create
     */
    zffinal void notifyOnCreate(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN void *nativeWindow
            ) {
        sysWindow->_ZFP_ZFUISysWindow_onCreate(nativeWindow);
    }
    /**
     * @brief implementation must call this to notify before window destroy
     */
    zffinal void notifyOnDestroy(ZF_IN ZFUISysWindow *sysWindow) {
        sysWindow->_ZFP_ZFUISysWindow_onDestroy();
    }
    /**
     * @brief implementation must call this to notify after window resume,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnResume(ZF_IN ZFUISysWindow *sysWindow) {
        sysWindow->_ZFP_ZFUISysWindow_onResume();
    }
    /**
     * @brief implementation must call this to notify before window pause,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnPause(ZF_IN ZFUISysWindow *sysWindow) {
        sysWindow->_ZFP_ZFUISysWindow_onPause();
    }
    /**
     * @brief implementation must call this to notify when window rotate
     */
    zffinal void notifyOnRotate(ZF_IN ZFUISysWindow *sysWindow) {
        sysWindow->_ZFP_ZFUISysWindow_onRotate();
    }
    /**
     * @brief implementation must call this to notify key event
     */
    zffinal void notifyKeyEvent(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN ZFUIKeyEvent *event
            ) {
        sysWindow->_ZFP_ZFUISysWindow_keyEvent(event);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUISysWindow_h_

