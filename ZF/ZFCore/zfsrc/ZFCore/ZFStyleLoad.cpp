#include "ZFStyleLoad.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFStyleLoadItemBegin)
ZFEVENT_GLOBAL_REGISTER(ZFStyleLoadItemEnd)
ZFEVENT_GLOBAL_REGISTER(ZFStyleLoadItemError)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfclassNotPOD _ZFP_ZFStyleListPrivate {
public:
    ZFCoreArray<zfstring> keyList;
    ZFCoreArray<ZFStyleable *> valueList;
};

ZFMETHOD_DEFINE_0(ZFStyleList, zfindex, itemCount) {
    return d->keyList.count();
}
ZFMETHOD_DEFINE_1(ZFStyleList, const zfstring &, itemKeyAt
        , ZFMP_IN(zfindex, index)
        ) {
    return d->keyList.get(index);
}
ZFMETHOD_DEFINE_1(ZFStyleList, ZFStyleable *, itemValueAt
        , ZFMP_IN(zfindex, index)
        ) {
    return d->valueList.get(index);
}
ZFMETHOD_DEFINE_2(ZFStyleList, void, itemForKey
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFStyleable *, value)
        ) {
    for(zfindex i = 0; i < d->keyList.count(); ++i) {
        if(zfstringIsEqual(d->keyList[i], key)) {
            if(value == zfnull) {
                zfRelease(d->valueList.removeAndGet(i));
            }
            else {
                zfRetainChange(d->valueList[i], value);
            }
            return;
        }
    }

    if(value != zfnull) {
        d->keyList.add(key);
        d->valueList.add(zfRetain(value));
    }
}

void ZFStyleList::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFStyleListPrivate);
}
void ZFStyleList::objectOnDealloc(void) {
    if(!d->keyList.isEmpty()) {
        ZFCoreArray<zfstring> keyListTmp = d->keyList;
        d->keyList = ZFCoreArray<zfstring>();
        ZFCoreArray<ZFStyleable *> valueListTmp = d->valueList;
        d->valueList = ZFCoreArray<ZFStyleable *>();
        for(zfindex i = 0; i < keyListTmp.count(); ++i) {
            zfRelease(valueListTmp[i]);
        }
    }
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfbool ZFStyleList::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {
        return zffalse;
    }
    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &elementData = serializableData.childAt(i);
        if(elementData.resolved()) {
            continue;
        }
        zfstring key = elementData.propertyName();
        if(key == zfnull) {
            continue;
        }
        zfauto value;
        if(!ZFObjectFromDataT(value, elementData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(zfcast(ZFStyleable *, value) == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, elementData,
                "%s not type of %s",
                value,
                ZFStyleable::ClassData()->className());
            return zffalse;
        }
        this->itemForKey(key, value);
    }
    return zftrue;
}
zfbool ZFStyleList::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuper::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {
        return zffalse;
    }
    if(refOwner != zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "reference not supported");
        return zffalse;
    }
    for(zfindex i = 0; i < d->keyList.count(); ++i) {
        ZFSerializableData elementData;
        if(!ZFObjectToDataT(elementData, d->valueList[i]->toObject(), outErrorHint)) {
            return zffalse;
        }
        elementData.propertyName(d->keyList[i]);
        serializableData.child(elementData);
    }
    return zftrue;
}

