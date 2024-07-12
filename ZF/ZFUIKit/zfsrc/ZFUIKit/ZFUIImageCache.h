/**
 * @file ZFUIImageCache.h
 * @brief load image with cache logic
 */

#ifndef _ZFI_ZFUIImageCache_h_
#define _ZFI_ZFUIImageCache_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load image with cache logic
 *
 * callback's param0 would be the result image if success,
 * param1 would be a #ZFResultType indicates load result\n
 * use #ZFIOCache to config cache logic
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfauto, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        )
/**
 * @brief cancel load task of #ZFUIImageLoad
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void, ZFUIImageLoadCancel
        , ZFMP_IN(const zfauto &, taskId)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageCache_h_

