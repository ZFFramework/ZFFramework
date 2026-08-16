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
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFJson *errorPos = zfnull
        ) {
    if(!jsonObject) {
        ZFSerializableUtilErrorOccurred(errorHint, "null json object");
        if(errorPos != zfnull) {
            *errorPos = jsonObject;
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
                ZFSerializableUtilErrorOccurred(errorHint,
                    "json item %s not type of %s",
                    jsonItem,
                    v_ZFJsonType::EnumNameForValue(v_ZFJsonType::e_Array));
                if(errorPos != zfnull) {
                    *errorPos = jsonItem;
                }
                return zffalse;
            }
            elementArray = jsonItem;
        }
        else {
            if(jsonItem.type() != v_ZFJsonType::e_Value) {
                ZFSerializableUtilErrorOccurred(errorHint,
                    "json item %s not type of %s",
                    jsonItem,
                    v_ZFJsonType::EnumNameForValue(v_ZFJsonType::e_Value));
                if(errorPos != zfnull) {
                    *errorPos = jsonItem;
                }
                return zffalse;
            }
            serializableData.attr(key, jsonItem.value());
        }
    }

    if(serializableData.itemClass() == zfnull) {
        ZFSerializableUtilErrorOccurred(errorHint, "missing class node (which looks like \"@ClassName\"");
        if(errorPos != zfnull) {
            *errorPos = jsonObject;
        }
        return zffalse;
    }

    if(elementArray) {
        for(zfindex i = 0; i < elementArray.childCount(); ++i) {
            ZFSerializableData childData;
            if(!_ZFP_ZFSerializableDataFromJson(childData, elementArray.childAt(i), errorHint, errorPos)) {
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
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        , ZFMP_OUT_OPT(ZFJson *, errorPos, zfnull)
        ) {
    return _ZFP_ZFSerializableDataFromJson(serializableData, jsonObject, errorHint, errorPos);
}
ZFMETHOD_FUNC_DEFINE_3(ZFSerializableData, ZFSerializableDataFromJson
        , ZFMP_IN(const ZFJson &, jsonObject)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        , ZFMP_OUT_OPT(ZFJson *, errorPos, zfnull)
        ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, jsonObject, errorHint, errorPos)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson
        , ZFMP_OUT(ZFJson &, jsonObject)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, errorPos, zfnull)
        ) {
    jsonObject = ZFSerializableDataToJson(serializableData, errorHint, errorPos);
    return jsonObject;
}
ZFMETHOD_FUNC_DEFINE_3(ZFJson, ZFSerializableDataToJson
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, errorPos, zfnull)
        ) {
    if(serializableData.itemClass() == zfnull) {
        ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, serializableData, "missing serializable class");
        return zfnull;
    }

    ZFJson ret(v_ZFJsonType::e_Object);

    for(zfiter it = serializableData.attrIter(); it; ++it) {
        ret.attr(serializableData.attrIterKey(it),
            serializableData.attrIterValue(it));
    }

    ZFJson elementArray(v_ZFJsonType::e_Array);
    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        ZFJson child = ZFSerializableDataToJson(serializableData.childAt(i), errorHint, errorPos);
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
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        ) {
    if(!input) {
        zfstringAppend(errorHint, "invalid input callback");
        return zffalse;
    }
    ZFJson jsonElement = ZFJsonFromInput(input, errorHint);
    if(!jsonElement) {
        return zffalse;
    }
    if(!ZFSerializableDataFromJson(ret, jsonElement, errorHint)) {
        return zffalse;
    }

    ret.pathInfo(input.pathInfo());
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(ZFSerializableData, ZFSerializableDataFromJson
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, input, errorHint)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        ) {
    ZFJson jsonObject;
    if(ZFSerializableDataToJson(jsonObject, serializableData, errorHint)) {
        zfbool ret = ZFJsonToOutput(outputCallback, jsonObject, token);
        outputCallback.execute("\n");
        if(!ret) {
            zfstringAppend(errorHint, "unable to convert json to string");
        }
        return ret;
    }
    else {
        return zffalse;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFObjectFromJsonT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        ) {
    ZFSerializableData data;
    if(ZFSerializableDataFromJson(data, input, errorHint)) {
        return ZFObjectFromDataT(ret, data, errorHint);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfauto, ZFObjectFromJson
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        ) {
    zfauto ret;
    ZFObjectFromJsonT(ret, input, errorHint);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFObjectToJson
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        ) {
    ZFSerializableData serializableData;
    if(!ZFObjectToDataT(serializableData, obj, errorHint)) {
        return zffalse;
    }
    else {
        return ZFSerializableDataToJson(outputCallback, serializableData, errorHint, token);
    }
}

ZF_NAMESPACE_GLOBAL_END

