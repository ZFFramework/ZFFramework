#include "ZFImpl_sys_SDL_View.h"
#include "ZFImpl_sys_SDL_SysWindow.h"
#include "ZFImpl_sys_SDL_KeyCode.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFImpl_sys_SDL_View::sysWindowAttach(ZF_IN ZFImpl_sys_SDL_SysWindow *sysWindow) {
    this->sysWindow = sysWindow;
    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i) {
        this->children[i]->sysWindowAttach(sysWindow);
    }
}
void ZFImpl_sys_SDL_View::sysWindowDetach(void) {
    if(this->sysWindow != zfnull) {
        if(this->sysWindow->focused == this) {
            this->sysWindow->focus(zfnull);
        }
        this->sysWindow = zfnull;
    }
    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i) {
        this->children[i]->sysWindowDetach();
    }
}

void ZFImpl_sys_SDL_View::childAttach(
        ZF_IN zfindex index
        , ZF_IN ZFImpl_sys_SDL_View *child
        ) {
    this->children.add(child, index);
    child->parent = this;
    if(this->sysWindow != zfnull) {
        child->sysWindowAttach(sysWindow);
    }
}
void ZFImpl_sys_SDL_View::childDetach(ZF_IN zfindex index) {
    ZFImpl_sys_SDL_View *child = this->children[index];
    child->parent = zfnull;
    this->children.remove(index);
    if(this->sysWindow != zfnull) {
        child->sysWindowDetach();
    }
}

void ZFImpl_sys_SDL_View::renderRequest(void) {
    if(!this->renderRequested) {
        this->renderRequested = zftrue;
        this->renderCacheValid = zffalse;
        if(this->parent != zfnull) {
            this->parent->renderRequest();
        }
        else if(this->sysWindow != zfnull) {
            this->sysWindow->renderRequest();
        }
    }
}
void ZFImpl_sys_SDL_View::layoutRequest(void) {
    if(this->sysWindow != zfnull) {
        this->sysWindow->layoutRequest();
    }
    this->renderRequest();
}

// ============================================================
void ZFImpl_sys_SDL_View::renderCachePrepare(
        ZF_IN SDL_Renderer *renderer
        , ZF_IN int w
        , ZF_IN int h
        ) {
    const int align = 32;
    int desiredWidth = (((w - 1) / align) + 1) * align;
    int desiredHeight = (((h - 1) / align) + 1) * align;
    if(this->renderCache != zfnull) {
        int w, h;
        if(SDL_QueryTexture(this->renderCache, zfnull, zfnull, &w, &h) != 0
            || w != desiredWidth || h != desiredHeight
        ) {
            this->renderCacheRemove();
        }
    }
    if(this->renderCache == zfnull) {
        SDL_RendererInfo rendererInfo;
        SDL_GetRendererInfo(renderer, &rendererInfo);
        this->renderCache = SDL_CreateTexture(
            renderer,
            (rendererInfo.num_texture_formats > 0 && rendererInfo.texture_formats[0] != SDL_PIXELFORMAT_UNKNOWN)
                ? rendererInfo.texture_formats[0]
                : SDL_PIXELFORMAT_ARGB32,
            SDL_TEXTUREACCESS_TARGET,
            desiredWidth,
            desiredHeight);
        SDL_SetTextureBlendMode(this->renderCache, SDL_BLENDMODE_BLEND);
    }
}

// ============================================================
static void _ZFP_ZFImpl_sys_SDL_View_render(
        ZF_IN ZFImpl_sys_SDL_View *view
        , ZF_IN SDL_Renderer *renderer
        , ZF_IN const SDL_Rect &childRect
        , ZF_IN const SDL_Rect &parentRect
        , ZF_IN zffloat treeAlpha
        ) {
    zfbool done = zffalse;
    for(zfindex i = 0; i < view->renderImpls.count(); ++i) {
        done |= view->renderImpls[i](renderer, view, childRect, parentRect, treeAlpha);
    }

    if(!done) {
        SDL_Rect parentRectNew;
        ZFImpl_sys_SDL_View::renderRectCalc(parentRectNew, childRect, parentRect);
        for(zfindex i = 0; i < view->children.count(); ++i) {
            ZFImpl_sys_SDL_View *child = view->children[i];
            SDL_Rect childRectNew;
            childRectNew.x = childRect.x + child->rect.x;
            childRectNew.y = childRect.y + child->rect.y;
            childRectNew.w = child->rect.w;
            childRectNew.h = child->rect.h;
            child->render(renderer, childRectNew, parentRectNew, child->ownerZFUIView != zfnull ? treeAlpha * child->ownerZFUIView->alpha() : 1);
        }
    }
}

