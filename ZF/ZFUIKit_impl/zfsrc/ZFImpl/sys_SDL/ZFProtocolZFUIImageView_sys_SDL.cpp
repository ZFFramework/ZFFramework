#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageView.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl_sys_SDL_Image.h"
#include "ZFUIKit/ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageViewImpl_sys_SDL, ZFUIImageView, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeImageViewCreate(
            ZF_IN ZFUIImageView *imageView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)imageView->nativeView();
        nativeView->renderImpls.add(zfself::renderCallback);

        // no actual impl view, use renderImpls
        return zfnull;
    }
    virtual void nativeImageViewDestroy(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN void *nativeImageView
            ) {
        // nothing to do
    }

    virtual void image(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN ZFUIImage *image
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)imageView->nativeView();
        nativeView->renderRequest();
    }

private:
    static zfbool renderCallback(
            ZF_IN SDL_Renderer *renderer
            , ZF_IN ZFImpl_sys_SDL_View *nativeView
            , ZF_IN const SDL_Rect &childRect
            , ZF_IN const SDL_Rect &parentRect
            , ZF_IN zffloat treeAlpha
            ) {
        ZFUIImageView *owner = zfcast(ZFUIImageView *, nativeView->ownerZFUIView);
        if(owner == zfnull) {
            return zffalse;
        }
        zfautoT<ZFUIImage> imageState = owner->imageState();
        if(imageState == zfnull) {
            return zffalse;
        }
        ZFImpl_sys_SDL_Image *nativeImage = (ZFImpl_sys_SDL_Image *)imageState->nativeImage();
        if(nativeImage == zfnull) {
            return zffalse;
        }

        SDL_Rect targetRect = ZFImpl_sys_SDL_ZFUIRectToSDL_Rect(ZFUIRectApplyScale(owner->nativeImplViewFrame(), owner->UIScaleFixed()));
        targetRect.x += childRect.x;
        targetRect.y += childRect.y;

        SDL_Texture *sdlTexture = nativeImage->sdlTexture(ZFUIWindow::sysWindowForView(owner));
        SDL_SetTextureAlphaMod(sdlTexture, treeAlpha != 1 ? (Uint8)(treeAlpha * 255) : (Uint8)255);
        if(imageState->imageNinePatch() == ZFUIMarginZero()) {
            SDL_RenderCopy(renderer, sdlTexture, zfnull, &targetRect);
        }
        else {
            ZFUIImageImplNinePatchDrawData drawDatas[9];
            zfmemset(drawDatas, 0, sizeof(drawDatas));
            zfindex drawDatasCount = ZFUIImageImplNinePatchCalc(
                drawDatas,
                ZFUISizeCreate((zffloat)nativeImage->sdlSurface()->w, (zffloat)nativeImage->sdlSurface()->h),
                ZFUIMarginApplyScale(imageState->imageNinePatch(), imageState->imageScaleFixed()),
                ZFUISizeCreate((zffloat)targetRect.w, (zffloat)targetRect.h));

            SDL_Rect srcRect;
            SDL_Rect dstRect;
            for(zfindex i = 0; i < drawDatasCount; ++i) {
                const ZFUIImageImplNinePatchDrawData &drawData = drawDatas[i];
                srcRect.x = (int)drawData.src.x;
                srcRect.y = (int)drawData.src.y;
                srcRect.w = (int)drawData.src.width;
                srcRect.h = (int)drawData.src.height;
                dstRect.x = (int)(drawData.dst.x + targetRect.x);
                dstRect.y = (int)(drawData.dst.y + targetRect.y);
                dstRect.w = (int)drawData.dst.width;
                dstRect.h = (int)drawData.dst.height;
                SDL_RenderCopy(renderer, sdlTexture, &srcRect, &dstRect);
            }
        }
        return zffalse;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageViewImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

