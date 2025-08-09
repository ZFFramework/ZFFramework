#ifndef _ZFI_ZFImpl_sys_SDL_ZF_impl_h_
#define _ZFI_ZFImpl_sys_SDL_ZF_impl_h_

#include "../ZFImpl_ZF_impl.h"
#include "ZFImpl_sys_SDL.h"

#if ZF_ENV_sys_SDL
#include "SDL3/SDL.h"
#endif

#include "ZFCore.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create SDL_IOStream from ZFInput
 *
 * when result is not null,
 * you must call SDL_CloseIO to close and release the SDL_IOStream
 */
extern ZFLIB_ZF_impl SDL_IOStream *ZFImpl_sys_SDL_ZFInputToSDL_IOStream(ZF_IN const ZFInput &callback);
/**
 * @brief util to create SDL_IOStream from ZFOutput
 *
 * when result is not null,
 * you must call SDL_CloseIO to close and release the SDL_IOStream
 */
extern ZFLIB_ZF_impl SDL_IOStream *ZFImpl_sys_SDL_ZFOutputToSDL_IOStream(ZF_IN const ZFOutput &callback);

// ============================================================
/**
 * @brief util to destroy surface when code block end
 */
#define ZFImpl_sys_SDL_zfscopeDestroySurface(sdlSurface) \
    _ZFP_ZFImpl_sys_SDL_zfscopeDestroySurface ZFUniqueName(sdl_surface)(sdlSurface)
zfclassNotPOD ZFLIB_ZF_impl _ZFP_ZFImpl_sys_SDL_zfscopeDestroySurface {
public:
    _ZFP_ZFImpl_sys_SDL_zfscopeDestroySurface(ZF_IN SDL_Surface *sdlSurface)
    : d(sdlSurface)
    {
    }
    ~_ZFP_ZFImpl_sys_SDL_zfscopeDestroySurface(void) {
        if(d) {
            SDL_DestroySurface(d);
        }
    }
private:
    SDL_Surface *d;
};

/**
 * @brief util to destroy texture when code block end
 */
#define ZFImpl_sys_SDL_zfscopeDestroyTexture(sdlTexture) \
    _ZFP_ZFImpl_sys_SDL_zfscopeDestroyTexture ZFUniqueName(sdl_texture)(sdlTexture)
zfclassNotPOD ZFLIB_ZF_impl _ZFP_ZFImpl_sys_SDL_zfscopeDestroyTexture {
public:
    _ZFP_ZFImpl_sys_SDL_zfscopeDestroyTexture(ZF_IN SDL_Texture *sdlTexture)
    : d(sdlTexture)
    {
    }
    ~_ZFP_ZFImpl_sys_SDL_zfscopeDestroyTexture(void) {
        if(d) {
            SDL_DestroyTexture(d);
        }
    }
private:
    SDL_Texture *d;
};

/**
 * @brief util to change render target within code block
 */
#define ZFImpl_sys_SDL_zfscopeRenderTarget(success, sdlRenderer, sdlTexture) \
    _ZFP_ZFImpl_sys_SDL_zfscopeRenderTarget success(sdlRenderer, sdlTexture)
zfclassNotPOD ZFLIB_ZF_impl _ZFP_ZFImpl_sys_SDL_zfscopeRenderTarget {
public:
    _ZFP_ZFImpl_sys_SDL_zfscopeRenderTarget(
            ZF_IN SDL_Renderer *sdlRenderer
            , ZF_IN SDL_Texture *sdlTexture
            )
    : _sdlRenderer(sdlRenderer)
    , _sdlTextureSaved(SDL_GetRenderTarget(sdlRenderer))
    {
        _success = SDL_SetRenderTarget(sdlRenderer, sdlTexture);
    }
    ~_ZFP_ZFImpl_sys_SDL_zfscopeRenderTarget(void) {
        if(_success) {
            SDL_SetRenderTarget(_sdlRenderer, _sdlTextureSaved);
        }
    }
    operator zfbool (void) const {
        return _success;
    }
private:
    zfbool _success;
    SDL_Renderer *_sdlRenderer;
    SDL_Texture *_sdlTextureSaved;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_ZF_impl_h_

