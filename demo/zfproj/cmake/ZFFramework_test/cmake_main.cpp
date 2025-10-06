#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_SDL
#include "ZFImpl/sys_SDL/ZFMainEntry_sys_SDL.h"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>

#if ZF_ENV_sys_Windows
#include <Windows.h>
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    LoadLibraryA("ZFCore");
    LoadLibraryA("ZFAlgorithm");
    LoadLibraryA("ZFUtility");
    LoadLibraryA("ZFUIKit");
    LoadLibraryA("ZFUIWidget");
    LoadLibraryA("ZFUIExt");
    LoadLibraryA("ZFLua");
    LoadLibraryA("ZFUIWebKit");
    LoadLibraryA("ZFNet");
    LoadLibraryA("ZFAppUtil");
    LoadLibraryA("ZF_impl");
    LoadLibraryA("ZFCore_impl");
    LoadLibraryA("ZFAlgorithm_impl");
    LoadLibraryA("ZFUIKit_impl");
    LoadLibraryA("ZFLua_impl");
    LoadLibraryA("ZFUIWebKit_impl");
    LoadLibraryA("ZFNet_impl");

    return ZFImpl_sys_SDL_AppInit(appstate, argc, argv);
}
#else
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    return ZFImpl_sys_SDL_AppInit(appstate, argc, argv);
}
#endif
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    return ZFImpl_sys_SDL_AppEvent(appstate, event);
}
SDL_AppResult SDL_AppIterate(void *appstate) {
    return ZFImpl_sys_SDL_AppIterate(appstate);
}
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    return ZFImpl_sys_SDL_AppQuit(appstate, result);
}

#endif

