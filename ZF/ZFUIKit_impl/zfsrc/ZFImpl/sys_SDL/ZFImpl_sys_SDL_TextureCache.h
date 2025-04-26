#ifndef _ZFI_ZFImpl_sys_SDL_TextureCache_h_
#define _ZFI_ZFImpl_sys_SDL_TextureCache_h_

#include "../ZFImpl_ZFUIKit_impl.h"
#include "ZFImpl/sys_SDL/ZFImpl_sys_SDL_ZF_impl.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD ZFLIB_ZFUIKit_impl ZFImpl_sys_SDL_TextureCache {
public:
    SDL_Texture *obtain(ZF_IN SDL_Renderer *renderer, ZF_IN int w, ZF_IN int h);
    void release(void);

public:
    ZFImpl_sys_SDL_TextureCache(void);
    ~ZFImpl_sys_SDL_TextureCache(void);
private:
    ZFImpl_sys_SDL_TextureCache(ZF_IN const ZFImpl_sys_SDL_TextureCache &ref);
    ZFImpl_sys_SDL_TextureCache &operator = (ZF_IN const ZFImpl_sys_SDL_TextureCache &ref);
private:
    SDL_Renderer *_renderer;
    SDL_Texture *_texture;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_TextureCache_h_

