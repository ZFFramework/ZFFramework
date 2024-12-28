/**
 * @file ZFCoreLog_CommonLog.h
 * @brief common log messages for ZFFramework
 *
 * used internally, you should use ZFLog instead
 */

#ifndef _ZFI_ZFCoreLog_CommonLog_h_
#define _ZFI_ZFCoreLog_CommonLog_h_

#include "ZFCoreLog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFCore void _ZFP_ZFCoreLogCriticalMessage(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        );
extern ZFLIB_ZFCore void _ZFP_ZFCoreCritical(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        );

// ============================================================
/**
 * @brief print a critical error message which is easy to notice
 *
 * log only, see also #ZFCoreCriticalMessage
 */
#define ZFCoreLogCriticalMessage(fmt, ...) _ZFP_ZFCoreLogCriticalMessage(ZFCallerInfoCreate(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #ZFCoreLogCriticalMessage */
#define ZFCoreLogCriticalMessageTrim(fmt, ...) _ZFP_ZFCoreLogCriticalMessage(ZFCallerInfoEmpty(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #ZFCoreLogCriticalMessage */
#define ZFCoreLogCriticalMessageDetail(callerInfo, fmt, ...) _ZFP_ZFCoreLogCriticalMessage(callerInfo, zfstr(fmt, ##__VA_ARGS__))

// ============================================================
/**
 * @brief assert exp is zftrue
 *
 * do nothing if exp is zftrue, otherwise,
 * print a critical error message,
 * then terminate app by #ZFCoreCriticalError
 * @note unlike zfassert, this function is always active, even if ZF_ENV_DEBUG is zftrue
 * @see zfassert
 */
#define ZFCoreAssert(exp) ZFCoreAssertWithMessage(exp, "assert failed for \"%s\"", #exp)

/** @brief see #ZFCoreAssert */
#define ZFCoreAssertTrim(exp) ZFCoreAssertWithMessageTrim(exp, "assert failed for \"%s\"", #exp)

/** @brief see #ZFCoreAssert */
#define ZFCoreAssertDetail(exp, callerInfo) ZFCoreAssertWithMessageDetail(exp, callerInfo, "assert failed for \"%s\"", #exp)

// ============================================================
/**
 * @brief print a critical error message and abort
 */
#define ZFCoreCriticalMessage(fmt, ...) _ZFP_ZFCoreCritical(ZFCallerInfoCreate(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #ZFCoreCriticalMessage */
#define ZFCoreCriticalMessageTrim(fmt, ...) _ZFP_ZFCoreCritical(ZFCallerInfoEmpty(), zfstr(fmt, ##__VA_ARGS__))

/** @brief see #ZFCoreCriticalMessage */
#define ZFCoreCriticalMessageDetail(callerInfo, fmt, ...) _ZFP_ZFCoreCritical(callerInfo, zfstr(fmt, ##__VA_ARGS__))

// ============================================================
/**
 * @brief #ZFCoreAssert with custom message
 */
#define ZFCoreAssertWithMessage(exp, fmt, ...) \
    do { \
        if(!(exp)) { \
            ZFCoreCriticalMessage(fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

/**
 * @brief see #ZFCoreAssertWithMessage
 */
#define ZFCoreAssertWithMessageTrim(exp, fmt, ...) \
    do { \
        if(!(exp)) { \
            ZFCoreCriticalMessageTrim(fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

/**
 * @brief see #ZFCoreAssertWithMessage
 */
#define ZFCoreAssertWithMessageDetail(exp, callerInfo, fmt, ...) \
    do { \
        if(!(exp)) { \
            ZFCoreCriticalMessageDetail(callerInfo, fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

// ============================================================
/**
 * @brief log that likes "[file function (line)] index i out of range [0, n)"
 */
#define ZFCoreCriticalIndexOutOfRange(index, range) \
    ZFCoreCriticalIndexOutOfRangeDetail(ZFCallerInfoCreate(), index, range)
/**
 * @brief see #ZFCoreCriticalIndexOutOfRange
 */
#define ZFCoreCriticalIndexOutOfRangeDetail(callerInfo, index, range) \
    ZFCoreCriticalMessageDetail(callerInfo, \
        "index %s out of range [0, %s)", \
        (zfindex)(index), \
        (zfindex)(range))

/**
 * @brief log that likes "[file function (line)] index i out of range [0, n)"
 */
#define ZFCoreAssertIndexRange(index, range) \
    ZFCoreAssertIndexRangeDetail(ZFCallerInfoCreate(), index, range)
/**
 * @brief see #ZFCoreAssertIndexRange
 */
inline void ZFCoreAssertIndexRangeDetail(ZF_IN const ZFCallerInfo &callerInfo, ZF_IN zfindex index, ZF_IN zfindex range) {
    if(index >= range) {
        ZFCoreCriticalIndexOutOfRangeDetail(callerInfo, index, range);
    }
}

// ============================================================
/**
 * @brief log that likes "[file function (line)] should not go here"
 */
#define ZFCoreCriticalShouldNotGoHere() \
    ZFCoreCriticalShouldNotGoHereDetail(ZFCallerInfoCreate())
/**
 * @brief see #ZFCoreCriticalShouldNotGoHere
 */
#define ZFCoreCriticalShouldNotGoHereDetail(callerInfo) \
    ZFCoreCriticalMessageDetail(callerInfo, "should not go here")

// ============================================================
/**
 * @brief log that likes "[file function (line)] not supported"
 */
#define ZFCoreCriticalNotSupported() \
    ZFCoreCriticalNotSupportedDetail(ZFCallerInfoCreate())
/**
 * @brief see #ZFCoreCriticalNotSupported
 */
#define ZFCoreCriticalNotSupportedDetail(callerInfo) \
    ZFCoreCriticalMessageDetail(callerInfo, "not supported")

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreLog_CommonLog_h_

