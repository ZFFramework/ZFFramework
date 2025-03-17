#include "ZFPropertyUtil.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsValueAccessed
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(zfany const &, ownerObj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsInitValue
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(zfany const &, ownerObj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCompareResult, ZFPropertyCompare
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(ZFObject *, obj0)
        , ZFMP_IN(ZFObject *, obj1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCompareResult, ZFPropertyCompareValue
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(ZFObject *, obj0)
        , ZFMP_IN(ZFObject *, obj1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFPropertyCopy
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(ZFObject *, dstObj)
        , ZFMP_IN(ZFObject *, srcObj)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

