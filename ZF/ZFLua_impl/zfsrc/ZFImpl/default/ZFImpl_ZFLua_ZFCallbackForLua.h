/**
 * @file ZFImpl_ZFLua_ZFCallbackForLua.h
 * @brief lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_ZFCallbackForLua_h_
#define _ZFI_ZFImpl_ZFLua_ZFCallbackForLua_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief for impl to create ZFListener from lua function
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_ZFCallbackForLua(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_ZFCallbackForLua_h_

