#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageIOImpl_sys_SDL, ZFUIImageIO, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *imageApplyScale(
            ZF_IN zffloat imageScale
            , ZF_IN void *nativeImage
            , ZF_IN const ZFUISize &newSize
            , ZF_IN const ZFUIMargin &ninePatch
            ) {
        SDL_Surface *nativeImageOld = (SDL_Surface *)nativeImage;
        SDL_Surface *nativeImageNew = SDL_CreateRGBSurfaceWithFormat(0, (int)newSize.width, (int)newSize.height, 0, nativeImageOld->format->format);
        if(nativeImageNew == zfnull) {
            return zfnull;
        }
        if(ninePatch == ZFUIMarginZero()) {
            SDL_BlitSurface(nativeImageOld, zfnull, nativeImageNew, zfnull);
            return nativeImageNew;
        }

        ZFUIImageImplNinePatchDrawData drawDatas[9];
        zfmemset(drawDatas, 0, sizeof(drawDatas));
        zfindex drawDatasCount = ZFUIImageImplNinePatchCalc(
            drawDatas,
            ZFUISizeCreate((zffloat)nativeImageOld->w, (zffloat)nativeImageOld->h),
            ninePatch,
            newSize);

        SDL_Rect srcRect;
        SDL_Rect dstRect;
        for(zfindex i = 0; i < drawDatasCount; ++i) {
            const ZFUIImageImplNinePatchDrawData &drawData = drawDatas[i];
            srcRect.x = (int)drawData.src.x;
            srcRect.y = (int)drawData.src.y;
            srcRect.w = (int)drawData.src.width;
            srcRect.h = (int)drawData.src.height;
            dstRect.x = (int)drawData.dst.x;
            dstRect.y = (int)drawData.dst.y;
            dstRect.w = (int)drawData.dst.width;
            dstRect.h = (int)drawData.dst.height;
            SDL_BlitSurface(nativeImageOld, &srcRect, nativeImageNew, &dstRect);
        }
        return nativeImageNew;
    }
    virtual void *imageLoadInFrame(
            ZF_IN zffloat imageScale
            , ZF_IN void *nativeImage
            , ZF_IN const ZFUIRect &frameInImage
            ) {
        SDL_Surface *nativeImageOld = (SDL_Surface *)nativeImage;
        SDL_Surface *nativeImageNew = SDL_CreateRGBSurfaceWithFormat(0, (int)frameInImage.width, (int)frameInImage.height, 0, ZFImpl_sys_SDL_PixelFormatPreferred());
        if(nativeImageNew != zfnull) {
            SDL_Rect srcRect;
            srcRect.x = (int)frameInImage.x;
            srcRect.y = (int)frameInImage.y;
            srcRect.w = (int)frameInImage.width;
            srcRect.h = (int)frameInImage.height;
            SDL_BlitSurface(nativeImageOld, &srcRect, nativeImageNew, zfnull);
        }
        return nativeImageNew;
    }
    virtual void *imageLoadFromColor(
            ZF_IN zffloat imageScale
            , ZF_IN const ZFUIColor &color
            , ZF_IN const ZFUISize &size
            ) {
        SDL_Surface *nativeImageNew = SDL_CreateRGBSurfaceWithFormat(0, (int)size.width, (int)size.height, 0, ZFImpl_sys_SDL_PixelFormatPreferred());
        if(nativeImageNew != zfnull) {
            SDL_FillRect(nativeImageNew, zfnull, SDL_MapRGBA(nativeImageNew->format
                , (Uint8)(ZFUIColorGetR(color) * 0xFF)
                , (Uint8)(ZFUIColorGetG(color) * 0xFF)
                , (Uint8)(ZFUIColorGetB(color) * 0xFF)
                , (Uint8)(ZFUIColorGetA(color) * 0xFF)
                ));
        }
        return nativeImageNew;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageIOImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

