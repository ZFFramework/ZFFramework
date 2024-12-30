#ifndef _ZFI_ZFImpl_sys_SDL_KeyCode_h_
#define _ZFI_ZFImpl_sys_SDL_KeyCode_h_

#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/ZFUIKeyCode.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief convert key code from impl
 */
extern ZFLIB_ZFUIKit_impl ZFUIKeyCode ZFImpl_sys_SDL_ZFUIKeyCodeFromRaw(ZF_IN SDL_KeyCode raw);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_KeyCode_h_

