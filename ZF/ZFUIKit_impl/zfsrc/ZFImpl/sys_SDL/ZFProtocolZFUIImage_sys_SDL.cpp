#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_SDL

#include "SDL_image.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_SDL, ZFUIImage, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_image")

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        IMG_Init(0
                | IMG_INIT_JPG
                | IMG_INIT_PNG
                | IMG_INIT_TIF
                | IMG_INIT_WEBP
            );
    }
    zfoverride
    virtual void protocolOnDealloc(void) {
        IMG_Quit();
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void *nativeImageFromInput(ZF_IN const ZFInput &inputCallback) {
        return IMG_Load_RW(ZFImpl_sys_SDL_ZFInputToSDL_RWops(inputCallback), 1);
    }
    virtual zfbool nativeImageToOutput(
            ZF_IN void *nativeImage
            , ZF_OUT const ZFOutput &outputCallback
            ) {
        return 0 == IMG_SavePNG_RW((SDL_Surface *)nativeImage, ZFImpl_sys_SDL_ZFOutputToSDL_RWops(outputCallback), 1);
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage) {
        SDL_Surface *nativeImageOld = (SDL_Surface *)nativeImage;
        SDL_Surface *nativeImageNew = SDL_CreateRGBSurfaceWithFormat(0, nativeImageOld->w, nativeImageOld->h, 0, nativeImageOld->format->format);
        if(nativeImageNew == zfnull) {
            return zfnull;
        }

        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = nativeImageOld->w;
        rect.h = nativeImageOld->h;

        SDL_Rect clipSaved;
        SDL_GetClipRect(nativeImageOld, &clipSaved);
        SDL_SetClipRect(nativeImageOld, &rect);
        SDL_BlitSurface(nativeImageOld, zfnull, nativeImageNew, zfnull);
        SDL_SetClipRect(nativeImageOld, &clipSaved);

        return nativeImageNew;
    }

    virtual void *nativeImageRetain(ZF_IN void *nativeImage) {
        ++((SDL_Surface *)nativeImage)->refcount;
        return nativeImage;
    }
    virtual void nativeImageRelease(ZF_IN void *nativeImage) {
        SDL_FreeSurface((SDL_Surface *)nativeImage);
    }

    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage) {
        SDL_Surface *nativeImageTmp = (SDL_Surface *)nativeImage;
        return ZFUISizeCreate((zffloat)nativeImageTmp->w, (zffloat)nativeImageTmp->h);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

