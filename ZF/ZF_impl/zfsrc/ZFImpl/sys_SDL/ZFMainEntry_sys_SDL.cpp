#include "ZFMainEntry_sys_SDL.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMainEntry_sys_SDL_setup, ZFLevelZFFrameworkStatic) {
    ZFGlobalObserver().observerAddForOnce(ZFApp::E_AppEntry(), ZFCallbackForFunc(zfself::_after), ZFLevelZFFrameworkPostEssential);
    zfself::_before();
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMainEntry_sys_SDL_setup) {
    ZFGlobalObserver().observerRemove(ZFApp::E_AppEntry(), ZFCallbackForFunc(zfself::_after));

    ZFImpl_sys_SDL_embedCleanup();
    if(builtinRenderer() != zfnull) {
        ZFImpl_sys_SDL_RendererNotifyDestroy(builtinRenderer());
        SDL_DestroyRenderer(builtinRenderer());
        builtinRenderer() = zfnull;
    }
    if(builtinWindow() != zfnull) {
        ZFImpl_sys_SDL_WindowNotifyDestroy(builtinWindow());
        SDL_DestroyWindow(builtinWindow());
        builtinWindow() = zfnull;;
    }
}
public:
    static SDL_Window *&builtinWindow(void) {
        static SDL_Window *d = zfnull;
        return d;
    }
    static SDL_Renderer *&builtinRenderer(void) {
        static SDL_Renderer *d = zfnull;
        return d;
    }
public:
    void initUI(void) {
        zfself::_initUI();
    }

private:
    static void _before(void) {
        if(ZFImpl_sys_SDL_embed) {
            return;
        }
        unsigned int sdlInitFlag = 0
            | SDL_INIT_AUDIO
            | SDL_INIT_VIDEO
            | SDL_INIT_JOYSTICK
            | SDL_INIT_HAPTIC
            | SDL_INIT_GAMEPAD
            | SDL_INIT_EVENTS
            ;
        if(!SDL_Init(sdlInitFlag)) {
            ZFCoreCriticalMessage("SDL init failed: %s", SDL_GetError());
        }
    }
    static void _initUI(void) {
        builtinWindow() = ZFImpl_sys_SDL_CreateWindow();
        if(builtinWindow() == zfnull) {
            ZFCoreCriticalMessage("SDL window create failed: %s", SDL_GetError());
            return;
        }
        SDL_RaiseWindow(builtinWindow());
        ZFImpl_sys_SDL_WindowNotifyCreate(builtinWindow());
        builtinRenderer() = SDL_CreateRenderer(builtinWindow(), zfnull);
        if(builtinRenderer() == zfnull) {
            ZFImpl_sys_SDL_WindowNotifyDestroy(builtinWindow());
            SDL_DestroyWindow(builtinWindow());
            builtinWindow() = zfnull;
            ZFCoreCriticalMessage("SDL renderer create failed: %s", SDL_GetError());
            return;
        }
        ZFImpl_sys_SDL_RendererNotifyCreate(builtinRenderer());
        SDL_SetRenderDrawBlendMode(builtinRenderer(), SDL_BLENDMODE_BLEND);

        ZFImpl_sys_SDL_embedInit(builtinWindow());
    }
    static void _after(ZF_IN const ZFArgs &zfargs) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFMainEntry_sys_SDL_setup) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMainEntry_sys_SDL_setup);

        zfbool quitFlag = zffalse;
        SDL_Event event;
        while(!quitFlag && SDL_WaitEvent(&event)) {
            do {
                if(!ZFImpl_sys_SDL_embedEventHandler(&event)) {
                    if(event.type == SDL_EVENT_QUIT) {
                        quitFlag = zftrue;
                        break;
                    }
                }
            } while(SDL_PollEvent(&event));
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFMainEntry_sys_SDL_setup)

// ============================================================
static SDL_Window *_ZFP_ZFImpl_sys_SDL_mainWindow = zfnull;
static SDL_Renderer *_ZFP_ZFImpl_sys_SDL_mainRenderer = zfnull;
SDL_Window *ZFImpl_sys_SDL_mainWindow(void) {
    if(!_ZFP_ZFImpl_sys_SDL_mainWindow) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMainEntry_sys_SDL_setup)->initUI();
    }
    return _ZFP_ZFImpl_sys_SDL_mainWindow;
}
SDL_Renderer *ZFImpl_sys_SDL_mainRenderer(void) {
    if(!_ZFP_ZFImpl_sys_SDL_mainWindow) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMainEntry_sys_SDL_setup)->initUI();
    }
    return _ZFP_ZFImpl_sys_SDL_mainRenderer;
}

SDL_Window *ZFImpl_sys_SDL_CreateWindow(ZF_IN_OPT zfuint windowFlags /* = 0 */) {
    SDL_Window *sdlWindow = SDL_CreateWindow(
        ""
        , 640, 480
        , windowFlags
            | SDL_WINDOW_RESIZABLE
            | SDL_WINDOW_INPUT_FOCUS
            | SDL_WINDOW_MOUSE_FOCUS
        );
    return sdlWindow;
}

