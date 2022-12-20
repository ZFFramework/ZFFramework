#include "ZFCallbackSerializable.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serialize routine
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFCallback, ZFCallback, {
        { // custom serialize logic
            const zfchar *customType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFCallback_callbackType);
            if(customType != zfnull)
            {
                const ZFSerializableData *customData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_callbackData);
                if(customData == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        "missing %s", ZFSerializableKeyword_ZFCallback_callbackData);
                    return zffalse;
                }

                _ZFP_ZFCallbackSerializeCustomCallback serializeCallback = _ZFP_ZFCallbackSerializeCustomTypeForName(customType);
                if(serializeCallback == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        "no such callback custom serialize type: %s", customType);
                    return zffalse;
                }
                if(!serializeCallback(v, *customData, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.callbackSerializeCustomType(customType);
                ZFSerializableData customDataTmp = customData->copy();
                customDataTmp.category(zfnull);
                v.callbackSerializeCustomData(customDataTmp);

                serializableData.resolveMark();
                return zftrue;
            }
        }

        if(zfstringIsEqual(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = ZFCallback();
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFCallback(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const ZFMethod *method = zfnull;
        ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFCallback_method, ZFMethod, method);

        if(method->methodOwnerClass() == zfnull)
        {
            v = ZFCallbackForMethod(method);
        }
        else if(method->methodType() == ZFMethodTypeStatic)
        {
            v = ZFCallbackForMethod(method);
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "member method callback is not supported");
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v.callbackSerializeCustomType() != zfnull)
        {
            if(v.callbackSerializeCustomDisabled())
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, "callback was marked as not serializable");
                return zffalse;
            }

            if(v.callbackSerializeCustomData() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, "missing callback serialize custom data");
                return zffalse;
            }
            serializableData.itemClass(ZFTypeId_ZFCallback());

            serializableData.attr(ZFSerializableKeyword_ZFCallback_callbackType, v.callbackSerializeCustomType());

            ZFSerializableData customData = v.callbackSerializeCustomData()->copy();
            customData.category(ZFSerializableKeyword_ZFCallback_callbackData);
            serializableData.childAdd(customData);

            return zftrue;
        }

        switch(v.callbackType())
        {
            case ZFCallbackTypeDummy:
                serializableData.itemClass(ZFSerializableKeyword_null);
                break;
            case ZFCallbackTypeMethod:
            {
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableData methodData;
                if(!ZFMethodToData(methodData, v.callbackMethod(), outErrorHint))
                {
                    return zffalse;
                }
                methodData.category(ZFSerializableKeyword_ZFCallback_method);
                serializableData.childAdd(methodData);
            }
                break;
            case ZFCallbackTypeMemberMethod:
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "member method callback is not supported");
                return zffalse;
            }
                break;
            case ZFCallbackTypeRawFunction:
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "raw function is not supported");
                return zffalse;
            case ZFCallbackTypeLambda:
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableUtil::errorOccurred(outErrorHint,
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
static zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &_ZFP_ZFCallbackSerializeCustomCallbackMap(void)
{
    static zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> d;
    return d;
}
void _ZFP_ZFCallbackSerializeCustomTypeRegister(ZF_IN const zfchar *customType,
                                                ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback)
{
    zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfCoreAssert(!zfstringIsEmpty(customType) && serializeCallback != zfnull);
    zfCoreAssertWithMessage(m.find(customType) == m.end(), "custom callback serialize type \"%s\" already registered", customType);

    m[customType] = serializeCallback;
}
void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType)
{
    _ZFP_ZFCallbackSerializeCustomCallbackMap().erase(customType);
}
_ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeForName(ZF_IN const zfchar *customType)
{
    zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfstlmap<zfstring, _ZFP_ZFCallbackSerializeCustomCallback>::iterator it = m.find(customType);
    if(it != m.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