void ZFImpl_sys_SDL_View::render(
        ZF_IN SDL_Renderer *renderer
        , ZF_IN const SDL_Rect &childRect
        , ZF_IN const SDL_Rect &parentRect
        , ZF_IN zffloat treeAlpha
        ) {
    this->renderRequested = zffalse;
    if(this->ownerZFUIView != zfnull && !this->ownerZFUIView->visible()) {
        return;
    }
    if(zffalse
        || childRect.x + childRect.w <= parentRect.x
        || childRect.x >= parentRect.x + parentRect.w
        || childRect.y + childRect.h <= parentRect.y
        || childRect.y >= parentRect.y + parentRect.h
    ) {
        return;
    }
    if(this->viewTransform == zfnull
        && this->aniTransform == zfnull
        && this->renderCacheRequired == 0
    ) {
        this->renderCacheRemove();
        _ZFP_ZFImpl_sys_SDL_View_render(this, renderer, childRect, parentRect, treeAlpha);
        return;
    }

    this->renderCachePrepare(renderer, this->rect.w, this->rect.h);
    if(this->renderCache == zfnull) {
        _ZFP_ZFImpl_sys_SDL_View_render(this, renderer, childRect, parentRect, treeAlpha);
        return;
    }
    if(!this->renderCacheValid) {
        ZFImpl_sys_SDL_zfblockedRenderTarget(success, renderer, this->renderCache);
        if(!success) {
            _ZFP_ZFImpl_sys_SDL_View_render(this, renderer, childRect, parentRect, treeAlpha);
            return;
        }

        Uint8 rOld, gOld, bOld, aOld;
        SDL_GetRenderDrawColor(renderer, &rOld, &gOld, &bOld, &aOld);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, rOld, gOld, bOld, aOld);

        SDL_Rect cacheRect;
        cacheRect.x = 0;
        cacheRect.y = 0;
        cacheRect.w = childRect.w;
        cacheRect.h = childRect.h;
        _ZFP_ZFImpl_sys_SDL_View_render(this, renderer, cacheRect, cacheRect, treeAlpha);
    }

    // support 2D transform only
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = this->rect.w;
    src.h = this->rect.h;
    SDL_Rect dst;
    dst.x = childRect.x;
    dst.y = childRect.y;
    dst.w = this->rect.w;
    dst.h = this->rect.h;
    double angle = 0;
    zffloat scaleX = 1;
    zffloat scaleY = 1;
    if(this->viewTransform != zfnull) {
        angle += (double)this->viewTransform->rotateZ;
        dst.x += (int)this->viewTransform->translateX;
        dst.y += (int)this->viewTransform->translateY;
        if(this->viewTransform->scaleX != 1) {
            scaleX *= this->viewTransform->scaleX;
        }
        if(this->viewTransform->scaleY != 1) {
            scaleY *= this->viewTransform->scaleY;
        }
    }
    if(this->aniTransform != zfnull) {
        angle += (double)this->aniTransform->rotateZ;
        dst.x += (int)this->aniTransform->translateX;
        dst.y += (int)this->aniTransform->translateY;
        if(this->aniTransform->scaleX != 1) {
            scaleX *= this->aniTransform->scaleX;
        }
        if(this->aniTransform->scaleY != 1) {
            scaleY *= this->aniTransform->scaleY;
        }
    }
    if(scaleX != 1) {
        zffloat offset = src.w * (scaleX - 1);
        dst.x -= (int)(offset / 2);
        dst.w += (int)offset;
    }
    if(scaleY != 1) {
        zffloat offset = src.h * (scaleY - 1);
        dst.y -= (int)(offset / 2);
        dst.h += (int)offset;
    }
    SDL_Rect clipSaved;
    SDL_RenderGetClipRect(renderer, &clipSaved);
    SDL_RenderSetClipRect(renderer, &parentRect);
    SDL_RenderCopyEx(renderer, this->renderCache, &src, &dst, angle, zfnull, SDL_FLIP_NONE);
    if(clipSaved.w > 0 && clipSaved.h > 0) {
        SDL_RenderSetClipRect(renderer, &clipSaved);
    }
    else {
        SDL_RenderSetClipRect(renderer, NULL);
    }
}

