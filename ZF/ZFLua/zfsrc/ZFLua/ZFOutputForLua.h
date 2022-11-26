/**
 * @file ZFOutputForLua.h
 * @brief util to create #ZFOutput for lua
 */

#ifndef _ZFI_ZFOutputForLua_h_
#define _ZFI_ZFOutputForLua_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFOutput for lua
 *
 * luaCallback's param0 would be a #v_zfstring,
 * and param1 holds a #v_zfindex that indicates the #ZFOutput's return value
 * @code
 *   ZFOutputForLua(function(zfargs)
 *       local text = zfargs:param0()
 *       local result = yourOutput(text)
 *       zfargs:param1():zfv(result)
 *   end)
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, ZFOutput, ZFOutputForLua,
                        ZFMP_IN(const ZFListener &, luaCallback))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForLua_h_

