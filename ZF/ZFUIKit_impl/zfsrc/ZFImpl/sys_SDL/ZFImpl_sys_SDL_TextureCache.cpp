#include "ZFImpl_sys_SDL_TextureCache.h"

#if ZF_ENV_sys_SDL

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#include "ZFCore/ZFSTLWrapper/zfstllist.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_SDL_TextureCache_ENABLE 1

zfclassNotPOD _ZFP_SDL_TextureCacheHolder {
public:
    SDL_Texture *obtain(ZF_IN SDL_Renderer *renderer, ZF_IN int w, ZF_IN int h) {
#if _ZFP_SDL_TextureCache_ENABLE
        w = ((int)((w + (AlignSize - 1)) / AlignSize)) * AlignSize;
        h = ((int)((h + (AlignSize - 1)) / AlignSize)) * AlignSize;

        {
            Key key;
            key.renderer = renderer;
            key.w = w;
            key.h = h;
            MapType::iterator it = m.find(key);
            if(it != m.end()) {
                Value *value = *(it->second.begin());
                it->second.erase(it->second.begin());
                if(it->second.empty()) {
                    m.erase(it);
                }
                l.erase(value->orderIt);
                SDL_Texture *ret = value->texture;
                zfpoolDelete(value);
                return ret;
            }
        }
#endif

        SDL_RendererInfo rendererInfo;
        SDL_GetRendererInfo(renderer, &rendererInfo);
        SDL_Texture *texture = SDL_CreateTexture(
                renderer
                , (rendererInfo.num_texture_formats > 0 && rendererInfo.texture_formats[0] != SDL_PIXELFORMAT_UNKNOWN)
                    ? rendererInfo.texture_formats[0]
                    : SDL_PIXELFORMAT_ARGB32
                , SDL_TEXTUREACCESS_TARGET
                , w
                , h
                );
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        return texture;
    }
    void release(ZF_IN SDL_Renderer *renderer, ZF_IN SDL_Texture *texture) {
#if _ZFP_SDL_TextureCache_ENABLE
        Key key;
        key.renderer = renderer;
        SDL_QueryTexture(texture, zfnull, zfnull, &(key.w), &(key.h));

        Value *value = zfpoolNew(Value);
        value->texture = texture;

        value->mapIt = m.find(key);
        if(value->mapIt == m.end()) {
            value->mapIt = m.insert(zfstlpair<Key, zfstllist<Value *> >(key, ListType())).first;
        }
        value->mapIt->second.push_back(value);
        value->listIt = value->mapIt->second.end();
        --(value->listIt);
        l.push_back(value);
        value->orderIt = l.end();
        --(value->orderIt);

        while(l.size() >= MaxCache) {
            value = *(l.begin());
            l.erase(l.begin());
            value->mapIt->second.erase(value->listIt);
            if(value->mapIt->second.empty()) {
                m.erase(value->mapIt);
            }
            SDL_DestroyTexture(value->texture);
            zfpoolDelete(value);
        }
#else
        SDL_DestroyTexture(texture);
#endif
    }

private:
    enum {
        AlignSize = 64,
        MaxCache = 32,
    };
    zfclassNotPOD Key {
    public:
        SDL_Renderer *renderer;
        int w;
        int h;
    public:
        inline zfbool operator < (ZF_IN const Key &ref) const {
            return 0
                + ((const zfbyte *)renderer - (const zfbyte *)ref.renderer)
                + (w - ref.w)
                + (h - ref.h)
                ;
        }
    };
    zfclassFwd Value;
    typedef zfstlmap<Key, zfstllist<Value *> > MapType;
    typedef zfstllist<Value *> ListType;
    zfclassNotPOD Value {
    public:
        SDL_Texture *texture;
        MapType::iterator mapIt;
        ListType::iterator listIt;
        ListType::iterator orderIt;
    };
private:
    MapType m;
    ListType l;
};

static _ZFP_SDL_TextureCacheHolder _ZFP_SDL_TextureCache;

// ============================================================
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
        _ZFP_SDL_TextureCache.release(_renderer, _texture);
        _texture = zfnull;
    }
    _renderer = renderer;
    _texture = _ZFP_SDL_TextureCache.obtain(renderer, w, h);
    return _texture;
}
void ZFImpl_sys_SDL_TextureCache::release(void) {
    if(_texture) {
        _ZFP_SDL_TextureCache.release(_renderer, _texture);
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

