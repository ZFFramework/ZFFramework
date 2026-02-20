#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl_sys_SDL_Image.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_SDL, ZFUIImage, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_image")

public:
    virtual void *nativeImageFromInput(ZF_IN const ZFInput &inputCallback) {
        SDL_Surface *sdlSurface = IMG_Load_IO(ZFImpl_sys_SDL_ZFInputToSDL_IOStream(inputCallback), true);
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
        return IMG_SavePNG_IO(sdlImg->sdlSurface(), ZFImpl_sys_SDL_ZFOutputToSDL_IOStream(outputCallback), true);
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage) {
        ZFImpl_sys_SDL_Image *sdlImgOld = (ZFImpl_sys_SDL_Image *)nativeImage;
        SDL_Surface *nativeImageOld = sdlImgOld->sdlSurface();
        SDL_Surface *nativeImageNew = SDL_DuplicateSurface(nativeImageOld);
        if(nativeImageNew == zfnull) {
            return zfnull;
        }
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

