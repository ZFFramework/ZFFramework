#include "ZFTextTemplateType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFTextTemplateIndexFlag)

// ============================================================
void ZFTextTemplateIndexData::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += ZFTOKEN_ZFObjectInfoLeft;

    zfstringAppend(ret, "%%%s[%s%s]",
        this->indexWidth,
        this->indexRadix,
        this->indexUpperCase ? "x" : "X");

    if(this->indexOffset >= 0) {
        zfstringAppend(ret, "[%s, %s)", this->indexStart, this->indexOffset);
    }
    else {
        zfstringAppend(ret, "(%s, %s]", this->indexOffset, this->indexStart);
    }

    ret += " ";
    ZFTextTemplateIndexFlagToStringT(ret, this->indexFlag);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTextTemplateIndexData, ZFTextTemplateIndexData, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFTextTemplateIndexData(), outErrorHint, outErrorPos) == zfnull) {
            return zffalse;
        }

        v.indexStart = 0;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, zfindex, v.indexStart, {
                    return zffalse;
                });

        v.indexOffset = 1;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, zfint, v.indexOffset, {
                    return zffalse;
                });

        v.indexRadix = 10;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, zfindex, v.indexRadix, {
                    return zffalse;
                });

        v.indexUpperCase = zftrue;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, zfbool, v.indexUpperCase, {
                    return zffalse;
                });

        v.indexWidth = 0;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, zfindex, v.indexWidth, {
                    return zffalse;
                });

        v.indexFlag = v_ZFTextTemplateIndexFlag::EnumDefault();
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, ZFTextTemplateIndexFlag, v.indexFlag, {
                    return zffalse;
                });

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_ZFTextTemplateIndexData());

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, zfindex, v.indexStart, 0, {
                    return zffalse;
                });

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, zfint, v.indexOffset, 1, {
                    return zffalse;
                });

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, zfindex, v.indexRadix, 10, {
                    return zffalse;
                });

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, zfbool, v.indexUpperCase, zftrue, {
                    return zffalse;
                });

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, zfindex, v.indexWidth, 0, {
                    return zffalse;
                });

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, ZFTextTemplateIndexFlag, v.indexFlag, v_ZFTextTemplateIndexFlag::EnumDefault(), {
                    return zffalse;
                });

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfindex, indexStart)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfint, indexOffset)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfindex, indexRadix)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfbool, indexUpperCase)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfindex, indexWidth)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, ZFTextTemplateIndexFlag, indexFlag)

// ============================================================
zfclassNotPOD _ZFP_ZFTextTemplateReplaceData {
public:
    zfstring key;
    zfstring value;
};
zfclassNotPOD _ZFP_ZFTextTemplateEnableData {
public:
    zfstring key;
    zfbool value;
};
zfclassNotPOD _ZFP_ZFTextTemplateIndexData {
public:
    zfstring key;
    ZFTextTemplateIndexData value;
};
zfclassNotPOD _ZFP_ZFTextTemplateParamPrivate {
public:
    zfuint refCount;

    ZFCoreArray<_ZFP_ZFTextTemplateReplaceData *> replaceDataList;
    ZFCoreMap replaceDataMap; // map to _ZFP_ZFTextTemplateReplaceData *

    zfbool enableDataDefault;
    ZFCoreArray<_ZFP_ZFTextTemplateEnableData *> enableDataList;
    ZFCoreMap enableDataMap; // map to _ZFP_ZFTextTemplateEnableData *

    ZFTextTemplateIndexData indexDataDefault;
    ZFCoreArray<_ZFP_ZFTextTemplateIndexData *> indexDataList;
    ZFCoreMap indexDataMap; // map to _ZFP_ZFTextTemplateIndexData *

public:
    _ZFP_ZFTextTemplateParamPrivate(void)
    : refCount(1)
    , replaceDataList()
    , replaceDataMap()
    , enableDataDefault(zffalse)
    , enableDataList()
    , enableDataMap()
    , indexDataDefault()
    , indexDataList()
    , indexDataMap()
    {
    }
};
// ============================================================
ZFTextTemplateParam::ZFTextTemplateParam(void)
: d(zfpoolNew(_ZFP_ZFTextTemplateParamPrivate))
{
}
ZFTextTemplateParam::ZFTextTemplateParam(ZF_IN const ZFTextTemplateParam &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFTextTemplateParam::~ZFTextTemplateParam(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
        d = zfnull;
    }
}
ZFTextTemplateParam &ZFTextTemplateParam::operator = (ZF_IN const ZFTextTemplateParam &ref) {
    if(d != ref.d) {
        _ZFP_ZFTextTemplateParamPrivate *t = d;
        d = ref.d;
        ++(d->refCount);
        --(t->refCount);
        if(t->refCount == 0) {
            zfpoolDelete(t);
        }
    }
    return *this;
}

