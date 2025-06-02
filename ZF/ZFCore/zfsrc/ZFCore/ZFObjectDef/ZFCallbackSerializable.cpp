#include "ZFCallbackSerializable.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serialize routine
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFCallback, ZFCallback, {
        { // custom serialize logic
            zfstring customType = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFCallback_callbackType);
            if(customType != zfnull) {
                _ZFP_ZFCallbackSerializeCustomCallback serializeCallback = _ZFP_ZFCallbackSerializeCustomTypeForName(customType);
                if(serializeCallback == zfnull) {
                    ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                        "no such callback custom serialize type: %s", customType);
                    return zffalse;
                }
                if(!serializeCallback(v, serializableData, outErrorHint, outErrorPos)) {
                    return zffalse;
                }
                v.callbackSerializeType(customType);
                ZFSerializableData customDataTmp = serializableData.copy();
                customDataTmp.category(zfnull);
                customDataTmp.propertyName(zfnull);
                v.callbackSerializeData(customDataTmp);

                serializableData.resolveMark();
                return zftrue;
            }
        }

        if(zfstringIsEqual(serializableData.itemClass(), ZFSerializableKeyword_null)) {
            v = zfnull;
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFCallback(), outErrorHint, outErrorPos) == zfnull) {
            return zffalse;
        }

        const ZFMethod *method = zfnull;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                require, ZFSerializableKeyword_ZFCallback_method, ZFMethod, method, {
                    return zffalse;
                });

        if(method->ownerClass() == zfnull) {
            v = ZFCallbackForMethod(method);
        }
        else if(method->methodType() == ZFMethodTypeStatic) {
            v = ZFCallbackForMethod(method);
        }
        else {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "member method callback is not supported");
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v.callbackSerializeType() != zfnull) {
            if(v.callbackSerializeDisable()) {
                ZFSerializableUtilErrorOccurred(outErrorHint, "callback was marked as not serializable");
                return zffalse;
            }

            if(v.callbackSerializeData() == zfnull) {
                ZFSerializableUtilErrorOccurred(outErrorHint, "missing callback serialize custom data");
                return zffalse;
            }

            const ZFSerializableData &customData = v.callbackSerializeData();
            for(zfiter it = customData.attrIter(); it; ++it) {
                serializableData.attr(customData.attrIterKey(it), customData.attrIterValue(it));
            }
            for(zfindex i = 0; i < customData.childCount(); ++i) {
                serializableData.child(customData.childAt(i).copy());
            }

            serializableData.itemClass(ZFTypeId_ZFCallback());
            serializableData.attr(ZFSerializableKeyword_ZFCallback_callbackType, v.callbackSerializeType());
            return zftrue;
        }

        switch(v.callbackType()) {
            case ZFCallbackTypeDummy:
                serializableData.itemClass(ZFSerializableKeyword_null);
                break;
            case ZFCallbackTypeMethod:
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                        ZFSerializableKeyword_ZFCallback_method, ZFMethod, v.callbackMethod(), (const ZFMethod *)zfnull, {
                            return zffalse;
                        });
                break;
            case ZFCallbackTypeMemberMethod: {
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "member method callback is not supported");
                return zffalse;
            }
                break;
            case ZFCallbackTypeRawFunction:
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "raw function is not supported");
                return zffalse;
            case ZFCallbackTypeLambda:
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "lambda function is not supported");
                return zffalse;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        return zftrue;
    })

// ============================================================
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFListener, ZFListener)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFIOCallback, ZFIOCallback)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFOutput, ZFOutput)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFInput, ZFInput)

// ============================================================
// custom serialize logic
static zfstlhashmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &_ZFP_ZFCallbackSerializeCustomCallbackMap(void) {
    static zfstlhashmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> d;
    return d;
}
void _ZFP_ZFCallbackSerializeCustomTypeRegister(
        ZF_IN const zfstring &customType
        , ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback
        ) {
    zfstlhashmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    ZFCoreAssert(customType && serializeCallback != zfnull);
    ZFCoreAssertWithMessage(m.find(customType) == m.end(), "custom callback serialize type \"%s\" already registered", customType);

    m[customType] = serializeCallback;
}
void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfstring &customType) {
    _ZFP_ZFCallbackSerializeCustomCallbackMap().erase(customType);
}
_ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeForName(ZF_IN const zfstring &customType) {
    zfstlhashmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfstlhashmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback>::iterator it = m.find(customType);
    if(it != m.end()) {
        return it->second;
    }
    else {
        return zfnull;
    }
}

// ============================================================
ZFMETHOD_USER_REGISTER_0({
        return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute();
    }, v_ZFCallback,
    zfbool, execute)
ZFMETHOD_USER_REGISTER_1({
        return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute(zfargs);
    }, v_ZFCallback,
    zfbool, execute
    , ZFMP_IN(const ZFArgs &, zfargs)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackClear)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackId
        , ZFMP_IN(const zfstring &, callbackId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfstring, callbackId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfidentity, callbackHash)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTag
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfany, callbackTag
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTagGetAllKeyValue
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, allKey)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, allValue)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackTagRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfauto, callbackTagRemoveAndGet
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFCallbackType, callbackType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfany, callbackOwnerObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFMethod *, callbackMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFFuncAddrType, callbackRawFunction)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRetain)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRelease)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackInfoCopy
        , ZFMP_IN(const ZFCallback &, src)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeType
        , ZFMP_IN(const zfstring &, customType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfstring, callbackSerializeType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeData
        , ZFMP_IN(const ZFSerializableData &, customData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFSerializableData, callbackSerializeData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeDisable
        , ZFMP_IN(zfbool, disable)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackSerializeDisable)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFPathInfo, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, pathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForMethod
        , ZFMP_IN(const ZFMethod *, zfmethod)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCallback, ZFCallbackForMemberMethod
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN(const ZFMethod *, zfmethod)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForFunc
        , ZFMP_IN(ZFFuncAddrType, callbackRawFunction)
        )

ZF_NAMESPACE_GLOBAL_END

