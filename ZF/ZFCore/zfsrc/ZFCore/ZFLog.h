/**
 * @file ZFLog.h
 * @brief thread-safe log utility
 */

#ifndef _ZFI_ZFLog_h_
#define _ZFI_ZFLog_h_

#include "ZFLogLevel.h"
#include "ZFMutex.h"
#include "ZFOutputForFormat.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief mutex used by #ZFLog,
 *   you may use this lock to prevent your log actions from being interrupted
 *
 * note, the mutex would be initialized only after #ZFLevelZFFrameworkEssential
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, ZFMutex *, ZFLogMutex)

/** @brief see #ZFLogHeader */
typedef zfstring (*ZFLogHeaderFunc)(ZF_IN const ZFCallerInfo &callerInfo);
/**
 * @brief callback to obtain log header for #ZFLog
 *
 * set to #ZFLogHeaderDefault at #ZFLevelZFFrameworkStatic,
 * set to null to disable header info\n
 * by default, the header would looks like this (not ensured):\n
 *   "12:34:56.123 [file scope::function (line)] "\n
 * the default header can be configured by:
 * -  #ZFLogHeaderDefault_logTime
 * -  #ZFLogHeaderDefault_logCaller
 */
extern ZFLIB_ZFCore ZFLogHeaderFunc ZFLogHeader;

/** @brief see #ZFLogHeader */
extern ZFLIB_ZFCore zfstring ZFLogHeaderDefault(ZF_IN const ZFCallerInfo &callerInfo);
/** @brief see #ZFLogHeader, enabled by default */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFCore, zfbool, ZFLogHeaderDefault_logTime)
/** @brief see #ZFLogHeader, enabled by default, set to false if #v_ZFLogLevel::e_Verbose is not active */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFCore, zfbool, ZFLogHeaderDefault_logCaller)

// ============================================================
/**
 * @brief thread-safe log utility
 *
 * typical usage:
 * @code
 *   // output anything with ZFLogHeader
 *   ZFLog() << anything;
 *   ZFLog("formated: %s", someText);
 *
 *   // or, trim version without ZFLogHeader
 *   ZFLogTrim() << anything;
 *   ZFLogTrim("formated: %s", someText);
 * @endcode
 *
 * ZFLog is actually an output callback,
 * you can output anything that #ZFOutput supported\n
 * final log content would be written to #ZFOutputDefault,
 * which can be easily redirected to other output destinations\n
 * ZFLog is thread-safe, implemented by #ZFLogMutex\n
 * \n
 * by default, ZFLog would automatically:
 * -  append space between each output
 * -  append endl after last output
 *
 * these behavior can be changed by:
 * @code
 *   zfanyT<ZFLogFormat> fmt = ZFOutputFormat::getFormat(output);
 *   fmt->autoEndl(xxx);
 * @endcode
 * and they would be reset to default state after each ZFLog call
 */
#define ZFLog(...) _ZFP_ZFLog((ZFLogHeader ? ZFLogHeader(ZFCallerInfoCreate()).cString() : zfnull), zfstr(__VA_ARGS__))

/** @brief see #ZFLog */
#define ZFLogTrim(...) _ZFP_ZFLog(zfnull, zfstr(__VA_ARGS__))

extern ZFLIB_ZFCore ZFOutput _ZFP_ZFLog(
        ZF_IN const zfchar *header
        , ZF_IN_OPT const zfchar *text = zfnull
        );

// ============================================================
/**
 * @brief output format for #ZFLog
 */
zfclass ZFLIB_ZFCore ZFLogFormat : zfextend ZFOutputFormat {
    ZFOBJECT_DECLARE(ZFLogFormat, ZFOutputFormat)

public:
    /** @brief whether auto append endl for each #ZFLog call */
    ZFPROPERTY_ASSIGN(zfbool, autoEndl, zftrue)

protected:
    zfoverride
    virtual void format(
            ZF_IN_OUT zfstring &ret
            , ZF_IN ZFOutputFormatStep outputStep
            , ZF_IN const zfchar *src
            , ZF_IN zfindex srcLen
            );
};

/**
 * @brief change default log format
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFLogFormatDefault
        , ZFMP_IN(ZFLogFormat *, fmt)
        )
/**
 * @brief get default log format
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, ZFLogFormat *, ZFLogFormatDefault)

// ============================================================
// other convenient method
extern ZFLIB_ZFCore zfstring _ZFP_ZFLogCurTimeString(void);
/** @cond ZFPrivateDoc */
#define ZFLogCurTimeString() _ZFP_ZFLogCurTimeString()
/** @endcond */
/**
 * @brief get a string contains current time with format HH:mm:ss.SSS
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, ZFLogCurTimeString)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_h_

