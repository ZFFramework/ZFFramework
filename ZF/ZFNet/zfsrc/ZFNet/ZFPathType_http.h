/**
 * @file ZFPathType_http.h
 * @brief #ZFPathInfo impl as http file
 */

#ifndef _ZFI_ZFPathType_http_h_
#define _ZFI_ZFPathType_http_h_

#include "ZFHttpRequest.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the http url
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFNet, http)

// ============================================================
// ZFInputForHttp
/**
 * @brief util to create a http file input callback
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFNet, ZFInput, ZFInputForHttp
        , ZFMP_IN(const zfstring &, url)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_http(), url), ZFFileOpenOption::e_Read);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_http_h_

