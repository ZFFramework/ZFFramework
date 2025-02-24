/**
 * @file ZFLogLevel.h
 * @brief log level define
 */

#ifndef _ZFI_ZFLogLevel_h_
#define _ZFI_ZFLogLevel_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief log level
 *
 * similar to Android's Log.v() to Log.e()\n
 * app's global log level can be set or get by #ZFLogLevelDefault,
 * you may achieve your own log level specified behavior\n
 * it's ensured lower level has smaller value
 */
ZFENUM_BEGIN(ZFLIB_ZFCore, ZFLogLevel)
    ZFENUM_VALUE(Verbose)
    ZFENUM_VALUE(Debug)
    ZFENUM_VALUE(Info)
    ZFENUM_VALUE(Warning)
    ZFENUM_VALUE(Error)
    ZFENUM_VALUE(Assert)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Verbose)
    ZFENUM_VALUE_REGISTER(Debug)
    ZFENUM_VALUE_REGISTER(Info)
    ZFENUM_VALUE_REGISTER(Warning)
    ZFENUM_VALUE_REGISTER(Error)
    ZFENUM_VALUE_REGISTER(Assert)
ZFENUM_END_WITH_DEFAULT(ZFLIB_ZFCore, ZFLogLevel, Warning)

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFLogLevelDefault changed
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, LogLevelOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

/** @brief see #ZFLogLevel, #v_ZFLogLevel::e_Warning by default */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFLogLevelDefault
        , ZFMP_IN(ZFLogLevel, level)
        )
/** @brief see #ZFLogLevel */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, ZFLogLevel, ZFLogLevelDefault)

/**
 * @brief true if specified level is active
 *   (i.e. #ZFLogLevelDefault not greater than the specified level)
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFLogLevelIsActive
        , ZFMP_IN(ZFLogLevel, level)
        ) {
    return (ZFLogLevelDefault() <= level);
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFLogLevel_h_

