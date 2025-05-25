/**
 * @file ZFLuaGC.h
 * @brief perform garbage collect in lua
 */

#ifndef _ZFI_ZFLuaGC_h_
#define _ZFI_ZFLuaGC_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief performa garbage collector in lua immediately,
 *   use #ZFLuaGC for performance
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaGCImmediately
        , ZFMP_IN_OPT(void *, L, zfnull)
        )

/**
 * @brief schedule #ZFLuaGC to run at proper time
 *
 * @note #ZFLuaGC would be called after each #ZFLuaExecute
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaGC
        , ZFMP_IN_OPT(void *, L, zfnull)
        )

/**
 * @brief run gc for all thread, for debug use only
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, void, ZFLuaGCForAllThread)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaGC_h_

