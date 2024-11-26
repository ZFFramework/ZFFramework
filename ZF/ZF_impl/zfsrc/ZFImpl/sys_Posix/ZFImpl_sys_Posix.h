#ifndef _ZFI_ZFImpl_sys_Posix_h_
#define _ZFI_ZFImpl_sys_Posix_h_

#include "ZFCore/ZFCoreDef.h"

#ifdef __APPLE__
    #include "TargetConditionals.h"
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief force to use Posix env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Posix
    #define ZF_ENV_FORCE_sys_Posix 0
#else
    #undef ZF_ENV_FORCE_sys_Posix
    #define ZF_ENV_FORCE_sys_Posix 1
#endif

/**
 * @brief force not to use Posix env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Posix
    #define ZF_ENV_FORCE_not_sys_Posix 0
#else
    #undef ZF_ENV_FORCE_not_sys_Posix
    #define ZF_ENV_FORCE_not_sys_Posix 1
#endif

/**
 * @brief true if under Posix
 */
#if !ZF_ENV_FORCE_not_sys_Posix && ((0 \
            || defined(__unix__) \
            || defined(__linux) \
            || defined(__APPLE__) \
        ) || ZF_ENV_FORCE_sys_Posix)
    #define ZF_ENV_sys_Posix 1
#else
    #define ZF_ENV_sys_Posix 0
#endif

// ============================================================

/**
 * @brief force to use MacOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_MacOS
    #define ZF_ENV_FORCE_sys_MacOS 0
#else
    #undef ZF_ENV_FORCE_sys_MacOS
    #define ZF_ENV_FORCE_sys_MacOS 1
#endif

/**
 * @brief force not to use MacOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_MacOS
    #define ZF_ENV_FORCE_not_sys_MacOS 0
#else
    #undef ZF_ENV_FORCE_not_sys_MacOS
    #define ZF_ENV_FORCE_not_sys_MacOS 1
#endif

/**
 * @brief true if under MacOS
 */
#if !ZF_ENV_FORCE_not_sys_MacOS && ((0 \
            || (defined(__APPLE__) && TARGET_OS_MAC) \
        ) || ZF_ENV_FORCE_sys_MacOS)
    #define ZF_ENV_sys_MacOS 1
#else
    #define ZF_ENV_sys_MacOS 0
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_sys_Posix_h_