// ============================================================
ZFImpl_sys_SDL_View *ZFImpl_sys_SDL_View::mouseTest(
        ZF_IN int x
        , ZF_IN int y
        , ZF_OUT_OPT zfbool *mouseGrab /* = zfnull */
        ) {
    if(mouseGrab != zfnull) {
        *mouseGrab = zffalse;
        if(this->sdlMouseGrabCallback != zfnull && this->sdlMouseGrabCallback(this, zfnull, x, y)) {
            *mouseGrab = zftrue;
            return this;
        }
    }
    if(this->ownerZFUIView == zfnull
        || !this->ownerZFUIView->viewUIEnableTree()
        || x < 0 || x >= this->rect.w
        || y < 0 || y >= this->rect.h
    ) {
        return zfnull;
    }

    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i) {
        ZFImpl_sys_SDL_View *child = this->children[i];
        child = child->mouseTest(x - child->rect.x, y - child->rect.y, mouseGrab);
        if(child != zfnull) {
            return child;
        }
    }
    if(this->ownerZFUIView->viewUIEnable()) {
        return this;
    }
    else {
        return zfnull;
    }
}
ZFImpl_sys_SDL_View *ZFImpl_sys_SDL_View::mouseHoverTest(
        ZF_IN int x
        , ZF_IN int y
        ) {
    if(this->ownerZFUIView == zfnull
        || !this->ownerZFUIView->viewUIEnableTree()
        || x < 0 || x >= this->rect.w
        || y < 0 || y >= this->rect.h
    ) {
        return zfnull;
    }

    ZFImpl_sys_SDL_View *child = zfnull;
    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i) {
        child = this->children[i];
        if(child->ownerZFUIView == zfnull) {
            continue;
        }
        ZFImpl_sys_SDL_View *tmp = child->mouseHoverTest(x - child->rect.x, y - child->rect.y);
        if(tmp != zfnull) {
            return tmp;
        }
    }
    if(this->ownerZFUIView->viewUIEnable() && this->ownerZFUIView->mouseHoverEnable()) {
        return this;
    }
    else {
        return zfnull;
    }
}

