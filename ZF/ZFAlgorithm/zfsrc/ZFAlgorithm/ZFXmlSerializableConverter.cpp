#include "ZFXmlSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfbool _ZFP_ZFSerializableDataFromXml(
            ZF_OUT ZFSerializableData &serializableData
            , ZF_IN const ZFXml &xmlElement
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFXml *outErrorPos = zfnull
            ) {
    if(!xmlElement) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "null xml element");
        if(outErrorPos != zfnull) {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }
    if(xmlElement.type() != v_ZFXmlType::e_Element) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "param not type of xml element");
        if(outErrorPos != zfnull) {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }

    if(xmlElement.name() == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "missing xml node name");
        if(outErrorPos != zfnull) {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }
    serializableData.itemClass(xmlElement.name());

    for(zfiter it = xmlElement.attrIter(); it; ++it) {
        serializableData.attr(xmlElement.attrIterKey(it), xmlElement.attrIterValue(it));
    }

    for(zfindex i = 0; i < xmlElement.childCount(); ++i) {
        ZFSerializableData childData;
        if(!_ZFP_ZFSerializableDataFromXml(childData, xmlElement.childAt(i), outErrorHint, outErrorPos)) {
            return zffalse;
        }
        serializableData.child(childData);
    }
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataFromXml
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_IN(const ZFXml &, xmlElement)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFXml *, outErrorPos, zfnull)
        ) {
    return _ZFP_ZFSerializableDataFromXml(serializableData, xmlElement, outErrorHint, outErrorPos);
}
ZFMETHOD_FUNC_DEFINE_3(ZFSerializableData, ZFSerializableDataFromXml
        , ZFMP_IN(const ZFXml &, xmlElement)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFXml *, outErrorPos, zfnull)
        ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromXml(ret, xmlElement, outErrorHint, outErrorPos)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToXml
        , ZFMP_OUT(ZFXml &, xmlElement)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        ) {
    xmlElement = ZFSerializableDataToXml(serializableData, outErrorHint, outErrorPos);
    return (xmlElement.type() != v_ZFXmlType::e_Null);
}
ZFMETHOD_FUNC_DEFINE_3(ZFXml, ZFSerializableDataToXml
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        ) {
    if(serializableData.itemClass() == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "missing serializable class");
        return zfnull;
    }

    ZFXml ret(v_ZFXmlType::e_Element);
    ret.name(serializableData.itemClass());

    for(zfiter it = serializableData.attrIter(); it; ++it) {
        ret.attr(serializableData.attrIterKey(it),
            serializableData.attrIterValue(it));
    }

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        ZFXml child = ZFSerializableDataToXml(serializableData.childAt(i), outErrorHint, outErrorPos);
        if(child.type() == v_ZFXmlType::e_Null) {
            return zfnull;
        }
        ret.child(child);
    }

    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFSerializableDataFromXml
            , ZFMP_OUT(ZFSerializableData &, ret)
            , ZFMP_IN(const ZFInput &, input)
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
            ) {
    if(!input) {
        zfstringAppend(outErrorHint, "invalid input callback");
        return zffalse;
    }
    ZFXml xmlElement = ZFXmlFromInput(input, outErrorHint).childElement();
    if(!xmlElement) {
        return zffalse;
    }
    if(!ZFSerializableDataFromXml(ret, xmlElement, outErrorHint)) {
        return zffalse;
    }

    ret.pathInfo(input.pathInfo());
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(ZFSerializableData, ZFSerializableDataFromXml
            , ZFMP_IN(const ZFInput &, input)
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
            ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromXml(ret, input, outErrorHint)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToXml
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFXmlOutputToken &, token, ZFXmlOutputTokenDefault())
        ) {
    ZFXml xmlElement;
    if(ZFSerializableDataToXml(xmlElement, serializableData, outErrorHint)) {
        zfbool ret = ZFXmlToOutput(outputCallback, xmlElement, token);
        outputCallback.execute("\n");
        if(!ret) {
            zfstringAppend(outErrorHint, "unable to convert xml to string");
        }
        return ret;
    }
    else {
        return zffalse;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFObjectFromXmlT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    ZFSerializableData data;
    if(ZFSerializableDataFromXml(data, input, outErrorHint)) {
        return ZFObjectFromDataT(ret, data, outErrorHint);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfauto, ZFObjectFromXml
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    zfauto ret;
    ZFObjectFromXmlT(ret, input, outErrorHint);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFObjectToXml
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFXmlOutputToken &, token, ZFXmlOutputTokenDefault())
        ) {
    ZFSerializableData serializableData;
    if(!ZFObjectToDataT(serializableData, obj, outErrorHint)) {
        return zffalse;
    }
    else {
        return ZFSerializableDataToXml(outputCallback, serializableData, outErrorHint, token);
    }
}

ZF_NAMESPACE_GLOBAL_END

