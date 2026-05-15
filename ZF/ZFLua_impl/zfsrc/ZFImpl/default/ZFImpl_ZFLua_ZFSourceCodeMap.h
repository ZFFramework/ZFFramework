/**
 * @file ZFImpl_ZFLua_ZFSourceCodeMap.h
 * @brief lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_ZFSourceCodeMap_h_
#define _ZFI_ZFImpl_ZFLua_ZFSourceCodeMap_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief for impl to alloc object
 */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_ZFSourceCodeMapUpdate(
        ZF_IN lua_State *L
        , ZF_IN const ZFArgs &zfargs
        , ZF_IN const zfstring &name
        );

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_ZFSourceCodeMap_h_

