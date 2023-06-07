#include "ZFImpl_sys_SDL_View.h"
#include "ZFImpl_sys_SDL_SysWindow.h"
#include "ZFImpl_sys_SDL_KeyCode.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFImpl_sys_SDL_View::sysWindowAttach(ZF_IN ZFImpl_sys_SDL_SysWindow *sysWindow)
{
    this->sysWindow = sysWindow;
    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i)
    {
        this->children[i]->sysWindowAttach(sysWindow);
    }
}
void ZFImpl_sys_SDL_View::sysWindowDetach(void)
{
    this->sysWindow = zfnull;
    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i)
    {
        this->children[i]->sysWindowDetach();
    }
}

void ZFImpl_sys_SDL_View::childAttach(ZF_IN zfindex index, ZF_IN ZFImpl_sys_SDL_View *child)
{
    this->children.add(index, child);
    child->parent = this;
    if(this->sysWindow != zfnull)
    {
        child->sysWindowAttach(sysWindow);
    }
}
void ZFImpl_sys_SDL_View::childDetach(ZF_IN zfindex index)
{
    ZFImpl_sys_SDL_View *child = this->children[index];
    child->parent = zfnull;
    this->children.remove(index);
    if(this->sysWindow != zfnull)
    {
        child->sysWindowDetach();
    }
}

// ============================================================
void ZFImpl_sys_SDL_View::renderCachePrepare(ZF_IN SDL_Renderer *renderer)
{
    const int align = 32;
    int desiredWidth = (((this->rect.w - 1) / align) + 1) * align;
    int desiredHeight = (((this->rect.h - 1) / align) + 1) * align;
    if(this->renderCache != zfnull)
    {
        int w, h;
        if(SDL_QueryTexture(this->renderCache, zfnull, zfnull, &w, &h) != 0
            || w != desiredWidth || h != desiredHeight
        ) {
            this->renderCacheRemove();
        }
    }
    if(this->renderCache == zfnull)
    {
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
    }
}

// ============================================================
static void _ZFP_ZFImpl_sys_SDL_View_render(ZF_IN ZFImpl_sys_SDL_View *view,
                                            ZF_IN SDL_Renderer *renderer,
                                            ZF_IN int viewX,
                                            ZF_IN int viewY)
{
    for(zfindex i = 0; i < view->renderImpls.count(); ++i)
    {
        view->renderImpls[i](renderer, view, viewX, viewY);
    }

    for(zfindex i = 0; i < view->children.count(); ++i)
    {
        ZFImpl_sys_SDL_View *child = view->children[i];
        child->render(renderer, viewX + child->rect.x, viewY + child->rect.y);
    }
}

void ZFImpl_sys_SDL_View::render(ZF_IN SDL_Renderer *renderer, ZF_IN int viewX, ZF_IN int viewY)
{
    if(this->ownerZFUIView != zfnull && !this->ownerZFUIView->viewVisible())
    {
        return;
    }
    if(this->viewTransform == zfnull && this->aniTransform == zfnull)
    {
        this->renderCacheRemove();
        _ZFP_ZFImpl_sys_SDL_View_render(this, renderer, viewX, viewY);
        return;
    }

    this->renderCachePrepare(renderer);
    if(this->renderCache == zfnull)
    {
        _ZFP_ZFImpl_sys_SDL_View_render(this, renderer, viewX, viewY);
        return;
    }
    SDL_Texture *targetSaved = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, this->renderCache);
    SDL_RenderClear(renderer);
    _ZFP_ZFImpl_sys_SDL_View_render(this, renderer, 0, 0);
    SDL_SetRenderTarget(renderer, targetSaved);

    // support 2D transform only
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = this->rect.w;
    src.h = this->rect.h;
    SDL_FRect dst;
    dst.x = viewX;
    dst.y = viewY;
    dst.w = this->rect.w;
    dst.h = this->rect.h;
    double angle = 0;
    zffloat scaleX = 1;
    zffloat scaleY = 1;
    if(this->viewTransform != zfnull)
    {
        angle += this->viewTransform->rotateZ;
        dst.x += this->viewTransform->translateX;
        dst.y += this->viewTransform->translateY;
        if(this->viewTransform->scaleX != 1)
        {
            scaleX *= this->viewTransform->scaleX;
        }
        if(this->viewTransform->scaleY != 1)
        {
            scaleY *= this->viewTransform->scaleY;
        }
    }
    if(this->aniTransform != zfnull)
    {
        angle += this->aniTransform->rotateZ;
        dst.x += this->aniTransform->translateX;
        dst.y += this->aniTransform->translateY;
        if(this->aniTransform->scaleX != 1)
        {
            scaleX *= this->aniTransform->scaleX;
        }
        if(this->aniTransform->scaleY != 1)
        {
            scaleY *= this->aniTransform->scaleY;
        }
    }
    if(scaleX != 1)
    {
        zffloat offset = src.w * (scaleX - 1);
        dst.x -= offset / 2;
        dst.w += offset;
    }
    if(scaleY != 1)
    {
        zffloat offset = src.h * (scaleY - 1);
        dst.y -= offset / 2;
        dst.h += offset;
    }
    SDL_RenderCopyExF(renderer, this->renderCache, &src, &dst, angle, zfnull, SDL_FLIP_NONE);
}

