#include "ZFObjectUtil.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCompareResult, ZFObjectCompare
        , ZFMP_IN(ZFObject *, e0)
        , ZFMP_IN(ZFObject *, e1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCompareResult, ZFObjectCompareValue
        , ZFMP_IN(ZFObject *, e0)
        , ZFMP_IN(ZFObject *, e1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFObjectInfoOfInstanceT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(ZFObject *, obj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFObjectInfoOfInstance
        , ZFMP_IN(ZFObject *, obj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFObjectInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(ZFObject *, obj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFObjectInfo
        , ZFMP_IN(ZFObject *, obj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFObjectHash
        , ZFMP_IN(ZFObject *, obj)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfauto, ZFObjectCreator
        , ZFMP_IN(const ZFListener &, impl)
        , ZFMP_IN_OPT(const ZFArgs &, zfargs, ZFArgs())
        )

ZF_NAMESPACE_GLOBAL_END

