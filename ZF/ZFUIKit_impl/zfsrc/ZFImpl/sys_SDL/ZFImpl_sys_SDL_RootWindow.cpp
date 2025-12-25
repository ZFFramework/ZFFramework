#include "ZFImpl_sys_SDL_RootWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIRootWindow.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCoreArray<ZFImpl_sys_SDL_RootWindow *> &ZFImpl_sys_SDL_RootWindow::allWindow(void) {
    static ZFCoreArray<ZFImpl_sys_SDL_RootWindow *> l;
    return l;
}

zfclassNotPOD _ZFP_ZFImpl_sys_SDL_RootWindowPrivate {
public:
    zfuint refCount;
    ZFImpl_sys_SDL_RootWindow *rootWindow;
    zfbool renderStarted;
    zfbool renderPending;
    zfbool renderRequested;
    zfbool layoutRequested;
    zfint lastWidth;
    zfint lastHeight;

    zfstlhashmap<zfidentity, ZFImpl_sys_SDL_MouseState> mouseState;

public:
    _ZFP_ZFImpl_sys_SDL_RootWindowPrivate(ZF_IN ZFImpl_sys_SDL_RootWindow *ownerWindow)
    : refCount(1)
    , rootWindow(ownerWindow)
    , renderStarted(zffalse)
    , renderPending(zffalse)
    , renderRequested(zftrue)
    , layoutRequested(zftrue)
    , lastWidth(0)
    , lastHeight(0)
    , mouseState()
    {
    }

public:
    void renderRequest(void);
};

ZFIMPL_SYS_SDL_USER_EVENT_HANDLER(RootWindowRender, ZFLevelZFFrameworkPostNormal) {
    _ZFP_ZFImpl_sys_SDL_RootWindowPrivate *d = (_ZFP_ZFImpl_sys_SDL_RootWindowPrivate *)sdlEvent->user.data1;
    d->renderPending = zffalse;
    if(d->rootWindow != zfnull) {
        if(d->layoutRequested) {
            d->layoutRequested = zffalse;
            if(d->rootWindow->rootView != zfnull && d->rootWindow->rootView->ownerZFUIView != zfnull) {
                int w = 0;
                int h = 0;
                SDL_GetWindowSize(d->rootWindow->sdlWindow, &w, &h);
                ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutView(d->rootWindow->rootView->ownerZFUIView, ZFUIRectCreate(0, 0, w, h));
            }
        }
        if(d->renderRequested) {
            d->renderRequested = zffalse;
            if(d->rootWindow->rootView != zfnull) {
                SDL_SetRenderDrawColor(d->rootWindow->sdlRenderer, 255, 255, 255, 0);
                SDL_RenderClear(d->rootWindow->sdlRenderer);
                d->rootWindow->rootView->render(d->rootWindow->sdlRenderer, d->rootWindow->rootView->rect, d->rootWindow->rootView->rect, 1);
                SDL_RenderPresent(d->rootWindow->sdlRenderer);
            }
        }
    }
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
    return zftrue;
}
void _ZFP_ZFImpl_sys_SDL_RootWindowPrivate::renderRequest(void) {
    if(!this->renderPending) {
        this->renderPending = zftrue;
        this->renderRequested = zftrue;
        ++(this->refCount);
        ZFIMPL_SYS_SDL_USER_EVENT_POST(RootWindowRender, this, zfnull);
    }
}

ZFImpl_sys_SDL_RootWindow::ZFImpl_sys_SDL_RootWindow(void)
: d(zfpoolNew(_ZFP_ZFImpl_sys_SDL_RootWindowPrivate, this))
, ownerZFUIRootWindow(zfnull)
, builtinWindow(zffalse)
, sdlWindow(zfnull)
, sdlRenderer(zfnull)
, rootView(zfnull)
, focused(zfnull)
, mouseIdGen()
{
    allWindow().add(this);
}
ZFImpl_sys_SDL_RootWindow::~ZFImpl_sys_SDL_RootWindow(void) {
    allWindow().removeElement(this);

    this->renderStop();
    if(!this->builtinWindow) {
        if(this->sdlRenderer != zfnull) {
            ZFImpl_sys_SDL_RendererNotifyDestroy(this->sdlRenderer);
            SDL_DestroyRenderer(this->sdlRenderer);
            this->sdlRenderer = zfnull;
        }
        if(this->sdlWindow != zfnull) {
            ZFImpl_sys_SDL_WindowNotifyDestroy(this->sdlWindow);
            SDL_DestroyWindow(this->sdlWindow);
            this->sdlWindow = zfnull;
        }
    }

    d->rootWindow = zfnull;
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
}

// ============================================================
void ZFImpl_sys_SDL_RootWindow::renderStart(void) {
    if(d->renderStarted) {
        return;
    }
    d->renderStarted = zftrue;
    d->renderRequest();
}
void ZFImpl_sys_SDL_RootWindow::renderStop(void) {
    if(!d->renderStarted) {
        return;
    }
    d->renderStarted = zffalse;
}

void ZFImpl_sys_SDL_RootWindow::renderRequest(void) {
    d->renderRequested = zftrue;
    d->renderRequest();
}

void ZFImpl_sys_SDL_RootWindow::layoutRequest(void) {
    d->layoutRequested = zftrue;
    d->renderRequested = zftrue;
    d->renderRequest();
}

