#include "ZFMainEntry_sys_SDL.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMainEntry_sys_SDL_setup, ZFLevelZFFrameworkStatic) {
    this->builtinWindow = zfnull;
    this->builtinRenderer = zfnull;

    this->afterListener = ZFCallbackForFunc(zfself::after);
    ZFGlobalObserver().observerAdd(
            ZFApp::E_AppEntry(),
            this->afterListener,
            ZFLevelZFFrameworkPostEssential
        );

    zfself::before(this);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMainEntry_sys_SDL_setup) {
    ZFGlobalObserver().observerRemove(ZFApp::E_AppEntry(), this->afterListener);

    if(this->builtinWindow != zfnull) {
        SDL_DestroyWindow(this->builtinWindow);
        this->builtinWindow = zfnull;
    }
}
public:
    SDL_Window *builtinWindow;
    SDL_Renderer *builtinRenderer;
private:
    ZFListener beforeListener;
    ZFListener afterListener;
private:
    static void before(ZF_IN zfself *d) {
        ZFCoreAssert(d->builtinWindow == zfnull);

        if(ZFImpl_sys_SDL_embed) {
            return;
        }
        unsigned int sdlInitFlag = SDL_INIT_EVERYTHING;
        do {
            if(SDL_Init(sdlInitFlag) == 0) {break;}
            zfstring errorHint = SDL_GetError();

            // ZFCoreLogTrim("[ZFMainEntry_sys_SDL] try init without audio");
            // sdlInitFlag &= (~SDL_INIT_AUDIO);
            // if(SDL_Init(sdlInitFlag) == 0) {break;}
            // zfstringAppend(errorHint, "\n    init without audio: %s", (const zfchar *)SDL_GetError());

            ZFCoreCriticalMessage("SDL init failed: %s", errorHint);
            return;
        } while(zffalse);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        d->builtinWindow = ZFImpl_sys_SDL_CreateWindow();
        if(d->builtinWindow == zfnull) {
            ZFCoreCriticalMessage("SDL window create failed: %s", SDL_GetError());
            return;
        }
        d->builtinRenderer = SDL_CreateRenderer(d->builtinWindow, -1, 0
                | SDL_RENDERER_ACCELERATED
                | SDL_RENDERER_TARGETTEXTURE
            );
        if(d->builtinRenderer == zfnull) {
            SDL_DestroyWindow(d->builtinWindow);
            d->builtinWindow = zfnull;
            ZFCoreCriticalMessage("SDL renderer create failed: %s", SDL_GetError());
            return;
        }
        SDL_SetRenderDrawBlendMode(d->builtinRenderer, SDL_BLENDMODE_BLEND);

        ZFImpl_sys_SDL_embedInit(d->builtinWindow);
    }
    static void after(ZF_IN const ZFArgs &zfargs) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFMainEntry_sys_SDL_setup) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMainEntry_sys_SDL_setup);
        ZFCoreAssert(d->builtinWindow != zfnull);

        zfbool quitFlag = zffalse;
        SDL_Event event;
        while(!quitFlag) {
            while(SDL_WaitEvent(&event)) {
                do {
                    if(!ZFImpl_sys_SDL_embedEventHandler(&event)) {
                        if(event.type == SDL_QUIT) {
                            quitFlag = zftrue;
                            break;
                        }
                    }
                } while(SDL_PollEvent(&event));
            }
        }
        ZFImpl_sys_SDL_embedCleanup();
        if(d->builtinRenderer != zfnull) {
            SDL_DestroyRenderer(d->builtinRenderer);
            d->builtinRenderer = zfnull;
        }
        if(d->builtinWindow != zfnull) {
            SDL_DestroyWindow(d->builtinWindow);
            d->builtinWindow = zfnull;;
        }
        SDL_Quit();
    }
ZF_GLOBAL_INITIALIZER_END(ZFMainEntry_sys_SDL_setup)

// ============================================================
static SDL_Window *_ZFP_ZFImpl_sys_SDL_mainWindow = zfnull;
static SDL_Renderer *_ZFP_ZFImpl_sys_SDL_mainRenderer = zfnull;
SDL_Window *ZFImpl_sys_SDL_mainWindow(void) {
    return _ZFP_ZFImpl_sys_SDL_mainWindow;
}
SDL_Renderer *ZFImpl_sys_SDL_mainRenderer(void) {
    return _ZFP_ZFImpl_sys_SDL_mainRenderer;
}

SDL_Window *ZFImpl_sys_SDL_CreateWindow(void) {
    SDL_Window *sdlWindow = SDL_CreateWindow(
        ""
        , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
        , 640, 480
        , SDL_WINDOW_SHOWN
            | SDL_WINDOW_RESIZABLE
        );
#if ZF_ENV_DEBUG
    if(sdlWindow != zfnull) {
        int windowDisplay = SDL_GetWindowDisplayIndex(sdlWindow);

        int windowW, windowH;
        SDL_GetWindowSize(sdlWindow, &windowW, &windowH);
        SDL_Rect client;
        SDL_GetDisplayUsableBounds(windowDisplay, &client);
        SDL_Rect screen;
        SDL_GetDisplayBounds(windowDisplay, &screen);
        ZFCoreLogTrim("SDL window created, display: %s, window: (%s, %s), client: (%s, %s, %s, %s), screen: (%s, %s, %s, %s)"
                , windowDisplay
                , windowW, windowH
                , client.x, client.y, client.w, client.h
                , screen.x, screen.y, screen.w, screen.h
                );
    }
#endif
    return sdlWindow;
}

// ============================================================
zfclassPOD _ZFP_ZFImpl_sys_SDL_EventHandlerData {
public:
    ZFImpl_sys_SDL_EventHandler handler;
    ZFLevel level;
};
typedef zfstlmap<Uint32, zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData> > _ZFP_ZFImpl_sys_SDL_EventHandlerMapType;
typedef zfstlmap<Sint32, zfstlvector<_ZFP_ZFImpl_sys_SDL_EventHandlerData> > _ZFP_ZFImpl_sys_SDL_UserEventHandlerMapType;
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
static Uint32 _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_PIXELFORMAT_ARGB8888;
Uint32 ZFImpl_sys_SDL_PixelFormatPreferred(void) {
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
    _ZFP_ZFImpl_sys_SDL_PixelFormatPreferred = SDL_GetWindowPixelFormat(window);
}
void ZFImpl_sys_SDL_embedCleanup(void) {
    ZFCoreAssert(_ZFP_ZFImpl_sys_SDL_mainWindow != zfnull);
    _ZFP_ZFImpl_sys_SDL_mainWindow = zfnull;
    _ZFP_ZFImpl_sys_SDL_mainRenderer = zfnull;
}

zfbool ZFImpl_sys_SDL_embedEventHandler(ZF_IN SDL_Event *event) {
    if(event->type == SDL_USEREVENT) {
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
        case SDL_QUIT:
            ZFApp::appExit();
            return zftrue;
        case SDL_APP_LOWMEMORY:
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppOnMemoryLow());
            return zftrue;
        default:
            break;
    }

    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

