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

    ZFJson elementArray;
    for(zfiter jsonItemIt = jsonObject.attrIter(); jsonItemIt; ++jsonItemIt) {
        zfstring key = jsonObject.attrIterKey(jsonItemIt);
        ZFJson jsonItem = jsonObject.attrIterValue(jsonItemIt);
        if(*key == _ZFP_ZFJsonSerializeKey_classPrefix) {
            serializableData.itemClass(key + 1);

            if(jsonItem.type() != v_ZFJsonType::e_Array) {
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "json item %s not type of %s",
                    jsonItem,
                    v_ZFJsonType::EnumNameForValue(v_ZFJsonType::e_Array));
                if(outErrorPos != zfnull) {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            elementArray = jsonItem;
        }
        else {
            if(jsonItem.type() != v_ZFJsonType::e_Value) {
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "json item %s not type of %s",
                    jsonItem,
                    v_ZFJsonType::EnumNameForValue(v_ZFJsonType::e_Value));
                if(outErrorPos != zfnull) {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            serializableData.attr(key, jsonItem.value());
        }
    }

    if(serializableData.itemClass() == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "missing class node (which looks like \"@ClassName\"");
        if(outErrorPos != zfnull) {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    if(elementArray) {
        for(zfindex i = 0; i < elementArray.childCount(); ++i) {
            ZFSerializableData childData;
            if(!_ZFP_ZFSerializableDataFromJson(childData, elementArray.childAt(i), outErrorHint, outErrorPos)) {
                return zffalse;
            }
            serializableData.child(childData);
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

    ZFJson ret(v_ZFJsonType::e_Object);

    for(zfiter it = serializableData.attrIter(); it; ++it) {
        ret.attr(serializableData.attrIterKey(it),
            serializableData.attrIterValue(it));
    }

    ZFJson elementArray(v_ZFJsonType::e_Array);
    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        ZFJson child = ZFSerializableDataToJson(serializableData.childAt(i), outErrorHint, outErrorPos);
        if(child.type() == v_ZFJsonType::e_Null) {
            return zfnull;
        }
        elementArray.child(child);
    }
    zfstring t;
    t += _ZFP_ZFJsonSerializeKey_classPrefix;
    t += serializableData.itemClass();
    ret.attr(t, elementArray);

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

