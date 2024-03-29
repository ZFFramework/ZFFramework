#include "ZFSerializableUtil.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSerializableUtil)

#define _ZFP_ZFSerializableUtil_DEBUG 0

#if _ZFP_ZFSerializableUtil_DEBUG
static void _ZFP_ZFSerializableUtilDebugAction(void) {
    zfCoreCriticalMessageTrim("[ZFSerializableUtil] errorOccurred");
}
#endif

void _ZFP_errorOccurredAt(
        ZF_OUT_OPT zfstring *outErrorHint
        , ZF_OUT_OPT ZFSerializableData *outErrorPos
        , ZF_IN const ZFSerializableData &errorPos
        , ZF_IN const zfchar *text
        ) {
    if(outErrorPos != zfnull) {
        *outErrorPos = errorPos;
    }

    if(outErrorHint != zfnull) {
        zfstringAppend(*outErrorHint, "%s, at:\n    %s", text, errorPos);
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}

void _ZFP_errorOccurred(
        ZF_OUT_OPT zfstring *outErrorHint
        , ZF_IN const zfchar *text
        ) {
    if(outErrorHint != zfnull) {
        *outErrorHint += text;
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}

const zfchar *checkItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredClass
        ) {
    const zfchar *serializableClass = serializableData.itemClass();
    if(zfstringIsEqual(desiredClass, ZFTypeId_none())) {
        return serializableClass;
    }
    else {
        if(zfstringIsEqual(serializableClass, desiredClass)) {
            return serializableClass;
        }
        else {
            return zfnull;
        }
    }
}
const zfchar *requireItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredClass
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    const zfchar *ret = checkItemClass(serializableData, desiredClass);
    if(ret == zfnull) {
        if(desiredClass == zfnull || *desiredClass == '\0') {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "missing serializable class");
        }
        else {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "serializable class must be \"%s\"", desiredClass);
        }
    }
    return ret;
}

const zfchar *checkAttribute(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredAttribute
        ) {
    zfiterator it = serializableData.attrIterFind(desiredAttribute);
    if(!serializableData.attrIterValid(it)) {
        return zfnull;
    }
    serializableData.attrIterResolveMark(it);
    return serializableData.attrIterValue(it);
}
const zfchar *requireAttribute(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredAttribute
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    const zfchar *ret = checkAttribute(serializableData, desiredAttribute);
    if(ret == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "missing attribute \"%s\"", desiredAttribute);
    }
    return ret;
}

const ZFSerializableData *checkElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementName
        ) {
    zfindex index = serializableData.childForName(desiredElementName);
    if(index == zfindexMax()) {
        return zfnull;
    }
    serializableData.childAt(index).resolvePropertyNameMark();
    return &(serializableData.childAt(index));
}
const ZFSerializableData *requireElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementName
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    const ZFSerializableData *ret = checkElementByName(serializableData, desiredElementName);
    if(ret == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "missing element with name \"%s\"",
            desiredElementName);
    }
    return ret;
}

const ZFSerializableData *checkElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementCategory
        ) {
    zfindex index = serializableData.childForCategory(desiredElementCategory);
    if(index == zfindexMax()) {
        return zfnull;
    }
    serializableData.childAt(index).resolveCategoryMark();
    return &(serializableData.childAt(index));
}
const ZFSerializableData *requireElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementCategory
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    const ZFSerializableData *ret = checkElementByCategory(serializableData, desiredElementCategory);
    if(ret == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "missing element with category \"%s\"",
            desiredElementCategory);
    }
    return ret;
}

static zfbool _ZFP_ZFSerializableUtilPrintResolveStatus(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const ZFOutput &outputCallback
        , ZF_IN zfindex level
        ) {
    if(!serializableData.resolvedAll()) {
        for(zfindex i = 0; i < level; ++i) {
            outputCallback << "    ";
        }
        outputCallback << serializableData.itemClass();
        if(!serializableData.resolved()) {
            outputCallback << "(unresolved)";
        }

        zfbool hasUnresolvedAttribute = zffalse;
        for(zfiterator it = serializableData.attrIter();
                serializableData.attrIterValid(it);
                serializableData.attrIterNext(it)
                ) {
            if(!serializableData.attrIterResolved(it)) {
                if(!hasUnresolvedAttribute) {
                    hasUnresolvedAttribute = zftrue;
                    outputCallback << " < ";
                }
                else {
                    outputCallback << "; ";
                }
                outputCallback
                    << serializableData.attrIterKey(it)
                    << "="
                    << serializableData.attrIterValue(it);
            }
        }
        if(hasUnresolvedAttribute) {
            outputCallback << " >";
        }

        outputCallback << "\n";

        for(zfindex i = 0; i < serializableData.childCount(); ++i) {
            _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData.childAt(i), outputCallback, level + 1);
        }
        return zftrue;
    }
    return zffalse;
}
zfbool printResolveStatus(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN_OPT const ZFOutput &outputCallback /* = ZFOutputDefault() */
        ) {
    if(!ZFSerializableDataResolveCheckEnable) {
        return zffalse;
    }
    zfstring tmp = "[ZFSerializable] not all resolved:\n";
    zfbool ret = _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData, ZFOutputForString(tmp), 1);
    if(ret) {
        outputCallback.execute(tmp.cString());
    }
    return ret;
}

ZF_NAMESPACE_END_WITH_REGISTER(ZFSerializableUtil, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_GLOBAL_END

