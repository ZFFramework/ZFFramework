/**
 * @file ZFInputForLua.h
 * @brief util to create #ZFInput for lua
 */

#ifndef _ZFI_ZFInputForLua_h_
#define _ZFI_ZFInputForLua_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFInput for lua
 *
 * luaCallback's param0 would be a #v_zfstring let lua code to output,
 * and param1 holds a #v_zfindex that indicates the #ZFInput's input length
 * or #zfindexMax to calculate the total input length (see #ZFInput),
 * upon input failure,
 * the lua code should change the param1's value
 * @code
 *   ZFInputForLua(function(zfargs)
 *       if zfargs:param1() == zfindexMax() then
 *           // return total input size
 *           zfargs:param1():zfv(yourTotalLength)
 *           return
 *       end
 *
 *       local buf = zfargs:param0()
 *       local count = zfargs:param1()
 *       // write to the buf
 *       buf:zfv(yourData)
 *       // save written length
 *       zfargs:param1():zfv(yourWrittenLength)
 *   end)
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, ZFInput, ZFInputForLua,
                        ZFMP_IN(const ZFListener &, luaCallback))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFInputForLua_h_