// ============================================================
ZFCoreArray<ZFImpl_sys_SDL_WindowLifeCycle> ZFImpl_sys_SDL_WindowOnCreate;
ZFCoreArray<ZFImpl_sys_SDL_WindowLifeCycle> ZFImpl_sys_SDL_WindowOnDestroy;
void ZFImpl_sys_SDL_WindowNotifyCreate(ZF_IN SDL_Window *owner) {
    for(zfindex i = 0; i < ZFImpl_sys_SDL_WindowOnCreate.count(); ++i) {
        ZFImpl_sys_SDL_WindowOnCreate[i](owner);
    }
}
void ZFImpl_sys_SDL_WindowNotifyDestroy(ZF_IN SDL_Window *owner) {
    for(zfindex i = 0; i < ZFImpl_sys_SDL_WindowOnDestroy.count(); ++i) {
        ZFImpl_sys_SDL_WindowOnDestroy[i](owner);
    }
}

ZFCoreArray<ZFImpl_sys_SDL_RendererLifeCycle> ZFImpl_sys_SDL_RendererOnCreate;
ZFCoreArray<ZFImpl_sys_SDL_RendererLifeCycle> ZFImpl_sys_SDL_RendererOnDestroy;
void ZFImpl_sys_SDL_RendererNotifyCreate(ZF_IN SDL_Renderer *owner) {
    for(zfindex i = 0; i < ZFImpl_sys_SDL_RendererOnCreate.count(); ++i) {
        ZFImpl_sys_SDL_RendererOnCreate[i](owner);
    }
}
void ZFImpl_sys_SDL_RendererNotifyDestroy(ZF_IN SDL_Renderer *owner) {
    for(zfindex i = 0; i < ZFImpl_sys_SDL_RendererOnDestroy.count(); ++i) {
        ZFImpl_sys_SDL_RendererOnDestroy[i](owner);
    }
}

// ============================================================
zfclassPOD _ZFP_ZFImpl_sys_SDL_EventHandlerData {
public:
    ZFImpl_sys_SDL_EventHandler handler;
    ZFLevel level;
};
typedef zfstlhashmap<Uint32, zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData> > _ZFP_ZFImpl_sys_SDL_EventHandlerMapType;
typedef zfstlhashmap<Sint32, zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData> > _ZFP_ZFImpl_sys_SDL_UserEventHandlerMapType;
static _ZFP_ZFImpl_sys_SDL_EventHandlerMapType _ZFP_ZFImpl_sys_SDL_EventHandlerMap;
static _ZFP_ZFImpl_sys_SDL_UserEventHandlerMapType _ZFP_ZFImpl_sys_SDL_UserEventHandlerMap;

void ZFImpl_sys_SDL_eventHandlerAdd(
        ZF_IN Uint32 type
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        , ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */
        ) {
    ZFCoreMutexLocker();
    ZFCoreAssert(eventHandler != zfnull);
    zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData> *list = zfnull;
    _ZFP_ZFImpl_sys_SDL_EventHandlerMapType::iterator it = _ZFP_ZFImpl_sys_SDL_EventHandlerMap.find(type);
    if(it == _ZFP_ZFImpl_sys_SDL_EventHandlerMap.end()) {
        list = &(_ZFP_ZFImpl_sys_SDL_EventHandlerMap[type]);
    }
    else {
        list = &(it->second);
    }

    zfstlsize pos = 0;
    while(pos < list->size()
        && level > (*list)[pos].level
    ) {
        ++pos;
    }
    _ZFP_ZFImpl_sys_SDL_EventHandlerData data;
    data.handler = eventHandler;
    data.level = level;
    list->insert(list->begin() + pos, data);
}
void ZFImpl_sys_SDL_eventHandlerRemove(
        ZF_IN Uint32 type
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        ) {
    ZFCoreMutexLocker();
    if(eventHandler != zfnull) {
        _ZFP_ZFImpl_sys_SDL_EventHandlerMapType::iterator it = _ZFP_ZFImpl_sys_SDL_EventHandlerMap.find(type);
        if(it != _ZFP_ZFImpl_sys_SDL_EventHandlerMap.end()) {
            for(zfstlsize i = 0; i < it->second.size(); ++i) {
                if(it->second[i].handler == eventHandler) {
                    it->second.erase(it->second.begin() + i);
                    break;
                }
            }
        }
    }
}

