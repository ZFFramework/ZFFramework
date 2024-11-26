#ifndef _ZFI_ZFImpl_sys_SDL_h_
#define _ZFI_ZFImpl_sys_SDL_h_

#include "ZFCore/ZFCoreDef.h"
#include "../sys_Android/ZFImpl_sys_Android.h"
#include "../sys_iOS/ZFImpl_sys_iOS.h"
#include "../sys_Qt/ZFImpl_sys_Qt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief force to use SDL env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_SDL
    #define ZF_ENV_FORCE_sys_SDL 0
#else
    #undef ZF_ENV_FORCE_sys_SDL
    #define ZF_ENV_FORCE_sys_SDL 1
#endif

/**
 * @brief force not to use SDL env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_SDL
    #define ZF_ENV_FORCE_not_sys_SDL 0
#else
    #undef ZF_ENV_FORCE_not_sys_SDL
    #define ZF_ENV_FORCE_not_sys_SDL 1
#endif

/**
 * @brief true if under SDL
 */
#if !ZF_ENV_FORCE_not_sys_SDL && ((0 \
        ) || ZF_ENV_FORCE_sys_SDL)
    #define ZF_ENV_sys_SDL 1
#else
    #define ZF_ENV_sys_SDL 0
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_sys_SDL_h_

