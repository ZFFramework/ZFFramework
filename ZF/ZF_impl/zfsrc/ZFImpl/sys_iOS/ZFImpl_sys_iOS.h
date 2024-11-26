#ifndef _ZFI_ZFImpl_sys_iOS_h_
#define _ZFI_ZFImpl_sys_iOS_h_

#include "ZFCore/ZFCoreDef.h"

#ifdef __APPLE__
    #include "TargetConditionals.h"
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief force to use iOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_iOS
    #define ZF_ENV_FORCE_sys_iOS 0
#else
    #undef ZF_ENV_FORCE_sys_iOS
    #define ZF_ENV_FORCE_sys_iOS 1
#endif

/**
 * @brief force not to use iOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_iOS
    #define ZF_ENV_FORCE_not_sys_iOS 0
#else
    #undef ZF_ENV_FORCE_not_sys_iOS
    #define ZF_ENV_FORCE_not_sys_iOS 1
#endif

/**
 * @brief true if under iOS
 */
#if !ZF_ENV_FORCE_not_sys_iOS && ((0 \
            || (defined(__APPLE__) && (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)) \
        ) || ZF_ENV_FORCE_sys_iOS)
    #define ZF_ENV_sys_iOS 1
#else
    #define ZF_ENV_sys_iOS 0
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_sys_iOS_h_

