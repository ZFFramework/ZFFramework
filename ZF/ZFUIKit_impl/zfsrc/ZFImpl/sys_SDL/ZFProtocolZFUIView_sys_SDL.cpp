#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_SDL, ZFUIView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUISysWindow, "ZFImpl_sys_SDL_SysWindow")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    static zfbool renderCallback(
            ZF_IN SDL_Renderer *renderer
            , ZF_IN ZFImpl_sys_SDL_View *nativeView
            , ZF_IN const SDL_FRect &childRect
            , ZF_IN const SDL_FRect &parentRect
            , ZF_IN zffloat treeAlpha
            ) {
        if(nativeView->ownerZFUIView != zfnull) {
            ZFUIColor bg = nativeView->ownerZFUIView->bgColor();
            if(ZFUIColorGetA(bg) != 0) {
                Uint8 rOld, gOld, bOld, aOld;
                SDL_GetRenderDrawColor(renderer, &rOld, &gOld, &bOld, &aOld);
                SDL_SetRenderDrawColor(renderer
                        , (Uint8)(0xFF * ZFUIColorGetR(bg))
                        , (Uint8)(0xFF * ZFUIColorGetG(bg))
                        , (Uint8)(0xFF * ZFUIColorGetB(bg))
                        , (Uint8)(0xFF * (ZFUIColorGetA(bg) * treeAlpha))
                    );
                SDL_FRect rect;
                ZFImpl_sys_SDL_View::renderRectCalc(rect, childRect, parentRect);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, rOld, gOld, bOld, aOld);
            }
        }
        return zffalse;
    }

public:
    virtual zfbool nativeViewCacheOnSave(ZF_IN void *nativeView) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        nativeViewTmp->resetForCache();
        return zftrue;
    }
    virtual void nativeViewCacheOnRestore(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeView
            ) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        nativeViewTmp->ownerZFUIView = view;
        nativeViewTmp->renderImpls.add(zfself::renderCallback);
    }
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view) {
        ZFImpl_sys_SDL_View *nativeView = zfnew(ZFImpl_sys_SDL_View);
        nativeView->ownerZFUIView = view;
        nativeView->renderImpls.add(zfself::renderCallback);
        return nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        zfdelete(nativeViewTmp);
    }

    virtual void nativeImplView(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeImplViewOld
            , ZF_IN void *nativeImplView
            , ZF_IN zfindex virtualIndex
            , ZF_IN zfbool nativeImplViewRequireVirtualIndex
            ) {
        // support, but recommended to use ZFImpl_sys_SDL_View::renderImpls instead
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeImplViewOld != zfnull) {
            nativeView->childDetach(virtualIndex);
        }
        if(nativeImplViewRequireVirtualIndex && nativeImplView != zfnull) {
            ZFImpl_sys_SDL_View *nativeImplViewTmp = (ZFImpl_sys_SDL_View *)nativeImplView;
            nativeView->childAttach(virtualIndex, nativeImplViewTmp);
        }
    }
    virtual void nativeImplViewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        ZFImpl_sys_SDL_View *nativeImplView = (ZFImpl_sys_SDL_View *)view->nativeImplView();
        if(nativeImplView != zfnull) {
            ZFUIRectRound(
                    nativeImplView->rect.x
                    , nativeImplView->rect.y
                    , nativeImplView->rect.w
                    , nativeImplView->rect.h
                    , rect
                    );
            nativeImplView->layoutRequest();
        }
    }
    virtual zffloat UIScaleForImpl(ZF_IN void *nativeView) {
        return this->UIScaleForPixel(nativeView);
    }
    virtual zffloat UIScaleForPixel(ZF_IN void *nativeView) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        return (zffloat)SDL_GetWindowDisplayScale(nativeViewTmp->sysWindow->sdlWindow);
    }

    // ============================================================
    // properties
public:
    virtual void visible(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool visible
            ) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)view->nativeView();
        nativeViewTmp->renderRequest();
    }
    virtual void alpha(
            ZF_IN ZFUIView *view
            , ZF_IN zffloat alpha
            ) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)view->nativeView();
        nativeViewTmp->renderRequest();
    }
    virtual void viewUIEnable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnable
            ) {
        // nothing to do
    }
    virtual void viewUIEnableTree(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnableTree
            ) {
        // nothing to do
    }
    virtual void bgColor(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIColor &bgColor
            ) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)view->nativeView();
        nativeViewTmp->renderRequest();
    }

public:
    virtual void child(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        ZFImpl_sys_SDL_View *nativeParent = (ZFImpl_sys_SDL_View *)parent->nativeView();
        ZFImpl_sys_SDL_View *nativeChild = (ZFImpl_sys_SDL_View *)child->nativeView();
        nativeParent->childAttach(virtualIndex, nativeChild);
        nativeParent->layoutRequest();
    }
    virtual void childRemove(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        ZFImpl_sys_SDL_View *nativeParent = (ZFImpl_sys_SDL_View *)parent->nativeView();
        nativeParent->childDetach(virtualIndex);
        nativeParent->layoutRequest();
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent) {
        ZFImpl_sys_SDL_View *nativeParent = (ZFImpl_sys_SDL_View *)parent->nativeView();
        ZFImpl_sys_SDL_View *nativeImplView = (ZFImpl_sys_SDL_View *)parent->nativeImplView();
        if(!(
                    nativeParent->children.isEmpty()
                    || (nativeImplView != NULL && nativeParent->children.count() == 1)
                    )) {
            for(zfindex i = 0; i < nativeParent->children.count(); ++i) {
                ZFImpl_sys_SDL_View *nativeChild = nativeParent->children[i];
                if(nativeChild != nativeImplView) {
                    nativeChild->parent = zfnull;
                    if(nativeChild->sysWindow != zfnull) {
                        nativeChild->sysWindowDetach();
                    }
                }
            }
            nativeParent->children.removeAll();
            nativeParent->children.add(nativeImplView);
        }
    }

public:
    virtual void viewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        ZFUIRectRound(
                nativeView->rect.x
                , nativeView->rect.y
                , nativeView->rect.w
                , nativeView->rect.h
                , rect
                );
        nativeView->layoutRequest();
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        nativeView->layoutRequest();
    }

    virtual void measureNativeView(
            ZF_OUT ZFUISize &ret
            , ZF_IN void *nativeView
            , ZF_IN const ZFUISize &sizeHint
            ) {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        if(nativeViewTmp != zfnull && nativeViewTmp->sdlMeasureCallback != zfnull) {
            nativeViewTmp->sdlMeasureCallback(ret, nativeViewTmp, sizeHint);
        }
        else {
            ret = ZFUISizeCreate(zfmMax(sizeHint.width, (zffloat)0), zfmMax(sizeHint.height, (zffloat)0));
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

