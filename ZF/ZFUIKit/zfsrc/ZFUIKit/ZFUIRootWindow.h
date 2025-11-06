/**
 * @file ZFUIRootWindow.h
 * @brief root native window
 */

#ifndef _ZFI_ZFUIRootWindow_h_
#define _ZFI_ZFUIRootWindow_h_

#include "ZFUIRootView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFUIRootWindow::mainWindow has attached,
 * either by access #ZFUIRootWindow::mainWindow,
 * or by #ZFUIRootWindow::mainWindowRegister
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, MainWindowOnAttach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfclassFwd ZFUIRootWindowEmbedImpl;
zfclassFwd _ZFP_ZFUIRootWindowPrivate;
/**
 * @brief root native window
 *
 * use #mainWindow to access app's main window,
 * use #modalWindowShow to show a new window,
 * use #nativeWindowEmbed to attach to native impl,
 * never create #ZFUIRootWindow's instance manually
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUIRootWindow : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIRootWindow, ZFObject)

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window created,
     * see #nativeWindowIsCreated for more info
     */
    ZFEVENT(WindowOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window about to destroy
     */
    ZFEVENT(WindowOnDestroy)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window become active
     */
    ZFEVENT(WindowOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window enter background
     */
    ZFEVENT(WindowOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window rotated,
     * new orientation can be accessed by #windowOrientation
     */
    ZFEVENT(WindowOnRotate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when #windowMargin changed,
     * param0 is a #v_ZFUIMargin that holds the old window margin
     */
    ZFEVENT(WindowMarginOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when #ZFUIKeyEvent fired to the window,
     * and none of it's view handled the event by #ZFUIEvent::eventResolved\n
     * param0 is the #ZFUIKeyEvent
     */
    ZFEVENT(WindowOnKeyEvent)

public:
    /**
     * @brief used to embed #ZFUIRootWindow to existing UI framework
     *
     * #ZFUIRootWindow would create internal native window by default,
     * however, you may create your own native window and attach #ZFUIRootWindow to it\n
     * to achieve this feature,
     * you must supply custom embed impl by #ZFUIRootWindowEmbedImpl,
     * call this method to get an attached #ZFUIRootWindow,
     * and manually manage the life cycle of the returned #ZFUIRootWindow\n
     * \n
     * the embedImpl would be retained until the created window destroy,
     * so that you only need to take care of the created window,
     * to explicitly clear all the contents,
     * simply use `window->nativeWindowEmbedImplDestroy()`\n
     * \n
     * see #ZFUIRootWindowEmbedImpl for all the impl that you need to implement
     */
    static zfautoT<ZFUIRootWindow> nativeWindowEmbed(ZF_IN ZFUIRootWindowEmbedImpl *embedImpl);

    /**
     * @brief see #nativeWindowEmbed
     */
    virtual ZFUIRootWindowEmbedImpl *nativeWindowEmbedImpl(void);

    /**
     * @brief util to destroy the window created by #nativeWindowEmbed
     *
     * equal to #ZFUIRootWindowEmbedImpl::notifyOnPause
     * + #ZFUIRootWindowEmbedImpl::notifyOnDestroy
     */
    ZFMETHOD_DECLARE_0(void, nativeWindowEmbedImplDestroy)

    /**
     * @brief default impl to attach window to native view
     *
     * note, after attach, you must destroy the window by #nativeWindowEmbedImplDestroy
     * before destroy the nativeParent\n
     * the nativeParent must support add one or more child which fill itself
     */
    ZFMETHOD_DECLARE_STATIC_1(zfautoT<ZFUIRootWindow>, nativeWindowEmbedNativeView
            , ZFMP_IN(void *, nativeParent)
            )

    /**
     * @brief util to attach window to native view,
     *   and register a getter method with specified name
     *
     * usage:
     * @code
     *   zfauto rootWindow = ZFUIRootWindow::nativeWindowEmbedNativeView(nativeParent, "MyWindow");
     *   if(rootWindow != zfnull) {
     *       zfobj<ZFUIWindow> window(ZFInvoke("MyWindow"));
     *       ...
     *   }
     * @endcode
     */
    ZFMETHOD_DECLARE_STATIC_2(zfautoT<ZFUIRootWindow>, nativeWindowEmbedNativeView
            , ZFMP_IN(void *, nativeParent)
            , ZFMP_IN(const zfstring &, rootWindowName)
            )
    /**
     * @brief remove window attached by #nativeWindowEmbedNativeView
     */
    ZFMETHOD_DECLARE_STATIC_1(void, nativeWindowEmbedNativeViewCleanup
            , ZFMP_IN(const zfstring &, rootWindowName)
            )

public:
    /**
     * @brief manually register main window, must be called before accessing #mainWindow
     *
     * this method is useful if you want to embed whole ZFFramework to existing UI framework,
     * to create a custom #ZFUIRootWindow, see #nativeWindowEmbed\n
     * @note you must manually manage the life cycle of the registered #ZFUIRootWindow
     * @note once registered, it can not be changed back to original internal window,
     *   until #ZFFrameworkCleanup
     * @note it's safe to unregister and register a new window,
     *   but you must ensure you won't access the children of the window after you do so
     */
    ZFMETHOD_DECLARE_STATIC_1(void, mainWindowRegister
            , ZFMP_IN(ZFUIRootWindow *, window)
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
    ZFMETHOD_DECLARE_STATIC_0(zfanyT<ZFUIRootWindow>, mainWindow)
    /**
     * @brief whether #mainWindow has attached
     */
    ZFMETHOD_DECLARE_STATIC_0(zfbool, mainWindowAttached)

    /**
     * @brief default window that #ZFUIWindow would attach to, set null to use #mainWindow
     */
    ZFMETHOD_DECLARE_STATIC_1(void, keyWindow
            , ZFMP_IN(ZFUIRootWindow *, window)
            )
    /**
     * @brief see #keyWindow, return #mainWindow if not set
     */
    ZFMETHOD_DECLARE_STATIC_0(zfanyT<ZFUIRootWindow>, keyWindow)

public:
    /**
     * @brief native window's size
     *
     * to change window's size or layout,
     * use #layoutParam\n
     * to observe window size change,
     * attach #ZFUIView::E_ViewLayoutOnLayoutFinish listener to #rootView
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, windowSize)

public:
    /**
     * @brief native window margin according to impl
     *
     * the margin usually used for mobile phones,
     * whose edge contains virtual button or front camera\n
     * by default, #ZFUIWindow would automatically exclude the margin,
     * you may change it by #ZFUIWindow::windowMarginShouldApply
     */
    ZFMETHOD_DECLARE_0(const ZFUIMargin &, windowMargin)
    void _ZFP_ZFUIRootWindow_windowMargin(ZF_IN const ZFUIMargin &windowMargin);
protected:
    /**
     * @brief see #E_WindowMarginOnUpdate
     */
    virtual void windowMarginOnUpdate(ZF_IN const ZFUIMargin &windowMarginOld);

    ZFOBJECT_PRIVATE_ALLOC("can only be created by ZFUIRootWindow::modalWindowShow")
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
     * #E_WindowOnCreate\n
     * note, this value can be null if window created by #nativeWindowEmbed
     */
    ZFMETHOD_DECLARE_0(void *, nativeWindow)

public:
    /**
     * @brief true if sys window created
     *
     * @note for some implementation,
     *   creating a ZFUIRootWindow may or may not create native window immediately,
     *   you may use this method as well as #E_WindowOnCreate to see when did window created,
     *   typical usage:
     *   @code
     *       ZFUIRootWindow *newWindow = ZFUIRootWindow::mainWindow()->modalWindowShow();
     *       if(!newWindow->nativeWindowIsCreated()) {
     *           ZFListener observer = ... {
     *               initWindow();
     *           };
     *           newWindow->observerAdd(ZFUIRootWindow::E_WindowOnCreate(), observer);
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
    ZFMETHOD_DECLARE_0(ZFUIOrientation, windowOrientation)
    /**
     * @brief set supported orientation, orientation may or may not change immediately
     */
    ZFMETHOD_DECLARE_1(void, windowOrientationFlags
            , ZFMP_IN(const ZFUIOrientationFlags &, windowOrientationFlags)
            )
    /**
     * @brief get supported orientation
     */
    ZFMETHOD_DECLARE_0(const ZFUIOrientationFlags &, windowOrientationFlags)

    /**
     * @brief whether prefer fullscreen
     */
    ZFPROPERTY_ASSIGN(zfbool, preferFullscreen, zffalse)
    ZFPROPERTY_ON_UPDATE_DECLARE(zfbool, preferFullscreen)

public:
    /**
     * @brief show a new modal window,
     *   may or may not show immediately,
     *   may have different behavior on different implementations
     *
     * use only if necessary\n
     * it's recommended that app have only one ZFUIRootWindow,
     * use #ZFUIWindow if necessary
     */
    ZFMETHOD_DECLARE_0(zfautoT<ZFUIRootWindow>, modalWindowShow)
    /**
     * @brief finish and destroy self as a modal window
     */
    ZFMETHOD_DECLARE_0(void, modalWindowHide)
    /**
     * @brief get self's current showing modal window
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIRootWindow>, modalWindowShowing)
    /**
     * @brief get self's owner if self is a showing modal window
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIRootWindow>, modalWindowOwner)

public:
    /**
     * @brief get window's layout param, fill parent by default
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUILayoutParam>, layoutParam)

    /**
     * @brief access the root view of the window
     *
     * the root view is ensured #ZFUIRootView type,
     * and you must not add it to another view
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIRootView>, rootView)

public:
    zffinal ZFUIRect _ZFP_ZFUIRootWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect);
    zffinal void _ZFP_ZFUIRootWindow_onCreate(ZF_IN void *nativeWindow);
    zffinal void _ZFP_ZFUIRootWindow_onDestroy(void);
    zffinal void _ZFP_ZFUIRootWindow_onResume(void);
    zffinal void _ZFP_ZFUIRootWindow_onPause(void);
    zffinal void _ZFP_ZFUIRootWindow_onRotate(void);
    zffinal void _ZFP_ZFUIRootWindow_layoutUpdate(void);
    zffinal void _ZFP_ZFUIRootWindow_keyEvent(ZF_IN ZFUIKeyEvent *event) {
        this->observerNotify(ZFUIRootWindow::E_WindowOnKeyEvent(), event);
    }

private:
    _ZFP_ZFUIRootWindowPrivate *d;
};

// ============================================================
/**
 * @brief impl for #ZFUIRootWindow::nativeWindowEmbed
 */
zfinterface ZFLIB_ZFUIKit ZFUIRootWindowEmbedImpl : zfextend ZFInterface {
    ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(ZFUIRootWindowEmbedImpl, ZFInterface)

public:
    zfanyT<ZFUIRootWindow> _ZFP_ownerZFUIRootWindow;
    /**
     * @brief the attached #ZFUIRootWindow
     */
    inline zfanyT<ZFUIRootWindow> const &ownerZFUIRootWindow(void) {
        return this->_ZFP_ownerZFUIRootWindow;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFUIRootWindowEmbedImpl(void) : _ZFP_ownerZFUIRootWindow(zfnull) {}
    /** @endcond */

    // ============================================================
public:
    /**
     * @brief called when owner #ZFUIRootWindow is about to destroy
     */
    virtual void nativeWindowOnCleanup(ZF_IN ZFUIRootWindow *rootWindow) {
    }

    /**
     * @brief called to add root view to native window container
     *
     * if nativeParentView was specified,
     * it would be used to copy some settings to the window's root view
     */
    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) zfpurevirtual;
    /**
     * @brief called to remove root view to native window container
     */
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUIRootWindow *rootWindow) zfpurevirtual;

    /**
     * @brief see #ZFUIRootWindow::modalWindowShow
     */
    virtual zfautoT<ZFUIRootWindow> modalWindowShow(ZF_IN ZFUIRootWindow *owner) zfpurevirtual;
    /**
     * @brief see #ZFUIRootWindow::modalWindowHide
     */
    virtual void modalWindowHide(
            ZF_IN ZFUIRootWindow *owner
            , ZF_IN ZFUIRootWindow *toHide
            ) zfpurevirtual;

    /**
     * @brief called to update suggested window layout param,
     *   fill with no margin by default
     */
    virtual void layoutParamOnInit(ZF_IN ZFUIRootWindow *rootWindow) {
    }
    /**
     * @brief called when window layout param changed
     */
    virtual void layoutParamOnUpdate(ZF_IN ZFUIRootWindow *rootWindow) {
    }

    /**
     * @brief see #ZFUIRootWindow::windowOrientation
     */
    virtual ZFUIOrientation windowOrientation(ZF_IN ZFUIRootWindow *rootWindow) {
        return v_ZFUIOrientation::e_Top;
    }
    /**
     * @brief see #ZFUIRootWindow::windowOrientationFlags,
     *  impl should have #v_ZFUIOrientation::e_Top as init value
     */
    virtual void windowOrientationFlags(
            ZF_IN ZFUIRootWindow *rootWindow
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
     * if you are embedding ZFUIRootWindow to native view with custom layout logic,
     * you may skip this method
     */
    zffinal ZFUIRect notifyMeasureWindow(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN const ZFUIRect &rootRefRect
            , ZF_IN const ZFUIMargin &windowMargin
            ) {
        rootWindow->_ZFP_ZFUIRootWindow_windowMargin(windowMargin);
        return rootWindow->_ZFP_ZFUIRootWindow_measureWindow(rootRefRect);
    }
    /**
     * @brief implementation must call this to notify after window create
     */
    zffinal void notifyOnCreate(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN void *nativeWindow
            ) {
        rootWindow->_ZFP_ZFUIRootWindow_onCreate(nativeWindow);
    }
    /**
     * @brief implementation must call this to notify before window destroy
     */
    zffinal void notifyOnDestroy(ZF_IN ZFUIRootWindow *rootWindow) {
        rootWindow->_ZFP_ZFUIRootWindow_onDestroy();
    }
    /**
     * @brief implementation must call this to notify after window resume,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnResume(ZF_IN ZFUIRootWindow *rootWindow) {
        rootWindow->_ZFP_ZFUIRootWindow_onResume();
    }
    /**
     * @brief implementation must call this to notify before window pause,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnPause(ZF_IN ZFUIRootWindow *rootWindow) {
        rootWindow->_ZFP_ZFUIRootWindow_onPause();
    }
    /**
     * @brief implementation must call this to notify when window rotate
     */
    zffinal void notifyOnRotate(ZF_IN ZFUIRootWindow *rootWindow) {
        rootWindow->_ZFP_ZFUIRootWindow_onRotate();
    }
    /**
     * @brief implementation must call this to notify key event
     */
    zffinal void notifyKeyEvent(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN ZFUIKeyEvent *event
            ) {
        rootWindow->_ZFP_ZFUIRootWindow_keyEvent(event);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIRootWindow_h_

