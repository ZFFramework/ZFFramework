/**
 * @file ZFLuaDef.h
 * @brief global header for ZFLua module
 */

#ifndef _ZFI_ZFLuaDef_h_
#define _ZFI_ZFLuaDef_h_

#include "ZFCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief used to export symbols */
#ifndef ZFLIB_ZFLua
    #define ZFLIB_ZFLua ZF_ENV_IMPORT
#endif

// ============================================================
/**
 * @brief callbacks that would be called when lua related error occurred,
 *   for debug use only
 *
 * by default, #ZFOutputDefault would be added during #ZFLevelZFFrameworkNormal
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, ZFCoreArray<ZFOutput> &, ZFLuaErrorCallbacks)

extern ZFLIB_ZFLua void _ZFP_ZFLuaErrorOccurred(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        );
/**
 * @brief util method to notify #ZFLuaErrorCallbacks
 */
#define ZFLuaErrorOccurredTrim(fmt, ...) \
    ZFLuaErrorOccurredDetail(ZFCallerInfoEmpty(), fmt, ##__VA_ARGS__)
/**
 * @brief util method to notify #ZFLuaErrorCallbacks
 */
#define ZFLuaErrorOccurred(fmt, ...) \
    ZFLuaErrorOccurredDetail(ZFCallerInfoCreate(), fmt, ##__VA_ARGS__)
/**
 * @brief util method to notify #ZFLuaErrorCallbacks
 */
#define ZFLuaErrorOccurredDetail(callerInfo, fmt, ...) \
    _ZFP_ZFLuaErrorOccurred(callerInfo, zfstr(fmt, ##__VA_ARGS__))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaDef_h_

