#ifndef _ZFI_ZFMainEntry_sys_SDL_h_
#define _ZFI_ZFMainEntry_sys_SDL_h_

#include "ZFCore.h"
#include "ZFImpl_sys_SDL_ZF_impl.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief the sdl's root window
 */
extern ZFLIB_ZF_impl SDL_Window *ZFImpl_sys_SDL_rootWindow(void);

/**
 * @brief event handler for sdl event loop
 *
 * return true if the event was handled and should not be processed further
 */
typedef zfbool (*ZFImpl_sys_SDL_EventHandler)(ZF_IN SDL_Event *event);

/** @brief add event handler, see #ZFImpl_sys_SDL_EventHandler */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_eventHandlerAdd(ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler,
                                                         ZF_IN_OPT ZFLevel level = ZFLevelAppNormal);
/** @brief remove event handler, see #ZFImpl_sys_SDL_EventHandler */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_eventHandlerRemove(ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler);

/**
 * @brief util to #ZFImpl_sys_SDL_eventHandlerAdd
 *
 * @code
 *   // declare event handler, proto type:
 *   //   zfbool eventHandler(SDL_Event *sdlEvent);
 *   ZFIMPL_SYS_SDL_EVENT_HANDLER(YourEvent, {
 *       // the event
 *       zfLogTrimT() << sdlEvent->user.code;
 *       return zftrue;
 *   })
 *
 *   // post the event
 *   SDL_Event e;
 *   e.type = SDL_USEREVENT;
 *   e.user.code = ZFIMPL_SYS_SDL_EVENT(eventName);
 *   SDL_PushEvent(&e);
 * @endcode
 */
#define ZFIMPL_SYS_SDL_EVENT_HANDLER(name, handlerAction) \
    ZFIDMAP_GLOBAL_DETAIL(_ZFP_SDLEvent, name) \
    ZFIDMAP_GLOBAL_REGISTER_DETAIL(_ZFP_SDLEvent, name) \
    static zfbool _ZFP_SDLEventH_##name(ZF_IN SDL_Event *sdlEvent) \
    { \
        if(sdlEvent->type != SDL_USEREVENT || sdlEvent->user.code != ZFIMPL_SYS_SDL_EVENT(name)) \
        { \
            return zffalse; \
        } \
        handlerAction \
    } \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(SDLEventReg_##name, ZFLevelZFFrameworkStatic) \
    { \
        ZFImpl_sys_SDL_eventHandlerAdd(_ZFP_SDLEventH_##name, ZFLevelZFFrameworkNormal); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(SDLEventReg_##name) \
    { \
        ZFImpl_sys_SDL_eventHandlerRemove(_ZFP_SDLEventH_##name); \
    } \
    ZF_GLOBAL_INITIALIZER_END(SDLEventReg_##name)
/**
 * @brief see #ZFIMPL_SYS_SDL_EVENT_HANDLER
 */
#define ZFIMPL_SYS_SDL_EVENT(name) \
    ((Sint32)_ZFP_SDLEvent##name())

// ============================================================
/**
 * @brief whether use embed impl
 *
 * false by default, which would use builtin SDL_Window and event loop\n
 * if you want to attach your custom impl:
 * -# set ZFImpl_sys_SDL_embed to true before
 *   #ZFApp::EventAppParamDispatch ZFLevelZFFrameworkNormal
 * -# attach custom SDL_Window by #ZFImpl_sys_SDL_embedInit
 * -# in your sdl event loop, resolve event by #ZFImpl_sys_SDL_embedEventHandler
 * -# #ZFImpl_sys_SDL_embedCleanup before app exit
 */
extern ZFLIB_ZF_impl zfbool ZFImpl_sys_SDL_embed;

/** @brief see #ZFImpl_sys_SDL_embed */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_embedInit(ZF_IN SDL_Window *window);
/** @brief see #ZFImpl_sys_SDL_embed */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_embedCleanup(void);

/**
 * @brief see #ZFImpl_sys_SDL_embed
 *
 * return true if the event was handled and should not be processed further
 */
extern ZFLIB_ZF_impl zfbool ZFImpl_sys_SDL_embedEventHandler(ZF_IN SDL_Event *event);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFMainEntry_sys_SDL_h_

