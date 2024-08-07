#include "ZFTypeIdCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFTypeIdWrapper)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTypeIdWrapper, ZFTypeIdWrapper *, wrappedValueAssign
        , ZFMP_IN(ZFTypeIdWrapper *, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTypeIdWrapper, const zfchar *, wrappedValueTypeId)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTypeIdWrapper, void, wrappedValueReset)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTypeIdWrapper, zfbool, wrappedValueIsInit)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFTypeIdWrapper, zfbool, wrappedValueFromData
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFTypeIdWrapper, zfbool, wrappedValueToData
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFTypeIdWrapper, zfbool, wrappedValueFromString
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTypeIdWrapper, zfbool, wrappedValueToString
        , ZFMP_IN_OUT(zfstring &, s)
        )

ZFOBJECT_ON_INIT_USER_REGISTER_2({
        invokerObject->to<ZFTypeIdWrapper *>()->wrappedValueFromString(src, srcLen);
    }, ZFTypeIdWrapper
    , ZFMP_IN(const zfchar *, src)
    , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
    )
ZFOBJECT_ON_INIT_USER_REGISTER_1({
        if(src != zfnull) {
            if(src->classData()->classIsTypeOf(invokerObject->classData())
                || (
                    invokerObject->classData()->classIsTypeOf(src->classData())
                    && zfstringIsEqual(src->wrappedValueTypeId(), invokerObject->to<ZFTypeIdWrapper *>()->wrappedValueTypeId())
                )
            ) {
                invokerObject->to<ZFTypeIdWrapper *>()->wrappedValueAssign(src);
            }
            else {
                zfbool success = zffalse;
                zfstring s;
                if(src->wrappedValueToString(s)) {
                    success = invokerObject->to<ZFTypeIdWrapper *>()->wrappedValueFromString(s);
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

ZF_NAMESPACE_GLOBAL_END
#endif

