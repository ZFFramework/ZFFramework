/**
 * @file ZFClassSerializable.h
 * @brief serialization for ZFClass
 */

#ifndef _ZFI_ZFClassSerializable_h_
#define _ZFI_ZFClassSerializable_h_

#include "ZFClass.h"
#include "ZFSerializable.h"
#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFTYPEID_DECLARE
 */
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFClassInstanceObserverAddParam, ZFClassInstanceObserverAddParam)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFClass value="ClassName" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFClass, const ZFClass *)
ZFOUTPUT_TYPE(ZFClass, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClassSerializable_h_

