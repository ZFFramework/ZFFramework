/**
 * @file ZFCoreLog_CommonLog.h
 * @brief common log messages for ZFFramework
 *
 * used internally, you should use zfLog instead
 */

#ifndef _ZFI_ZFCoreLog_CommonLog_h_
#define _ZFI_ZFCoreLog_CommonLog_h_

#include "ZFCoreLog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFCore void _ZFP_zfCoreLogCriticalMessage(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        );
extern ZFLIB_ZFCore void _ZFP_zfCoreCritical(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        );

// ============================================================
/**
 * @brief print a critical error message which is easy to notice
 *
 * log only, see also #zfCoreCriticalMessage
 */
#define zfCoreLogCriticalMessage(fmt, ...) _ZFP_zfCoreLogCriticalMessage(ZFCallerInfoCreate(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #zfCoreLogCriticalMessage */
#define zfCoreLogCriticalMessageTrim(fmt, ...) _ZFP_zfCoreLogCriticalMessage(ZFCallerInfoEmpty(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #zfCoreLogCriticalMessage */
#define zfCoreLogCriticalMessageDetail(callerInfo, fmt, ...) _ZFP_zfCoreLogCriticalMessage(callerInfo, zfstr(fmt, ##__VA_ARGS__))

// ============================================================
/**
 * @brief assert exp is zftrue
 *
 * do nothing if exp is zftrue, otherwise,
 * print a critical error message,
 * then terminate app by #zfCoreCriticalError
 * @note unlike zfassert, this function is always active, even if ZF_ENV_DEBUG is zftrue
 * @see zfassert
 */
#define zfCoreAssert(exp) zfCoreAssertWithMessage(exp, "assert failed for \"%s\"", #exp)

/** @brief see #zfCoreAssert */
#define zfCoreAssertTrim(exp) zfCoreAssertWithMessageTrim(exp, "assert failed for \"%s\"", #exp)

/** @brief see #zfCoreAssert */
#define zfCoreAssertDetail(exp, callerInfo) zfCoreAssertWithMessageDetail(exp, callerInfo, "assert failed for \"%s\"", #exp)

// ============================================================
/**
 * @brief print a critical error message and abort
 */
#define zfCoreCriticalMessage(fmt, ...) _ZFP_zfCoreCritical(ZFCallerInfoCreate(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #zfCoreCriticalMessage */
#define zfCoreCriticalMessageTrim(fmt, ...) _ZFP_zfCoreCritical(ZFCallerInfoEmpty(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #zfCoreCriticalMessage */
#define zfCoreCriticalMessageDetail(callerInfo, fmt, ...) _ZFP_zfCoreCritical(callerInfo, zfstr(fmt, ##__VA_ARGS__))

// ============================================================
/**
 * @brief #zfCoreAssert with custom message
 */
#define zfCoreAssertWithMessage(exp, fmt, ...) \
    do { \
        if(!(exp)) { \
            zfCoreCriticalMessage(fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

/**
 * @brief see #zfCoreAssertWithMessage
 */
#define zfCoreAssertWithMessageTrim(exp, fmt, ...) \
    do { \
        if(!(exp)) { \
            zfCoreCriticalMessageTrim(fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

/**
 * @brief see #zfCoreAssertWithMessage
 */
#define zfCoreAssertWithMessageDetail(exp, callerInfo, fmt, ...) \
    do { \
        if(!(exp)) { \
            zfCoreCriticalMessageDetail(callerInfo, fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

// ============================================================
/**
 * @brief log that likes "[file function (line)] index i out of range [0, n)"
 */
#define zfCoreCriticalIndexOutOfRange(index, range) \
    zfCoreCriticalIndexOutOfRangeDetail(ZFCallerInfoCreate(), index, range)
/**
 * @brief see #zfCoreCriticalIndexOutOfRange
 */
#define zfCoreCriticalIndexOutOfRangeDetail(callerInfo, index, range) \
    zfCoreCriticalMessageDetail(callerInfo, \
        "index %s out of range [0, %s)", \
        (zfindex)(index), \
        (zfindex)(range))

// ============================================================
/**
 * @brief log that likes "[file function (line)] should not go here"
 */
#define zfCoreCriticalShouldNotGoHere() \
    zfCoreCriticalShouldNotGoHereDetail(ZFCallerInfoCreate())
/**
 * @brief see #zfCoreCriticalShouldNotGoHere
 */
#define zfCoreCriticalShouldNotGoHereDetail(callerInfo) \
    zfCoreCriticalMessageDetail(callerInfo, "should not go here")

// ============================================================
/**
 * @brief log that likes "[file function (line)] not supported"
 */
#define zfCoreCriticalNotSupported() \
    zfCoreCriticalNotSupportedDetail(ZFCallerInfoCreate())
/**
 * @brief see #zfCoreCriticalNotSupported
 */
#define zfCoreCriticalNotSupportedDetail(callerInfo) \
    zfCoreCriticalMessageDetail(callerInfo, "not supported")

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreLog_CommonLog_h_

