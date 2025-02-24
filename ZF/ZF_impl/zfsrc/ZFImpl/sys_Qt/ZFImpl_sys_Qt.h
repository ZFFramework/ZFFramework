#ifndef _ZFI_ZFImpl_sys_Qt_h_
#define _ZFI_ZFImpl_sys_Qt_h_

#include "ZFCore/ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief force to use Qt env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Qt
    #define ZF_ENV_FORCE_sys_Qt 0
#else
    #undef ZF_ENV_FORCE_sys_Qt
    #define ZF_ENV_FORCE_sys_Qt 1
#endif

/**
 * @brief force not to use Qt env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Qt
    #define ZF_ENV_FORCE_not_sys_Qt 0
#else
    #undef ZF_ENV_FORCE_not_sys_Qt
    #define ZF_ENV_FORCE_not_sys_Qt 1
#endif

/**
 * @brief true if under QT
 */
#if !ZF_ENV_FORCE_not_sys_Qt && ((0 \
            || defined(QT_VERSION) \
            || defined(QT_CORE_LIB) \
        ) || ZF_ENV_FORCE_sys_Qt)
    #define ZF_ENV_sys_Qt 1
#else
    #define ZF_ENV_sys_Qt 0
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_sys_Qt_h_