// ============================================================
ZFImpl_sys_SDL_View *ZFImpl_sys_SDL_View::mouseTest(ZF_IN int x, ZF_IN int y)
{
    if(this->ownerZFUIView == zfnull
        || !this->ownerZFUIView->viewUIEnableTree()
        || x < 0 || x >= this->rect.w
        || y < 0 || y >= this->rect.h
    ) {
        return zfnull;
    }

    ZFImpl_sys_SDL_View *child = zfnull;
    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i)
    {
        child = this->children[i];
        if(child->ownerZFUIView == zfnull)
        {
            continue;
        }
        ZFImpl_sys_SDL_View *tmp = child->mouseTest(x - child->rect.x, y - child->rect.y);
        if(tmp != zfnull)
        {
            return tmp;
        }
    }
    if(this->ownerZFUIView->viewUIEnable())
    {
        return this;
    }
    else
    {
        return zfnull;
    }
}
ZFImpl_sys_SDL_View *ZFImpl_sys_SDL_View::mouseHoverTest(ZF_IN int x, ZF_IN int y)
{
    if(this->ownerZFUIView == zfnull
        || !this->ownerZFUIView->viewUIEnableTree()
        || x < 0 || x >= this->rect.w
        || y < 0 || y >= this->rect.h
    ) {
        return zfnull;
    }

    ZFImpl_sys_SDL_View *child = zfnull;
    for(zfindex i = this->children.count() - 1; i != zfindexMax(); --i)
    {
        child = this->children[i];
        if(child->ownerZFUIView == zfnull)
        {
            continue;
        }
        ZFImpl_sys_SDL_View *tmp = child->mouseHoverTest(x - child->rect.x, y - child->rect.y);
        if(tmp != zfnull)
        {
            return tmp;
        }
    }
    if(this->ownerZFUIView->viewUIEnable() && this->ownerZFUIView->viewMouseHoverEventEnable())
    {
        return this;
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
void ZFImpl_sys_SDL_View::posOnWindow(ZF_OUT int &x, ZF_OUT int &y)
{
    x = 0;
    y = 0;
    ZFImpl_sys_SDL_View *p = this;
    while(p != zfnull)
    {
        x += p->rect.x;
        y += p->rect.y;
        p = p->parent;
    }
}
void ZFImpl_sys_SDL_View::dispatchMouseEvent(ZF_IN SDL_Event *sdlEvent)
{
    if(this->sdlEventCallback != zfnull && this->sdlEventCallback(this, sdlEvent)) {return;}
    if(this->sysWindow == zfnull)
    {
        return;
    }

    ZFUIView *target = zfnull;
    zfblockedAlloc(ZFUIMouseEvent, event);
    switch(sdlEvent->type)
    {
        case SDL_MOUSEMOTION: {
            Uint8 sdlButton = (Uint8)-1;
            if((sdlEvent->motion.state & SDL_BUTTON_LMASK) == SDL_BUTTON_LMASK)
            {
                sdlButton = SDL_BUTTON_LEFT;
                event->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
            }
            else if((sdlEvent->motion.state & SDL_BUTTON_RMASK) == SDL_BUTTON_RMASK)
            {
                sdlButton = SDL_BUTTON_RIGHT;
                event->mouseButton = ZFUIMouseButton::e_MouseButtonRight;
            }
            else if((sdlEvent->motion.state & SDL_BUTTON_MMASK) == SDL_BUTTON_MMASK)
            {
                sdlButton = SDL_BUTTON_MIDDLE;
                event->mouseButton = ZFUIMouseButton::e_MouseButtonCenter;
            }
            else
            {
                sdlButton = SDL_BUTTON_LEFT;
                event->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
            }
            ZFImpl_sys_SDL_MouseState &mouseState = this->sysWindow->mouseState(sdlEvent->motion.which, sdlButton);
            if(mouseState.viewDown != zfnull)
            { // mouse move
                if(mouseState.mouseId == zfidentityInvalid())
                {
                    mouseState.mouseId = this->sysWindow->mouseIdGen.idAcquire();
                }
                target = mouseState.viewDown->ownerZFUIView;

                event->mouseId = mouseState.mouseId;
                event->mouseAction = ZFUIMouseAction::e_MouseMove;
                event->mousePoint.x = (zffloat)sdlEvent->motion.x;
                event->mousePoint.y = (zffloat)sdlEvent->motion.y;
                this->posFromGlobal(event->mousePoint.x, event->mousePoint.y);
            }
            else
            { // mouse hover
                ZFImpl_sys_SDL_View *viewHover = this->mouseHoverTestGlobal(sdlEvent->motion.x, sdlEvent->motion.y);
                if(mouseState.viewHover != viewHover)
                {
                    if(mouseState.viewHover != zfnull)
                    {
                        if(mouseState.viewHover->ownerZFUIView != zfnull && mouseState.mouseId != zfidentityInvalid())
                        {
                            zfblockedAlloc(ZFUIMouseEvent, hoverExit);
                            hoverExit->mouseId = mouseState.mouseId;
                            hoverExit->mouseAction = ZFUIMouseAction::e_MouseHoverExit;
                            hoverExit->mousePoint.x = (zffloat)sdlEvent->motion.x;
                            hoverExit->mousePoint.y = (zffloat)sdlEvent->motion.y;
                            this->posFromGlobal(hoverExit->mousePoint.x, hoverExit->mousePoint.y);
                            ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(mouseState.viewHover->ownerZFUIView, hoverExit);
                        }
                        mouseState.viewHover = zfnull;
                    }
                    if(mouseState.mouseId != zfidentityInvalid())
                    {
                        this->sysWindow->mouseIdGen.idRelease(mouseState.mouseId);
                        mouseState.mouseId = zfidentityInvalid();
                    }
                    if(viewHover != zfnull && viewHover->ownerZFUIView != zfnull)
                    {
                        mouseState.viewHover = viewHover;

                        mouseState.mouseId = this->sysWindow->mouseIdGen.idAcquire();
                        zfblockedAlloc(ZFUIMouseEvent, hoverEnter);
                        hoverEnter->mouseId = mouseState.mouseId;
                        hoverEnter->mouseAction = ZFUIMouseAction::e_MouseHoverEnter;
                        hoverEnter->mousePoint.x = (zffloat)sdlEvent->motion.x;
                        hoverEnter->mousePoint.y = (zffloat)sdlEvent->motion.y;
                        this->posFromGlobal(hoverEnter->mousePoint.x, hoverEnter->mousePoint.y);
                        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(viewHover->ownerZFUIView, hoverEnter);
                    }
                }

                if(mouseState.viewHover != zfnull && mouseState.viewHover->ownerZFUIView != zfnull)
                {
                    zfCoreAssert(mouseState.mouseId != zfidentityInvalid());
                    target = mouseState.viewHover->ownerZFUIView;

                    event->mouseId = mouseState.mouseId;
                    event->mouseAction = ZFUIMouseAction::e_MouseHover;
                    event->mousePoint.x = (zffloat)sdlEvent->motion.x;
                    event->mousePoint.y = (zffloat)sdlEvent->motion.y;
                    this->posFromGlobal(event->mousePoint.x, event->mousePoint.y);
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            ZFImpl_sys_SDL_MouseState &mouseState = this->sysWindow->mouseState(sdlEvent->motion.which, sdlEvent->button.button);
            if(sdlEvent->type == SDL_MOUSEBUTTONDOWN)
            {
                ZFImpl_sys_SDL_View *viewDown = this->mouseTestGlobal(sdlEvent->motion.x, sdlEvent->motion.y);
                if(viewDown != zfnull && mouseState.viewDown == viewDown)
                {
                    return;
                }

                if(mouseState.mouseId != zfidentityInvalid())
                {
                    this->sysWindow->mouseIdGen.idRelease(mouseState.mouseId);
                    mouseState.mouseId = zfidentityInvalid();
                }
                if(viewDown == zfnull || viewDown->ownerZFUIView == zfnull)
                {
                    return;
                }

                target = viewDown->ownerZFUIView;
                mouseState.viewDown = viewDown;
                mouseState.mouseId = this->sysWindow->mouseIdGen.idAcquire();
                event->mouseId = mouseState.mouseId;
                event->mouseAction = ZFUIMouseAction::e_MouseDown;
            }
            else
            {
                zfidentity mouseIdPrev = mouseState.mouseId;
                ZFImpl_sys_SDL_View *viewDownPrev = mouseState.viewDown;
                if(mouseState.mouseId != zfidentityInvalid())
                {
                    this->sysWindow->mouseIdGen.idRelease(mouseState.mouseId);
                    mouseState.mouseId = zfidentityInvalid();
                }
                mouseState.viewDown = zfnull;

                if(mouseIdPrev != zfidentityInvalid() && viewDownPrev != zfnull)
                {
                    target = viewDownPrev->ownerZFUIView;
                    event->mouseId = mouseIdPrev;
                    event->mouseAction = ZFUIMouseAction::e_MouseUp;
                }
            }
            event->mousePoint.x = (zffloat)sdlEvent->button.x;
            event->mousePoint.y = (zffloat)sdlEvent->button.y;
            break;
        }
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    if(target != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(target, event);
    }
}
void ZFImpl_sys_SDL_View::dispatchWheelEvent(ZF_IN SDL_Event *sdlEvent)
{
    if(this->sdlEventCallback != zfnull && this->sdlEventCallback(this, sdlEvent)) {return;}
    zfCoreAssert(sdlEvent->type == SDL_MOUSEWHEEL);
    ZFUIView *target = zfnull;
    if(this->sysWindow != zfnull)
    {
        if(this->sysWindow->viewFocused != zfnull)
        {
            target = this->sysWindow->viewFocused->ownerZFUIView;
        }
        else
        {
            target = this->ownerZFUIView;
        }
    }
    if(target != zfnull)
    {
        zfblockedAlloc(ZFUIWheelEvent, event);
        event->wheelX = (zffloat)sdlEvent->wheel.x;
        event->wheelY = (zffloat)sdlEvent->wheel.y;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(target, event);
    }
}
void ZFImpl_sys_SDL_View::dispatchKeyEvent(ZF_IN SDL_Event *sdlEvent)
{
    if(this->sdlEventCallback != zfnull && this->sdlEventCallback(this, sdlEvent)) {return;}
    ZFUIView *target = zfnull;
    if(this->sysWindow != zfnull)
    {
        if(this->sysWindow->viewFocused != zfnull)
        {
            target = this->sysWindow->viewFocused->ownerZFUIView;
        }
        else
        {
            target = this->ownerZFUIView;
        }
    }
    if(target != zfnull)
    {
        zfblockedAlloc(ZFUIKeyEvent, event);
        event->keyId = zfidentityHash(sdlEvent->key.windowID, sdlEvent->key.keysym.sym, sdlEvent->key.keysym.scancode);
        if(sdlEvent->key.repeat)
        {
            event->keyAction = ZFUIKeyAction::e_KeyRepeat;
        }
        else if(sdlEvent->type == SDL_KEYDOWN)
        {
            event->keyAction = ZFUIKeyAction::e_KeyDown;
        }
        else if(sdlEvent->type == SDL_KEYUP)
        {
            event->keyAction = ZFUIKeyAction::e_KeyUp;
        }
        else
        {
            zfCoreCriticalShouldNotGoHere();
        }
        event->keyCode = ZFImpl_sys_SDL_ZFUIKeyCodeFromRaw((SDL_KeyCode)sdlEvent->key.keysym.sym);
        event->keyCodeRaw = sdlEvent->key.keysym.sym;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(target, event);
    }
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

