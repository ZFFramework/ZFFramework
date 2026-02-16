#include "ZFImpl_sys_SDL_Image.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_AppIcon, ZFLevelZFFrameworkEssential) {
    ZFImpl_sys_SDL_WindowOnCreate.add(zfself::_action);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_AppIcon) {
    ZFImpl_sys_SDL_WindowOnCreate.removeElement(zfself::_action);
}
public:
    static void _action(ZF_IN SDL_Window *owner) {
        ZFInput input = ZFInputForRes("ZFUIKit_impl/sys_SDL/AppIcon.png");
        if(input) {
            SDL_Surface *sdlSurface = IMG_Load_IO(ZFImpl_sys_SDL_ZFInputToSDL_IOStream(input), true);
            SDL_SetWindowIcon(owner, sdlSurface);
            SDL_DestroySurface(sdlSurface);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_AppIcon)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

