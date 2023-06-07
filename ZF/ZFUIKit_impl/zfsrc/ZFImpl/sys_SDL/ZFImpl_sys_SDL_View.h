#ifndef _ZFI_ZFImpl_sys_SDL_View_h_
#define _ZFI_ZFImpl_sys_SDL_View_h_

#include "../ZFImpl_ZFUIKit_impl.h"
#include "ZFImpl/sys_SDL/ZFImpl_sys_SDL_ZF_impl.h"
#include "ZFUIKit/ZFUIView.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl/sys_SDL/ZFMainEntry_sys_SDL.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief extra transform for view or animation transform */
zfclassNotPOD ZFImpl_sys_SDL_Transform
{
public:
    zffloat translateX; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat translateY; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat translateZ; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat scaleX; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat scaleY; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat scaleZ; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat rotateX; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat rotateY; /**< @brief see #ZFImpl_sys_SDL_Transform */
    zffloat rotateZ; /**< @brief see #ZFImpl_sys_SDL_Transform */
public:
    /** @cond ZFPrivateDoc */
    ZFImpl_sys_SDL_Transform(void)
    : translateX(0)
    , translateY(0)
    , translateZ(0)
    , scaleX(1)
    , scaleY(1)
    , scaleZ(1)
    , rotateX(0)
    , rotateY(0)
    , rotateZ(0)
    {
    }
    /** @endcond */
};

zfclassFwd ZFImpl_sys_SDL_SysWindow;
/**
 * @brief native view impl for SDL
 */
