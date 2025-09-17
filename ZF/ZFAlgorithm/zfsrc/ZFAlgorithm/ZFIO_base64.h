/**
 * @file ZFIO_base64.h
 * @brief base64 #ZFPathInfo
 */

#ifndef _ZFI_ZFIO_base64_h_
#define _ZFI_ZFIO_base64_h_

#include "ZFBase64.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the refPathInfo, a typical case:
 * @code
 *   base64!!file!!some/path/to/a.zip
 *   base64!!res!!some/path/to/a.zip
 *   base64!!http!!http://abc.com/some/path/to/a.zip
 * @endcode
 */
ZFIO_DECLARE(ZFLIB_ZFAlgorithm, base64)

// ============================================================
// ZFInputForBase64
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the base64 file path to use,
 *   see #ZFPathType_base64 for more info
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForBase64
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        )

// ============================================================
// ZFOutputForBase64
/**
 * @brief util to create a base64 file output callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the base64 file path to use,
 *   see #ZFPathType_base64 for more info
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForBase64
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        )

// ============================================================
// ZFInputForBase64
/**
 * @brief util to create a file input callback
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForBase64
        , ZFMP_IN(const ZFInput &, refInput)
        )

// ============================================================
// ZFOutputForBase64
/**
 * @brief util to create a base64 file output callback
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForBase64
        , ZFMP_IN(const ZFOutput &, refOutput)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIO_base64_h_

