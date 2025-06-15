#include "ZFImpl_sys_SDL_Image.h"

#if ZF_ENV_sys_SDL

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#include "ZFCore/ZFSTLWrapper/zfstllist.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFImpl_sys_SDL_ImageCache {
public:
    SDL_Texture *sdlTexture(ZF_IN SDL_Surface *sdlSurface, ZF_IN SDL_Renderer *owner) {
        if(sdlSurface == zfnull || owner == zfnull) {
            return zfnull;
        }
        ZFCoreMutexLocker();

        SurfaceMap::iterator surfaceIt = s.find(sdlSurface);
        if(surfaceIt == s.end()) {
            surfaceIt = s.insert(zfstlpair<SDL_Surface *, SurfaceSubMap>(sdlSurface, SurfaceSubMap())).first;
        }

        SurfaceSubMap::iterator surfaceSubIt = surfaceIt->second.find(owner);
        if(surfaceSubIt != surfaceIt->second.end()) {
            Cache *cache = surfaceSubIt->second.pointerValue();
            l.erase(cache->cacheIt);
            l.push_back(cache);
            --(cache->cacheIt = l.end());
            return cache->sdlTexture;
        }

        Cache *cache = zfpoolNew(Cache);
        cache->sdlTexture = SDL_CreateTextureFromSurface(owner, sdlSurface);
        SDL_SetTextureBlendMode(cache->sdlTexture, SDL_BLENDMODE_BLEND);
        cache->surfaceIt = surfaceIt;
        cache->surfaceSubIt = surfaceIt->second.insert(zfstlpair<SDL_Renderer *, CacheHolder>(owner, CacheHolder(cache))).first;
        cache->rendererIt = r.find(owner);
        if(cache->rendererIt == r.end()) {
            cache->rendererIt = r.insert(zfstlpair<SDL_Renderer *, CacheList>(owner, CacheList())).first;
        }
        cache->rendererIt->second.push_back(cache);
        --(cache->rendererCacheIt = cache->rendererIt->second.end());
        l.push_back(cache);
        --(cache->cacheIt = l.end());

        return cache->sdlTexture;
    }
    void implOwnerDestroyed(ZF_IN SDL_Renderer *owner) {
        ZFCoreAssert(owner != zfnull);
        ZFCoreMutexLocker();
        RendererMap::iterator rendererIt = r.find(owner);
        if(rendererIt == r.end()) {
            return;
        }
        while(!rendererIt->second.empty()) {
            Cache *cache = *(rendererIt->second.begin());
            rendererIt->second.pop_front();
            l.erase(cache->cacheIt);
            SurfaceMap::iterator surfaceIt = cache->surfaceIt;
            cache->surfaceIt->second.erase(cache->surfaceSubIt);
            if(surfaceIt->second.empty()) {
                s.erase(surfaceIt);
            }
        }
        r.erase(rendererIt);
    }
    void implDestroyed(ZF_IN SDL_Surface *sdlSurface) {
        ZFCoreAssert(sdlSurface != zfnull);
        SurfaceMap::iterator surfaceIt = s.find(sdlSurface);
        if(surfaceIt == s.end()) {
            return;
        }
        while(!surfaceIt->second.empty()) {
            Cache *cache = surfaceIt->second.begin()->second.pointerValue();
            l.erase(cache->cacheIt);
            cache->rendererIt->second.erase(cache->rendererCacheIt);
            if(cache->rendererIt->second.empty()) {
                r.erase(cache->rendererIt);
            }
            if(surfaceIt->second.size() == 1) {
                s.erase(surfaceIt);
                break;
            }
            else {
                surfaceIt->second.erase(cache->surfaceSubIt);
            }
        }
    }
private:
    enum {
        MaxCount = 64,
    };
    zfclassFwd Cache;
    typedef ZFCorePointerForPoolObject<Cache *> CacheHolder;

    typedef zfstllist<Cache *> CacheList;
    typedef zfstlhashmap<SDL_Renderer *, CacheHolder> SurfaceSubMap;
    typedef zfstlhashmap<SDL_Surface *, SurfaceSubMap> SurfaceMap;
    typedef zfstlhashmap<SDL_Renderer *, CacheList> RendererMap;

    zfclassNotPOD Cache {
    public:
        SDL_Texture *sdlTexture;
        SurfaceMap::iterator surfaceIt;
        SurfaceSubMap::iterator surfaceSubIt;
        RendererMap::iterator rendererIt;
        CacheList::iterator rendererCacheIt;
        CacheList::iterator cacheIt;
    public:
        Cache(void) : sdlTexture(zfnull) {}
        ~Cache(void) {
            if(this->sdlTexture) {
                SDL_DestroyTexture(this->sdlTexture);
            }
        }
    };
private:
    SurfaceMap s;
    RendererMap r;
    CacheList l;
};
static _ZFP_ZFImpl_sys_SDL_ImageCache &_ZFP_ZFImpl_sys_SDL_ImageCacheObj(void) {
    static _ZFP_ZFImpl_sys_SDL_ImageCache d;
    return d;
}

// ============================================================
void ZFImpl_sys_SDL_Image::implOwnerDestroyed(ZF_IN SDL_Renderer *owner) {
    _ZFP_ZFImpl_sys_SDL_ImageCacheObj().implOwnerDestroyed(owner);
}

ZFImpl_sys_SDL_Image *ZFImpl_sys_SDL_Image::implCreate(void) {
    return zfpoolNew(ZFImpl_sys_SDL_Image);
}
ZFImpl_sys_SDL_Image *ZFImpl_sys_SDL_Image::implCreate(ZF_IN SDL_Surface *sdlSurface) {
    ZFImpl_sys_SDL_Image *ret = zfpoolNew(ZFImpl_sys_SDL_Image);
    if(sdlSurface) {
        ret->implUpdate(sdlSurface);
    }
    return ret;
}

void ZFImpl_sys_SDL_Image::implRetain(void) {
    ++_refCount;
}
void ZFImpl_sys_SDL_Image::implRelease(void) {
    --_refCount;
    if(_refCount == 0) {
        zfpoolDelete(this);
    }
}
void ZFImpl_sys_SDL_Image::implUpdate(ZF_IN SDL_Surface *sdlSurface) {
    if(_sdlSurface) {
        _ZFP_ZFImpl_sys_SDL_ImageCacheObj().implDestroyed(_sdlSurface);
        SDL_DestroySurface(_sdlSurface);
        _sdlSurface = zfnull;
    }
    _sdlSurface = sdlSurface;
    if(sdlSurface) {
        SDL_SetSurfaceBlendMode(sdlSurface, SDL_BLENDMODE_BLEND);
    }
}
SDL_Surface *ZFImpl_sys_SDL_Image::sdlSurface(void) {
    return _sdlSurface;
}
SDL_Texture *ZFImpl_sys_SDL_Image::sdlTexture(ZF_IN SDL_Renderer *owner) {
    if(owner == zfnull || _sdlSurface == zfnull) {
        return zfnull;
    }
    return _ZFP_ZFImpl_sys_SDL_ImageCacheObj().sdlTexture(_sdlSurface, owner);
}

ZFImpl_sys_SDL_Image::ZFImpl_sys_SDL_Image(void)
: _refCount(1)
, _sdlSurface(zfnull)
{
}
ZFImpl_sys_SDL_Image::~ZFImpl_sys_SDL_Image(void) {
    this->implUpdate(zfnull);
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

