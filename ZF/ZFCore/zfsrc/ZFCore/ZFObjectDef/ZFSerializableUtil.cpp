#include "ZFSerializableUtil.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSerializableUtil)

// #define _ZFP_ZFSerializableUtil_DEBUG 1

#if _ZFP_ZFSerializableUtil_DEBUG
static void _ZFP_ZFSerializableUtilDebugAction(void) {
    ZFCoreCriticalMessageTrim("[ZFSerializableUtil] errorOccurred");
}
#endif

void _ZFP_errorOccurredAt(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_OUT_OPT ZFSerializableData *errorPos
        , ZF_IN const ZFSerializableData &pos
        , ZF_IN const zfstring &text
        ) {
    if(errorPos != zfnull) {
        *errorPos = pos;
    }

    if(errorHint != zfnull) {
        zfstringAppend(*errorHint, "%s, at:\n    %s", text, pos);
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}

void _ZFP_errorOccurred(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const zfstring &text
        ) {
    if(errorHint != zfnull) {
        *errorHint += text;
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}

zfstring checkItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredClass
        ) {
    zfstring serializableClass = serializableData.itemClass();
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
zfstring requireItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredClass
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *errorPos /* = zfnull */
        ) {
    zfstring ret = checkItemClass(serializableData, desiredClass);
    if(ret == zfnull) {
        if(desiredClass == zfnull || *desiredClass == '\0') {
            ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, serializableData,
                "missing serializable class");
        }
        else {
            ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, serializableData,
                "serializable class must be \"%s\"", desiredClass);
        }
    }
    return ret;
}

zfstring checkAttr(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredAttr
        ) {
    zfiter it = serializableData.attrIterFind(desiredAttr);
    if(!it) {
        return zfnull;
    }
    serializableData.attrIterResolveMark(it);
    return serializableData.attrIterValue(it);
}
zfstring requireAttr(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredAttr
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *errorPos /* = zfnull */
        ) {
    zfstring ret = checkAttr(serializableData, desiredAttr);
    if(ret == zfnull) {
        ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, serializableData,
            "missing attribute \"%s\"", desiredAttr);
    }
    return ret;
}

ZFSerializableData checkElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementName
        ) {
    zfindex index = serializableData.childForName(desiredElementName);
    if(index == zfindexMax()) {
        return zfnull;
    }
    serializableData.childAt(index).resolvePropertyNameMark();
    return serializableData.childAt(index);
}
ZFSerializableData requireElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementName
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *errorPos /* = zfnull */
        ) {
    ZFSerializableData ret = checkElementByName(serializableData, desiredElementName);
    if(ret == zfnull) {
        ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, serializableData,
            "missing element with name \"%s\"",
            desiredElementName);
    }
    return ret;
}

ZFSerializableData checkElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementCategory
        ) {
    zfindex index = serializableData.childForCategory(desiredElementCategory);
    if(index == zfindexMax()) {
        return zfnull;
    }
    serializableData.childAt(index).resolveCategoryMark();
    return serializableData.childAt(index);
}
ZFSerializableData requireElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementCategory
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *errorPos /* = zfnull */
        ) {
    ZFSerializableData ret = checkElementByCategory(serializableData, desiredElementCategory);
    if(ret == zfnull) {
        ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, serializableData,
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

        zfbool hasUnresolvedAttr = zffalse;
        for(zfiter it = serializableData.attrIter(); it; ++it) {
            if(!serializableData.attrIterResolved(it)) {
                if(!hasUnresolvedAttr) {
                    hasUnresolvedAttr = zftrue;
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
        if(hasUnresolvedAttr) {
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
    zfstring tmp = "[ZFSerializable] not all resolved:\n";
    zfbool ret = _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData, ZFOutputForString(tmp), 1);
    if(ret) {
        outputCallback.execute(tmp.cString());
    }
    return ret;
}

ZF_NAMESPACE_END(ZFSerializableUtil)
ZF_NAMESPACE_GLOBAL_END

