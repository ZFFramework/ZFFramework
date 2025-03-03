/**
 * @file ZFImplOutput.h
 * @brief interact with native log output
 */

#ifndef _ZFI_ZFImplOutput_h_
#define _ZFI_ZFImplOutput_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// core log
/**
 * @brief used to output log to native env
 */
extern ZFLIB_ZFCore void ZFImplOutputCoreLog(ZF_IN const zfchar *src);

// ============================================================
// normal log
/**
 * @brief used to output log to native env
 */
extern ZFLIB_ZFCore void ZFImplOutput(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImplOutput_h_

