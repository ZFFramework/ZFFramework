#ifndef _ZFI_ZFImpl_sys_Windows_h_
#define _ZFI_ZFImpl_sys_Windows_h_

#include "ZFCore/ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief force to use Windows env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Windows
    #define ZF_ENV_FORCE_sys_Windows 0
#else
    #undef ZF_ENV_FORCE_sys_Windows
    #define ZF_ENV_FORCE_sys_Windows 1
#endif

/**
 * @brief force not to use Windows env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Windows
    #define ZF_ENV_FORCE_not_sys_Windows 0
#else
    #undef ZF_ENV_FORCE_not_sys_Windows
    #define ZF_ENV_FORCE_not_sys_Windows 1
#endif

/**
 * @brief true if under Windows
 */
#if !ZF_ENV_FORCE_not_sys_Windows && ((0 \
            || defined(WIN32) \
            || defined(_WIN32) \
        ) || ZF_ENV_FORCE_sys_Windows)
    #define ZF_ENV_sys_Windows 1
#else
    #define ZF_ENV_sys_Windows 0
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_sys_Windows_h_

