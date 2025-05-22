/**
 * @file ZFPathType_res_raw.h
 * @brief #ZFPathInfo impl as res file
 */

#ifndef _ZFI_ZFPathType_res_raw_h_
#define _ZFI_ZFPathType_res_raw_h_

#include "ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the raw res path (see #ZFResRaw)
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, resraw)

// ============================================================
// ZFInputForResRaw
/**
 * @brief util to create a resource file input callback,
 *   see #ZFResOpen for what resource file is
 *
 * param:
 * -  (const zfchar *)resFilePath: resource file path to use
 *
 * auto open and auto close files, may return a null callback if open file error
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFCore, ZFInput, ZFInputForResRaw
        , ZFMP_IN(const zfstring &, resFilePath)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_resraw(), resFilePath), v_ZFFileOpenOption::e_Read);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_res_raw_h_

