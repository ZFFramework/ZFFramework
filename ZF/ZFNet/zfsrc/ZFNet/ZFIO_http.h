/**
 * @file ZFIO_http.h
 * @brief #ZFPathInfo impl as http file
 */

#ifndef _ZFI_ZFIO_http_h_
#define _ZFI_ZFIO_http_h_

#include "ZFHttpRequest.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the http url
 */
ZFIO_DECLARE(ZFLIB_ZFNet, http)

// ============================================================
// ZFInputForHttp
/**
 * @brief util to create a http file input callback
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFNet, ZFInput, ZFInputForHttp
        , ZFMP_IN(const zfstring &, url)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_http(), url));
    return ret;
}

/**
 * @brief util method to check whether specified http response should be treated as dir
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFNet, zfbool, ZFHttpIsDir
        , ZFMP_IN(ZFHttpResponse *, recv)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIO_http_h_

