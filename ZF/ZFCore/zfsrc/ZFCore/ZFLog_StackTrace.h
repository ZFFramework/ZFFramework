/**
 * @file ZFLog_StackTrace.h
 * @brief utility to log stack
 */

#ifndef _ZFI_ZFLog_StackTrace_h_
#define _ZFI_ZFLog_StackTrace_h_

#include "ZFLog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief true if #ZFLogStackTrace and #ZFLogCallerInfo is available
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfbool, ZFLogStackTraceAvailable)

/**
 * @brief get the function call stack
 *
 * output may be different depending on the OS and implementation
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, void, ZFLogStackTrace
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(const zfchar *, prefix, zfnull)
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        , ZFMP_IN_OPT(zfindex, maxLevel, 20)
        )
/** @brief see #ZFLogStackTrace */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfstring, ZFLogStackTrace
        , ZFMP_IN_OPT(const zfchar *, prefix, zfnull)
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        , ZFMP_IN_OPT(zfindex, maxLevel, 20)
        )

/**
 * @brief get the caller's info
 *
 * see #ZFLogStackTrace for how to make it available
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFLogCallerInfo
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        )
/** @brief see ZFLogCallerInfo */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFLogCallerInfo
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_StackTrace_h_