zfclassNotPOD ZFLIB_ZFUIKit_impl ZFImpl_sys_SDL_View
{
public:
    /** @brief callback for impl to render contents */
    typedef void (*RenderCallback)(ZF_IN SDL_Renderer *renderer, ZF_IN ZFImpl_sys_SDL_View *nativeView,
                                   ZF_IN int viewX, ZF_IN int viewY);

public:
    /** @brief owner sys window, valid when added to window */
    ZFImpl_sys_SDL_SysWindow *sysWindow;
    /** @brief owner view, null for #ZFUIView::nativeImplView */
    ZFUIView *ownerZFUIView;
    /** @brief parent */
    ZFImpl_sys_SDL_View *parent;
    /** @brief native rect, relative to parent */
    SDL_Rect rect;
    /** @brief callback for impl to render contents */
    ZFCoreArrayPOD<RenderCallback> renderImpls;
    /** @brief render cache, used for transform and animation */
    SDL_Texture *renderCache;
    /** @brief extra transform for view */
    ZFImpl_sys_SDL_Transform *viewTransform;
    /** @brief extra transform for animation impl */
    ZFImpl_sys_SDL_Transform *aniTransform;
    /** @brief all children */
    ZFCoreArray<ZFImpl_sys_SDL_View *> children;

    /** @brief extra callback for impl to achieve custom event dispatch logic */
    zfbool (*sdlEventCallback)(ZF_IN ZFImpl_sys_SDL_View *owner,
                               ZF_IN SDL_Event *sdlEvent);
    /** @brief extra callback for impl to achieve custom measure logic */
    void (*sdlMeasureCallback)(ZF_OUT ZFUISize &ret,
                               ZF_IN ZFImpl_sys_SDL_View *owner,
                               ZF_IN const ZFUISize &sizeHint);

public:
    /** @brief attached to sysWindow */
    void sysWindowAttach(ZF_IN ZFImpl_sys_SDL_SysWindow *sysWindow);
    /** @brief detached from sysWindow */
    void sysWindowDetach(void);

    /** @brief child attached */
    void childAttach(ZF_IN zfindex index, ZF_IN ZFImpl_sys_SDL_View *child);
    /** @brief child detached */
    void childDetach(ZF_IN zfindex index);

public:
    /** @brief render the view to the target */
    void render(ZF_IN SDL_Renderer *renderer, ZF_IN int viewX, ZF_IN int viewY);

    /** @brief find child under mouse event, xy indicates offset relative to this view */
    ZFImpl_sys_SDL_View *mouseTest(ZF_IN int x, ZF_IN int y);
    /** @brief find child under mouse event, xy indicates offset relative to window */
    ZFImpl_sys_SDL_View *mouseTestGlobal(ZF_IN int x, ZF_IN int y)
    {
        this->posFromGlobal(x, y);
        return this->mouseTest(x, y);
    }

    /** @brief find child under mouse event, xy indicates offset relative to this view */
    ZFImpl_sys_SDL_View *mouseHoverTest(ZF_IN int x, ZF_IN int y);
    /** @brief find child under mouse event, xy indicates offset relative to window */
    ZFImpl_sys_SDL_View *mouseHoverTestGlobal(ZF_IN int x, ZF_IN int y)
    {
        this->posFromGlobal(x, y);
        return this->mouseHoverTest(x, y);
    }

    /** @brief get pos of this view's origin relative to window */
    void posOnWindow(ZF_OUT int &x, ZF_OUT int &y);
    /** @brief convert pos from local to window */
    template<typename T_int>
    void posToGlobal(ZF_IN_OUT T_int &x, ZF_IN_OUT T_int &y)
    {
        int relX, relY;
        this->posOnWindow(relX, relY);
        x += (T_int)relX;
        y += (T_int)relY;
    }
    /** @brief convert pos from window to local */
    template<typename T_int>
    void posFromGlobal(ZF_IN_OUT T_int &x, ZF_IN_OUT T_int &y)
    {
        int relX, relY;
        this->posOnWindow(relX, relY);
        x -= (T_int)relX;
        y -= (T_int)relY;
    }
    /** @brief dispatch mouse event */
    void dispatchMouseEvent(ZF_IN SDL_Event *sdlEvent);
    /** @brief dispatch wheel event */
    void dispatchWheelEvent(ZF_IN SDL_Event *sdlEvent);
    /** @brief dispatch key event */
    void dispatchKeyEvent(ZF_IN SDL_Event *sdlEvent);

public:
    /** @brief reset to prepare for cache */
    void resetForCache(void)
    {
        this->renderCacheRemove();
        this->viewTransformRemove();
        this->aniTransformRemove();
        this->sysWindow = zfnull;
        this->ownerZFUIView = zfnull;
        this->parent = zfnull;
        this->renderImpls.removeAll();
        this->children.removeAll();
    }

    /** @brief prepare renderCache */
    void renderCachePrepare(ZF_IN SDL_Renderer *renderer);

    /** @brief remove renderCache */
    void renderCacheRemove(void)
    {
        if(this->renderCache != zfnull)
        {
            SDL_DestroyTexture(this->renderCache);
            this->renderCache = zfnull;
        }
    }

    /** @brief prepare viewTransform */
    void viewTransformPrepare(void)
    {
        if(this->viewTransform == zfnull)
        {
            this->viewTransform = zfpoolNew(ZFImpl_sys_SDL_Transform);
        }
    }

    /** @brief remove viewTransform */
    void viewTransformRemove(void)
    {
        if(this->viewTransform != zfnull)
        {
            zfpoolDelete(this->viewTransform);
            this->viewTransform = zfnull;
        }
    }

    /** @brief prepare aniTransform */
    void aniTransformPrepare(void)
    {
        if(this->aniTransform == zfnull)
        {
            this->aniTransform = zfpoolNew(ZFImpl_sys_SDL_Transform);
        }
    }

    /** @brief remove aniTransform */
    void aniTransformRemove(void)
    {
        if(this->aniTransform != zfnull)
        {
            zfpoolDelete(this->aniTransform);
            this->aniTransform = zfnull;
        }
    }

    /** @cond ZFPrivateDoc */
public:
    ZFImpl_sys_SDL_View(void)
    : sysWindow(zfnull)
    , ownerZFUIView(zfnull)
    , parent(zfnull)
    , rect()
    , renderImpls()
    , viewTransform(zfnull)
    , aniTransform(zfnull)
    , renderCache(zfnull)
    , children()
    , sdlEventCallback(zfnull)
    , sdlMeasureCallback(zfnull)
    {
    }
    ~ZFImpl_sys_SDL_View(void)
    {
        this->resetForCache();
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_View_h_

