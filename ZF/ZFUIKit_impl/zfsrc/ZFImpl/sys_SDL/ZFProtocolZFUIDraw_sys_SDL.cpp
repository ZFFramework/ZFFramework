#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIDraw.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl_sys_SDL_Image.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUIDrawImpl_sys_SDL : zfextend ZFImpl_sys_SDL_View {

public:
    zfoverride
    virtual const void *implType(void) {
        return ZFUIDrawableView::ClassData();
    }

public:
    typedef enum {
        Invalid,
        View,
        Image,
    } Type;

public:
    Type type;
    SDL_Renderer *sdlRenderer;
    SDL_Rect framePixel;
    zffloat treeAlpha;
    SDL_Surface *nativeImage;

public:
    _ZFP_ZFUIDrawImpl_sys_SDL(void)
    : type(Invalid)
    , sdlRenderer(zfnull)
    , framePixel()
    , treeAlpha(1)
    , nativeImage(zfnull)
    {
    }
};

// ============================================================
// ZFUIDrawForView
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForViewImpl_sys_SDL, ZFUIDrawForView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeDrawableViewCreate(
            ZF_IN ZFUIDrawableView *drawableView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)drawableView->nativeView();
        nativeView->renderImpls.add(zfself::_renderCallback);

        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = zfnew(_ZFP_ZFUIDrawImpl_sys_SDL);
        return (void *)(ZFImpl_sys_SDL_View *)drawImpl;
    }
    virtual void nativeDrawableViewDestroy(
            ZF_IN ZFUIDrawableView *drawableView
            , ZF_IN void *nativeDrawableView
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)drawableView->nativeView();
        nativeView->renderImpls.removeElement(zfself::_renderCallback);

        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)(ZFImpl_sys_SDL_View *)nativeDrawableView;
        zfdelete(drawImpl);
    }

    virtual void drawRequest(ZF_IN ZFUIDrawableView *drawableView) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)drawableView->nativeView();
        nativeView->renderRequest();
    }

public:
    virtual zfbool beginForView(ZF_IN_OUT ZFUIDrawToken &token) {
        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)(ZFImpl_sys_SDL_View *)token.target->to<ZFUIView *>()->nativeImplView();
        drawImpl->type = _ZFP_ZFUIDrawImpl_sys_SDL::View;
        token.impl = drawImpl;
        return zftrue;
    }
    virtual void endForView(ZF_IN_OUT ZFUIDrawToken &token) {
        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)(ZFImpl_sys_SDL_View *)token.target->to<ZFUIDrawableView *>()->nativeImplView();
        drawImpl->type = _ZFP_ZFUIDrawImpl_sys_SDL::Invalid;
    }
private:
    static zfbool _renderCallback(
            ZF_IN SDL_Renderer *renderer
            , ZF_IN ZFImpl_sys_SDL_View *nativeView
            , ZF_IN const SDL_Rect &childRect
            , ZF_IN const SDL_Rect &parentRect
            , ZF_IN zffloat treeAlpha
            ) {
        ZFUIDrawableView *drawableView = zfcast(ZFUIDrawableView *, nativeView->ownerZFUIView);
        if(drawableView == zfnull) {
            return zffalse;
        }
        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)(ZFImpl_sys_SDL_View *)drawableView->nativeImplView();
        if(drawImpl == zfnull) {
            return zffalse;
        }

        drawImpl->sdlRenderer = renderer;
        drawImpl->framePixel = childRect;
        drawImpl->treeAlpha = treeAlpha;

        ZFPROTOCOL_ACCESS(ZFUIDrawForView)->notifyOnDraw(drawableView);
        return zffalse;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForViewImpl_sys_SDL)

// ============================================================
// ZFUIDrawForImage
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForImageImpl_sys_SDL, ZFUIDrawForImage, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual zfbool beginForImage(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN const ZFUISize &imageSizePixel
            ) {
        SDL_Surface *nativeImage = SDL_CreateRGBSurfaceWithFormat(0, (int)imageSizePixel.width, (int)imageSizePixel.height, 0, ZFImpl_sys_SDL_PixelFormatPreferred());
        if(nativeImage == zfnull) {
            return zffalse;
        }
        SDL_Renderer *sdlRenderer = SDL_CreateSoftwareRenderer(nativeImage);
        if(sdlRenderer == zfnull) {
            SDL_FreeSurface(nativeImage);
            return zffalse;
        }

        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = zfnew(_ZFP_ZFUIDrawImpl_sys_SDL);
        drawImpl->type = _ZFP_ZFUIDrawImpl_sys_SDL::Image;
        drawImpl->sdlRenderer = sdlRenderer;
        drawImpl->framePixel.x = 0;
        drawImpl->framePixel.y = 0;
        drawImpl->framePixel.w = (int)imageSizePixel.width;
        drawImpl->framePixel.h = (int)imageSizePixel.height;
        drawImpl->treeAlpha = 1;
        drawImpl->nativeImage = nativeImage;

        token.impl = drawImpl;
        return zftrue;
    }
    virtual void *endForImage(ZF_IN_OUT ZFUIDrawToken &token) {
        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)token.impl;
        zfblockedDelete(drawImpl);

        if(drawImpl->sdlRenderer != zfnull) {
            SDL_DestroyRenderer(drawImpl->sdlRenderer);
            drawImpl->sdlRenderer = zfnull;
        }

        return ZFImpl_sys_SDL_Image::implCreate(drawImpl->nativeImage);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForImageImpl_sys_SDL)

