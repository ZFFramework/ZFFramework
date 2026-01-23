/**
 * @file ZFIO_encrypt.h
 * @brief encrypt #ZFPathInfo
 */

#ifndef _ZFI_ZFIO_encrypt_h_
#define _ZFI_ZFIO_encrypt_h_

#include "ZFEncrypt.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is "refPathInfo|encryptKey",
 * see #ZFPathInfoChainDecode for more info
 *
 * typical case:
 * @code
 *   encrypt!!file!!some/path/to/file|encryptKey
 *   encrypt!!res!!some/path/to/file|encryptKey
 *   encrypt!!http!!http://abc.com/some/path/to/file|encryptKey
 * @endcode
 */
ZFIO_DECLARE(ZFLIB_ZFAlgorithm, encrypt)

// ============================================================
// ZFInputForEncrypt
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the encrypt file path to use,
 *   see #ZFPathType_encrypt for more info
 * -  (const zfstring &)encryptKey : the encrypt key to use
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfstring &, encryptKey)
        )

// ============================================================
// ZFOutputForEncrypt
/**
 * @brief util to create a encrypt file output callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the encrypt file path to use,
 *   see #ZFPathType_encrypt for more info
 * -  (const zfstring &)encryptKey : the encrypt key to use
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfstring &, encryptKey)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        )

// ============================================================
// ZFInputForEncrypt
/**
 * @brief util to create a file input callback
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForEncrypt
        , ZFMP_IN(const ZFInput &, refInput)
        , ZFMP_IN(const zfstring &, encryptKey)
        )

// ============================================================
// ZFOutputForEncrypt
/**
 * @brief util to create a encrypt file output callback
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForEncrypt
        , ZFMP_IN(const ZFOutput &, refOutput)
        , ZFMP_IN(const zfstring &, encryptKey)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIO_encrypt_h_

