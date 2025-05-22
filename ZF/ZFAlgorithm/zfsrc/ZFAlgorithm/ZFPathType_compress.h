/**
 * @file ZFPathType_compress.h
 * @brief #ZFPathInfo impl as compressed file
 */

#ifndef _ZFI_ZFPathType_compress_h_
#define _ZFI_ZFPathType_compress_h_

#include "ZFCompress.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is "refPathInfo|relPath"
 *
 * the refPathInfo is a chained #ZFPathInfo, a typical case:
 * @code
 *   compress:file:some/path/to/a.zip|inner/path/to/content.txt
 *   compress:res:some/path/to/a.zip|inner/path/to/content.txt
 *   compress:http:http://abc.com/some/path/to/a.zip|inner/path/to/content.txt
 * @endcode
 *
 * see #ZFPathInfoChainDecode for more info
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFAlgorithm, compress)

// ============================================================
// ZFInputForCompress
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the compress file path to use,
 *   see #ZFPathType_compress for more info
 * -  (const zfchar *)relPath: the relative file path inside the compress file
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )

// ============================================================
// ZFOutputForCompress
/**
 * @brief util to create a compress file output callback
 *
 * param:
 * -  (const ZFPathInfo &)refPathInfo: the compress file path to use,
 *   see #ZFPathType_compress for more info
 * -  (const zfchar *)relPath: the relative file path inside the compress file
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_compress_h_

