/**
 * @file ZFPathType_encrypt.h
 * @brief #ZFPathInfo impl as encrypted file
 */

#ifndef _ZFI_ZFPathType_encrypt_h_
#define _ZFI_ZFPathType_encrypt_h_

#include "ZFEncrypt.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is "encryptKey|refPathInfo"
 *
 * the refPathInfo is a chained #ZFPathInfo, a typical case:
 * @code
 *   encrypt:file:some/path/to/a.zip|encryptKey
 *   encrypt:res:some/path/to/a.zip|encryptKey
 *   encrypt:http:http://abc.com/some/path/to/a.zip|encryptKey
 * @endcode
 *
 * see #ZFPathInfoChainDecode for more info
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFAlgorithm, encrypt)

// ============================================================
// ZFInputForEncrypt
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the encrypt file path to use,
 *   see #ZFPathType_encrypt for more info
 * -  (const zfchar *)encryptKey: the encrypt key
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, encryptKey)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )

// ============================================================
// ZFOutputForEncrypt
/**
 * @brief util to create a encrypt file output callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the encrypt file path to use,
 *   see #ZFPathType_encrypt for more info
 * -  (const zfchar *)encryptKey: the encrypt key
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, encryptKey)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_encrypt_h_