// ============================================================
void ZFImpl_sys_SDL_View::posOnWindow(
        ZF_OUT int &x
        , ZF_OUT int &y
        ) {
    x = 0;
    y = 0;
    ZFImpl_sys_SDL_View *p = this;
    while(p != zfnull) {
        x += p->rect.x;
        y += p->rect.y;
        p = p->parent;
    }
}
void ZFImpl_sys_SDL_View::dispatchMouseEvent(ZF_IN SDL_Event *sdlEvent) {
    if(this->sysWindow == zfnull) {
        return;
    }

    Uint8 sdlButton = (Uint8)sdlEvent->button.button;
    ZFUIMouseButton mouseButton = ZFUIMouseButton::e_Left;
    switch(sdlButton) {
        case SDL_BUTTON_RIGHT:
            mouseButton = ZFUIMouseButton::e_Right;
            break;
        case SDL_BUTTON_MIDDLE:
            mouseButton = ZFUIMouseButton::e_Center;
            break;
        case SDL_BUTTON_LEFT:
        default:
            mouseButton = ZFUIMouseButton::e_Left;
            break;
    }

    switch(sdlEvent->type) {
        case SDL_MOUSEMOTION: {
            ZFImpl_sys_SDL_MouseState &mouseState = this->sysWindow->mouseState(sdlEvent->motion.which, sdlButton);
            if(mouseState.viewDown != zfnull) { // mouse move
                if(mouseState.mouseId == zfidentityInvalid()) {
                    mouseState.mouseId = this->sysWindow->mouseIdGen.idAcquire();
                }
                if(mouseState.mouseGrab) {
                    zfint x = sdlEvent->motion.x;
                    zfint y = sdlEvent->motion.y;
                    mouseState.viewDown->posFromGlobal(x, y);
                    mouseState.viewDown->sdlMouseGrabCallback(mouseState.viewDown, sdlEvent, x, y);
                }
                else {
                    zfobj<ZFUIMouseEvent> event;
                    event->mouseId = mouseState.mouseId;
                    event->mouseAction = ZFUIMouseAction::e_Move;
                    event->mouseButton = mouseButton;
                    event->mousePoint.x = (zffloat)sdlEvent->motion.x;
                    event->mousePoint.y = (zffloat)sdlEvent->motion.y;
                    this->posFromGlobal(event->mousePoint.x, event->mousePoint.y);
                    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(mouseState.viewDown->ownerZFUIView, event);
                }
            }
            else { // mouse hover
                ZFImpl_sys_SDL_View *viewHover = this->mouseHoverTestGlobal(sdlEvent->motion.x, sdlEvent->motion.y);
                if(mouseState.viewHover != viewHover) {
                    if(mouseState.viewHover != zfnull) {
                        if(mouseState.viewHover->ownerZFUIView != zfnull && mouseState.mouseId != zfidentityInvalid()) {
                            zfobj<ZFUIMouseEvent> hoverExit;
                            hoverExit->mouseId = mouseState.mouseId;
                            hoverExit->mouseAction = ZFUIMouseAction::e_HoverExit;
                            hoverExit->mouseButton = mouseButton;
                            hoverExit->mousePoint.x = (zffloat)sdlEvent->motion.x;
                            hoverExit->mousePoint.y = (zffloat)sdlEvent->motion.y;
                            this->posFromGlobal(hoverExit->mousePoint.x, hoverExit->mousePoint.y);
                            ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(mouseState.viewHover->ownerZFUIView, hoverExit);
                        }
                        mouseState.viewHover = zfnull;
                    }
                    if(mouseState.mouseId != zfidentityInvalid()) {
                        this->sysWindow->mouseIdGen.idRelease(mouseState.mouseId);
                        mouseState.mouseId = zfidentityInvalid();
                    }
                    if(viewHover != zfnull && viewHover->ownerZFUIView != zfnull) {
                        mouseState.viewHover = viewHover;

                        mouseState.mouseId = this->sysWindow->mouseIdGen.idAcquire();
                        zfobj<ZFUIMouseEvent> hoverEnter;
                        hoverEnter->mouseId = mouseState.mouseId;
                        hoverEnter->mouseAction = ZFUIMouseAction::e_HoverEnter;
                        hoverEnter->mouseButton = mouseButton;
                        hoverEnter->mousePoint.x = (zffloat)sdlEvent->motion.x;
                        hoverEnter->mousePoint.y = (zffloat)sdlEvent->motion.y;
                        this->posFromGlobal(hoverEnter->mousePoint.x, hoverEnter->mousePoint.y);
                        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(viewHover->ownerZFUIView, hoverEnter);
                    }
                }

                if(mouseState.viewHover != zfnull && mouseState.viewHover->ownerZFUIView != zfnull) {
                    ZFCoreAssert(mouseState.mouseId != zfidentityInvalid());
                    zfobj<ZFUIMouseEvent> event;
                    event->mouseId = mouseState.mouseId;
                    event->mouseAction = ZFUIMouseAction::e_Hover;
                    event->mouseButton = mouseButton;
                    event->mousePoint.x = (zffloat)sdlEvent->motion.x;
                    event->mousePoint.y = (zffloat)sdlEvent->motion.y;
                    this->posFromGlobal(event->mousePoint.x, event->mousePoint.y);
                    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(mouseState.viewHover->ownerZFUIView, event);
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            ZFImpl_sys_SDL_MouseState &mouseState = this->sysWindow->mouseState(sdlEvent->motion.which, sdlEvent->button.button);
            ZFImpl_sys_SDL_View *viewDown = this->mouseTestGlobal(sdlEvent->motion.x, sdlEvent->motion.y, &mouseState.mouseGrab);
            if(mouseState.mouseGrab) {
                mouseState.viewDown = viewDown;
                zfint x = sdlEvent->button.x;
                zfint y = sdlEvent->button.y;
                viewDown->posFromGlobal(x, y);
                viewDown->sdlMouseGrabCallback(viewDown, sdlEvent, x, y);
                return;
            }
            if(viewDown != zfnull && mouseState.viewDown == viewDown) {
                return;
            }

            if(mouseState.mouseId != zfidentityInvalid()) {
                this->sysWindow->mouseIdGen.idRelease(mouseState.mouseId);
                mouseState.mouseId = zfidentityInvalid();
            }
            if(viewDown == zfnull || viewDown->ownerZFUIView == zfnull) {
                return;
            }

            mouseState.viewDown = viewDown;
            mouseState.mouseId = this->sysWindow->mouseIdGen.idAcquire();
            zfobj<ZFUIMouseEvent> event;
            event->mouseId = mouseState.mouseId;
            event->mouseAction = ZFUIMouseAction::e_Down;
            event->mouseButton = mouseButton;
            event->mousePoint.x = (zffloat)sdlEvent->button.x;
            event->mousePoint.y = (zffloat)sdlEvent->button.y;
            viewDown->posFromGlobal(event->mousePoint.x, event->mousePoint.y);
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(viewDown->ownerZFUIView, event);
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            ZFImpl_sys_SDL_MouseState &mouseState = this->sysWindow->mouseState(sdlEvent->motion.which, sdlEvent->button.button);
            zfidentity mouseIdPrev = mouseState.mouseId;
            ZFImpl_sys_SDL_View *viewDownPrev = mouseState.viewDown;
            if(mouseState.mouseId != zfidentityInvalid()) {
                this->sysWindow->mouseIdGen.idRelease(mouseState.mouseId);
                mouseState.mouseId = zfidentityInvalid();
            }
            mouseState.viewDown = zfnull;

            if(mouseIdPrev != zfidentityInvalid() && viewDownPrev != zfnull) {
                ZFUIMouseAction mouseAction = (sdlEvent->button.x > ZFImpl_sys_SDL_View::MouseCancel && sdlEvent->button.y > ZFImpl_sys_SDL_View::MouseCancel)
                    ? ZFUIMouseAction::e_Up
                    : ZFUIMouseAction::e_Cancel;
                if(mouseState.mouseGrab) {
                    mouseState.mouseGrab = zffalse;
                    zfint x = sdlEvent->button.x;
                    zfint y = sdlEvent->button.y;
                    viewDownPrev->posFromGlobal(x, y);
                    viewDownPrev->sdlMouseGrabCallback(viewDownPrev, sdlEvent, x, y);
                }
                else {
                    zfobj<ZFUIMouseEvent> event;
                    event->mouseId = mouseIdPrev;
                    event->mouseAction = mouseAction;
                    event->mouseButton = mouseButton;
                    event->mousePoint.x = (zffloat)sdlEvent->button.x;
                    event->mousePoint.y = (zffloat)sdlEvent->button.y;
                    viewDownPrev->posFromGlobal(event->mousePoint.x, event->mousePoint.y);
                    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(viewDownPrev->ownerZFUIView, event);
                }
            }
            break;
        }
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
}
void ZFImpl_sys_SDL_View::dispatchWheelEvent(ZF_IN SDL_Event *sdlEvent) {
    ZFCoreAssert(sdlEvent->type == SDL_MOUSEWHEEL);
    ZFUIView *target = zfnull;
    {
        ZFImpl_sys_SDL_View *sdlView = this->mouseTestGlobal(sdlEvent->wheel.mouseX, sdlEvent->wheel.mouseY);
        while(sdlView != zfnull && sdlView->ownerZFUIView == zfnull) {
            sdlView = sdlView->parent;
        }
        if(sdlView != zfnull) {
            target = sdlView->ownerZFUIView;
        }
    }
    if(target == zfnull && this->sysWindow != zfnull) {
        if(this->sysWindow->focused != zfnull) {
            target = this->sysWindow->focused->ownerZFUIView;
        }
        else {
            target = this->ownerZFUIView;
        }
    }
    if(target != zfnull) {
        zfobj<ZFUIWheelEvent> event;
        event->wheelX = (zffloat)sdlEvent->wheel.x;
        event->wheelY = (zffloat)sdlEvent->wheel.y;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(target, event);
    }
}
void ZFImpl_sys_SDL_View::dispatchKeyEvent(ZF_IN SDL_Event *sdlEvent) {
    ZFUIView *target = zfnull;
    if(this->sysWindow != zfnull) {
        if(this->sysWindow->focused != zfnull) {
            target = this->sysWindow->focused->ownerZFUIView;
        }
        else {
            target = this->ownerZFUIView;
        }
    }
    if(target != zfnull) {
        zfobj<ZFUIKeyEvent> event;
        event->keyId = zfidentityHash(sdlEvent->key.windowID, sdlEvent->key.keysym.sym, sdlEvent->key.keysym.scancode);
        if(sdlEvent->key.repeat) {
            event->keyAction = ZFUIKeyAction::e_Repeat;
        }
        else if(sdlEvent->type == SDL_KEYDOWN) {
            event->keyAction = ZFUIKeyAction::e_Down;
        }
        else if(sdlEvent->type == SDL_KEYUP) {
            event->keyAction = ZFUIKeyAction::e_Up;
        }
        else {
            ZFCoreCriticalShouldNotGoHere();
        }
        event->keyCode = ZFImpl_sys_SDL_ZFUIKeyCodeFromRaw((SDL_KeyCode)sdlEvent->key.keysym.sym);
        event->keyCodeRaw = sdlEvent->key.keysym.sym;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(target, event);
    }
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

