/**
 * @file ZFProtocolZFUIRootWindow.h
 * @brief protocol for ZFUIRootWindow
 */

#ifndef _ZFI_ZFProtocolZFUIRootWindow_h_
#define _ZFI_ZFProtocolZFUIRootWindow_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUIRootWindow.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIRootWindow
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIRootWindow)
    // ============================================================
    // main window impl
public:
    /**
     * @brief see #ZFUIRootWindow::mainWindow
     *
     * for implementations,
     * you should create a new window if not initialized,
     * or return the created one if exist
     */
    virtual ZFUIRootWindow *mainWindow(void) zfpurevirtual;
    /**
     * @brief called to cleanup native main window
     *
     * this is ensured to be called during #ZFFrameworkCleanup
     * with level #ZFLevelZFFrameworkLow\n
     * after cleanup, you must ensured it's reentrant to access the main window again
     */
    virtual void mainWindowOnCleanup(void) zfpurevirtual;
    /**
     * @brief called when main window destroyed by #notifyOnDestroy
     */
    virtual void mainWindowOnDestroy(void) zfpurevirtual;

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
    virtual zfauto modalWindowShow(ZF_IN ZFUIRootWindow *owner) zfpurevirtual;
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
    virtual ZFUIOrientation windowOrientation(ZF_IN ZFUIRootWindow *rootWindow) zfpurevirtual;
    /**
     * @brief see #ZFUIRootWindow::windowOrientationFlags,
     *  impl should have #v_ZFUIOrientation::e_Top as init value
     */
    virtual void windowOrientationFlags(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) zfpurevirtual;

    /**
     * @brief called when #ZFUIRootWindow::windowColor changed
     */
    virtual void windowColor(ZF_IN ZFUIRootWindow *rootWindow) {
        rootWindow->rootView()->bgColor(rootWindow->windowColor());
    }

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must call this method to measure window's frame,
     *   and layout window using the result frame
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
ZFPROTOCOL_INTERFACE_END(ZFUIRootWindow)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIRootWindow_h_