// ============================================================
void ZFImpl_sys_SDL_RootWindow::focus(ZF_IN ZFImpl_sys_SDL_View *view) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *focusImpl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    ZFCoreAssert(focusImpl != zfnull);
    if(view == zfnull) {
        if(this->focused != zfnull) {
            ZFUIView *owner = this->focused->ownerZFUIView;
            this->focused = zfnull;
            if(owner != zfnull) {
                if(focusImpl != zfnull) {
                    focusImpl->notifyViewFocusUpdate(owner);
                }
            }
        }
        return;
    }

    while(view != zfnull && (view->ownerZFUIView == zfnull || !view->ownerZFUIView->focusable())) {
        view = view->parent;
    }
    if(view == zfnull || view == this->focused) {
        return;
    }

    ZFUIView *focusOld = (this->focused != zfnull ? this->focused->ownerZFUIView : zfnull);
    ZFUIView *focusNew = view->ownerZFUIView;
    this->focused = view;
    if(focusOld != zfnull) {
        focusImpl->notifyViewFocusUpdate(focusOld);
    }
    if(focusNew != zfnull) {
        focusImpl->notifyViewFocusUpdate(focusNew);
    }
}

ZFImpl_sys_SDL_MouseState &ZFImpl_sys_SDL_RootWindow::mouseState(
        ZF_IN Uint32 which
        , ZF_IN Uint8 button
        ) {
    return d->mouseState[zfidentityHash(which, button)];
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_RootWindow_EventDispatch, ZFLevelZFFrameworkNormal) {
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_WINDOW_RESIZED, onWindowResize, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED, onWindowResize, ZFLevelZFFrameworkNormal);

    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_WINDOW_SHOWN, onWindowResume, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_WINDOW_FOCUS_GAINED, onWindowResume, ZFLevelZFFrameworkNormal);

    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_WINDOW_HIDDEN, onWindowPause, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_WINDOW_FOCUS_LOST, onWindowPause, ZFLevelZFFrameworkNormal);

    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_MOUSE_MOTION, onMouseEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_MOUSE_BUTTON_DOWN, onMouseEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_MOUSE_BUTTON_UP, onMouseEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_MOUSE_WHEEL, onWheelEvent, ZFLevelZFFrameworkNormal);

    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_KEY_DOWN, onKeyEvent, ZFLevelZFFrameworkNormal);
    ZFImpl_sys_SDL_eventHandlerAdd(SDL_EVENT_KEY_UP, onKeyEvent, ZFLevelZFFrameworkNormal);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_RootWindow_EventDispatch) {
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_WINDOW_RESIZED, onWindowResize);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED, onWindowResize);

    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_WINDOW_SHOWN, onWindowResume);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_WINDOW_FOCUS_GAINED, onWindowResume);

    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_WINDOW_HIDDEN, onWindowPause);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_WINDOW_FOCUS_LOST, onWindowPause);

    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_MOUSE_MOTION, onMouseEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_MOUSE_BUTTON_DOWN, onMouseEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_MOUSE_BUTTON_UP, onMouseEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_MOUSE_WHEEL, onWheelEvent);

    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_KEY_DOWN, onKeyEvent);
    ZFImpl_sys_SDL_eventHandlerRemove(SDL_EVENT_KEY_UP, onKeyEvent);
}
public:
    static ZFImpl_sys_SDL_RootWindow *rootWindowForWindowId(ZF_IN Uint32 sdlWindowId) {
        ZFCoreArray<ZFImpl_sys_SDL_RootWindow *> &l = ZFImpl_sys_SDL_RootWindow::allWindow();
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
    static zfbool onWindowResize(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_RootWindow *rootWindow = rootWindowForWindowId(sdlEvent->window.windowID);
        if(rootWindow != zfnull) {
            if(zffalse
                    || sdlEvent->window.data1 != rootWindow->_ZFP_ZFImpl_sys_SDL_RootWindow_d()->lastWidth
                    || sdlEvent->window.data2 != rootWindow->_ZFP_ZFImpl_sys_SDL_RootWindow_d()->lastHeight
                    ) {
                rootWindow->_ZFP_ZFImpl_sys_SDL_RootWindow_d()->lastWidth = sdlEvent->window.data1;
                rootWindow->_ZFP_ZFImpl_sys_SDL_RootWindow_d()->lastHeight = sdlEvent->window.data2;
                rootWindow->rootView->layoutRequest();
            }
        }
        return zffalse;
    }
    static zfbool onWindowResume(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_RootWindow *rootWindow = rootWindowForWindowId(sdlEvent->window.windowID);
        if(rootWindow != zfnull) {
            ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnResume(rootWindow->ownerZFUIRootWindow);
        }
        return zffalse;
    }
    static zfbool onWindowPause(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_RootWindow *rootWindow = rootWindowForWindowId(sdlEvent->window.windowID);
        if(rootWindow != zfnull) {
            ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnPause(rootWindow->ownerZFUIRootWindow);
        }
        return zffalse;
    }

    static zfbool onMouseEvent(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_RootWindow *rootWindow = rootWindowForWindowId(sdlEvent->button.windowID);
        if(rootWindow != zfnull) {
            rootWindow->rootView->dispatchMouseEvent(sdlEvent);
            return zftrue;
        }
        return zffalse;
    }
    static zfbool onWheelEvent(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_RootWindow *rootWindow = rootWindowForWindowId(sdlEvent->wheel.windowID);
        if(rootWindow != zfnull) {
            rootWindow->rootView->dispatchWheelEvent(sdlEvent);
            return zftrue;
        }
        return zffalse;
    }
    static zfbool onKeyEvent(ZF_IN SDL_Event *sdlEvent) {
        ZFImpl_sys_SDL_RootWindow *rootWindow = rootWindowForWindowId(sdlEvent->key.windowID);
        if(rootWindow != zfnull) {
            rootWindow->rootView->dispatchKeyEvent(sdlEvent);
            return zftrue;
        }
        return zffalse;
    }
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_RootWindow_EventDispatch)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

