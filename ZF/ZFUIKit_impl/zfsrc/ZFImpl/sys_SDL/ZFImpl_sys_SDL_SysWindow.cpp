#include "ZFImpl_sys_SDL_SysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCoreArray<ZFImpl_sys_SDL_SysWindow *> &ZFImpl_sys_SDL_SysWindow::allWindow(void) {
    static ZFCoreArray<ZFImpl_sys_SDL_SysWindow *> l;
    return l;
}

zfclassNotPOD _ZFP_ZFImpl_sys_SDL_SysWindowPrivate {
public:
    zfuint refCount;
    ZFImpl_sys_SDL_SysWindow *sysWindow;
    ZFListener renderCallback;
    zfbool renderStarted;
    zfbool renderRequested;
    zfbool layoutRequested;

    zfstlmap<zfidentity, ZFImpl_sys_SDL_MouseState> mouseState;

public:
    _ZFP_ZFImpl_sys_SDL_SysWindowPrivate(ZF_IN ZFImpl_sys_SDL_SysWindow *sysWindow)
    : refCount(1)
    , sysWindow(sysWindow)
    , renderCallback()
    , renderStarted(zffalse)
    , renderRequested(zftrue)
    , layoutRequested(zftrue)
    , mouseState()
    {
    }
};

ZFIMPL_SYS_SDL_USER_EVENT_HANDLER(SysWindowRender, ZFLevelZFFrameworkPostNormal) {
    _ZFP_ZFImpl_sys_SDL_SysWindowPrivate *d = (_ZFP_ZFImpl_sys_SDL_SysWindowPrivate *)sdlEvent->user.data1;
    if(d->sysWindow == zfnull) {
        return zftrue;
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl = ZFPROTOCOL_ACCESS(ZFUIView);
    if(d->layoutRequested) {
        d->layoutRequested = zffalse;
        if(d->sysWindow->rootView != zfnull && d->sysWindow->rootView->ownerZFUIView != zfnull) {
            int w, h;
            SDL_GetWindowSize(d->sysWindow->sdlWindow, &w, &h);
            impl->notifyLayoutView(d->sysWindow->rootView->ownerZFUIView, ZFUIRectMake(0, 0, w, h));
        }
    }
    if(d->renderRequested) {
        d->renderRequested = zffalse;
        if(d->sysWindow->rootView != zfnull) {
            SDL_SetRenderDrawColor(d->sysWindow->sdlRenderer, 255, 255, 255, 255);
            SDL_RenderClear(d->sysWindow->sdlRenderer);
            d->sysWindow->rootView->render(d->sysWindow->sdlRenderer, d->sysWindow->rootView->rect, d->sysWindow->rootView->rect, 1);
            SDL_RenderPresent(d->sysWindow->sdlRenderer);
        }
    }
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    return zftrue;
}

ZFImpl_sys_SDL_SysWindow::ZFImpl_sys_SDL_SysWindow(void)
: d(zfnew(_ZFP_ZFImpl_sys_SDL_SysWindowPrivate, this))
, ownerZFUISysWindow(zfnull)
, builtinWindow(zffalse)
, sdlWindow(zfnull)
, sdlRenderer(zfnull)
, rootView(zfnull)
, viewFocused(zfnull)
, mouseIdGen()
{
    allWindow().add(this);

    ZFLISTENER_1(renderCallback
            , _ZFP_ZFImpl_sys_SDL_SysWindowPrivate *, d
            ) {
        if(d->layoutRequested || d->renderRequested) {
            if(d->refCount == 1) {
                ++(d->refCount);
                ZFIMPL_SYS_SDL_USER_EVENT_POST(SysWindowRender, d, zfnull);
            }
        }
    } ZFLISTENER_END()
    d->renderCallback = renderCallback;
}
ZFImpl_sys_SDL_SysWindow::~ZFImpl_sys_SDL_SysWindow(void) {
    allWindow().removeElement(this);

    this->renderStop();
    if(this->builtinWindow) {
        if(this->sdlRenderer != zfnull) {
            SDL_DestroyRenderer(this->sdlRenderer);
            this->sdlRenderer = zfnull;
        }
        if(this->sdlWindow != zfnull) {
            SDL_DestroyWindow(this->sdlWindow);
            this->sdlWindow = zfnull;
        }
    }

    d->sysWindow = zfnull;
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}

// ============================================================
void ZFImpl_sys_SDL_SysWindow::renderStart(void) {
    if(d->renderStarted) {
        return;
    }
    d->renderStarted = zftrue;
    ZFGlobalTimerAttach(d->renderCallback, ZFLevelZFFrameworkPostNormal);
}
void ZFImpl_sys_SDL_SysWindow::renderStop(void) {
    if(!d->renderStarted) {
        return;
    }
    d->renderStarted = zffalse;
    ZFGlobalTimerDetach(d->renderCallback);
}

void ZFImpl_sys_SDL_SysWindow::renderRequest(void) {
    d->renderRequested = zftrue;
}

void ZFImpl_sys_SDL_SysWindow::layoutRequest(void) {
    d->layoutRequested = zftrue;
    d->renderRequested = zftrue;
}

// ============================================================
void ZFImpl_sys_SDL_SysWindow::viewFocusChange(ZF_IN ZFImpl_sys_SDL_View *view) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *focusImpl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    zfCoreAssert(focusImpl != zfnull);
    if(view == zfnull) {
        if(this->viewFocused != zfnull) {
            ZFUIView *owner = this->viewFocused->ownerZFUIView;
            this->viewFocused = zfnull;
            if(owner != zfnull) {
                if(focusImpl != zfnull) {
                    focusImpl->notifyViewFocusChanged(owner);
                }
            }
        }
        return;
    }

    while(view != zfnull && (view->ownerZFUIView == zfnull || !view->ownerZFUIView->viewFocusable())) {
        view = view->parent;
    }
    if(view == zfnull || view == this->viewFocused) {
        return;
    }

    ZFUIView *focusOld = (this->viewFocused != zfnull ? this->viewFocused->ownerZFUIView : zfnull);
    ZFUIView *focusNew = view->ownerZFUIView;
    this->viewFocused = view;
    if(focusOld != zfnull) {
        focusImpl->notifyViewFocusChanged(focusOld);
    }
    if(focusNew != zfnull) {
        focusImpl->notifyViewFocusChanged(focusNew);
    }
}

