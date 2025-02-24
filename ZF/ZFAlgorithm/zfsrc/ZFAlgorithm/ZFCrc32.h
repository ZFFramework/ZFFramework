/**
 * @file ZFCrc32.h
 * @brief CRC32 utility
 */

#ifndef _ZFI_ZFCrc32_h_
#define _ZFI_ZFCrc32_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief invalid value for CRC32
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, zfflags, ZFCrc32Invalid)

/**
 * @brief calculate CRC32, return #ZFCrc32Invalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
extern ZFLIB_ZFAlgorithm zfflags ZFCrc32(
        ZF_IN const void *src
        , ZF_IN zfindex srcLen
        , ZF_IN_OPT zfflags prevResult = 0
        );
/**
 * @brief calculate CRC32, return #ZFCrc32Invalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfflags, ZFCrc32
        , ZFMP_IN(const ZFInput &, callback)
        , ZFMP_IN_OPT(zfflags, prevResult, 0)
        )
/**
 * @brief see #ZFCrc32
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfflags, ZFCrc32
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(zfflags, prevResult, 0)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCrc32_h_

