/**
 * @file ZFIO_compress.h
 * @brief compress #ZFPathInfo
 */

#ifndef _ZFI_ZFIO_compress_h_
#define _ZFI_ZFIO_compress_h_

#include "ZFCompress.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is "refPathInfo|itemPath",
 * see #ZFPathInfoChainDecode for more info
 *
 * typical case:
 * @code
 *   compress:file:some/path/to/file.zip|item/path
 *   compress:res:some/path/to/file.zip|item/path
 *   compress:http:http://abc.com/some/path/to/file.zip|item/path
 * @endcode
 */
ZFIO_DECLARE(ZFLIB_ZFAlgorithm, compress)

// ============================================================
// ZFInputForCompress
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the compress file path to use,
 *   see #ZFPathType_compress for more info
 * -  (const zfstring &)itemPath : the inner item path
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        )

// ============================================================
// ZFOutputForCompress
/**
 * @brief util to create a compress file output callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the compress file path to use,
 *   see #ZFPathType_compress for more info
 * -  (const zfstring &)itemPath : the inner item path
 * -  (ZFIOOpenOptionFlags)flags : how to open the compress file
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Modify)
        )

// ============================================================
// ZFInputForCompress
/**
 * @brief util to create a file input callback
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForCompress
        , ZFMP_IN(const ZFInput &, refInput)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        )

// ============================================================
// ZFOutputForCompress
/**
 * @brief util to create a compress file output callback
 *
 * note, the target compress file specified by refOutput would be overwrite instead of append,
 * use refPathInfo verion of ZFOutputForCompress is recommended
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForCompress
        , ZFMP_IN(const ZFOutput &, refOutput)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIO_compress_h_