// ============================================================
void ZFTextTemplateParam::replaceData(
        ZF_IN const zfstring &key
        , ZF_IN const zfstring &value
        ) {
    if(key == zfnull) {
        return;
    }
    zfiter it = d->replaceDataMap.iterFind(key);
    if(it) {
        _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataMap.iterValue<_ZFP_ZFTextTemplateReplaceData *>(it);
        if(value == zfnull) {
            d->replaceDataList.removeElement(replaceData);
            d->replaceDataMap.iterRemove(it);
        }
        else {
            replaceData->value = value;
        }
    }
    else {
        if(value != zfnull) {
            _ZFP_ZFTextTemplateReplaceData *replaceData = zfpoolNew(_ZFP_ZFTextTemplateReplaceData);
            replaceData->key = key;
            replaceData->value = value;
            d->replaceDataList.add(replaceData);
            d->replaceDataMap.set(key, ZFCorePointerForPoolObject<_ZFP_ZFTextTemplateReplaceData *>(replaceData));
        }
    }
}
const zfstring &ZFTextTemplateParam::replaceData(ZF_IN const zfstring &key) const {
    _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataMap.get<_ZFP_ZFTextTemplateReplaceData *>(key);
    if(replaceData != zfnull) {
        return replaceData->value;
    }
    else {
        return zfstring::Empty();
    }
}
zfindex ZFTextTemplateParam::replaceDataCount(void) const {
    return d->replaceDataMap.count();
}
const zfstring &ZFTextTemplateParam::replaceDataNameAt(ZF_IN zfindex index) const {
    return d->replaceDataList.get(index)->key;
}
const zfstring &ZFTextTemplateParam::replaceDataAt(ZF_IN zfindex index) const {
    return d->replaceDataList.get(index)->value;
}
void ZFTextTemplateParam::replaceDataRemove(ZF_IN const zfstring &key) {
    zfiter it = d->replaceDataMap.iterFind(key);
    if(it) {
        d->replaceDataList.removeElement(d->replaceDataMap.iterValue<_ZFP_ZFTextTemplateReplaceData *>(it));
        d->replaceDataMap.iterRemove(it);
    }
}
void ZFTextTemplateParam::replaceDataRemoveAt(ZF_IN zfindex index) {
    zfstring key = d->replaceDataList.get(index)->key;
    d->replaceDataList.remove(index);
    d->replaceDataMap.remove(key);
}
void ZFTextTemplateParam::replaceDataRemoveAll(void) {
    d->replaceDataList.removeAll();
    d->replaceDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::enableDataDefault(ZF_IN zfbool enableDataDefault) {
    d->enableDataDefault = enableDataDefault;
}
zfbool ZFTextTemplateParam::enableDataDefault(void) const {
    return d->enableDataDefault;
}

void ZFTextTemplateParam::enableData(
        ZF_IN const zfstring &key
        , ZF_IN zfbool value
        ) {
    if(key == zfnull) {
        return;
    }
    zfiter it = d->enableDataMap.iterFind(key);
    if(it) {
        _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataMap.iterValue<_ZFP_ZFTextTemplateEnableData *>(it);
        enableData->value = value;
    }
    else {
        _ZFP_ZFTextTemplateEnableData *enableData = zfpoolNew(_ZFP_ZFTextTemplateEnableData);
        enableData->key = key;
        enableData->value = value;
        d->enableDataList.add(enableData);
        d->enableDataMap.set(key, ZFCorePointerForPoolObject<_ZFP_ZFTextTemplateEnableData *>(enableData));
    }
}
const zfbool *ZFTextTemplateParam::enableData(ZF_IN const zfstring &key) const {
    _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataMap.get<_ZFP_ZFTextTemplateEnableData *>(key);
    if(enableData != zfnull) {
        return &(enableData->value);
    }
    else {
        return zfnull;
    }
}
zfbool ZFTextTemplateParam::enableDataValue(ZF_IN const zfstring &key) const {
    const zfbool *value = this->enableData(key);
    if(value == zfnull) {
        return this->enableDataDefault();
    }
    else {
        return *value;
    }
}
zfindex ZFTextTemplateParam::enableDataCount(void) const {
    return d->enableDataMap.count();
}
const zfstring &ZFTextTemplateParam::enableDataNameAt(ZF_IN zfindex index) const {
    return d->enableDataList.get(index)->key;
}
zfbool ZFTextTemplateParam::enableDataAt(ZF_IN zfindex index) const {
    return d->enableDataList.get(index)->value;
}
void ZFTextTemplateParam::enableDataRemove(ZF_IN const zfstring &key) {
    zfiter it = d->enableDataMap.iterFind(key);
    if(it) {
        d->enableDataList.removeElement(d->enableDataMap.iterValue<_ZFP_ZFTextTemplateEnableData *>(it));
        d->enableDataMap.iterRemove(it);
    }
}
void ZFTextTemplateParam::enableDataRemoveAt(ZF_IN zfindex index) {
    zfstring key = d->enableDataList.get(index)->key;
    d->enableDataList.remove(index);
    d->enableDataMap.remove(key);
}
void ZFTextTemplateParam::enableDataRemoveAll(void) {
    d->enableDataList.removeAll();
    d->enableDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::indexDataDefault(ZF_IN const ZFTextTemplateIndexData &indexDataDefault) {
    d->indexDataDefault = indexDataDefault;
}
const ZFTextTemplateIndexData &ZFTextTemplateParam::indexDataDefault(void) const {
    return d->indexDataDefault;
}

void ZFTextTemplateParam::indexData(
        ZF_IN const zfstring &key
        , ZF_IN const ZFTextTemplateIndexData &value
        ) {
    if(key == zfnull) {
        return;
    }
    zfiter it = d->indexDataMap.iterFind(key);
    if(it) {
        _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.iterValue<_ZFP_ZFTextTemplateIndexData *>(it);
        indexData->value = value;
    }
    else {
        _ZFP_ZFTextTemplateIndexData *indexData = zfpoolNew(_ZFP_ZFTextTemplateIndexData);
        indexData->key = key;
        indexData->value = value;
        d->indexDataList.add(indexData);
        d->indexDataMap.set(key, ZFCorePointerForPoolObject<_ZFP_ZFTextTemplateIndexData *>(indexData));
    }
}
const ZFTextTemplateIndexData *ZFTextTemplateParam::indexData(ZF_IN const zfstring &key) const {
    _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.get<_ZFP_ZFTextTemplateIndexData *>(key);
    if(indexData != zfnull) {
        return &(indexData->value);
    }
    else {
        return zfnull;
    }
}
ZFTextTemplateIndexData *ZFTextTemplateParam::indexData(ZF_IN const zfstring &key) {
    _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.get<_ZFP_ZFTextTemplateIndexData *>(key);
    if(indexData != zfnull) {
        return &(indexData->value);
    }
    else {
        return zfnull;
    }
}
zfindex ZFTextTemplateParam::indexDataCount(void) const {
    return d->indexDataMap.count();
}
const zfstring &ZFTextTemplateParam::indexDataNameAt(ZF_IN zfindex index) const {
    return d->indexDataList.get(index)->key;
}
const ZFTextTemplateIndexData *ZFTextTemplateParam::indexDataAt(ZF_IN zfindex index) const {
    return &(d->indexDataList.get(index)->value);
}
ZFTextTemplateIndexData *ZFTextTemplateParam::indexDataAt(ZF_IN zfindex index) {
    return &(d->indexDataList.get(index)->value);
}
void ZFTextTemplateParam::indexDataRemove(ZF_IN const zfstring &key) {
    zfiter it = d->indexDataMap.iterFind(key);
    if(it) {
        d->indexDataList.removeElement(d->indexDataMap.iterValue<_ZFP_ZFTextTemplateIndexData *>(it));
        d->indexDataMap.iterRemove(it);
    }
}
void ZFTextTemplateParam::indexDataRemoveAt(ZF_IN zfindex index) {
    zfstring key = d->indexDataList.get(index)->key;
    d->indexDataList.remove(index);
    d->indexDataMap.remove(key);
}
void ZFTextTemplateParam::indexDataRemoveAll(void) {
    d->indexDataList.removeAll();
    d->indexDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::copyFrom(ZF_IN const ZFTextTemplateParam &ref) {
    if(d == ref.d) {
        return;
    }

    d->replaceDataList.removeAll();
    d->replaceDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->replaceDataList.count(); ++i) {
        const _ZFP_ZFTextTemplateReplaceData *replaceDataRef = ref.d->replaceDataList[i];
        _ZFP_ZFTextTemplateReplaceData *replaceData = zfpoolNew(_ZFP_ZFTextTemplateReplaceData);
        replaceData->key = replaceDataRef->key;
        replaceData->value = replaceDataRef->value;
        d->replaceDataList.add(replaceData);
        d->replaceDataMap.set(replaceData->key, ZFCorePointerForPoolObject<_ZFP_ZFTextTemplateReplaceData *>(replaceData));
    }

    d->enableDataDefault = ref.d->enableDataDefault;

    d->enableDataList.removeAll();
    d->enableDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->enableDataList.count(); ++i) {
        const _ZFP_ZFTextTemplateEnableData *enableDataRef = ref.d->enableDataList[i];
        _ZFP_ZFTextTemplateEnableData *enableData = zfpoolNew(_ZFP_ZFTextTemplateEnableData);
        enableData->key = enableDataRef->key;
        enableData->value = enableDataRef->value;
        d->enableDataList.add(enableData);
        d->enableDataMap.set(enableData->key, ZFCorePointerForPoolObject<_ZFP_ZFTextTemplateEnableData *>(enableData));
    }

    d->indexDataDefault.copyFrom(ref.d->indexDataDefault);

    d->indexDataList.removeAll();
    d->indexDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->indexDataList.count(); ++i) {
        const _ZFP_ZFTextTemplateIndexData *indexDataRef = ref.d->indexDataList[i];
        _ZFP_ZFTextTemplateIndexData *indexData = zfpoolNew(_ZFP_ZFTextTemplateIndexData);
        indexData->key = indexDataRef->key;
        indexData->value.copyFrom(indexDataRef->value);
        d->indexDataList.add(indexData);
        d->indexDataMap.set(indexData->key, ZFCorePointerForPoolObject<_ZFP_ZFTextTemplateIndexData *>(indexData));
    }
}

