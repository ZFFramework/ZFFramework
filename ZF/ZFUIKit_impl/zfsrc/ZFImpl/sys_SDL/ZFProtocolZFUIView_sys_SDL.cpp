#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_SDL, ZFUIView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUISysWindow, "ZFImpl_sys_SDL_SysWindow")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    static void renderCallback(ZF_IN SDL_Renderer *renderer, ZF_IN ZFImpl_sys_SDL_View *nativeView,
                               ZF_IN int viewX, ZF_IN int viewY)
    {
        if(nativeView->ownerZFUIView != zfnull)
        {
            ZFUIColor bg = nativeView->ownerZFUIView->viewBackgroundColor();
            if(ZFUIColorGetA(bg) != 0)
            {
                Uint8 rOld, gOld, bOld, aOld;
                SDL_GetRenderDrawColor(renderer, &rOld, &gOld, &bOld, &aOld);
                SDL_SetRenderDrawColor(renderer
                        , 255 * ZFUIColorGetR(bg), 255 * ZFUIColorGetG(bg), 255 * ZFUIColorGetB(bg)
                        , 255 * nativeView->ownerZFUIView->viewAlpha()
                    );
                SDL_Rect rect;
                rect.x = viewX;
                rect.y = viewY;
                rect.w = nativeView->rect.w;
                rect.h = nativeView->rect.h;
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, rOld, gOld, bOld, aOld);
            }
        }
    }

public:
    virtual zfbool nativeViewCacheOnSave(ZF_IN void *nativeView)
    {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        nativeViewTmp->resetForCache();
        return zftrue;
    }
    virtual void nativeViewCacheOnRestore(ZF_IN ZFUIView *view,
                                          ZF_IN void *nativeView)
    {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        nativeViewTmp->ownerZFUIView = view;
        nativeViewTmp->renderImpls.add(zfself::renderCallback);
    }
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        ZFImpl_sys_SDL_View *nativeView = zfnew(ZFImpl_sys_SDL_View);
        nativeView->ownerZFUIView = view;
        nativeView->renderImpls.add(zfself::renderCallback);
        return nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView)
    {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        zfdelete(nativeViewTmp);
    }

    virtual zfbool nativeImplViewRequireVirtualIndex(void)
    {
        return zffalse;
    }
    virtual void nativeImplView(ZF_IN ZFUIView *view,
                                ZF_IN void *nativeImplViewOld,
                                ZF_IN void *nativeImplView,
                                ZF_IN zfindex virtualIndex)
    {
        // use ZFImpl_sys_SDL_View::renderImpls instead
        if(nativeImplView != zfnull)
        {
            zfCoreCriticalShouldNotGoHere();
        }
    }
    virtual void nativeImplViewFrame(ZF_IN ZFUIView *view,
                                     ZF_IN const ZFUIRect &rect)
    {
        // nothing to do
    }
    virtual zffloat UIScaleForImpl(ZF_IN void *nativeView)
    {
        return 1;
    }
    virtual zffloat UIScaleForPixel(ZF_IN void *nativeView)
    {
        return 1;
    }

    // ============================================================
    // properties
public:
    virtual void viewVisible(ZF_IN ZFUIView *view,
                             ZF_IN zfbool viewVisible)
    {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeViewTmp->sysWindow != zfnull)
        {
            nativeViewTmp->sysWindow->renderRequest();
        }
    }
    virtual void viewAlpha(ZF_IN ZFUIView *view,
                           ZF_IN zffloat viewAlpha)
    {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeViewTmp->sysWindow != zfnull)
        {
            nativeViewTmp->sysWindow->renderRequest();
        }
    }
    virtual void viewUIEnable(ZF_IN ZFUIView *view,
                              ZF_IN zfbool viewUIEnable)
    {
        // nothing to do
    }
    virtual void viewUIEnableTree(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewUIEnableTree)
    {
        // nothing to do
    }
    virtual void viewBackgroundColor(ZF_IN ZFUIView *view,
                                     ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeViewTmp->sysWindow != zfnull)
        {
            nativeViewTmp->sysWindow->renderRequest();
        }
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        ZFImpl_sys_SDL_View *nativeParent = (ZFImpl_sys_SDL_View *)parent->nativeView();
        ZFImpl_sys_SDL_View *nativeChild = (ZFImpl_sys_SDL_View *)child->nativeView();
        nativeParent->childAttach(virtualIndex, nativeChild);
        if(nativeParent->sysWindow != zfnull)
        {
            nativeParent->sysWindow->layoutRequest();
            nativeParent->sysWindow->renderRequest();
        }
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        ZFImpl_sys_SDL_View *nativeParent = (ZFImpl_sys_SDL_View *)parent->nativeView();
        nativeParent->childDetach(virtualIndex);
        if(nativeParent->sysWindow != zfnull)
        {
            nativeParent->sysWindow->layoutRequest();
            nativeParent->sysWindow->renderRequest();
        }
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent)
    {
        ZFImpl_sys_SDL_View *nativeParent = (ZFImpl_sys_SDL_View *)parent->nativeView();
        nativeParent->children.removeAll();
    }

public:
    virtual void viewFrame(ZF_IN ZFUIView *view,
                           ZF_IN const ZFUIRect &rect)
    {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        nativeView->rect.x = (int)rect.x;
        nativeView->rect.y = (int)rect.y;
        nativeView->rect.w = (int)rect.width;
        nativeView->rect.h = (int)rect.height;
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeView->sysWindow != zfnull)
        {
            nativeView->sysWindow->layoutRequest();
        }
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint)
    {
        ZFImpl_sys_SDL_View *nativeViewTmp = (ZFImpl_sys_SDL_View *)nativeView;
        if(nativeViewTmp != zfnull && nativeViewTmp->sdlMeasureCallback != zfnull)
        {
            nativeViewTmp->sdlMeasureCallback(ret, nativeViewTmp, sizeHint);
        }
        else
        {
            ret = ZFUISizeMake(zfmMax(sizeHint.width, (zffloat)0), zfmMax(sizeHint.height, (zffloat)0));
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_SDL)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