ZFImpl_sys_SDL_MouseState &ZFImpl_sys_SDL_SysWindow::mouseState(
        ZF_IN Uint32 which
        , ZF_IN Uint8 button
        ) {
    return d->mouseState[zfidentityHash(which, button)];
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_SysWindow_EventDispatch, ZFLevelZFFrameworkNormal) {
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_WINDOWEVENT, onWindowEvent, ZFLevelZFFrameworkNormal);

    ZFImpl_sys_SDL_eventHandlerAdd(SDL_MOUSEMOTION, onMouseEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_MOUSEBUTTONDOWN, onMouseEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_MOUSEBUTTONUP, onMouseEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_MOUSEWHEEL, onWheelEvent, ZFLevelZFFrameworkNormal);

    ZFImpl_sys_SDL_eventHandlerAdd(SDL_KEYDOWN, onKeyEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_KEYUP, onKeyEvent, ZFLevelZFFrameworkNormal);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_SysWindow_EventDispatch) {
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_MOUSEMOTION, onMouseEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_MOUSEBUTTONDOWN, onMouseEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_MOUSEBUTTONUP, onMouseEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_MOUSEWHEEL, onWheelEvent);

    ZFImpl_sys_SDL_eventHandlerRemove(SDL_KEYDOWN, onKeyEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_KEYUP, onKeyEvent);
}
public:
    static ZFImpl_sys_SDL_SysWindow *sysWindowForWindowId(ZF_IN Uint32 sdlWindowId) {
        ZFCoreArray<ZFImpl_sys_SDL_SysWindow *> &l = ZFImpl_sys_SDL_SysWindow::allWindow();
        for(zfindex i = l.count() - 1; i != zfindexMax(); --i) {
            if(SDL_GetWindowID(l[i]->sdlWindow) == sdlWindowId
                && l[i]->rootView != zfnull
            ) {
                return l[i];
            }
        }
        return zfnull;
    }
private:
    static zfbool onWindowEvent(ZF_IN SDL_Event *sdlEvent) {
        switch(sdlEvent->window.event) {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                ZFImpl_sys_SDL_SysWindow *sysWindow = sysWindowForWindowId(sdlEvent->window.windowID);
                if(sysWindow != zfnull) {
                    sysWindow->rootView->layoutRequest();
                    return zftrue;
                }
                break;
            }
            case SDL_WINDOWEVENT_SHOWN: {
                ZFImpl_sys_SDL_SysWindow *sysWindow = sysWindowForWindowId(sdlEvent->window.windowID);
                if(sysWindow != zfnull) {
                    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnResume(sysWindow->ownerZFUISysWindow);
                    return zftrue;
                }
                break;
            }
            case SDL_WINDOWEVENT_HIDDEN: {
                ZFImpl_sys_SDL_SysWindow *sysWindow = sysWindowForWindowId(sdlEvent->window.windowID);
                if(sysWindow != zfnull) {
                    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnPause(sysWindow->ownerZFUISysWindow);
                    return zftrue;
                }
                break;
            }
            default:
                break;
        }
        return zffalse;
    }

    static zfbool onMouseEvent(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_SysWindow *sysWindow = sysWindowForWindowId(sdlEvent->button.windowID);
        if(sysWindow != zfnull) {
            sysWindow->rootView->dispatchMouseEvent(sdlEvent);
            return zftrue;
        }
        return zffalse;
    }
    static zfbool onWheelEvent(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_SysWindow *sysWindow = sysWindowForWindowId(sdlEvent->wheel.windowID);
        if(sysWindow != zfnull) {
            sysWindow->rootView->dispatchWheelEvent(sdlEvent);
            return zftrue;
        }
        return zffalse;
    }
    static zfbool onKeyEvent(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_SysWindow *sysWindow = sysWindowForWindowId(sdlEvent->key.windowID);
        if(sysWindow != zfnull) {
            sysWindow->rootView->dispatchKeyEvent(sdlEvent);
            return zftrue;
        }
        return zffalse;
    }
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_SysWindow_EventDispatch)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