zfbool ZFTextTemplateParam::operator == (ZF_IN const ZFTextTemplateParam &ref) const {
    if(d == ref.d) {
        return zftrue;
    }
    if(d->replaceDataList.count() != ref.d->replaceDataList.count()
            || d->enableDataDefault != ref.d->enableDataDefault
            || d->enableDataList.count() != ref.d->enableDataList.count()
            || d->indexDataDefault != ref.d->indexDataDefault
            || d->indexDataList.count() != ref.d->indexDataList.count()
            ) {
        return zffalse;
    }
    for(zfindex i = 0; i < d->replaceDataList.count(); ++i) {
        const _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataList[i];
        const _ZFP_ZFTextTemplateReplaceData *replaceDataRef = ref.d->replaceDataList[i];
        if(replaceData->key != replaceDataRef->key || replaceData->value != replaceDataRef->value) {
            return zffalse;
        }
    }
    for(zfindex i = 0; i < d->enableDataList.count(); ++i) {
        const _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataList[i];
        const _ZFP_ZFTextTemplateEnableData *enableDataRef = ref.d->enableDataList[i];
        if(enableData->key != enableDataRef->key || enableData->value != enableDataRef->value) {
            return zffalse;
        }
    }
    for(zfindex i = 0; i < d->indexDataList.count(); ++i) {
        const _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataList[i];
        const _ZFP_ZFTextTemplateIndexData *indexDataRef = ref.d->indexDataList[i];
        if(indexData->key != indexDataRef->key || indexData->value != indexDataRef->value) {
            return zffalse;
        }
    }
    return zftrue;
}

