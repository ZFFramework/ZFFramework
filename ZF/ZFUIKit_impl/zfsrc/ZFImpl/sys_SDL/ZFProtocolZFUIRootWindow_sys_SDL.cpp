#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIRootWindow.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFIMPL_SYS_SDL_USER_EVENT_HANDLER(RootWindowResume, ZFLevelZFFrameworkPostNormal) {
    zfweakT<ZFUIRootWindow> *rootWindowHolder = (zfweakT<ZFUIRootWindow> *)sdlEvent->user.data1;
    ZFUIRootWindow *rootWindow = *rootWindowHolder;
    if(rootWindow && !rootWindow->nativeWindowIsResumed()) {
        ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnResume(rootWindow);
    }
    zfdelete(rootWindowHolder);
    return zftrue;
}

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIRootWindowImpl_sys_SDL, ZFUIRootWindow, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_RootWindow")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void) {
        this->mainWindowOnCleanup();
        zfsuper::protocolOnDeallocPrepare();
    }
public:
    virtual ZFUIRootWindow *mainWindow(void) {
        if(this->_mainWindow == zfnull) {
            this->_mainWindow = zfRetain(ZFUIRootWindow::ClassData()->newInstance().to<ZFUIRootWindow *>());
            ZFImpl_sys_SDL_RootWindow *nativeWindow = zfnew(ZFImpl_sys_SDL_RootWindow);
            nativeWindow->ownerZFUIRootWindow = this->_mainWindow;
            nativeWindow->builtinWindow = zftrue;
            nativeWindow->sdlWindow = ZFImpl_sys_SDL_mainWindow();
            nativeWindow->sdlRenderer = ZFImpl_sys_SDL_mainRenderer();
            this->notifyOnCreate(this->_mainWindow, nativeWindow);

            ZFIMPL_SYS_SDL_USER_EVENT_POST(RootWindowResume, zfnew(zfweakT<ZFUIRootWindow>, this->_mainWindow), zfnull);
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void) {
        if(this->_mainWindow != zfnull) {
            ZFImpl_sys_SDL_RootWindow *nativeWindow = (ZFImpl_sys_SDL_RootWindow *)this->_mainWindow->nativeWindow();
            if(this->_mainWindow->nativeWindowIsResumed()) {
                this->notifyOnPause(this->_mainWindow);
            }
            this->notifyOnDestroy(this->_mainWindow);
            zfscopeRelease(this->_mainWindow);
            this->_mainWindow = zfnull;
            zfdelete(nativeWindow);
        }
    }
    virtual void mainWindowOnDestroy(void) {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUIRootWindow *rootWindow) {
    }

    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_RootWindow *nativeWindow = (ZFImpl_sys_SDL_RootWindow *)rootWindow->nativeWindow();
        nativeWindow->rootView = (ZFImpl_sys_SDL_View *)rootWindow->rootView()->nativeView();
        nativeWindow->rootView->rootWindowAttach(nativeWindow);
        nativeWindow->renderStart();
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_RootWindow *nativeWindow = (ZFImpl_sys_SDL_RootWindow *)rootWindow->nativeWindow();
        nativeWindow->rootView->rootWindowDetach();
        nativeWindow->renderStop();
        nativeWindow->rootView = zfnull;
    }

    virtual zfauto modalWindowShow(ZF_IN ZFUIRootWindow *owner) {
        if(owner->nativeWindow() == zfnull) {
            return zfnull;
        }
        zfauto modalWindow = ZFUIRootWindow::ClassData()->newInstance();
        ZFImpl_sys_SDL_RootWindow *nativeWindow = zfnew(ZFImpl_sys_SDL_RootWindow);
        nativeWindow->ownerZFUIRootWindow = this->_mainWindow;
        nativeWindow->builtinWindow = zffalse;
        nativeWindow->sdlWindow = ZFImpl_sys_SDL_CreateWindow();
        ZFImpl_sys_SDL_WindowNotifyCreate(nativeWindow->sdlWindow);
        ZFCoreAssert(nativeWindow->sdlWindow != zfnull);
        nativeWindow->sdlRenderer = SDL_CreateRenderer(nativeWindow->sdlWindow, zfnull);
        ZFImpl_sys_SDL_RendererNotifyCreate(nativeWindow->sdlRenderer);
        ZFCoreAssert(nativeWindow->sdlRenderer != zfnull);
        SDL_SetRenderDrawBlendMode(nativeWindow->sdlRenderer, SDL_BLENDMODE_BLEND);
        this->notifyOnCreate(modalWindow, nativeWindow);

        ZFIMPL_SYS_SDL_USER_EVENT_POST(RootWindowResume, zfnew(zfweakT<ZFUIRootWindow>, modalWindow), zfnull);
        return modalWindow;
    }
    virtual void modalWindowHide(
            ZF_IN ZFUIRootWindow *owner
            , ZF_IN ZFUIRootWindow *toHide
            ) {
        if(toHide->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_RootWindow *nativeWindow = (ZFImpl_sys_SDL_RootWindow *)toHide->nativeWindow();
        if(toHide->nativeWindowIsResumed()) {
            this->notifyOnPause(toHide);
        }
        this->notifyOnDestroy(toHide);
        zfdelete(nativeWindow);
    }

    virtual void layoutParamOnInit(ZF_IN ZFUIRootWindow *rootWindow) {
        ZFUILayoutParam *lp = rootWindow->layoutParam();
        lp->sizeFill(960, 720);
        lp->alignCenter();
    }
    virtual void layoutParamOnUpdate(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_RootWindow *nativeWindow = (ZFImpl_sys_SDL_RootWindow *)rootWindow->nativeWindow();
        ZFUILayoutParam *lp = rootWindow->layoutParam();
        zffloat UIScale = (zffloat)SDL_GetWindowDisplayScale(nativeWindow->sdlWindow);
        SDL_Rect sdlRect;
        if(!SDL_GetDisplayUsableBounds(SDL_GetDisplayForWindow(nativeWindow->sdlWindow), &sdlRect)) {
            sdlRect.x = 0;
            sdlRect.y = 0;
            sdlRect.w = 640;
            sdlRect.h = 480;
        }
        ZFUIRect rect = this->notifyMeasureWindow(
                rootWindow
                , ZFUIRectCreate(sdlRect.x, sdlRect.y, sdlRect.w, sdlRect.h)
                , ZFUIMarginZero()
                );
        SDL_SetWindowPosition(nativeWindow->sdlWindow, (int)rect.x, (int)rect.y);
        SDL_SetWindowSize(nativeWindow->sdlWindow, (int)rect.width, (int)rect.height);

        // setting a windows size that exceeds screen size, may cause sdl motion event's x/y to be incorrect
        if(zffalse
                || lp->sizeParam().width == ZFUISizeType::e_Fill
                || lp->sizeParam().height == ZFUISizeType::e_Fill
                ) {
            int w = 0;
            int h = 0;
            SDL_GetWindowSize(nativeWindow->sdlWindow, &w, &h);
            SDL_SetWindowSize(nativeWindow->sdlWindow, w - 10, h - 10);
            SDL_SetWindowSize(nativeWindow->sdlWindow, w, h);
        }

        if(rootWindow->preferFullscreen()) {
            SDL_SetWindowFullscreen(nativeWindow->sdlWindow, SDL_WINDOW_FULLSCREEN);
        }
        else {
            SDL_SetWindowFullscreen(nativeWindow->sdlWindow, 0);
        }
    }

    virtual ZFUIOrientation windowOrientation(ZF_IN ZFUIRootWindow *rootWindow) {
        return v_ZFUIOrientation::e_Top;
    }
    virtual void windowOrientationFlags(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) {
    }

private:
    ZFUIRootWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIRootWindowImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

