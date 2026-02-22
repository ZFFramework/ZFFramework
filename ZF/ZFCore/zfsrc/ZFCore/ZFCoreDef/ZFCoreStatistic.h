/**
 * @file ZFCoreStatistic.h
 * @brief utilities to log function's invoke time
 */

#ifndef _ZFI_ZFCoreStatistic_h_
#define _ZFI_ZFCoreStatistic_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN_REGISTER(ZFCoreStatistic, ZF_NAMESPACE_GLOBAL)

/** @brief see #ZFCoreStatistic::invokeCountGet */
extern ZFLIB_ZFCore void invokeCountLog(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeCountGet */
extern ZFLIB_ZFCore void invokeCountRemove(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeCountGet */
extern ZFLIB_ZFCore void invokeCountRemoveAll(void);
/**
 * @brief get #ZFCoreStatistic::invokeCountLog's invoke count
 *
 * for debug performance use only, used to log a method's invoke count, usage:
 * @code
 *   void yourFuncToTest(void) {
 *       ZFCoreStatistic::invokeCountLog(yourKeyOrNull);
 *   }
 *   void YourTestFunc(void) {
 *       // this is yourFuncToTest's invoke times
 *       zfindex yourFuncCalledTime = ZFCoreStatistic::invokeCountGet(yourKeyOrNull);
 *       // if you want to reset it:
 *       ZFCoreStatistic::invokeCountReset(yourKeyOrNull);
 *   }
 * @endcode
 * @note this method is not thread-safe
 */
extern ZFLIB_ZFCore zfindex invokeCountGet(ZF_IN const zfstring &key);

/** @brief see #ZFCoreStatistic::invokeCountGet */
extern ZFLIB_ZFCore ZFCoreArray<zfstring> invokeCountGetAllKey(void);

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStatistic_h_

