/**
 * @file ZFUIImageCache.h
 * @brief load image with cache logic
 */

#ifndef _ZFI_ZFUIImageCache_h_
#define _ZFI_ZFUIImageCache_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load image with cache logic
 *
 * callback's param0 would be the result image if success,
 * param1 would be a #ZFResultType indicates load result\n
 * use #ZFIOCache to config cache logic
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIWidget, zfautoT<ZFTaskId>, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageCache_h_