// ============================================================
static void _ZFP_ZFStyleLoad_ZFStyleSet(
        ZF_IN const zfstring &styleKey
        , ZF_IN ZFStyleable *styleValue
        ) {
    if(styleValue != zfnull && styleValue->classData()->classIsTypeOf(ZFStyleList::ClassData())) {
        ZFStyleLoadItem(zfcast(ZFStyleList *, styleValue));
    }
    else {
        ZFStyleSet(styleKey, styleValue);
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFStyleLoadCheck
        , ZFMP_IN(const zfstring &, fileName)
        ) {
    if(fileName.isEmpty()
            || fileName[0] == '.'
            || fileName[0] == '_'
            ) {
        return zffalse;
    }
    zfindex dotPos = zfstringFindReversely(fileName, ".");
    if(dotPos != zfindexMax()
            && (dotPos > 0 && fileName[dotPos - 1] == '_')
            ) {
        return zffalse;
    }
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFStyleLoadItem
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const zfstring &, childPath, zfnull)
        ) {
    return ZFStyleLoadItem(ZFIOImplForPathType(pathInfo.pathType()), pathInfo.pathData(), childPath);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFStyleLoadItem
        , ZFMP_IN(ZFIOImpl *, ioImpl)
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN_OPT(const zfstring &, childPath, zfnull)
        ) {
    zfstring pathDataAbs;
    zfstring styleKey;
    if(childPath) {
        if(ioImpl->ioIsDir(pathData)) {
            if(!ioImpl->ioToChild(pathDataAbs, pathData, childPath)
                    || !ZFPathOfWithoutAllExtT(styleKey, childPath)
                    ) {
                return zffalse;
            }
        }
        else {
            if(!ioImpl->ioToParent(pathDataAbs, pathData)
                    || !ioImpl->ioToChild(pathDataAbs, pathDataAbs, childPath)
                    || !ZFPathOfWithoutAllExtT(styleKey, childPath)
                    ) {
                return zffalse;
            }
        }
    }
    else {
        if(!ioImpl->ioToFileName(styleKey, pathData)
                || !ZFPathOfWithoutAllExtT(styleKey, styleKey)
                ) {
            return zffalse;
        }
        pathDataAbs = pathData;
    }
    zfobj<v_ZFInput> input(ZFInputForIOToken(ioImpl->ioOpen(pathDataAbs, v_ZFIOOpenOption::e_Read)));
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFStyleLoadItemBegin(), input);
    zfauto styleValue;
    zfstring errorHint;
    if(!ZFObjectIOLoadT(styleValue, input->zfv, &errorHint)) {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFStyleLoadItemError(), input, zfobj<v_zfstring>(errorHint));
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFStyleLoadItemEnd(), input, zfnull);
        return zffalse;
    }
    ZFStyleUpdateBlock();
    _ZFP_ZFStyleLoad_ZFStyleSet(styleKey, styleValue);
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFStyleLoadItemEnd(), input, styleValue);
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFStyleLoadItem
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        ) {
    zfbool allSuccess = zftrue;
    if(serializableData.childCount() > 0) {
        ZFStyleUpdateBlock();
        for(zfindex i = 0; i < serializableData.childCount(); ++i) {
            const ZFSerializableData &child = serializableData.childAt(i);
            zfstring styleKey = ZFSerializableUtil::checkPropertyName(child);
            if(styleKey != zfnull) {
                zfauto styleValueHolder;
                zfstring errorHint;
                if(!ZFObjectFromDataT(styleValueHolder, child, &errorHint)) {
                    allSuccess = zffalse;
                    ZFGlobalObserver().observerNotify(
                            ZFGlobalEvent::E_ZFStyleLoadItemError()
                            , zfobj<v_ZFSerializableData>(child)
                            , zfobj<v_zfstring>(errorHint)
                            );
                    continue;
                }
                ZFStyleable *styleValue = styleValueHolder;
                if(styleValue != zfnull) {
                    _ZFP_ZFStyleLoad_ZFStyleSet(styleKey, styleValue);
                }
            }
        }
    }
    return allSuccess;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFStyleLoadItem
        , ZFMP_IN(ZFStyleList *, styleList)
        ) {
    if(styleList != zfnull && styleList->itemCount() > 0) {
        ZFStyleUpdateBlock();
        for(zfindex i = 0; i < styleList->itemCount(); ++i) {
            _ZFP_ZFStyleLoad_ZFStyleSet(styleList->itemKeyAt(i), styleList->itemValueAt(i));
        }
    }
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFStyleLoad
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(!ioImpl) {
        return zffalse;
    }
    ZFStyleUpdateBlock();
    zfstring pathDataBase = pathInfo.pathData();
    zfbool allSuccess = zftrue;
    ZFLISTENER_3(impl
            , zfautoT<ZFIOImpl>, ioImpl
            , zfstring, pathDataBase
            , zfbool &, allSuccess
            ) {
        zfstring fileName;
        const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
        if(!ioImpl->ioToFileName(fileName, pathInfo.pathData())) {
            return;
        }
        if(!ZFStyleLoadCheck(fileName)) {
            zfargs.param0(zfnull);
            return;
        }
        if(ioImpl->ioIsDir(pathInfo.pathData())) {
            return;
        }
        if(!ZFStyleLoadItem(ioImpl, pathDataBase, zfargs.param1().to<v_zfstring *>()->zfv)) {
            allSuccess = zffalse;
        }
    } ZFLISTENER_END()
    ZFIOForEach(pathInfo, impl);
    return allSuccess;
}

ZF_NAMESPACE_GLOBAL_END