void ZFTextTemplateParam::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfbool first = zftrue;

    if(!d->replaceDataList.isEmpty()) {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "replace: [";
        for(zfindex i = 0; i < d->replaceDataList.count(); ++i) {
            if(i > 0) {
                ret += ", ";
            }
            const _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataList[i];
            zfstringAppend(ret, "<%s, %s>", replaceData->key, replaceData->value);
        }
        ret += "]";
    }

    if(!d->enableDataList.isEmpty()) {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "enable: [";
        for(zfindex i = 0; i < d->enableDataList.count(); ++i) {
            if(i > 0) {
                ret += ", ";
            }
            const _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataList[i];
            zfstringAppend(ret, "<%s, %s>", enableData->key, enableData->value);
        }
        ret += "]";
    }

    {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "enableDataDefault: ";
        zfboolToStringT(ret, this->enableDataDefault());
    }

    if(!d->indexDataList.isEmpty()) {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "index: ";
        for(zfindex i = 0; i < d->indexDataList.count(); ++i) {
            if(i > 0) {
                ret += ", ";
            }
            const _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataList[i];
            ret += "<";
            ret += indexData->key;
            ret += ", ";
            indexData->value.objectInfoT(ret);
            ret += ">";
        }
        ret += "]";
    }

    {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "indexDataDefault: ";
        d->indexDataDefault.objectInfoT(ret);
    }

    ret += ZFTOKEN_ZFObjectInfoRight;

    ZFUNUSED(first);
}

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTextTemplateParam, ZFTextTemplateParam, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFTextTemplateParam(), outErrorHint, outErrorPos) == zfnull) {
            return zffalse;
        }

        ZFSerializableData element;

        v.replaceDataRemoveAll();
        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_replace);
        if(element != zfnull) {
            for(zfindex i = 0; i < element.childCount(); ++i) {
                const ZFSerializableData &item = element.childAt(i);
                zfstring key = item.propertyName();
                if(key == zfnull) {
                    ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, item, "missing item name");
                    return zffalse;
                }
                item.resolvePropertyNameMark();
                zfstring value;
                if(!zfstringFromDataT(value, item, outErrorHint, outErrorPos)) {
                    return zffalse;
                }
                v.replaceData(key, value);
            }
            element.resolveMark();
        }

        v.enableDataRemoveAll();
        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_enable);
        if(element != zfnull) {
            for(zfindex i = 0; i < element.childCount(); ++i) {
                const ZFSerializableData &item = element.childAt(i);
                zfstring key = item.propertyName();
                if(key == zfnull) {
                    ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, item, "missing item name");
                    return zffalse;
                }
                item.resolvePropertyNameMark();
                zfbool value = zffalse;
                if(!zfboolFromDataT(value, item, outErrorHint, outErrorPos)) {
                    return zffalse;
                }
                v.enableData(key, value);
            }
            element.resolveMark();
        }

        zfbool enableDataDefault = zffalse;
        ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault, zfbool, enableDataDefault, {
                    return zffalse;
                });
        v.enableDataDefault(enableDataDefault);

        v.indexDataRemoveAll();
        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_index);
        if(element != zfnull) {
            for(zfindex i = 0; i < element.childCount(); ++i) {
                const ZFSerializableData &item = element.childAt(i);
                zfstring key = item.propertyName();
                if(key == zfnull) {
                    ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, item, "missing item name");
                    return zffalse;
                }
                item.resolvePropertyNameMark();
                ZFTextTemplateIndexData value;
                if(!ZFTextTemplateIndexDataFromDataT(value, item, outErrorHint, outErrorPos)) {
                    return zffalse;
                }
                v.indexData(key, value);
            }
            element.resolveMark();
        }

        ZFTextTemplateIndexData indexDataDefault;
        ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault, ZFTextTemplateIndexData, indexDataDefault, {
                    return zffalse;
                });
        v.indexDataDefault(indexDataDefault);

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_ZFTextTemplateParam());

        if(v.replaceDataCount() > 0) {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.replaceDataCount(); ++i) {
                ZFSerializableData itemData;
                if(!zfstringToDataT(itemData, v.replaceDataAt(i), outErrorHint)) {
                    return zffalse;
                }
                itemData.propertyName(v.replaceDataNameAt(i));
                nodeData.child(itemData);
            }

            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_replace);
            serializableData.child(nodeData);
        }

        if(v.enableDataCount() > 0) {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.enableDataCount(); ++i) {
                ZFSerializableData itemData;
                if(!zfboolToDataT(itemData, v.enableDataAt(i), outErrorHint)) {
                    return zffalse;
                }
                itemData.propertyName(v.enableDataNameAt(i));
                nodeData.child(itemData);
            }

            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_enable);
            serializableData.child(nodeData);
        }

        ZFSerializableUtilSerializeCategoryToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault, zfbool, v.enableDataDefault(), zffalse, {
                    return zffalse;
                });

        if(v.indexDataCount() > 0) {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.indexDataCount(); ++i) {
                ZFSerializableData itemData;
                if(!ZFTextTemplateIndexDataToDataT(itemData, *(v.indexDataAt(i)), outErrorHint)) {
                    return zffalse;
                }
                itemData.propertyName(v.indexDataNameAt(i));
                nodeData.child(itemData);
            }

            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_index);
            serializableData.child(nodeData);
        }

        ZFSerializableUtilSerializeCategoryToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault, ZFTextTemplateIndexData, v.indexDataDefault(), ZFTextTemplateIndexData(), {
                    return zffalse;
                });

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFTextTemplateParam, void, replaceData
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfstring &, replaceData
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfindex, replaceDataCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfstring &, replaceDataNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfstring &, replaceDataAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, replaceDataRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, replaceDataRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, void, replaceDataRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, enableDataDefault
        , ZFMP_IN(zfbool, enableDataDefault)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfbool, enableDataDefault)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFTextTemplateParam, void, enableData
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(zfbool, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfbool *, enableData
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, zfbool, enableDataValue
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfindex, enableDataCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfstring &, enableDataNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, zfbool, enableDataAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, enableDataRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, enableDataRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, void, enableDataRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, indexDataDefault
        , ZFMP_IN(const ZFTextTemplateIndexData &, indexDataDefault)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, const ZFTextTemplateIndexData &, indexDataDefault)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFTextTemplateParam, void, indexData
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const ZFTextTemplateIndexData &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const ZFTextTemplateIndexData *, indexData
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfindex, indexDataCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfstring &, indexDataNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const ZFTextTemplateIndexData *, indexDataAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, indexDataRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, indexDataRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, void, indexDataRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, copyFrom
        , ZFMP_IN(const ZFTextTemplateParam &, ref)
        )

ZF_NAMESPACE_GLOBAL_END

