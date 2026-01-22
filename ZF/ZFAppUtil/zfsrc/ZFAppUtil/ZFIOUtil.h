/**
 * @file ZFIOUtil.h
 * @brief debug server
 */

#ifndef _ZFI_ZFIOUtil_h_
#define _ZFI_ZFIOUtil_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief remove outdated files
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIORemoveOutdate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zftimet, outdate, zftimetOneWeek())
        )

/**
 * @brief remove empty dir, excluding the pathInfo itself
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIORemoveEmptyDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOUtil_h_

