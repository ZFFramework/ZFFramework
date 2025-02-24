/**
 * @file ZFOutputForImpl.h
 * @brief util to create #ZFOutput for custom impl
 */

#ifndef _ZFI_ZFOutputForImpl_h_
#define _ZFI_ZFOutputForImpl_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFOutput for custom impl, useful for script
 *
 * impl's params: (see #ZFOutput)
 * -  param0 holds a #v_zfstring indicates a source that impl should output
 * -  param1 holds a #v_zfindex indicates the source's size
 * -  result holds a #v_zfindex that has source length as init value,
 *   impl may update the result value to indicates bytes actually written
 *
 * this is useful for script to supply IO logic, lua for example:
 * @code
 *   ZFOutputForImpl(function(zfargs)
 *       local src = zfargs:param0()
 *       local size = zfargs:param1()
 *       local result = zfargs:result()
 *       local yourWrittenLength = yourOutput(buf, count)
 *       -- return size actually written
 *       result:zfv(yourWrittenLength)
 *   end)
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFOutput, ZFOutputForImpl
        , ZFMP_IN(const ZFListener &, impl)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForImpl_h_

