/**
 * @file ZFImpl_ZFLua_zfl_alloc.h
 * @brief lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_zfl_alloc_h_
#define _ZFI_ZFImpl_ZFLua_zfl_alloc_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief for impl to alloc object
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_zfl_alloc(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN const ZFClass *cls
        , ZF_IN int paramCount
        , ZF_IN int luaParamOffset
        , ZF_IN_OPT zfstring *errorHint = zfnull
        );

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_zfl_alloc_h_

