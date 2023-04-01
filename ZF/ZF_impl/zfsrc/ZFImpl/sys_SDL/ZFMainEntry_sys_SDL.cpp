#include "ZFMainEntry_sys_SDL.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMainEntry_sys_SDL_setup, ZFLevelZFFrameworkStatic)
{
    this->beforeListener = ZFCallbackForFunc(zfself::before);
    ZFGlobalObserver().observerAdd(
            ZFApp::EventAppParamDispatch(),
            this->beforeListener,
            ZFLevelZFFrameworkNormal
        );
    this->afterListener = ZFCallbackForFunc(zfself::after);
    ZFGlobalObserver().observerAdd(
            ZFApp::EventAppParamDispatch(),
            this->afterListener,
            ZFLevelZFFrameworkPostHigh
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMainEntry_sys_SDL_setup)
{
    ZFGlobalObserver().observerRemove(ZFApp::EventAppParamDispatch(), this->beforeListener);
    ZFGlobalObserver().observerRemove(ZFApp::EventAppParamDispatch(), this->afterListener);
}
private:
    ZFListener beforeListener;
    ZFListener afterListener;
private:
    static void before(ZF_IN const ZFArgs &zfargs)
    {
        if(ZFImpl_sys_SDL_embed)
        {
            return;
        }
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            zfCoreCriticalMessage("SDL init failed: %s", SDL_GetError());
            return;
        }

        SDL_Window *window = SDL_CreateWindow(
            ""
            , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
            , 640, 480
            , SDL_WINDOW_SHOWN
            );
        if(window == zfnull)
        {
            zfCoreCriticalMessage("SDL window create failed: %s", SDL_GetError());
            return;
        }

        ZFImpl_sys_SDL_embedInit(window);
    }
    static void after(ZF_IN const ZFArgs &zfargs)
    {
        zfbool quitFlag = zffalse;
        SDL_Event event;
        while(!quitFlag)
        {
            while(SDL_WaitEvent(&event))
            {
                if(!ZFImpl_sys_SDL_embedEventHandler(&event))
                {
                    if(event.type == SDL_QUIT)
                    {
                        quitFlag = zftrue;
                        break;
                    }
                }
            }
        }
        ZFImpl_sys_SDL_embedCleanup();
        SDL_DestroyWindow(ZFImpl_sys_SDL_rootWindow());
        SDL_Quit();
    }
ZF_GLOBAL_INITIALIZER_END(ZFMainEntry_sys_SDL_setup)

// ============================================================
static SDL_Window *_ZFP_ZFImpl_sys_SDL_rootWindow = zfnull;
SDL_Window *ZFImpl_sys_SDL_rootWindow(void)
{
    return _ZFP_ZFImpl_sys_SDL_rootWindow;
}

static zfstlvector<ZFImpl_sys_SDL_EventHandler> _ZFP_ZFImpl_sys_SDL_eventHandlerList;
static zfstlvector<ZFLevel> _ZFP_ZFImpl_sys_SDL_eventHandlerLevelList;
void ZFImpl_sys_SDL_eventHandlerAdd(ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler,
                                    ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */)
{
    zfCoreMutexLocker();
    zfCoreAssert(eventHandler != zfnull);
    zfstlsize pos = 0;
    while(pos < _ZFP_ZFImpl_sys_SDL_eventHandlerList.size()
        && level > _ZFP_ZFImpl_sys_SDL_eventHandlerLevelList[pos]
    ) {
        ++pos;
    }
    _ZFP_ZFImpl_sys_SDL_eventHandlerList.insert(_ZFP_ZFImpl_sys_SDL_eventHandlerList.begin() + pos, eventHandler);
    _ZFP_ZFImpl_sys_SDL_eventHandlerLevelList.insert(_ZFP_ZFImpl_sys_SDL_eventHandlerLevelList.begin() + pos, level);
}
void ZFImpl_sys_SDL_eventHandlerRemove(ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler)
{
    zfCoreMutexLocker();
    if(eventHandler != zfnull)
    {
        for(zfstlsize i = 0; i < _ZFP_ZFImpl_sys_SDL_eventHandlerList.size(); ++i)
        {
            if(_ZFP_ZFImpl_sys_SDL_eventHandlerList[i] == eventHandler)
            {
                _ZFP_ZFImpl_sys_SDL_eventHandlerList.erase(_ZFP_ZFImpl_sys_SDL_eventHandlerList.begin() + i);
                _ZFP_ZFImpl_sys_SDL_eventHandlerLevelList.erase(_ZFP_ZFImpl_sys_SDL_eventHandlerLevelList.begin() + i);
                break;
            }
        }
    }
}

// ============================================================
zfbool ZFImpl_sys_SDL_embed = zffalse;

void ZFImpl_sys_SDL_embedInit(ZF_IN SDL_Window *window)
{
    zfCoreAssert(window != zfnull);
    zfCoreAssert(_ZFP_ZFImpl_sys_SDL_rootWindow == zfnull);
    _ZFP_ZFImpl_sys_SDL_rootWindow = window;
}
void ZFImpl_sys_SDL_embedCleanup(void)
{
    zfCoreAssert(_ZFP_ZFImpl_sys_SDL_rootWindow != zfnull);
    _ZFP_ZFImpl_sys_SDL_rootWindow = zfnull;
}

zfbool ZFImpl_sys_SDL_embedEventHandler(ZF_IN SDL_Event *event)
{
    for(zfstlsize i = 0; i < _ZFP_ZFImpl_sys_SDL_eventHandlerList.size(); ++i)
    {
        if(_ZFP_ZFImpl_sys_SDL_eventHandlerList[i](event))
        {
            return zftrue;
        }
    }
    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

