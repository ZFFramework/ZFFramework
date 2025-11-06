#ifndef _ZFI_ZFMainEntry_sys_SDL_h_
#define _ZFI_ZFMainEntry_sys_SDL_h_

#include "ZFCore.h"
#include "ZFImpl_sys_SDL_ZF_impl.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief the sdl's window
 */
extern ZFLIB_ZF_impl SDL_Window *ZFImpl_sys_SDL_mainWindow(void);
/**
 * @brief the sdl's renderer
 */
extern ZFLIB_ZF_impl SDL_Renderer *ZFImpl_sys_SDL_mainRenderer(void);

/**
 * @brief create window by default setting
 */
extern ZFLIB_ZF_impl SDL_Window *ZFImpl_sys_SDL_CreateWindow(void);

/**
 * @brief get preferred pixel format for app
 */
extern ZFLIB_ZF_impl SDL_PixelFormat ZFImpl_sys_SDL_PixelFormatPreferred(void);
/**
 * @brief set preferred pixel format for app
 */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_PixelFormatPreferred(ZF_IN SDL_PixelFormat f);

// ============================================================
typedef void (*ZFImpl_sys_SDL_WindowLifeCycle)(ZF_IN SDL_Window *owner);
extern ZFLIB_ZF_impl ZFCoreArray<ZFImpl_sys_SDL_WindowLifeCycle> ZFImpl_sys_SDL_WindowOnCreate;
extern ZFLIB_ZF_impl ZFCoreArray<ZFImpl_sys_SDL_WindowLifeCycle> ZFImpl_sys_SDL_WindowOnDestroy;
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_WindowNotifyCreate(ZF_IN SDL_Window *owner);
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_WindowNotifyDestroy(ZF_IN SDL_Window *owner);

typedef void (*ZFImpl_sys_SDL_RendererLifeCycle)(ZF_IN SDL_Renderer *owner);
extern ZFLIB_ZF_impl ZFCoreArray<ZFImpl_sys_SDL_RendererLifeCycle> ZFImpl_sys_SDL_RendererOnCreate;
extern ZFLIB_ZF_impl ZFCoreArray<ZFImpl_sys_SDL_RendererLifeCycle> ZFImpl_sys_SDL_RendererOnDestroy;
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_RendererNotifyCreate(ZF_IN SDL_Renderer *owner);
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_RendererNotifyDestroy(ZF_IN SDL_Renderer *owner);

// ============================================================
/**
 * @brief event handler for sdl event loop
 *
 * return true if the event was handled and should not be processed further
 */
typedef zfbool (*ZFImpl_sys_SDL_EventHandler)(ZF_IN SDL_Event *event);

/** @brief add event handler, see #ZFImpl_sys_SDL_EventHandler */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_eventHandlerAdd(
        ZF_IN Uint32 type
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        , ZF_IN_OPT ZFLevel level = ZFLevelAppNormal
        );
/** @brief remove event handler, see #ZFImpl_sys_SDL_EventHandler */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_eventHandlerRemove(
        ZF_IN Uint32 type
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        );

/** @brief add user event handler, see #ZFIMPL_SYS_SDL_USER_EVENT_HANDLER */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_userEventHandlerAdd(
        ZF_IN Sint32 userEventCode
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        , ZF_IN_OPT ZFLevel level = ZFLevelAppNormal
        );
/** @brief remove user event handler, see #ZFIMPL_SYS_SDL_USER_EVENT_HANDLER */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_userEventHandlerRemove(
        ZF_IN Sint32 userEventCode
        , ZF_IN ZFImpl_sys_SDL_EventHandler eventHandler
        );

/**
 * @brief util to declare event handler
 *
 * @code
 *   // declare event handler, proto type:
 *   //   zfbool eventHandler(SDL_Event *sdlEvent);
 *   ZFIMPL_SYS_SDL_EVENT_HANDLER(YourModuleName, sdlEventId, ZFLevelAppNormal) {
 *       // the event
 *       ZFLogTrim() << sdlEvent->user.code;
 *       return zftrue;
 *   }
 * @endcode
 */
