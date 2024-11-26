#ifndef _ZFI_ZFImpl_sys_Android_h_
#define _ZFI_ZFImpl_sys_Android_h_

#include "ZFCore/ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief force to use Android env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Android
    #define ZF_ENV_FORCE_sys_Android 0
#else
    #undef ZF_ENV_FORCE_sys_Android
    #define ZF_ENV_FORCE_sys_Android 1
#endif

/**
 * @brief force not to use Android env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Android
    #define ZF_ENV_FORCE_not_sys_Android 0
#else
    #undef ZF_ENV_FORCE_not_sys_Android
    #define ZF_ENV_FORCE_not_sys_Android 1
#endif

/**
 * @brief true if under Android
 */
#if !ZF_ENV_FORCE_not_sys_Android && ((0 \
            || defined(ANDROID) \
            || defined(__ANDROID__) \
        ) || ZF_ENV_FORCE_sys_Android)
    #define ZF_ENV_sys_Android 1
#else
    #define ZF_ENV_sys_Android 0
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_sys_Android_h_

