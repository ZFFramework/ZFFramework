#ifndef _ZFI_ZFImpl_sys_SDL_View_h_
#define _ZFI_ZFImpl_sys_SDL_View_h_

#include "ZFImpl_sys_SDL_TextureCache.h"
#include "ZFUIKit/ZFUIView.h"
#include "ZFImpl/sys_SDL/ZFMainEntry_sys_SDL.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief extra transform for view or animation transform */
zfclassNotPOD ZFImpl_sys_SDL_Transform {
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

zfclassFwd ZFImpl_sys_SDL_RootWindow;
/**
 * @brief native view impl for SDL
 */
zfclassNotPOD ZFLIB_ZFUIKit_impl ZFImpl_sys_SDL_View {
public:
    /**
     * @brief callback for impl to render contents
     *
     * childRect and parentRect relative to root renderer\n
     * return true if the impl has done all necessary render task,
     * no child should be rendered any more
     */
    typedef zfbool (*RenderCallback)(
            ZF_IN SDL_Renderer *renderer
            , ZF_IN ZFImpl_sys_SDL_View *nativeView
            , ZF_IN const SDL_FRect &childRect
            , ZF_IN const SDL_FRect &parentRect
            , ZF_IN zffloat treeAlpha
            );

    enum {
        /** @brief mouse event with this x/y value, would be treated as mouse cancel event */
        MouseCancel = -9999,
    };

public:
    /** @brief owner root window, valid when added to window */
    ZFImpl_sys_SDL_RootWindow *rootWindow;
    /** @brief owner view, null for #ZFUIView::nativeImplView */
    ZFUIView *ownerZFUIView;
    /** @brief parent */
    ZFImpl_sys_SDL_View *parent;
    /** @brief native rect, relative to parent */
    SDL_FRect rect;
    /** @brief callback for impl to render contents */
    RenderCallback renderImpl;
    /** @brief whether #renderRequest called and not rendered */
    zfbool renderRequested;
    /**
     * @brief whether renderCache valid and no need to redraw,
     *   reset when #renderRequest and #renderCacheRemove
     */
    zfbool renderCacheValid;
    /**
     * @brief whether this view require to build renderCache
     *
     * when not 0, renderCache would be created and updated if need\n
     * note renderCache is forced on when the view has transform or animation\n
     * renderCacheRequired would be reset during #resetForCache
     */
    zft_zfuint16 renderCacheRequired;
    /** @brief render cache, used for transform and animation */
    ZFImpl_sys_SDL_TextureCache renderCache;
    /** @brief extra transform for view */
    ZFImpl_sys_SDL_Transform *viewTransform;
    /** @brief extra transform for animation impl */
    ZFImpl_sys_SDL_Transform *aniTransform;
    /** @brief all children */
    ZFCoreArray<ZFImpl_sys_SDL_View *> children;

    /**
     * @brief extra callback for impl to achieve custom mouse event dispatch logic
     *
     * the dispatch logic:
     * -# during mouseTest, called with null sdlEvent for each sdl view,
     *   to check whether the view want to grab mouse event
     * -# if a sdl view grabbed mouse down event,
     *   all later events would be sent to that view's sdlMouseGrabCallback directly,
     *   until next mouse down event
     */
    zfbool (*sdlMouseGrabCallback)(
            ZF_IN ZFImpl_sys_SDL_View *owner
            , ZF_IN SDL_Event *sdlEvent
            , ZF_IN zfint x
            , ZF_IN zfint y
            );
    /** @brief extra callback for impl to achieve custom measure logic */
    void (*sdlMeasureCallback)(
            ZF_OUT ZFUISize &ret
            , ZF_IN ZFImpl_sys_SDL_View *owner
            , ZF_IN const ZFUISize &sizeHint
            );

public:
    /** @brief for impl to check class type */
    virtual const void *implType(void) {
        return ZFUIView::ClassData();
    }

public:
    /** @brief attached to rootWindow */
    void rootWindowAttach(ZF_IN ZFImpl_sys_SDL_RootWindow *rootWindow);
    /** @brief detached from rootWindow */
    void rootWindowDetach(void);

    /** @brief child attached */
    void childAttach(
            ZF_IN zfindex index
            , ZF_IN ZFImpl_sys_SDL_View *child
            );
    /** @brief child detached */
    void childDetach(ZF_IN zfindex index);

public:
    /** @brief schedule render */
    void renderRequest(void);
    /** @brief schedule layout */
    void layoutRequest(void);

public:
    /**
     * @brief util for impl to calc render rect
     *
     * childRect and parentRect relative to root renderer
     */
    static void renderRectCalc(
            ZF_OUT SDL_FRect &ret
            , ZF_IN const SDL_FRect &childRect
            , ZF_IN const SDL_FRect &parentRect
            ) {
        ret.x = childRect.x >= parentRect.x
            ? childRect.x
            : parentRect.x;
        ret.y = childRect.y >= parentRect.y
            ? childRect.y
            : parentRect.y;
        ret.w = childRect.x + childRect.w <= parentRect.x + parentRect.w
            ? childRect.x + childRect.w - ret.x
            : parentRect.x + parentRect.w - ret.x;
        ret.h = childRect.y + childRect.h <= parentRect.y + parentRect.h
            ? childRect.y + childRect.h - ret.y
            : parentRect.y + parentRect.h - ret.y;
    }

    /**
     * @brief render the view to the target
     *
     * childRect and parentRect relative to root renderer
     */
    void render(
            ZF_IN SDL_Renderer *renderer
            , ZF_IN const SDL_FRect &childRect
            , ZF_IN const SDL_FRect &parentRect
            , ZF_IN zffloat treeAlpha
            );

    /**
     * @brief find child under mouse event, xy indicates offset relative to this view
     *
     * when mouseGrab is not null, #sdlMouseGrabCallback would be called to check
     * whether the sdl view want to grab the mouse,
     * and the grab test result would be stored to mouseGrab
     */
    ZFImpl_sys_SDL_View *mouseTest(
            ZF_IN int x
            , ZF_IN int y
            , ZF_OUT_OPT zfbool *mouseGrab = zfnull
            );
    /** @brief find child under mouse event, xy indicates offset relative to window */
    ZFImpl_sys_SDL_View *mouseTestGlobal(
            ZF_IN int xGlobal
            , ZF_IN int yGlobal
            , ZF_OUT_OPT zfbool *mouseGrab = zfnull
            ) {
        this->posFromGlobal(xGlobal, yGlobal);
        return this->mouseTest(xGlobal, yGlobal, mouseGrab);
    }

    /** @brief find child under mouse event, xy indicates offset relative to this view */
    ZFImpl_sys_SDL_View *mouseHoverTest(
            ZF_IN int x
            , ZF_IN int y
            );
    /** @brief find child under mouse event, xy indicates offset relative to window */
    ZFImpl_sys_SDL_View *mouseHoverTestGlobal(
            ZF_IN int xGlobal
            , ZF_IN int yGlobal
            ) {
        this->posFromGlobal(xGlobal, yGlobal);
        return this->mouseHoverTest(xGlobal, yGlobal);
    }

    /** @brief get pos of this view's origin relative to window */
    void posOnWindow(
            ZF_OUT int &x
            , ZF_OUT int &y
            );
    /** @brief convert pos from local to window */
    template<typename T_int>
    void posToGlobal(
            ZF_IN_OUT T_int &x
            , ZF_IN_OUT T_int &y
            ) {
        int relX, relY;
        this->posOnWindow(relX, relY);
        x += (T_int)relX;
        y += (T_int)relY;
    }
    /** @brief convert pos from window to local */
    template<typename T_int>
    void posFromGlobal(
            ZF_IN_OUT T_int &x
            , ZF_IN_OUT T_int &y
            ) {
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
    void resetForCache(void) {
        this->renderCacheValid = zffalse;
        this->renderCacheRequired = 0;
        this->viewTransformRemove();
        this->aniTransformRemove();
        this->rootWindow = zfnull;
        this->ownerZFUIView = zfnull;
        this->parent = zfnull;
        this->rect.x = this->rect.y = this->rect.w = this->rect.h = 0;
        this->renderImpl = zfnull;
        this->children.removeAll();
        this->renderRequested = zftrue;
        this->sdlMouseGrabCallback = zfnull;
        this->sdlMeasureCallback = zfnull;
    }

    /** @brief prepare viewTransform */
    void viewTransformPrepare(void) {
        if(this->viewTransform == zfnull) {
            this->viewTransform = zfpoolNew(ZFImpl_sys_SDL_Transform);
        }
    }

    /** @brief remove viewTransform */
    void viewTransformRemove(void) {
        if(this->viewTransform != zfnull) {
            zfpoolDelete(this->viewTransform);
            this->viewTransform = zfnull;
        }
    }

    /** @brief prepare aniTransform */
    void aniTransformPrepare(void) {
        if(this->aniTransform == zfnull) {
            this->aniTransform = zfpoolNew(ZFImpl_sys_SDL_Transform);
        }
    }

    /** @brief remove aniTransform */
    void aniTransformRemove(void) {
        if(this->aniTransform != zfnull) {
            zfpoolDelete(this->aniTransform);
            this->aniTransform = zfnull;
        }
    }

    /** @cond ZFPrivateDoc */
public:
    ZFImpl_sys_SDL_View(void)
    : rootWindow(zfnull)
    , ownerZFUIView(zfnull)
    , parent(zfnull)
    , rect()
    , renderImpl()
    , viewTransform(zfnull)
    , aniTransform(zfnull)
    , renderRequested(zftrue)
    , renderCacheValid(zffalse)
    , renderCacheRequired(0)
    , renderCache()
    , children()
    , sdlMouseGrabCallback(zfnull)
    , sdlMeasureCallback(zfnull)
    {
    }
    virtual ~ZFImpl_sys_SDL_View(void) {
        this->viewTransformRemove();
        this->aniTransformRemove();
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_View_h_