#define ZFIMPL_SYS_SDL_EVENT_HANDLER(name, sdlEventId, level) \
    static zfbool _ZFP_SDLEventH_##name(ZF_IN SDL_Event *sdlEvent); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(SDLEventReg_##name, ZFLevelZFFrameworkStatic) { \
        ZFImpl_sys_SDL_eventHandlerAdd(sdlEventId, _ZFP_SDLEventH_##name, level); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(SDLEventReg_##name) { \
        ZFImpl_sys_SDL_eventHandlerRemove(sdlEventId, _ZFP_SDLEventH_##name); \
    } \
    ZF_GLOBAL_INITIALIZER_END(SDLEventReg_##name) \
    static zfbool _ZFP_SDLEventH_##name(ZF_IN SDL_Event *sdlEvent)

/**
 * @brief util to declare a user event and event handler
 *
 * @code
 *   // declare event handler, proto type:
 *   //   zfbool eventHandler(SDL_Event *sdlEvent);
 *   ZFIMPL_SYS_SDL_USER_EVENT_HANDLER(YourEvent, ZFLevelAppNormal) {
 *       // the event
 *       ZFLogTrim() << sdlEvent->user.code;
 *       return zftrue;
 *   }
 *
 *   // post the event
 *   SDL_Event e;
 *   SDL_zero(e);
 *   e.type = SDL_EVENT_USER;
 *   e.user.code = ZFIMPL_SYS_SDL_USER_EVENT(eventName);
 *   SDL_PushEvent(&e);
 *   // or for short
 *   ZFIMPL_SYS_SDL_USER_EVENT_POST(eventName, zfnull, zfnull);
 * @endcode
 */
#define ZFIMPL_SYS_SDL_USER_EVENT_HANDLER(name, level) \
    ZFIDMAP_GLOBAL_REGISTER_DETAIL(_ZFP_SDLUEvent, name) \
    static zfbool _ZFP_SDLUEventH_##name(ZF_IN SDL_Event *sdlEvent); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(SDLEventReg_##name, ZFLevelZFFrameworkStatic) { \
        ZFImpl_sys_SDL_userEventHandlerAdd(((Sint32)_ZFP_SDLUEvent##name()), _ZFP_SDLUEventH_##name, level); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(SDLEventReg_##name) { \
        ZFImpl_sys_SDL_userEventHandlerRemove(((Sint32)_ZFP_SDLUEvent##name()), _ZFP_SDLUEventH_##name); \
    } \
    ZF_GLOBAL_INITIALIZER_END(SDLEventReg_##name) \
    static zfbool _ZFP_SDLUEventH_##name(ZF_IN SDL_Event *sdlEvent)
/**
 * @brief see #ZFIMPL_SYS_SDL_USER_EVENT_HANDLER
 */
#define ZFIMPL_SYS_SDL_USER_EVENT(name) \
    ((Sint32)_ZFP_SDLUEvent##name())

/**
 * @brief see #ZFIMPL_SYS_SDL_USER_EVENT_HANDLER
 */
#define ZFIMPL_SYS_SDL_USER_EVENT_POST(name, data1_, data2_) \
    do { \
        SDL_Event e; \
        SDL_zero(e); \
        e.type = SDL_EVENT_USER; \
        e.user.code = ZFIMPL_SYS_SDL_USER_EVENT(name); \
        e.user.data1 = data1_; \
        e.user.data2 = data2_; \
        SDL_PushEvent(&e); \
    } while(zffalse)

// ============================================================
/**
 * @brief whether use embed impl
 *
 * false by default, which would use builtin SDL_Window and event loop\n
 * if you want to attach your custom impl:
 * -# set ZFImpl_sys_SDL_embed to true
 *   using #ZF_GLOBAL_INITIALIZER_INIT
 * -# attach custom SDL_Renderer by #ZFImpl_sys_SDL_embedInit
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

