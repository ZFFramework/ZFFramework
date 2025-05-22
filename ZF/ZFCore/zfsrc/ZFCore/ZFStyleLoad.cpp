#include "ZFStyleLoad.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFStyleLoadErrorCallbackForConsole(ZF_IN const ZFArgs &zfargs) {
    ZFStyleLoadErrorHint(ZFOutputDefault(), zfargs);
}

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFListener, ZFStyleLoadErrorCallbackForConsole)
ZFEXPORT_VAR_DEFINE(ZFListener, ZFStyleLoadErrorCallbackDefault, ZFStyleLoadErrorCallbackForConsole())
ZFMETHOD_FUNC_DEFINE_2(void, ZFStyleLoadErrorHint
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFArgs &, zfargs)
        ) {
    v_ZFSerializableData *pos = zfargs.param0();
    v_zfstring *errorHint = zfargs.param1();

    output << "style load failed: ";

    if(pos != zfnull) {
        output << ZFSerializableDataToString(pos->zfv);
    }
    else {
        output << zfargs.param0();
    }

    output << ", reason: " << errorHint << "\n";
}

// ============================================================
static void _ZFP_ZFStyleLoad_ZFStyleSet(
        ZF_IN const zfstring &styleKey
        , ZF_IN ZFStyleable *styleValue
        ) {
    if(styleValue != zfnull && styleValue->classData()->classIsTypeOf(ZFStyleList::ClassData())) {
        ZFStyleLoad(zfcast(ZFStyleList *, styleValue));
    }
    else {
        ZFStyleSet(styleKey, styleValue);
    }
}
static void _ZFP_ZFStyleLoadImpl(
        ZF_IN const ZFPathInfoImpl &fileImpl
        , ZF_IN const zfstring &pathType
        , ZF_IN const zfstring &pathData
        , ZF_IN const zfstring &relativePath
        , ZF_IN_OUT zfbool &allSuccess
        , ZF_IN const ZFListener &errorCallback
        ) {
    ZFFileFindData fd;
    if(fileImpl.implFindFirst(fd, pathData)) {
        do {
            if(*fd.name() == '.' || *fd.name() == '_') {
                continue;
            }
            zfindex dotPos = zfstringFindReversely(fd.name(), '.');
            if(dotPos != zfindexMax()
                    && (dotPos > 0 && fd.name()[dotPos - 1] == '_')
                    ) {
                continue;
            }

            zfstring relativePathTmp = relativePath;
            if(!relativePathTmp.isEmpty()) {
                relativePathTmp += '/';
            }
            relativePathTmp += fd.name();

            zfstring pathDataChild;
            fileImpl.implToChild(pathDataChild, pathData, fd.name());
            if(fd.isDir()) {
                _ZFP_ZFStyleLoadImpl(fileImpl, pathType, pathDataChild, relativePathTmp, allSuccess, errorCallback);
            }
            else {
                zfauto styleValue;
                zfstring errorHint;
                if(!ZFObjectIOLoadT(styleValue, ZFInputForPathInfo(ZFPathInfo(pathType, pathDataChild)), &errorHint)) {
                    allSuccess = zffalse;
                    errorCallback.execute(ZFArgs()
                            .param0(zfobj<v_ZFPathInfo>(ZFPathInfo(pathType, pathDataChild)))
                            .param1(zfobj<v_zfstring>(errorHint))
                            );
                    continue;
                }
                ZFPathOfWithoutExtT(relativePathTmp, relativePathTmp);
                _ZFP_ZFStyleLoad_ZFStyleSet(relativePathTmp, styleValue);
            }
        } while(fileImpl.implFindNext(fd));
        fileImpl.implFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFStyleLoad
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, errorCallback, ZFStyleLoadErrorCallbackDefault())
        ) {
    const ZFPathInfoImpl *fileImpl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(fileImpl == zfnull) {
        return zffalse;
    }
    ZFStyleUpdateBlock();

    if(!fileImpl->implIsDir(pathInfo.pathData())) {
        zfstring fileName;
        if(!fileImpl->implToFileName(fileName, pathInfo.pathData())
                || !fileName
                ) {
            return zffalse;
        }
        zfauto styleValue;
        zfstring errorHint;
        if(!ZFObjectIOLoadT(styleValue, ZFInputForPathInfo(pathInfo), &errorHint)) {
            errorCallback.execute(ZFArgs()
                    .param0(zfobj<v_ZFPathInfo>(pathInfo))
                    .param1(zfobj<v_zfstring>(errorHint))
                    );
            return zffalse;
        }
        ZFPathOfWithoutExtT(fileName, fileName);
        _ZFP_ZFStyleLoad_ZFStyleSet(fileName, styleValue);
        return zftrue;
    }

    zfbool allSuccess = zftrue;
    _ZFP_ZFStyleLoadImpl(*fileImpl, pathInfo.pathType(), pathInfo.pathData(), zfnull, allSuccess, errorCallback);
    return allSuccess;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFStyleLoad
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_IN_OPT(const ZFListener &, errorCallback, ZFStyleLoadErrorCallbackDefault())
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
                    errorCallback.execute(ZFArgs()
                            .param0(zfobj<v_ZFSerializableData>(child))
                            .param1(zfobj<v_zfstring>(errorHint))
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
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFStyleLoad
        , ZFMP_IN(ZFStyleList *, styleList)
        , ZFMP_IN_OPT(const ZFListener &, errorCallback, ZFStyleLoadErrorCallbackDefault())
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

ZF_NAMESPACE_GLOBAL_END

