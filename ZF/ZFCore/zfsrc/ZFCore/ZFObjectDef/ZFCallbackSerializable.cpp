#include "ZFCallbackSerializable.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serialize routine
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFCallback, ZFCallback, {
        { // custom serialize logic
            zfstring customType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFCallback_callbackType);
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
                v.callbackSerializeCustomType(customType);
                ZFSerializableData customDataTmp = serializableData.copy();
                customDataTmp.category(zfnull);
                customDataTmp.propertyName(zfnull);
                v.callbackSerializeCustomData(customDataTmp);

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
        ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
                require, ZFSerializableKeyword_ZFCallback_method, ZFMethod, method, {
                    return zffalse;
                });

        if(method->methodOwnerClass() == zfnull) {
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
        if(v.callbackSerializeCustomType() != zfnull) {
            if(v.callbackSerializeCustomDisabled()) {
                ZFSerializableUtilErrorOccurred(outErrorHint, "callback was marked as not serializable");
                return zffalse;
            }

            if(v.callbackSerializeCustomData() == zfnull) {
                ZFSerializableUtilErrorOccurred(outErrorHint, "missing callback serialize custom data");
                return zffalse;
            }

            const ZFSerializableData &customData = *(v.callbackSerializeCustomData());
            for(zfiterator it = customData.attrIter(); customData.attrIterValid(it); customData.attrIterNext(it)) {
                serializableData.attr(customData.attrIterKey(it), customData.attrIterValue(it));
            }
            for(zfindex i = 0; i < customData.childCount(); ++i) {
                serializableData.childAdd(customData.childAt(i).copy());
            }

            serializableData.itemClass(ZFTypeId_ZFCallback());
            serializableData.attr(ZFSerializableKeyword_ZFCallback_callbackType, v.callbackSerializeCustomType());
            return zftrue;
        }

        switch(v.callbackType()) {
            case ZFCallbackTypeDummy:
                serializableData.itemClass(ZFSerializableKeyword_null);
                break;
            case ZFCallbackTypeMethod:
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableUtilSerializeAttributeToDataNoRef(serializableData, outErrorHint,
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
                zfCoreCriticalShouldNotGoHere();
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
        ZF_IN const zfchar *customType
        , ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback
        ) {
    zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfCoreAssert(!zfstringIsEmpty(customType) && serializeCallback != zfnull);
    zfCoreAssertWithMessage(m.find(customType) == m.end(), "custom callback serialize type \"%s\" already registered", customType);

    m[customType] = serializeCallback;
}
void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType) {
    _ZFP_ZFCallbackSerializeCustomCallbackMap().erase(customType);
}
_ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeForName(ZF_IN const zfchar *customType) {
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

