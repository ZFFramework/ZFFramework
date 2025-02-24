/**
 * @file ZFNamespace.h
 * @brief global namespace define
 */
#ifndef _ZFI_ZFNamespace_h_
#define _ZFI_ZFNamespace_h_

#include "ZFCoreEnvDef.h"

// ============================================================
class ZFLIB_ZFCore _ZFP_ZFNamespaceHolder {
public:
    _ZFP_ZFNamespaceHolder(const char *parent, const char *child);
    ~_ZFP_ZFNamespaceHolder(void);
public:
    const char *ns(void) const {
        return _ns;
    }
private:
    const char *_ns;
};

/**
 * @brief begin namespace
 *
 * when declaring your own new namespace,
 * you must register it by #ZF_NAMESPACE_BEGIN_REGISTER before any other
 * access or declare of the same namespace
 * @code
 *   // YourModule.h
 *   ZF_NAMESPACE_BEGIN_REGISTER(YourNS, ParentNS)
 *   ZF_NAMESPACE_END(YourNS)
 *
 *   // OtherModule.h
 *   #include "YourModule.h" // include the registered YourNS
 *   ZF_NAMESPACE_BEGIN(YourNS) // extend the YourNS
 *   ZF_NAMESPACE_END(YourNS)
 * @endcode
 */
#define ZF_NAMESPACE_BEGIN(NameSpace) \
    namespace NameSpace { \
        template<typename T> \
        const char *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void);

/**
 * @brief begin and register namespace
 */
#define ZF_NAMESPACE_BEGIN_REGISTER(NameSpace, ParentNameSpace) \
    /** @brief \n */ \
    namespace NameSpace { \
        template<typename T> \
        const char *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void) { \
            static _ZFP_ZFNamespaceHolder d(ParentNameSpace::_ZFP_ZF_NAMESPACE_NOT_REGISTERED<T>(), #NameSpace); \
            return d.ns(); \
        }

/**
 * @brief end namespace
 */
#define ZF_NAMESPACE_END(NameSpace) \
    }

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
#endif

ZF_NAMESPACE_GLOBAL_BEGIN
    template<typename T>
    const char *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void) {
        return 0;
    }
ZF_NAMESPACE_GLOBAL_END
ZF_NAMESPACE_GLOBAL_USE

#endif // #ifndef _ZFI_ZFNamespace_h_

