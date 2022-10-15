/**
 * @file ZFOutputDefault.h
 * @brief default callback for output
 */

#ifndef _ZFI_ZFOutputDefault_h_
#define _ZFI_ZFOutputDefault_h_

#include "ZFObjectObserver.h"
#include "ZFIOCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief default output callback, output source must be zfchar *string
 */
extern ZFLIB_ZFCore const ZFOutput &ZFOutputDefault(void);
/** @brief see #ZFOutputDefault  */
extern ZFLIB_ZFCore void ZFOutputDefault(ZF_IN const ZFOutput &v);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputDefault_h_

