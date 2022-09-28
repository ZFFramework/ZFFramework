#include "ZFJsonSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * <v_zfstring myAttr="myAttrValue" >
 *     <zfstring category="value" value="123" />
 *     <zfstring k1="123" k2="123" />
 * </v_zfstring>
 *
 * {
 *     "@v_zfstring" : [
 *         {
 *            "@zfstring" : [],
 *            "category" : "value",
 *            "value" : "123"
 *         },
 *         {
 *            "@zfstring" : [],
 *            "k1" : "123",
 *            "k2" : "123"
 *         }
 *     ],
 *     "myAttr" : "myAttrValue"
 * }
 */

#define _ZFP_ZFJsonSerializeKey_classPrefix '@'

// ============================================================
static zfbool _ZFP_ZFSerializableDataFromJson(ZF_OUT ZFSerializableData &serializableData,
                                              ZF_IN const ZFJsonItem &jsonObject,
                                              ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                              ZF_OUT_OPT ZFJsonItem *outErrorPos = zfnull)
{
    if(!jsonObject.jsonTypeValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, "null json object");
        if(outErrorPos != zfnull)
        {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    ZFJsonItem elementJsonArray;
    for(zfiterator jsonItemIt = jsonObject.attrIter(); jsonObject.attrIterValid(jsonItemIt); jsonObject.attrIterNext(jsonItemIt))
    {
        const zfchar *key = jsonObject.attrIterKey(jsonItemIt);
        ZFJsonItem jsonItem = jsonObject.attrIterValue(jsonItemIt);
        if(*key == _ZFP_ZFJsonSerializeKey_classPrefix)
        {
            serializableData.itemClass(key + 1);

            if(jsonItem.jsonType() != ZFJsonType::e_JsonArray)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "json item %s not type of %s",
                    jsonItem.objectInfo().cString(),
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonArray));
                if(outErrorPos != zfnull)
                {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            elementJsonArray = jsonItem;
        }
        else
        {
            if(jsonItem.jsonType() != ZFJsonType::e_JsonValue)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "json item %s not type of %s",
                    jsonItem.objectInfo().cString(),
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonValue));
                if(outErrorPos != zfnull)
                {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            serializableData.attr(key, jsonItem.jsonValue());
        }
    }

    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, "missing class node (which looks like \"@ClassName\"");
        if(outErrorPos != zfnull)
        {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    if(elementJsonArray.jsonTypeValid())
    {
        for(zfindex i = 0; i < elementJsonArray.childCount(); ++i)
        {
            ZFSerializableData childData;
            if(!_ZFP_ZFSerializableDataFromJson(childData, elementJsonArray.childAtIndex(i), outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            serializableData.childAdd(childData);
        }
    }

    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataFromJson,
                       ZFMP_OUT(ZFSerializableData &, serializableData),
                       ZFMP_IN(const ZFJsonItem &, jsonObject),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFJsonItem *, outErrorPos, zfnull))
{
    return _ZFP_ZFSerializableDataFromJson(serializableData, jsonObject, outErrorHint, outErrorPos);
}
ZFMETHOD_FUNC_DEFINE_3(ZFSerializableData, ZFSerializableDataFromJson,
                       ZFMP_IN(const ZFJsonItem &, jsonObject),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFJsonItem *, outErrorPos, zfnull))
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, jsonObject, outErrorHint, outErrorPos))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson,
                       ZFMP_OUT(ZFJsonItem &, jsonObject),
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
{
    jsonObject = ZFSerializableDataToJson(serializableData, outErrorHint, outErrorPos);
    return jsonObject.jsonTypeValid();
}
ZFMETHOD_FUNC_DEFINE_3(ZFJsonItem, ZFSerializableDataToJson,
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
{
    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, "missing serializable class");
        return ZFJsonItem();
    }

    ZFJsonItem ret(ZFJsonType::e_JsonObject);

    for(zfiterator it = serializableData.attrIter();
        serializableData.attrIterValid(it);
        serializableData.attrIterNext(it))
    {
        ret.attr(serializableData.attrIterKey(it),
            serializableData.attrIterValue(it));
    }

    ZFJsonItem elementJsonArray(ZFJsonType::e_JsonArray);
    for(zfindex i = 0; i < serializableData.childCount(); ++i)
    {
        ZFJsonItem child = ZFSerializableDataToJson(serializableData.childAtIndex(i), outErrorHint, outErrorPos);
        if(child.jsonType() == ZFJsonType::e_JsonNull)
        {
            return ZFJsonItem();
        }
        elementJsonArray.childAdd(child);
    }
    zfstring t;
    t += _ZFP_ZFJsonSerializeKey_classPrefix;
    t += serializableData.itemClass();
    ret.attr(t.cString(), elementJsonArray);

    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFSerializableDataFromJson,
                       ZFMP_OUT(ZFSerializableData &, ret),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    if(!input.callbackIsValid())
    {
        zfstringAppend(outErrorHint, "invalid input callback");
        return zffalse;
    }
    ZFJsonItem jsonElement = ZFJsonItemFromInput(input);
    if(jsonElement.jsonType() == ZFJsonType::e_JsonNull)
    {
        zfstringAppend(outErrorHint, "unable to parse json from input");
        return zffalse;
    }
    if(!ZFSerializableDataFromJson(ret, jsonElement, outErrorHint))
    {
        return zffalse;
    }

    ret.pathInfo(input.pathInfo());
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(ZFSerializableData, ZFSerializableDataFromJson,
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, input, outErrorHint))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson,
                       ZFMP_IN(const ZFOutput &, outputCallback),
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_IN_OPT(const ZFJsonOutputFlags &, flags, ZFJsonOutputFlagsDefault()))
{
    ZFJsonItem jsonObject;
    if(ZFSerializableDataToJson(jsonObject, serializableData, outErrorHint))
    {
        zfbool ret = ZFJsonItemToOutput(outputCallback, jsonObject, flags);
        outputCallback.execute("\n");
        if(!ret)
        {
            zfstringAppend(outErrorHint, "unable to convert json to string");
        }
        return ret;
    }
    else
    {
        return zffalse;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFObjectFromJson,
                       ZFMP_OUT(zfautoObject &, ret),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    ZFSerializableData data;
    if(ZFSerializableDataFromJson(data, input, outErrorHint))
    {
        return ZFObjectFromData(ret, data, outErrorHint);
    }
    else
    {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfautoObject, ZFObjectFromJson,
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    zfautoObject ret;
    ZFObjectFromJson(ret, input, outErrorHint);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFObjectToJson,
                       ZFMP_IN(const ZFOutput &, outputCallback),
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_IN_OPT(const ZFJsonOutputFlags &, flags, ZFJsonOutputFlagsDefault()))
{
    ZFSerializableData serializableData;
    if(!ZFObjectToData(serializableData, obj, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFSerializableDataToJson(outputCallback, serializableData, outErrorHint, flags);
    }
}

ZF_NAMESPACE_GLOBAL_END

