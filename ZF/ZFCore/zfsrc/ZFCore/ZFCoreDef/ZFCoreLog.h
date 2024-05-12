/**
 * @file ZFCoreLog.h
 * @brief internal log utility
 *
 * used internally, you should use zfLog instead
 */

#ifndef _ZFI_ZFCoreLog_h_
#define _ZFI_ZFCoreLog_h_

#include "ZFCallerInfo.h"
#include "zfstr.h"

/**
 * @brief same as assert defined for future use
 *
 * active only if ZF_ENV_DEBUG is zftrue
 * @see zfCoreAssert
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
 * @brief output callback for zfCoreLog
 */
typedef void (*ZFCoreLogOutputCallbackType)(ZF_IN const zfchar *s);
extern ZFLIB_ZFCore void _ZFP_ZFCoreLogOutputCallbackDefault(ZF_IN const zfchar *s);
/**
 * @brief default output callback for zfCoreLog
 */
#define ZFCoreLogOutputCallbackDefault _ZFP_ZFCoreLogOutputCallbackDefault

/**
 * @brief set the output callback for zfCoreLog
 * @warning within the callback, you must not access any other ZFFramework's module,
 *   otherwise dead lock may be occurred
 */
extern ZFLIB_ZFCore void zfCoreLogOutputCallback(ZF_IN ZFCoreLogOutputCallbackType callback);
/**
 * @brief get the output callback for zfCoreLog
 */
extern ZFLIB_ZFCore ZFCoreLogOutputCallbackType zfCoreLogOutputCallback(void);

// ============================================================
extern ZFLIB_ZFCore void _ZFP_zfCoreLog(
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
 * @see zfCoreLogOutputCallback, zfCoreCriticalError
 */
#define zfCoreLog(format, ...) _ZFP_zfCoreLog(ZFCallerInfoMake(), zfstr(format, ##__VA_ARGS__))

/**
 * @brief log without header info, see #zfCoreLog
 */
#define zfCoreLogTrim(format, ...) _ZFP_zfCoreLog(ZFCallerInfoEmpty(), zfstr(format, ##__VA_ARGS__))

/**
 * @brief log with custom header info, see #zfCoreLog
 */
#define zfCoreLogDetail(callerInfo, format, ...) _ZFP_zfCoreLog(callerInfo, zfstr(format, ##__VA_ARGS__))

// ============================================================
/**
 * @brief callback which would be called if zfCoreCriticalError is called
 */
typedef void (*ZFCoreCriticalErrorCallback)(ZF_IN const ZFCallerInfo &callerInfo);

/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZFLIB_ZFCore void zfCoreCriticalErrorPrepareCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback);
/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZFLIB_ZFCore void zfCoreCriticalErrorPrepareCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback);

/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZFLIB_ZFCore void zfCoreCriticalErrorCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback);
/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZFLIB_ZFCore void zfCoreCriticalErrorCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback);

extern ZFLIB_ZFCore void _ZFP_zfCoreCriticalErrorPrepare(ZF_IN const ZFCallerInfo &callerInfo);
extern ZFLIB_ZFCore void _ZFP_zfCoreCriticalError(ZF_IN const ZFCallerInfo &callerInfo);
/**
 * @brief ready to output critical error, see #zfCoreCriticalError
 *
 * usage:
 * @code
 *   zfCoreCriticalErrorPrepare();
 *   zfCoreLog(xxx);
 *   zfCoreCriticalError();
 * @endcode
 * this is useful to redirect #zfCoreLog by #zfCoreLogOutputCallback,
 * to save critical error messages to proper location
 */
#define zfCoreCriticalErrorPrepare() _ZFP_zfCoreCriticalErrorPrepare(ZFCallerInfoMake())
/** @brief see #zfCoreCriticalErrorPrepare */
#define zfCoreCriticalErrorPrepareDetail(callerInfo) _ZFP_zfCoreCriticalErrorPrepare(callerInfo)

/**
 * @brief used when error occurred, to terminate the application
 *
 * @warning this function is for internal use,
 *   calling this method would cause app to be terminated
 */
#define zfCoreCriticalError() _ZFP_zfCoreCriticalError(ZFCallerInfoMake())
/** @brief see #zfCoreCriticalError */
#define zfCoreCriticalErrorDetail(callerInfo) _ZFP_zfCoreCriticalError(callerInfo)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreLog_h_

