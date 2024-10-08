/**
 * @file ZFNamespace.h
 * @brief global namespace define
 */
#ifndef _ZFI_ZFNamespace_h_
#define _ZFI_ZFNamespace_h_

#include "ZFCoreEnvDef.h"

// ============================================================
/**
 * @brief begin namespace
 */
#define ZF_NAMESPACE_BEGIN(NameSpace) \
    /** @brief \n */ \
    namespace NameSpace { \
        extern ZFLIB_ZFCore const char *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void);

/**
 * @brief end namespace
 */
#define ZF_NAMESPACE_END(NameSpace) \
    }
/**
 * @brief end namespace
 */
#define ZF_NAMESPACE_END_WITH_REGISTER(NameSpace, ParentNameSpace) \
    ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace) \
    ZF_NAMESPACE_END(NameSpace)

/**
 * @brief use namespace
 */
#define ZF_NAMESPACE_USE(NameSpace) \
    using namespace NameSpace;

/**
 * @brief global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_ID ZF
/**
 * @brief global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_NAME ZFM_TOSTRING(ZF_NAMESPACE_GLOBAL_ID)
/**
 * @brief abbreviation for global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_ABBR_ID zf
/**
 * @brief global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_ABBR_NAME ZFM_TOSTRING(ZF_NAMESPACE_GLOBAL_ABBR_ID)
/**
 * @def ZF_NAMESPACE_GLOBAL
 * @brief namespace of ZFFramework
 * @def ZF_NAMESPACE_GLOBAL_BEGIN
 * @brief begin namespace ZFFramework
 * @def ZF_NAMESPACE_GLOBAL_END
 * @brief end namespace ZFFramework
 * @def ZF_NAMESPACE_GLOBAL_USE
 * @brief use namespace ZFFramework
 */
#if !ZF_ENV_NAMESPACE_GLOBAL_DISABLE
    #define ZF_NAMESPACE_GLOBAL ZF_NAMESPACE_GLOBAL_ID
    #define ZF_NAMESPACE_GLOBAL_BEGIN ZF_NAMESPACE_BEGIN(ZF_NAMESPACE_GLOBAL)
    #define ZF_NAMESPACE_GLOBAL_END ZF_NAMESPACE_END(ZF_NAMESPACE_GLOBAL)
    #define ZF_NAMESPACE_GLOBAL_USE ZF_NAMESPACE_USE(ZF_NAMESPACE_GLOBAL)
#else
    #define ZF_NAMESPACE_GLOBAL
    #define ZF_NAMESPACE_GLOBAL_BEGIN
    #define ZF_NAMESPACE_GLOBAL_END
    #define ZF_NAMESPACE_GLOBAL_USE
    extern ZFLIB_ZFCore const char *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void);
#endif

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_GLOBAL_END
ZF_NAMESPACE_GLOBAL_USE

#endif // #ifndef _ZFI_ZFNamespace_h_