// ============================================================
// ZFUIDraw
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawImpl_sys_SDL, ZFUIDraw, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForImage, "SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void drawClear(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)token.impl;
        ZFUIColor color = ZFUIColorCreate(0, 0, 0, 0);
        Uint8 rOld, gOld, bOld, aOld;
        SDL_GetRenderDrawColor(drawImpl->sdlRenderer, &rOld, &gOld, &bOld, &aOld);
        SDL_SetRenderDrawColor(drawImpl->sdlRenderer
                , (Uint8)(0xFF * ZFUIColorGetR(color))
                , (Uint8)(0xFF * ZFUIColorGetG(color))
                , (Uint8)(0xFF * ZFUIColorGetB(color))
                , (Uint8)(0xFF * (ZFUIColorGetA(color) * drawImpl->treeAlpha))
            );
        SDL_RenderFillRect(drawImpl->sdlRenderer, &(drawImpl->framePixel));
        SDL_SetRenderDrawColor(drawImpl->sdlRenderer, rOld, gOld, bOld, aOld);
    }
    virtual void drawColor(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN const ZFUIColor &color
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)token.impl;
        SDL_Rect rect;
        rect.x = (int)(drawImpl->framePixel.x + targetFramePixel.x);
        rect.y = (int)(drawImpl->framePixel.y + targetFramePixel.y);
        rect.w = (int)targetFramePixel.width;
        rect.h = (int)targetFramePixel.height;
        ZFImpl_sys_SDL_View::renderRectCalc(rect, rect, drawImpl->framePixel);;

        Uint8 rOld, gOld, bOld, aOld;
        SDL_GetRenderDrawColor(drawImpl->sdlRenderer, &rOld, &gOld, &bOld, &aOld);
        SDL_SetRenderDrawColor(drawImpl->sdlRenderer
                , (Uint8)(0xFF * ZFUIColorGetR(color))
                , (Uint8)(0xFF * ZFUIColorGetG(color))
                , (Uint8)(0xFF * ZFUIColorGetB(color))
                , (Uint8)(0xFF * (ZFUIColorGetA(color) * drawImpl->treeAlpha))
            );
        SDL_RenderFillRect(drawImpl->sdlRenderer, &rect);
        SDL_SetRenderDrawColor(drawImpl->sdlRenderer, rOld, gOld, bOld, aOld);
    }
    virtual void drawImage(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN ZFUIImage *image
            , ZF_IN const ZFUIRect &imageFramePixel
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        _ZFP_ZFUIDrawImpl_sys_SDL *drawImpl = (_ZFP_ZFUIDrawImpl_sys_SDL *)token.impl;

        SDL_Rect rect;
        rect.x = (int)(drawImpl->framePixel.x + targetFramePixel.x);
        rect.y = (int)(drawImpl->framePixel.y + targetFramePixel.y);
        rect.w = (int)(targetFramePixel.width);
        rect.h = (int)(targetFramePixel.height);

        SDL_Rect rectClipped;
        ZFImpl_sys_SDL_View::renderRectCalc(rectClipped, rect, drawImpl->framePixel);;

        if(rectClipped.w <= 0 || rectClipped.h <= 0 || imageFramePixel.width <= 0 || imageFramePixel.height <= 0) {
            return;
        }

        SDL_Rect srcRect;

        ZFImpl_sys_SDL_Image *sdlImg = (ZFImpl_sys_SDL_Image *)image->nativeImage();
        SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(drawImpl->sdlRenderer, sdlImg->sdlSurface());
        ZFImpl_sys_SDL_zfblockedDestroyTexture(sdlTexture);
        if(zfmemcmp(&rect, &rectClipped, sizeof(SDL_Rect)) == 0) {
            srcRect.x = (int)imageFramePixel.x;
            srcRect.y = (int)imageFramePixel.y;
            srcRect.w = (int)imageFramePixel.width;
            srcRect.h = (int)imageFramePixel.height;
        }
        else {
            srcRect.x = (int)((float)imageFramePixel.width * (rectClipped.x - rect.x) / rect.w);
            srcRect.y = (int)((float)imageFramePixel.height * (rectClipped.y - rect.y) / rect.w);
            srcRect.w = (int)((float)imageFramePixel.width * rectClipped.w / rect.w);
            srcRect.h = (int)((float)imageFramePixel.height * rectClipped.h / rect.h);
        }

        SDL_RenderCopy(drawImpl->sdlRenderer, sdlTexture, &srcRect, &rectClipped);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

