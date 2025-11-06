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
            ) {
        ZFImpl_sys_SDL_View *nativeImageView = zfpoolNew(ZFImpl_sys_SDL_View);
        nativeImageView->renderImpl = zfself::renderCallback;
        return nativeImageView;
    }
    virtual void nativeImageViewDestroy(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN void *nativeImageView
            ) {
        ZFImpl_sys_SDL_View *nativeImageViewTmp = (ZFImpl_sys_SDL_View *)nativeImageView;
        nativeImageViewTmp->renderImpl = zfnull;
        zfpoolDelete(nativeImageViewTmp);
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
            , ZF_IN const SDL_FRect &childRect
            , ZF_IN const SDL_FRect &parentRect
            , ZF_IN zffloat treeAlpha
            ) {
        ZFUIImageView *owner = zfcast(ZFUIImageView *, nativeView->parent->ownerZFUIView);
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

        SDL_Texture *sdlTexture = nativeImage->sdlTexture(renderer);
        SDL_SetTextureAlphaMod(sdlTexture, treeAlpha != 1 ? (Uint8)(treeAlpha * 255) : (Uint8)255);
        if(imageState->imageNinePatch() == ZFUIMarginZero()) {
            SDL_RenderTexture(renderer, sdlTexture, zfnull, &childRect);
        }
        else {
            ZFUIImageImplNinePatchDrawData drawDatas[9];
            zfmemset(drawDatas, 0, sizeof(drawDatas));
            zfindex drawDatasCount = ZFUIImageImplNinePatchCalc(
                drawDatas,
                ZFUISizeCreate((zffloat)nativeImage->sdlSurface()->w, (zffloat)nativeImage->sdlSurface()->h),
                ZFUIMarginApplyScale(imageState->imageNinePatch(), imageState->imageScaleFixed()),
                ZFUISizeCreate((zffloat)childRect.w, (zffloat)childRect.h));

            SDL_FRect srcRect;
            SDL_FRect dstRect;
            for(zfindex i = 0; i < drawDatasCount; ++i) {
                const ZFUIImageImplNinePatchDrawData &drawData = drawDatas[i];
                srcRect.x = (int)drawData.src.x;
                srcRect.y = (int)drawData.src.y;
                srcRect.w = (int)drawData.src.width;
                srcRect.h = (int)drawData.src.height;
                dstRect.x = (int)(drawData.dst.x + childRect.x);
                dstRect.y = (int)(drawData.dst.y + childRect.y);
                dstRect.w = (int)drawData.dst.width;
                dstRect.h = (int)drawData.dst.height;
                SDL_RenderTexture(renderer, sdlTexture, &srcRect, &dstRect);
            }
        }
        return zffalse;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageViewImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

