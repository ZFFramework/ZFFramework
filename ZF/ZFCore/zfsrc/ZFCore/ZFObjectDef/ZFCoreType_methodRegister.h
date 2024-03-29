/**
 * @file ZFCoreType_methodRegister.h
 * @brief export some methods for ZFObject core
 */

#ifndef _ZFI_ZFCoreType_methodRegister_h_
#define _ZFI_ZFCoreType_methodRegister_h_

#include "ZFExport.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @cond ZFPrivateDoc */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfint, ZF_ENV_ZFVERSION_MAIN)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfint, ZF_ENV_ZFVERSION_SUB)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfint, ZF_ENV_ZFVERSION_MINOR)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, const zfchar *, ZF_ENV_ZFVERSION)

ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, const zfchar *, ZFNamespaceSeparator)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfindex, ZFNamespaceSeparatorLen)

ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfindex, zfindexMax)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfindex, zfindexZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfint, zfintZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfuint, zfuintZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zffloat, zffloatZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfdouble, zfdoubleZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zflongdouble, zflongdoubleZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfbyte, zfbyteZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zftimet, zftimetZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfflags, zfflagsZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfidentity, zfidentityZero)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfidentity, zfidentityInvalid)

ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, zfiterator, zfiteratorInvalid, zfiteratorInvalid())

ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFIndexRange, ZFIndexRangeZero, ZFIndexRangeZero())
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFIndexRange, ZFIndexRangeMax, ZFIndexRangeMax())

ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfindex, ZFVERSION_MAIN)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfindex, ZFVERSION_SUB)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfindex, ZFVERSION_MINOR)
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFCore, zfindex, ZFVERSION_BUILD)

ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFTokenForContainer, ZFTokenForContainerTrim, ZFTokenForContainerTrim())
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFTokenForContainer, ZFTokenForContainerDetail, ZFTokenForContainerDetail())
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFTokenForContainer, ZFTokenForContainerPlainList, ZFTokenForContainerPlainList())
ZFEXPORT_VAR_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFTokenForContainer, ZFTokenForContainerDefault, ZFTokenForContainerDefault())

ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerTrim, ZFTokenForKeyValueContainerTrim())
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerDetail, ZFTokenForKeyValueContainerDetail())
ZFEXPORT_VAR_VALUEREF_DECLARE(ZFLIB_ZFCore, ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerDefault, ZFTokenForKeyValueContainerDefault())
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreType_methodRegister_h_

