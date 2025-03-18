#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl_sys_SDL_Image.h"

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
        SDL_Surface *sdlSurface = IMG_Load_RW(ZFImpl_sys_SDL_ZFInputToSDL_RWops(inputCallback), 1);
        if(sdlSurface == zfnull) {
            return zfnull;
        }
        return ZFImpl_sys_SDL_Image::implCreate(sdlSurface);
    }
    virtual zfbool nativeImageToOutput(
            ZF_IN void *nativeImage
            , ZF_OUT const ZFOutput &outputCallback
            ) {
        ZFImpl_sys_SDL_Image *sdlImg = (ZFImpl_sys_SDL_Image *)nativeImage;
        return 0 == IMG_SavePNG_RW(sdlImg->sdlSurface(), ZFImpl_sys_SDL_ZFOutputToSDL_RWops(outputCallback), 1);
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage) {
        ZFImpl_sys_SDL_Image *sdlImgOld = (ZFImpl_sys_SDL_Image *)nativeImage;
        SDL_Surface *nativeImageOld = sdlImgOld->sdlSurface();
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

        return ZFImpl_sys_SDL_Image::implCreate(nativeImageNew);
    }

    virtual void *nativeImageRetain(ZF_IN void *nativeImage) {
        ZFImpl_sys_SDL_Image *sdlImg = (ZFImpl_sys_SDL_Image *)nativeImage;
        sdlImg->implRetain();
        return nativeImage;
    }
    virtual void nativeImageRelease(ZF_IN void *nativeImage) {
        ZFImpl_sys_SDL_Image *sdlImg = (ZFImpl_sys_SDL_Image *)nativeImage;
        sdlImg->implRelease();
    }

    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage) {
        ZFImpl_sys_SDL_Image *sdlImg = (ZFImpl_sys_SDL_Image *)nativeImage;
        return ZFUISizeCreate((zffloat)sdlImg->sdlSurface()->w, (zffloat)sdlImg->sdlSurface()->h);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

