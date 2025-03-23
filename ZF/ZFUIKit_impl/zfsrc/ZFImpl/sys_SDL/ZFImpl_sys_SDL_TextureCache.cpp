#include "ZFImpl_sys_SDL_TextureCache.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

SDL_Texture *ZFImpl_sys_SDL_TextureCache::obtainFixed(ZF_IN SDL_Renderer *renderer, ZF_IN int w, ZF_IN int h) {
    if(renderer == zfnull || w <= 0 || h <= 0) {
        return zfnull;
    }
    if(_texture != zfnull) {
        if(_renderer == renderer) {
            int wOld, hOld;
            SDL_QueryTexture(_texture, zfnull, zfnull, &wOld, &hOld);
            if(w == wOld && h == hOld) {
                return _texture;
            }
        }
        SDL_DestroyTexture(_texture);
        _texture = zfnull;
    }
    _renderer = renderer;
    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(renderer, &rendererInfo);
    _texture = SDL_CreateTexture(
            renderer
            , (rendererInfo.num_texture_formats > 0 && rendererInfo.texture_formats[0] != SDL_PIXELFORMAT_UNKNOWN)
                ? rendererInfo.texture_formats[0]
                : SDL_PIXELFORMAT_ARGB32
            , SDL_TEXTUREACCESS_TARGET
            , w
            , h
            );
    SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    return _texture;
}
SDL_Texture *ZFImpl_sys_SDL_TextureCache::obtain(ZF_IN SDL_Renderer *renderer, ZF_IN int w, ZF_IN int h) {
    if(renderer == zfnull || w <= 0 || h <= 0) {
        return zfnull;
    }
    if(_texture != zfnull) {
        if(_renderer == renderer) {
            int wOld, hOld;
            SDL_QueryTexture(_texture, zfnull, zfnull, &wOld, &hOld);
            if(w <= wOld && h <= hOld
                    && !((wOld >= 256 && w < wOld / 2) || (hOld >= 256 && h < hOld / 2))
                    ) {
                return _texture;
            }
        }
        SDL_DestroyTexture(_texture);
        _texture = zfnull;
    }
    _renderer = renderer;
    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(renderer, &rendererInfo);
    _texture = SDL_CreateTexture(
            renderer
            , (rendererInfo.num_texture_formats > 0 && rendererInfo.texture_formats[0] != SDL_PIXELFORMAT_UNKNOWN)
                ? rendererInfo.texture_formats[0]
                : SDL_PIXELFORMAT_ARGB32
            , SDL_TEXTUREACCESS_TARGET
            , ((int)((w + 63) / 64)) * 64
            , ((int)((h + 63) / 64)) * 64
            );
    SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    return _texture;
}
void ZFImpl_sys_SDL_TextureCache::release(void) {
    if(_texture) {
        SDL_DestroyTexture(_texture);
        _texture = zfnull;
    }
}

ZFImpl_sys_SDL_TextureCache::ZFImpl_sys_SDL_TextureCache(void)
: _renderer(zfnull)
, _texture(zfnull)
{
}
ZFImpl_sys_SDL_TextureCache::~ZFImpl_sys_SDL_TextureCache(void) {
    this->release();
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