void ZFImpl_sys_SDL_userEventHandlerAdd(
        ZF_IN Sint32 userEventCode
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        , ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */
        ) {
    ZFCoreMutexLocker();
    ZFCoreAssert(eventHandler != zfnull);
    zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData> *list = zfnull;
    _ZFP_ZFImpl_sys_SDL_UserEventHandlerMapType::iterator it = _ZFP_ZFImpl_sys_SDL_UserEventHandlerMap.find(userEventCode);
    if(it == _ZFP_ZFImpl_sys_SDL_UserEventHandlerMap.end()) {
        list = &(_ZFP_ZFImpl_sys_SDL_UserEventHandlerMap[userEventCode]);
    }
    else {
        list = &(it->second);
    }

    zfstlsize pos = 0;
    while(pos < list->size()
        && level > (*list)[pos].level
    ) {
        ++pos;
    }
    _ZFP_ZFImpl_sys_SDL_EventHandlerData data;
    data.handler = eventHandler;
    data.level = level;
    list->insert(list->begin() + pos, data);
}
void ZFImpl_sys_SDL_userEventHandlerRemove(
        ZF_IN Sint32 userEventCode
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        ) {
    ZFCoreMutexLocker();
    if(eventHandler != zfnull) {
        _ZFP_ZFImpl_sys_SDL_UserEventHandlerMapType::iterator it = _ZFP_ZFImpl_sys_SDL_UserEventHandlerMap.find(userEventCode);
        if(it != _ZFP_ZFImpl_sys_SDL_UserEventHandlerMap.end()) {
            for(zfstlsize i = 0; i < it->second.size(); ++i) {
                if(it->second[i].handler == eventHandler) {
                    it->second.erase(it->second.begin() + i);
                    break;
                }
            }
        }
    }
}

// ============================================================
static SDL_PixelFormat _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_PIXELFORMAT_ARGB8888;
SDL_PixelFormat ZFImpl_sys_SDL_PixelFormatPreferred(void) {
    return _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred;
}

zfbool ZFImpl_sys_SDL_embed = zffalse;

void ZFImpl_sys_SDL_embedInit(ZF_IN SDL_Window *window) {
    ZFCoreAssert(window != zfnull);
    SDL_Renderer *renderer = SDL_GetRenderer(window);
    ZFCoreAssert(renderer != zfnull);
    ZFCoreAssert(_ZFP_ZFImpl_sys_SDL_mainWindow == zfnull);
    _ZFP_ZFImpl_sys_SDL_mainWindow = window;
    _ZFP_ZFImpl_sys_SDL_mainRenderer = renderer;
    SDL_PixelFormat nativePixelFormat = SDL_GetWindowPixelFormat(window);

    const SDL_PixelFormatDetails *d = SDL_GetPixelFormatDetails(nativePixelFormat);
    // fix:
    //     RGBX => RGBA
    //     XRGB => ARGB
    if(d != NULL
            && d->bits_per_pixel == 32
            && d->Abits == 0
            ) {
        if(zffalse) {
        }
        else if(!ZFBitTest(0xFF, d->Rmask|d->Gmask|d->Bmask)) {
            _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_GetPixelFormatForMasks(32, d->Rmask, d->Gmask, d->Bmask, 0xFF);
        }
        else if(!ZFBitTest(0xFF00, d->Rmask|d->Gmask|d->Bmask)) {
            _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_GetPixelFormatForMasks(32, d->Rmask, d->Gmask, d->Bmask, 0xFF00);
        }
        else if(!ZFBitTest(0xFF0000, d->Rmask|d->Gmask|d->Bmask)) {
            _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_GetPixelFormatForMasks(32, d->Rmask, d->Gmask, d->Bmask, 0xFF0000);
        }
        else if(!ZFBitTest(0xFF000000, d->Rmask|d->Gmask|d->Bmask)) {
            _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_GetPixelFormatForMasks(32, d->Rmask, d->Gmask, d->Bmask, 0xFF000000);
        }
        else {
            _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_PIXELFORMAT_RGBA8888;
        }
    }
    else {
        _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_PIXELFORMAT_RGBA8888;
    }
}
void ZFImpl_sys_SDL_embedCleanup(void) {
    _ZFP_ZFImpl_sys_SDL_mainWindow = zfnull;
    _ZFP_ZFImpl_sys_SDL_mainRenderer = zfnull;
}

zfbool ZFImpl_sys_SDL_embedEventHandler(ZF_IN SDL_Event *event) {
    if(event->type == SDL_EVENT_USER) {
        SDL_UserEvent *userEvent = (SDL_UserEvent *)event;
        _ZFP_ZFImpl_sys_SDL_UserEventHandlerMapType::iterator it = _ZFP_ZFImpl_sys_SDL_UserEventHandlerMap.find(userEvent->code);
        if(it != _ZFP_ZFImpl_sys_SDL_UserEventHandlerMap.end()) {
            for(zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData>::iterator i = it->second.begin(); i != it->second.end(); ++i) {
                if((*i).handler(event)) {
                    return zftrue;
                }
            }
        }
    }
    else {
        _ZFP_ZFImpl_sys_SDL_EventHandlerMapType::iterator it = _ZFP_ZFImpl_sys_SDL_EventHandlerMap.find(event->type);
        if(it != _ZFP_ZFImpl_sys_SDL_EventHandlerMap.end()) {
            for(zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData>::iterator i = it->second.begin(); i != it->second.end(); ++i) {
                if((*i).handler(event)) {
                    return zftrue;
                }
            }
        }
    }

    // default impl
    switch(event->type) {
        case SDL_EVENT_LOW_MEMORY:
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppOnMemoryLow());
            return zftrue;
        default:
            break;
    }

    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

