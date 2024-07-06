/**
 * @file ZFTypeIdDynamicRegister.h
 * @brief dynamic register #ZFTypeInfo
 */

#ifndef _ZFI_ZFTypeIdDynamicRegister_h_
#define _ZFI_ZFTypeIdDynamicRegister_h_

#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dynamic register #ZFTypeInfo
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 */
extern ZFLIB_ZFCore zfbool ZFTypeIdDynamicRegister(
        ZF_IN const zfstring &typeIdName
        , ZF_IN const ZFCorePointerForObject<ZFTypeInfo *> &typeIdData
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief see #ZFTypeIdDynamicRegister
 */
extern ZFLIB_ZFCore void ZFTypeIdDynamicUnregister(ZF_IN const zfstring &typeIdName);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdDynamicRegister_h_

