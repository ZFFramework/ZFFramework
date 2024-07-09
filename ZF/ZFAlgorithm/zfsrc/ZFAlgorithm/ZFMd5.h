/**
 * @file ZFMd5.h
 * @brief MD5 utility
 */

#ifndef _ZFI_ZFMd5_h_
#define _ZFI_ZFMd5_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief calculate MD5, return empty string if failed
 */
extern ZFLIB_ZFAlgorithm void ZFMd5T(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const void *src
        , ZF_IN zfindex srcLen
        , ZF_IN_OPT zfbool upperCase = zftrue
        );
/** @brief see #ZFMd5 */
inline zfstring ZFMd5(
        ZF_IN const void *src
        , ZF_IN zfindex srcLen
        , ZF_IN_OPT zfbool upperCase = zftrue
        ) {
    zfstring ret;
    ZFMd5T(ret, src, srcLen, upperCase);
    return ret;
}

/** @brief see #ZFMd5 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFMd5T
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFInput &, callback)
        , ZFMP_IN_OPT(zfbool, upperCase, zftrue)
        )
/** @brief see #ZFMd5 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfstring, ZFMd5
        , ZFMP_IN(const ZFInput &, callback)
        , ZFMP_IN_OPT(zfbool, upperCase, zftrue)
        )

/** @brief see #ZFMd5 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, void, ZFMd5T
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(zfbool, upperCase, zftrue)
        )
/** @brief see #ZFMd5 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfstring, ZFMd5
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(zfbool, upperCase, zftrue)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMd5_h_

