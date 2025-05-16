#include "ZFImpl_sys_SDL_Image.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl_sys_SDL_SysWindow.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_Image_init, ZFLevelZFFrameworkEssential) {
    IMG_Init(0
            | IMG_INIT_JPG
            | IMG_INIT_PNG
            | IMG_INIT_TIF
            | IMG_INIT_WEBP
            );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_Image_init) {
    IMG_Quit();
}
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_Image_init)

// ============================================================
typedef zfstlhashmap<ZFUISysWindow *, SDL_Texture *> _ZFP_ZFImpl_sys_SDL_ImageTextureMap;

ZFImpl_sys_SDL_Image::ZFImpl_sys_SDL_Image(void)
: _refCount(1)
, _sdlSurface(zfnull)
, _sdlTexture(zfnull)
, _sdlTextureMap(zfnull)
{
}
ZFImpl_sys_SDL_Image::~ZFImpl_sys_SDL_Image(void) {
    this->implUpdate(zfnull);
    if(_sdlTextureMap) {
        zfpoolDelete((_ZFP_ZFImpl_sys_SDL_ImageTextureMap *)_sdlTextureMap);
    }
}

void ZFImpl_sys_SDL_Image::_attach(ZF_IN ZFUISysWindow *owner) {
    if(!_detachListener) {
        ZFImpl_sys_SDL_Image *pimplOwner = this;
        ZFLISTENER_1(impl
                , ZFImpl_sys_SDL_Image *, pimplOwner
                ) {
            _ZFP_ZFImpl_sys_SDL_ImageTextureMap &m = *(_ZFP_ZFImpl_sys_SDL_ImageTextureMap *)pimplOwner->_sdlTextureMap;
            _ZFP_ZFImpl_sys_SDL_ImageTextureMap::iterator it = m.find(zfargs.sender());
            ZFCoreAssert(it != m.end());
            SDL_DestroyTexture(it->second);
            m.erase(it);
        } ZFLISTENER_END()
        _detachListener = impl;
    }
    owner->observerAdd(ZFUISysWindow::E_SysWindowOnDestroy(), _detachListener);
    owner->observerAdd(ZFUISysWindow::E_ObjectBeforeDealloc(), _detachListener);
}
void ZFImpl_sys_SDL_Image::_detach(ZF_IN ZFUISysWindow *owner) {
    owner->observerRemove(ZFUISysWindow::E_SysWindowOnDestroy(), _detachListener);
    owner->observerRemove(ZFUISysWindow::E_ObjectBeforeDealloc(), _detachListener);
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
    if(_sdlTextureMap) {
        _ZFP_ZFImpl_sys_SDL_ImageTextureMap &m = *(_ZFP_ZFImpl_sys_SDL_ImageTextureMap *)_sdlTextureMap;
        for(_ZFP_ZFImpl_sys_SDL_ImageTextureMap::iterator it = m.begin(); it != m.end(); ++it) {
            _detach(it->first);
            SDL_DestroyTexture(it->second);
        }
        m.clear();
    }
    if(_sdlTexture) {
        SDL_DestroyTexture(_sdlTexture);
        _sdlTexture = zfnull;
    }
    if(_sdlSurface) {
        SDL_FreeSurface(_sdlSurface);
        _sdlSurface = zfnull;
    }
    _sdlSurface = sdlSurface;
}
SDL_Surface *ZFImpl_sys_SDL_Image::sdlSurface(void) {
    return _sdlSurface;
}
SDL_Texture *ZFImpl_sys_SDL_Image::sdlTexture(ZF_IN ZFUISysWindow *owner) {
    if(!_sdlSurface || owner == zfnull) {
        return zfnull;
    }
    ZFImpl_sys_SDL_SysWindow *sdlWindow = (ZFImpl_sys_SDL_SysWindow *)owner->nativeWindow();
    if(owner == ZFUISysWindow::mainWindow()) {
        if(!_sdlTexture) {
            _sdlTexture = SDL_CreateTextureFromSurface(sdlWindow->sdlRenderer, _sdlSurface);
        }
        return _sdlTexture;
    }

    if(!_sdlTextureMap) {
        _sdlTextureMap = zfpoolNew(_ZFP_ZFImpl_sys_SDL_ImageTextureMap);
    }
    _ZFP_ZFImpl_sys_SDL_ImageTextureMap &m = *(_ZFP_ZFImpl_sys_SDL_ImageTextureMap *)_sdlTextureMap;
    _ZFP_ZFImpl_sys_SDL_ImageTextureMap::iterator it = m.find(owner);
    if(it != m.end()) {
        return it->second;
    }
    _attach(owner);
    return (m[owner] = SDL_CreateTextureFromSurface(sdlWindow->sdlRenderer, _sdlSurface));
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

