#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_SDL
#include "ZFImpl/sys_SDL/ZFMainEntry_sys_SDL.h"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    return ZFImpl_sys_SDL_AppInit(appstate, argc, argv);
}
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

