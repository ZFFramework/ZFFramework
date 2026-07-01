/**
 * @file ZFSourceCodeMap.h
 * @brief source code map for debug use only
 */

#ifndef _ZFI_ZFSourceCodeMap_h_
#define _ZFI_ZFSourceCodeMap_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief source code map for debug use only
 *
 * ZFFramework is fully dynamic,
 * you may use #ZFDynamic to register namespaces, classes, methods, properties,
 * but making it hard to navigate and find definitions\n
 *
 * to solve this, you may use ZFSourceCodeMap
 * to attach additional location info for them,
 * related methods:
 * -  #ZFSourceCodeMap::attachNamespace
 * -  #ZFSourceCodeMap::attachClass
 * -  #ZFSourceCodeMap::attachMethod
 * -  #ZFSourceCodeMap::checkNamespace
 * -  #ZFSourceCodeMap::checkClas
 * -  #ZFSourceCodeMap::checkMethod
 */
ZF_NAMESPACE_BEGIN_REGISTER(ZFSourceCodeMap, ZF_NAMESPACE_GLOBAL)

/** @brief info for #ZFSourceCodeMap */
zfclassLikePOD ZFLIB_ZFCore Info {
public:
    /** @brief position */
    ZFPathInfo pathInfo;
    /** @brief line */
    zfindex line;
public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN Info const &ref) const {
        return zftrue
            && this->pathInfo == ref.pathInfo
            && this->line == ref.line
            ;
    }
    zfbool operator != (ZF_IN Info const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, Info, Info)

/**
 * @brief whether source code map should be enabled
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFCore, zfbool, Enable)

// ============================================================
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, attachNamespace
        , ZFMP_IN(const zfstring &, v)
        , ZFMP_IN(const Info &, info)
        )
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, detachNamespace
        , ZFMP_IN(const zfstring &, v)
        , ZFMP_IN(const Info &, info)
        )
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFCoreArray<Info>, checkNamespace
        , ZFMP_IN(const zfstring &, v)
        )

// ============================================================
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, attachClass
        , ZFMP_IN(const ZFClass *, v)
        , ZFMP_IN(const Info &, info)
        )
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, detachClass
        , ZFMP_IN(const ZFClass *, v)
        , ZFMP_IN(const Info &, info)
        )
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFCoreArray<Info>, checkClass
        , ZFMP_IN(const ZFClass *, v)
        )

// ============================================================
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, attachMethod
        , ZFMP_IN(const ZFMethod *, v)
        , ZFMP_IN(const Info &, info)
        )
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, detachMethod
        , ZFMP_IN(const ZFMethod *, v)
        , ZFMP_IN(const Info &, info)
        )
/** @brief see #ZFSourceCodeMap */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFCoreArray<Info>, checkMethod
        , ZFMP_IN(const ZFMethod *, v)
        )

ZF_NAMESPACE_END(ZFSourceCodeMap)

ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, Info, Info, ZFSourceCodeMap)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSourceCodeMap_h_

