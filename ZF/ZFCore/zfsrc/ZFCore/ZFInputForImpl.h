/**
 * @file ZFInputForImpl.h
 * @brief util to create #ZFInput for custom impl
 */

#ifndef _ZFI_ZFInputForImpl_h_
#define _ZFI_ZFInputForImpl_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFInput for custom impl, useful for script
 *
 * impl's params: (see #ZFInput)
 * -  param0 holds a #v_zfstring let impl to output
 * -  param1 holds a #v_zfindex that indicates the #ZFInput's input length,
 *   or #zfindexMax to calculate the total input length
 * -  result holds a #v_zfindex indicates bytes actually read
 *
 * this is useful for script to supply IO logic, lua for example:
 * @code
 *   ZFInputForImpl(function(zfargs)
 *       local buf = zfargs:param0()
 *       local size = zfargs:param1()
 *       local result = zfargs:result()
 *       if zfl_eq(size, zfindexMax()) then
 *           -- return total input size
 *           result:zfv(yourTotalLength)
 *           return
 *       end
 *
 *       -- write to the buf
 *       buf:append(yourData, size)
 *       -- return written length
 *       result:zfv(yourWrittenLength)
 *   end)
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFInput, ZFInputForImpl
        , ZFMP_IN(const ZFListener &, impl)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFInputForImpl_h_

