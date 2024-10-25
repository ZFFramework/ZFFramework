#include "ZFTypeIdCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFTypeIdWrapper)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTypeIdWrapper, ZFTypeIdWrapper *, zfvAssign
        , ZFMP_IN(ZFTypeIdWrapper *, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTypeIdWrapper, const zfchar *, zfvTypeId)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTypeIdWrapper, void, zfvReset)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTypeIdWrapper, zfbool, zfvIsInit)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFTypeIdWrapper, zfbool, zfvFromData
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFTypeIdWrapper, zfbool, zfvToData
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFTypeIdWrapper, zfbool, zfvFromString
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTypeIdWrapper, zfbool, zfvToString
        , ZFMP_IN_OUT(zfstring &, s)
        )

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        if(src != zfnull) {
            if(src->classData()->classIsTypeOf(invokerObject->classData())
                || (
                    invokerObject->classData()->classIsTypeOf(src->classData())
                    && zfstringIsEqual(src->zfvTypeId(), invokerObject->to<ZFTypeIdWrapper *>()->zfvTypeId())
                )
            ) {
                invokerObject->to<ZFTypeIdWrapper *>()->zfvAssign(src);
            }
            else {
                zfbool success = zffalse;
                zfstring s;
                if(src->zfvToString(s)) {
                    success = invokerObject->to<ZFTypeIdWrapper *>()->zfvFromString(s);
                }
                if(!success) {
                    zfobj<v_zfstring> errorHint;
                    zfstringAppend(errorHint->zfv, "unable to construct %s from (%s)%s",
                        invokerObject->classData()->className(),
                        src->classData()->className(),
                        src);
                    invokerObject->objectTag(ZFObjectTagKeyword_newInstanceGenericFailed, errorHint);
                }
            }
        }
    }, ZFTypeIdWrapper
    , ZFMP_IN(ZFTypeIdWrapper *, src)
    )
ZFOBJECT_ON_INIT_USER_REGISTER_2({
        invokerObject->to<ZFTypeIdWrapper *>()->zfvFromString(src, srcLen);
    }, ZFTypeIdWrapper
    , ZFMP_IN(const zfchar *, src)
    , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
    )

ZF_NAMESPACE_GLOBAL_END
#endif

