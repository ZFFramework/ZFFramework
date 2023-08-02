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
 * @brief mutex used by #zfLog,
 *   you may use this lock to prevent your log actions from being interrupted
 *
 * note, the mutex would be initialized only after #ZFLevelZFFrameworkEssential
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, ZFMutex *, ZFLogMutex)

extern ZFLIB_ZFCore zfstring _ZFP_ZFLogHeaderString(ZF_IN const ZFCallerInfo &callerInfo);
/**
 * @brief a convenient debug header string
 *
 * typically looks like this (not ensured):\n
 *   "timestamp [file scope::function (line)] "
 */
#define ZFLOG_HEADER_STRING (_ZFP_ZFLogHeaderString(ZFCallerInfoMake()).cString())

// ============================================================
/**
 * @brief thread-safe log utility
 *
 * typical usage:
 * @code
 *   // output anything with ZFLOG_HEADER_STRING
 *   zfLog() << anything;
 *   zfLog("formated: %s", someText);
 *
 *   // or, trim version without ZFLOG_HEADER_STRING
 *   zfLogTrim() << anything;
 *   zfLogTrim("formated: %s", someText);
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
 *   ZFOutputFormat::getFormat<ZFLogFormat *>()->autoEndl(xxx);
 * @endcode
 * and they would be reset to default state after each zfLog call
 */
#define zfLog(...) _ZFP_zfLog(ZFLOG_HEADER_STRING, zfstr(__VA_ARGS__))

/** @brief see #zfLog */
#define zfLogTrim(...) _ZFP_zfLog(zfnull, zfstr(__VA_ARGS__))

extern ZFLIB_ZFCore ZFOutput _ZFP_zfLog(
        ZF_IN const zfchar *header
        , ZF_IN_OPT const zfchar *text = zfnull
        );

// ============================================================
/**
 * @brief output format for #zfLog
 */
zfclass ZFLIB_ZFCore ZFLogFormat : zfextends ZFObject, zfimplements ZFOutputFormat {
    ZFOBJECT_DECLARE(ZFLogFormat, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFOutputFormat)

public:
    ZFPROPERTY_ASSIGN(zfbool, autoEndl, zftrue)

protected:
    zfoverride
    virtual void format(
            ZF_IN_OUT zfstring &ret
            , ZF_IN ZFOutputFormatStepEnum outputStep
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
extern ZFLIB_ZFCore zfstring _ZFP_zfLogCurTimeString(void);
/** @cond ZFPrivateDoc */
#define zfLogCurTimeString() (_ZFP_zfLogCurTimeString().cString())
/** @endcond */
/**
 * @brief get a string contains current time with format HH:mm:ss.SSS
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, zfLogCurTimeString)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_h_

