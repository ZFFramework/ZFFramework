/**
 * @file ZFPathType_ZFCompress.h
 * @brief #ZFPathInfo impl as compressed file
 */

#ifndef _ZFI_ZFPathType_ZFCompress_h_
#define _ZFI_ZFPathType_ZFCompress_h_

#include "ZFCompress.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is "compressFilePathInfo|relPath"
 *
 * the compressFilePathInfo is a chained #ZFPathInfo, a typical case:
 * @code
 *   ZFCompress:file:some/path/to/a.zip|inner/path/to/content.txt
 *   ZFCompress:res:some/path/to/a.zip|inner/path/to/content.txt
 *   ZFCompress:http:http://abc.com/some/path/to/a.zip|inner/path/to/content.txt
 * @endcode
 *
 * see #ZFPathInfoChainDecode for more info
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFAlgorithm, ZFCompress)

// ============================================================
// ZFInputForCompressFile
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const zfchar *)compressFilePathInfo: the compress file path to use,
 *   can be file path or #ZFPathInfo string,
 *   see #ZFPathType_ZFCompress for more info
 * -  (const zfchar *)relPath: the relative file path inside the compress file
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForCompressFile
        , ZFMP_IN(const zfchar *, compressFilePathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )
/** @brief see #ZFInputForCompressFile */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFInput, ZFInputForCompressFile
        , ZFMP_IN(const ZFPathInfo &, compressFilePathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )

// ============================================================
// ZFOutputForCompressFile
/**
 * @brief util to create a compress file output callback
 *
 * param:
 * -  (const zfchar *)compressFilePathInfo: the compress file path to use,
 *   can be file path or #ZFPathInfo string,
 *   see #ZFPathType_ZFCompress for more info
 * -  (const zfchar *)relPath: the relative file path inside the compress file
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForCompressFile
        , ZFMP_IN(const zfchar *, compressFilePathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )
/** @brief see #ZFOutputForCompressFile */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFOutput, ZFOutputForCompressFile
        , ZFMP_IN(const ZFPathInfo &, compressFilePathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_file_h_

