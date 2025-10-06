/**
 * @file ZFCoreLog.h
 * @brief internal log utility
 *
 * used internally, you should use ZFLog instead
 */

#ifndef _ZFI_ZFCoreLog_h_
#define _ZFI_ZFCoreLog_h_

#include "ZFCallerInfo.h"
#include "zfstr.h"

/**
 * @brief same as assert defined for future use
 *
 * active only if ZF_ENV_DEBUG is zftrue
 * @see ZFCoreAssert
 */
#if ZF_ENV_DEBUG
    #include <cassert>
    #define zfassert(b) assert(b)
#else
    #define zfassert(b) ((void)0)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief assert at compile time
 */
#define ZFASSERT_STATIC(cond, name) typedef char assert_failed_##name[(cond) ? 1 : -1];

// ============================================================
/**
 * @brief output callback for ZFCoreLog
 */
typedef void (*ZFCoreLogOutputCallbackType)(ZF_IN const zfchar *s);
extern ZFLIB_ZFCore void _ZFP_ZFCoreLogOutputCallbackDefault(ZF_IN const zfchar *s);
/**
 * @brief default output callback for ZFCoreLog
 */
#define ZFCoreLogOutputCallbackDefault _ZFP_ZFCoreLogOutputCallbackDefault

/**
 * @brief set the output callback for ZFCoreLog
 * @warning within the callback, you must not access any other ZFFramework's module,
 *   otherwise dead lock may be occurred
 */
extern ZFLIB_ZFCore void ZFCoreLogOutputCallback(ZF_IN ZFCoreLogOutputCallbackType callback);
/**
 * @brief get the output callback for ZFCoreLog
 */
extern ZFLIB_ZFCore ZFCoreLogOutputCallbackType ZFCoreLogOutputCallback(void);

// ============================================================
extern ZFLIB_ZFCore void _ZFP_ZFCoreLog(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        );
/**
 * @brief log utility used internally
 *
 * quite similar to printf, except that this function would automatically add header info to head,
 * which includes file name, function name and file line info,
 * and an extra '\\n' to tail\n
 * for all the format supported, please refer to #zfstringAppend
 * @warning this function is for internal use,
 *   usually to output critical error messages,
 *   you should not use other log method in your app
 * @warning this method is not thread safe
 * @see ZFCoreLogOutputCallback, ZFCoreCriticalError
 */
#define ZFCoreLog(format, ...) _ZFP_ZFCoreLog(ZFCallerInfoCreate(), zfstr(format, ##__VA_ARGS__))

/**
 * @brief log without header info, see #ZFCoreLog
 */
#define ZFCoreLogTrim(format, ...) _ZFP_ZFCoreLog(ZFCallerInfoEmpty(), zfstr(format, ##__VA_ARGS__))

/**
 * @brief log with custom header info, see #ZFCoreLog
 */
#define ZFCoreLogDetail(callerInfo, format, ...) _ZFP_ZFCoreLog(callerInfo, zfstr(format, ##__VA_ARGS__))

// ============================================================
/**
 * @brief callback which would be called if ZFCoreCriticalError is called
 */
typedef void (*ZFCoreCriticalErrorCallback)(ZF_IN const ZFCallerInfo &callerInfo, ZF_IN_OUT zfstring &errorHint);

/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZFLIB_ZFCore void ZFCoreCriticalErrorCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback);
/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZFLIB_ZFCore void ZFCoreCriticalErrorCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback);

extern ZFLIB_ZFCore void _ZFP_ZFCoreCriticalError(ZF_IN const ZFCallerInfo &callerInfo, ZF_IN const zfchar *errorHint);

/**
 * @brief used when error occurred, to terminate the application
 *
 * @warning this function is for internal use,
 *   calling this method would cause app to be terminated
 */
#define ZFCoreCriticalError(errorHint) _ZFP_ZFCoreCriticalError(ZFCallerInfoCreate(), errorHint)
/** @brief see #ZFCoreCriticalError */
#define ZFCoreCriticalErrorDetail(callerInfo, errorHint) _ZFP_ZFCoreCriticalError(callerInfo, errorHint)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreLog_h_

