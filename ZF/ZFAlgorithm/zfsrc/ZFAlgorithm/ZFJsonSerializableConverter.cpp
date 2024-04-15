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
static zfbool _ZFP_ZFSerializableDataFromJson(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN const ZFJson &jsonObject
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFJson *outErrorPos = zfnull
        ) {
    if(!jsonObject) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "null json object");
        if(outErrorPos != zfnull) {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    ZFJson elementJsonArray;
    for(zfiterator jsonItemIt = jsonObject.attrIter(); jsonObject.attrIterValid(jsonItemIt); jsonObject.attrIterNext(jsonItemIt)) {
        const zfchar *key = jsonObject.attrIterKey(jsonItemIt);
        ZFJson jsonItem = jsonObject.attrIterValue(jsonItemIt);
        if(*key == _ZFP_ZFJsonSerializeKey_classPrefix) {
            serializableData.itemClass(key + 1);

            if(jsonItem.jsonType() != ZFJsonType::e_JsonArray) {
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "json item %s not type of %s",
                    jsonItem,
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonArray));
                if(outErrorPos != zfnull) {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            elementJsonArray = jsonItem;
        }
        else {
            if(jsonItem.jsonType() != ZFJsonType::e_JsonValue) {
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "json item %s not type of %s",
                    jsonItem,
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonValue));
                if(outErrorPos != zfnull) {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            serializableData.attr(key, jsonItem.jsonValue());
        }
    }

    if(serializableData.itemClass() == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "missing class node (which looks like \"@ClassName\"");
        if(outErrorPos != zfnull) {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    if(elementJsonArray) {
        for(zfindex i = 0; i < elementJsonArray.childCount(); ++i) {
            ZFSerializableData childData;
            if(!_ZFP_ZFSerializableDataFromJson(childData, elementJsonArray.childAt(i), outErrorHint, outErrorPos)) {
                return zffalse;
            }
            serializableData.childAdd(childData);
        }
    }

    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataFromJson
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_IN(const ZFJson &, jsonObject)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFJson *, outErrorPos, zfnull)
        ) {
    return _ZFP_ZFSerializableDataFromJson(serializableData, jsonObject, outErrorHint, outErrorPos);
}
ZFMETHOD_FUNC_DEFINE_3(ZFSerializableData, ZFSerializableDataFromJson
        , ZFMP_IN(const ZFJson &, jsonObject)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFJson *, outErrorPos, zfnull)
        ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, jsonObject, outErrorHint, outErrorPos)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson
        , ZFMP_OUT(ZFJson &, jsonObject)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        ) {
    jsonObject = ZFSerializableDataToJson(serializableData, outErrorHint, outErrorPos);
    return jsonObject;
}
ZFMETHOD_FUNC_DEFINE_3(ZFJson, ZFSerializableDataToJson
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        ) {
    if(serializableData.itemClass() == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "missing serializable class");
        return zfnull;
    }

    ZFJson ret(ZFJsonType::e_JsonObject);

    for(zfiterator it = serializableData.attrIter();
            serializableData.attrIterValid(it);
            serializableData.attrIterNext(it)
            ) {
        ret.attr(serializableData.attrIterKey(it),
            serializableData.attrIterValue(it));
    }

    ZFJson elementJsonArray(ZFJsonType::e_JsonArray);
    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        ZFJson child = ZFSerializableDataToJson(serializableData.childAt(i), outErrorHint, outErrorPos);
        if(child.jsonType() == ZFJsonType::e_JsonNull) {
            return zfnull;
        }
        elementJsonArray.childAdd(child);
    }
    zfstring t;
    t += _ZFP_ZFJsonSerializeKey_classPrefix;
    t += serializableData.itemClass();
    ret.attr(t, elementJsonArray);

    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFSerializableDataFromJson
        , ZFMP_OUT(ZFSerializableData &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    if(!input) {
        zfstringAppend(outErrorHint, "invalid input callback");
        return zffalse;
    }
    ZFJson jsonElement = ZFJsonFromInput(input, outErrorHint);
    if(!jsonElement) {
        return zffalse;
    }
    if(!ZFSerializableDataFromJson(ret, jsonElement, outErrorHint)) {
        return zffalse;
    }

    ret.pathInfo(input.pathInfo());
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(ZFSerializableData, ZFSerializableDataFromJson
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, input, outErrorHint)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        ) {
    ZFJson jsonObject;
    if(ZFSerializableDataToJson(jsonObject, serializableData, outErrorHint)) {
        zfbool ret = ZFJsonToOutput(outputCallback, jsonObject, token);
        outputCallback.execute("\n");
        if(!ret) {
            zfstringAppend(outErrorHint, "unable to convert json to string");
        }
        return ret;
    }
    else {
        return zffalse;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFObjectFromJson
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    ZFSerializableData data;
    if(ZFSerializableDataFromJson(data, input, outErrorHint)) {
        return ZFObjectFromDataT(ret, data, outErrorHint);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfauto, ZFObjectFromJson
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    zfauto ret;
    ZFObjectFromJson(ret, input, outErrorHint);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFObjectToJson
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        ) {
    ZFSerializableData serializableData;
    if(!ZFObjectToDataT(serializableData, obj, outErrorHint)) {
        return zffalse;
    }
    else {
        return ZFSerializableDataToJson(outputCallback, serializableData, outErrorHint, token);
    }
}

ZF_NAMESPACE_GLOBAL_END

