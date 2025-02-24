#include "ZFCallbackSerializable.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlmap.h"

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
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "raw function is not supported");
                return zffalse;
            case ZFCallbackTypeLambda:
                serializableData.itemClass(ZFTypeId_ZFCallback());
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
static zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &_ZFP_ZFCallbackSerializeCustomCallbackMap(void) {
    static zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> d;
    return d;
}
void _ZFP_ZFCallbackSerializeCustomTypeRegister(
        ZF_IN const zfstring &customType
        , ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback
        ) {
    zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    ZFCoreAssert(customType && serializeCallback != zfnull);
    ZFCoreAssertWithMessage(m.find(customType) == m.end(), "custom callback serialize type \"%s\" already registered", customType);

    m[customType] = serializeCallback;
}
void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfstring &customType) {
    _ZFP_ZFCallbackSerializeCustomCallbackMap().erase(customType);
}
_ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeForName(ZF_IN const zfstring &customType) {
    zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback>::iterator it = m.find(customType);
    if(it != m.end()) {
        return it->second;
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

