#ifndef _ZFI_ZFImpl_sys_SDL_Image_h_
#define _ZFI_ZFImpl_sys_SDL_Image_h_

#include "ZFImpl_sys_SDL_View.h"

#if ZF_ENV_sys_SDL

#include "SDL3_image/SDL_image.h"
#include "ZFUIKit/ZFUISysWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD ZFLIB_ZFUIKit_impl ZFImpl_sys_SDL_Image {
public:
    static void implOwnerDestroyed(ZF_IN SDL_Renderer *owner);

    static ZFImpl_sys_SDL_Image *implCreate(void);
    static ZFImpl_sys_SDL_Image *implCreate(ZF_IN SDL_Surface *sdlSurface);

    void implRetain(void);
    void implRelease(void);
    void implUpdate(ZF_IN SDL_Surface *sdlSurface);
    SDL_Surface *sdlSurface(void);
    /*
     * result must not be cached
     * when owner destroyed, must call implOwnerDestroyed(owner) to cleanup
     */
    SDL_Texture *sdlTexture(ZF_IN SDL_Renderer *owner);

private:
    zfuint _refCount;
    SDL_Surface *_sdlSurface;
private:
    ZFImpl_sys_SDL_Image(void);
    ~ZFImpl_sys_SDL_Image(void);
private:
    typedef ZFImpl_sys_SDL_Image zfself;
    zfpoolDeclareFriend()
    ZFImpl_sys_SDL_Image(ZF_IN ZFImpl_sys_SDL_Image const &ref);
    ZFImpl_sys_SDL_Image &operator = (ZF_IN ZFImpl_sys_SDL_Image const &ref);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_Image_h_

