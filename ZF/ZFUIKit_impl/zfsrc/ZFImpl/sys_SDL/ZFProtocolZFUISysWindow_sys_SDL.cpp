#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFIMPL_SYS_SDL_USER_EVENT_HANDLER(SysWindowResume, ZFLevelZFFrameworkPostNormal) {
    zfweakT<ZFUISysWindow> *sysWindowHolder = (zfweakT<ZFUISysWindow> *)sdlEvent->user.data1;
    ZFUISysWindow *sysWindow = *sysWindowHolder;
    if(sysWindow && !sysWindow->nativeWindowIsResumed()) {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnResume(sysWindow);
    }
    zfdelete(sysWindowHolder);
    return zftrue;
}

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_SDL, ZFUISysWindow, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_SysWindow")
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
    virtual ZFUISysWindow *mainWindow(void) {
        if(this->_mainWindow == zfnull) {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());
            ZFImpl_sys_SDL_SysWindow *nativeWindow = zfnew(ZFImpl_sys_SDL_SysWindow);
            nativeWindow->ownerZFUISysWindow = this->_mainWindow;
            nativeWindow->builtinWindow = zftrue;
            nativeWindow->sdlWindow = ZFImpl_sys_SDL_mainWindow();
            nativeWindow->sdlRenderer = ZFImpl_sys_SDL_mainRenderer();
            this->notifyOnCreate(this->_mainWindow, nativeWindow);

            ZFIMPL_SYS_SDL_USER_EVENT_POST(SysWindowResume, zfnew(zfweakT<ZFUISysWindow>, this->_mainWindow), zfnull);
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void) {
        if(this->_mainWindow != zfnull) {
            ZFImpl_sys_SDL_SysWindow *nativeWindow = (ZFImpl_sys_SDL_SysWindow *)this->_mainWindow->nativeWindow();
            if(this->_mainWindow->nativeWindowIsResumed()) {
                this->notifyOnPause(this->_mainWindow);
            }
            this->notifyOnDestroy(this->_mainWindow);
            zfblockedRelease(this->_mainWindow);
            this->_mainWindow = zfnull;
            zfdelete(nativeWindow);
        }
    }
    virtual void mainWindowOnDestroy(void) {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow) {
    }

    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        if(sysWindow->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_SysWindow *nativeWindow = (ZFImpl_sys_SDL_SysWindow *)sysWindow->nativeWindow();
        nativeWindow->rootView = (ZFImpl_sys_SDL_View *)sysWindow->rootView()->nativeView();
        nativeWindow->rootView->sysWindowAttach(nativeWindow);
        nativeWindow->renderStart();
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow) {
        if(sysWindow->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_SysWindow *nativeWindow = (ZFImpl_sys_SDL_SysWindow *)sysWindow->nativeWindow();
        nativeWindow->rootView->sysWindowDetach();
        nativeWindow->renderStop();
        nativeWindow->rootView = zfnull;
    }

    virtual zfauto modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner) {
        if(sysWindowOwner->nativeWindow() == zfnull) {
            return zfnull;
        }
        zfauto modalWindow = ZFUISysWindow::ClassData()->newInstance();
        ZFImpl_sys_SDL_SysWindow *nativeWindow = zfnew(ZFImpl_sys_SDL_SysWindow);
        nativeWindow->ownerZFUISysWindow = this->_mainWindow;
        nativeWindow->builtinWindow = zffalse;
        nativeWindow->sdlWindow = ZFImpl_sys_SDL_CreateWindow();
        ZFCoreAssert(nativeWindow->sdlWindow != zfnull);
        nativeWindow->sdlRenderer = SDL_CreateRenderer(nativeWindow->sdlWindow, -1, 0
                | SDL_RENDERER_ACCELERATED
                | SDL_RENDERER_TARGETTEXTURE
            );
        ZFCoreAssert(nativeWindow->sdlRenderer != zfnull);
        SDL_SetRenderDrawBlendMode(nativeWindow->sdlRenderer, SDL_BLENDMODE_BLEND);
        this->notifyOnCreate(modalWindow, nativeWindow);

        ZFIMPL_SYS_SDL_USER_EVENT_POST(SysWindowResume, zfnew(zfweakT<ZFUISysWindow>, modalWindow), zfnull);
        return modalWindow;
    }
    virtual void modalWindowFinish(
            ZF_IN ZFUISysWindow *sysWindowOwner
            , ZF_IN ZFUISysWindow *sysWindowToFinish
            ) {
        if(sysWindowToFinish->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_SysWindow *nativeWindow = (ZFImpl_sys_SDL_SysWindow *)sysWindowToFinish->nativeWindow();
        if(sysWindowToFinish->nativeWindowIsResumed()) {
            this->notifyOnPause(sysWindowToFinish);
        }
        this->notifyOnDestroy(sysWindowToFinish);
        zfdelete(nativeWindow);
    }

    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow) {
        SDL_Rect sdlRect;
        SDL_GetDisplayUsableBounds(0, &sdlRect);
        sysWindow->sysWindowLayoutParam()->sizeHint(ZFUISizeCreate(sdlRect.w, sdlRect.h));
    }
    virtual void sysWindowLayoutParamOnUpdate(ZF_IN ZFUISysWindow *sysWindow) {
        if(sysWindow->nativeWindow() == zfnull) {
            return;
        }
        ZFImpl_sys_SDL_SysWindow *nativeWindow = (ZFImpl_sys_SDL_SysWindow *)sysWindow->nativeWindow();
        SDL_Rect sdlRect;
        SDL_GetDisplayUsableBounds(0, &sdlRect);
        ZFUIRect rect;
        ZFUILayoutParam::layoutParamApplyT(
                rect
                , ZFUIRectCreate(sdlRect.x, sdlRect.y, sdlRect.w, sdlRect.h)
                , sysWindow->rootView()
                , sysWindow->sysWindowLayoutParam()
            );
        SDL_SetWindowPosition(nativeWindow->sdlWindow, (int)rect.x, (int)rect.y);
        SDL_SetWindowSize(nativeWindow->sdlWindow, (int)rect.width, (int)rect.height);

        // setting a windows size that exceeds screen size, may cause sdl motion event's x/y to be incorrect
        if(zffalse
                || sysWindow->sysWindowLayoutParam()->sizeParam().width == ZFUISizeType::e_Fill
                || sysWindow->sysWindowLayoutParam()->sizeParam().height == ZFUISizeType::e_Fill
                ) {
            int w = 0;
            int h = 0;
            SDL_GetWindowSize(nativeWindow->sdlWindow, &w, &h);
            SDL_SetWindowSize(nativeWindow->sdlWindow, w - 10, h - 10);
            SDL_SetWindowSize(nativeWindow->sdlWindow, w, h);
        }

        if(sysWindow->preferFullscreen()) {
            SDL_SetWindowFullscreen(nativeWindow->sdlWindow, SDL_WINDOW_FULLSCREEN);
        }
        else {
            SDL_SetWindowFullscreen(nativeWindow->sdlWindow, 0);
        }
    }

    virtual ZFUIOrientation sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow) {
        return v_ZFUIOrientation::e_Top;
    }
    virtual void sysWindowOrientationFlags(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) {
    }

private:
    ZFUISysWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

