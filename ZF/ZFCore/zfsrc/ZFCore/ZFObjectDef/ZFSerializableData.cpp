#include "ZFSerializableData.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableUtil.h"
#include "ZFSerializableDataSerializableConverter.h"

#include "ZFCore/ZFSTLWrapper/zfstldeque.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFSerializableDataResolveCheckEnable = zftrue;

// ============================================================
// _ZFP_ZFSerializableDataPrivate
zfclassNotPOD _ZFP_ZFSerializableDataAttributeData {
public:
    zfstring attrValue;
    zfbool resolved;
public:
    _ZFP_ZFSerializableDataAttributeData(void)
    : attrValue()
    , resolved(zffalse)
    {
    }
    _ZFP_ZFSerializableDataAttributeData(
            ZF_IN const zfchar *attrValue
            , ZF_IN_OPT zfbool resolved = zffalse
            )
    : attrValue(attrValue)
    , resolved(resolved)
    {
    }
};
typedef zfimplmap<zfstring, _ZFP_ZFSerializableDataAttributeData> _ZFP_ZFSerializableDataAttributeMapType;
typedef zfstlmap<zfstring, zfauto> _ZFP_ZFSerializableDataTagMapType;
zfclassNotPOD _ZFP_ZFSerializableDataPrivate {
public:
    zfuint refCount;
    _ZFP_ZFSerializableDataPrivate *serializableDataParent;
    zfstring classNameFull;
    zfbool resolved;
    ZFPathInfo *pathInfo;
    _ZFP_ZFSerializableDataAttributeMapType attributes;
    zfstldeque<ZFSerializableData> elements;
    _ZFP_ZFSerializableDataTagMapType serializableDataTagMap;

public:
    void removeAll(void) {
        this->classNameFull.removeAll();
        this->attributes.clear();
        if(!this->elements.empty()) {
            for(zfstlsize i = this->elements.size() - 1; i != (zfstlsize)-1; --i) {
                this->elements[i].d->serializableDataParent = zfnull;
            }
            this->elements.clear();
        }

        this->serializableDataTagMap.clear();
    }

public:
    _ZFP_ZFSerializableDataPrivate(void)
    : refCount(1)
    , serializableDataParent(zfnull)
    , classNameFull()
    , resolved(zffalse)
    , pathInfo(zfnull)
    , attributes()
    , elements()
    , serializableDataTagMap()
    {
    }
    ~_ZFP_ZFSerializableDataPrivate(void) {
        if(!this->elements.empty()) {
            for(zfstlsize i = 0; i < this->elements.size(); ++i) {
                this->elements[i].d->serializableDataParent = zfnull;
            }
        }

        zfdelete(this->pathInfo);
    }
};

// ============================================================
// ZFSerializableData
ZFSerializableData::ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d) {
    this->d = d;
    ++(d->refCount);
}
ZFSerializableData::ZFSerializableData(void) {
    d = zfnew(_ZFP_ZFSerializableDataPrivate);
}
ZFSerializableData::ZFSerializableData(ZF_IN const ZFSerializableData &ref) {
    d = ref.d;
    ++(d->refCount);
}
ZFSerializableData &ZFSerializableData::operator = (ZF_IN const ZFSerializableData &ref) {
    _ZFP_ZFSerializableDataPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0) {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFSerializableData::operator == (ZF_IN const ZFSerializableData &ref) const {
    return (d == ref.d);
}
ZFSerializableData::~ZFSerializableData(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}

void ZFSerializableData::copyFrom(ZF_IN const ZFSerializableData &ref) {
    if(d == ref.d) {
        return;
    }

    d->classNameFull = ref.d->classNameFull;
    d->attributes = ref.d->attributes;

    if(d->pathInfo == zfnull) {
        if(ref.d->pathInfo != zfnull) {
            d->pathInfo = zfnew(ZFPathInfo, *(ref.d->pathInfo));
        }
    }
    else {
        if(ref.d->pathInfo == zfnull) {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
        else {
            *(d->pathInfo) = *(ref.d->pathInfo);
        }
    }

    d->serializableDataTagMap = ref.d->serializableDataTagMap;

    this->childRemoveAll();
    for(zfstlsize i = 0; i < ref.d->elements.size(); ++i) {
        this->childAdd(ref.d->elements[i].copy());
    }
}

zfindex ZFSerializableData::objectRetainCount(void) const {
    return d->refCount;
}

// ============================================================
// local path logic
const ZFPathInfo *ZFSerializableData::pathInfo(void) const {
    return d->pathInfo;
}
void ZFSerializableData::pathInfo(ZF_IN const ZFPathInfo *pathInfo) {
    if(pathInfo != zfnull && !pathInfo->isEmpty()) {
        if(d->pathInfo == zfnull) {
            d->pathInfo = zfnew(ZFPathInfo, *pathInfo);
        }
        else {
            *(d->pathInfo) = *pathInfo;
        }
    }
    else {
        if(d->pathInfo != zfnull) {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
void ZFSerializableData::pathInfo(
        ZF_IN const zfchar *pathType
        , ZF_IN const zfchar *pathData
        ) {
    if(!zfstringIsEmpty(pathType) || !zfstringIsEmpty(pathData)) {
        if(d->pathInfo == zfnull) {
            d->pathInfo = zfnew(ZFPathInfo, pathType, pathData);
        }
        else {
            d->pathInfo->pathType = pathType;
            d->pathInfo->pathData = pathData;
        }
    }
    else {
        if(d->pathInfo != zfnull) {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
const ZFPathInfo *ZFSerializableData::pathInfoCheck(void) const {
    _ZFP_ZFSerializableDataPrivate *check = d;
    do {
        if(check->pathInfo != zfnull && !check->pathInfo->isEmpty()) {
            return check->pathInfo;
        }
        check = check->serializableDataParent;
    } while(check != zfnull);
    return zfnull;
}

// ============================================================
// parent
zfbool ZFSerializableData::serializableDataParent(ZF_OUT ZFSerializableData &ret) const {
    if(d->serializableDataParent != zfnull) {
        ret = ZFSerializableData(d->serializableDataParent);
        return zftrue;
    }
    else {
        return zffalse;
    }
}

// ============================================================
// class
void ZFSerializableData::itemClass(ZF_IN const zfchar *classNameFull) {
    if(classNameFull == zfnull) {
        d->classNameFull.removeAll();
    }
    else {
        d->classNameFull = classNameFull;
    }
}
const zfchar *ZFSerializableData::itemClass(void) const {
    return d->classNameFull.isEmpty() ? zfnull : d->classNameFull.cString();
}

// ============================================================
// other types
void ZFSerializableData::removeAll(void) {
    d->removeAll();
}

// ============================================================
void ZFSerializableData::serializableDataTag(
        ZF_IN const zfchar *key
        , ZF_IN ZFObject *tag
        ) {
    if(key == zfnull) {
        return;
    }

    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
    if(it == m.end()) {
        if(tag != zfnull) {
            m[key] = tag;
        }
    }
    else {
        if(tag == zfnull) {
            zfauto holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else {
            zfauto holder = it->second;
            ZFUNUSED(holder);
            it->second = tag;
        }
    }
}
zfany ZFSerializableData::serializableDataTag(ZF_IN const zfchar *key) const {
    if(key != zfnull) {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end()) {
            return it->second;
        }
    }
    return zfnull;
}
void ZFSerializableData::serializableDataTagGetAllKeyValue(
        ZF_IN_OUT ZFCoreArray<zfstring> &allKey
        , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
        ) const {
    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    allKey.capacity(allKey.count() + m.size());
    allValue.capacity(allValue.count() + m.size());
    for(_ZFP_ZFSerializableDataTagMapType::iterator it = m.begin(); it != m.end(); ++it) {
        allKey.add(it->first);
        allValue.add(it->second);
    }
}
void ZFSerializableData::serializableDataTagRemove(ZF_IN const zfchar *key) {
    if(key != zfnull) {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end()) {
            m.erase(it);
        }
    }
}
zfauto ZFSerializableData::serializableDataTagRemoveAndGet(ZF_IN const zfchar *key) {
    if(key != zfnull) {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end()) {
            zfauto ret = it->second;
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFSerializableData::serializableDataTagRemoveAll(void) {
    if(!d->serializableDataTagMap.empty()) {
        _ZFP_ZFSerializableDataTagMapType tmp;
        tmp.swap(d->serializableDataTagMap);
    }
}

// ============================================================
// attribute
void ZFSerializableData::attr(
        ZF_IN const zfchar *name
        , ZF_IN const zfchar *value
        ) {
    if(name != zfnull) {
        if(value != zfnull) {
            _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
            if(it != d->attributes.end()) {
                it->second = _ZFP_ZFSerializableDataAttributeData(value);
            }
            else {
                d->attributes[name] = _ZFP_ZFSerializableDataAttributeData(value);
            }
        }
        else {
            d->attributes.erase(name);
        }
    }
}
const zfchar *ZFSerializableData::attr(ZF_IN const zfchar *name) const {
    if(name != zfnull) {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end()) {
            return it->second.attrValue;
        }
    }
    return zfnull;
}

zfindex ZFSerializableData::attrCount(void) const {
    return (zfindex)d->attributes.size();
}
void ZFSerializableData::attrRemove(ZF_IN const zfchar *name) {
    if(name != zfnull) {
        d->attributes.erase(name);
    }
}
void ZFSerializableData::attrRemoveAll(void) {
    d->attributes.clear();
}

zfiterator ZFSerializableData::attrIterFind(ZF_IN const zfchar *name) const {
    if(name == zfnull) {
        return zfiterator();
    }
    return d->attributes.iterFind(name);
}
zfiterator ZFSerializableData::attrIter(void) const {
    return d->attributes.iter();
}
zfbool ZFSerializableData::attrIterValid(ZF_IN const zfiterator &it) const {
    return d->attributes.iterValid(it);
}
const zfchar *ZFSerializableData::attrIterKey(ZF_IN const zfiterator &it) const {
    return d->attributes.iterKey(it);
}
const zfchar *ZFSerializableData::attrIterValue(ZF_IN const zfiterator &it) const {
    return d->attributes.iterValue(it).attrValue;
}
void ZFSerializableData::attrIterValue(
        ZF_IN_OUT zfiterator &it
        , ZF_IN const zfchar *value
        ) {
    if(value == zfnull) {
        this->attrIterRemove(it);
        return;
    }
    d->attributes.iterValue(it, value);
}
void ZFSerializableData::attrIterRemove(ZF_IN_OUT zfiterator &it) {
    d->attributes.iterRemove(it);
}
void ZFSerializableData::attrIterNext(ZF_IN_OUT zfiterator &it) const {
    d->attributes.iterNext(it);
}

zfbool ZFSerializableData::attrIterResolved(ZF_IN const zfiterator &it) const {
    if(ZFSerializableDataResolveCheckEnable) {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull) {
            return (*data)->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::attrIterResolveMark(ZF_IN const zfiterator &it) const {
    if(ZFSerializableDataResolveCheckEnable) {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull) {
            (*data)->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::attrIterResolveUnmark(ZF_IN const zfiterator &it) const {
    if(ZFSerializableDataResolveCheckEnable) {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull) {
            (*data)->second.resolved = zftrue;
        }
    }
}

// ============================================================
// element
void ZFSerializableData::childAdd(ZF_IN const ZFSerializableData &element) {
    zfCoreAssertWithMessage(d != element.d, "adding self is not allowed");
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, "adding a data which already has parent");
    d->elements.push_back(element);
    element.d->serializableDataParent = d;
}
void ZFSerializableData::childAdd(
        ZF_IN const ZFSerializableData &element
        , ZF_IN zfindex atIndex
        ) {
    zfCoreAssertWithMessage(d != element.d, "adding self is not allowed");
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, "adding a data which already has parent");
    if(atIndex == zfindexMax()) {
        atIndex = (zfindex)d->elements.size();
    }
    if(atIndex > (zfindex)d->elements.size()) {
        zfCoreCriticalIndexOutOfRange(atIndex, (zfindex)(d->elements.size() + 1));
        return;
    }

    d->elements.insert(d->elements.begin() + atIndex, element);
    element.d->serializableDataParent = d;
}

void ZFSerializableData::childReplace(
        ZF_IN zfindex index
        , ZF_IN const ZFSerializableData &element
        ) {
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements[(zfstlsize)index] = element;
    element.d->serializableDataParent = d;
}

zfindex ZFSerializableData::childForName(ZF_IN const zfchar *name) const {
    if(name != zfnull && *name != '\0') {
        for(zfstlsize i = 0; i < d->elements.size(); ++i) {
            if(zfstringIsEqual(d->elements[i].propertyName(), name)) {
                return i;
            }
        }
    }
    return zfindexMax();
}
zfindex ZFSerializableData::childForCategory(ZF_IN const zfchar *category) const {
    if(category != zfnull && *category != '\0') {
        for(zfstlsize i = 0; i < d->elements.size(); ++i) {
            if(zfstringIsEqual(d->elements[i].category(), category)) {
                return i;
            }
        }
    }
    return zfindexMax();
}

zfindex ZFSerializableData::childCount(void) const {
    return (zfindex)d->elements.size();
}
const ZFSerializableData &ZFSerializableData::childAt(ZF_IN zfindex index) const {
    return d->elements[(zfstlsize)index];
}
ZFSerializableData &ZFSerializableData::childAt(ZF_IN zfindex index) {
    return d->elements[(zfstlsize)index];
}

void ZFSerializableData::childRemoveAt(ZF_IN zfindex index) {
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements.erase(d->elements.begin() + (zfstlsize)index);
}
void ZFSerializableData::childRemoveAll(void) {
    if(!d->elements.empty()) {
        for(zfstlsize i = 0; i < d->elements.size(); ++i) {
            d->elements[i].d->serializableDataParent = zfnull;
        }
        d->elements.clear();
    }
}

// ============================================================
// resolved state
zfbool ZFSerializableData::resolved(void) const {
    return ZFSerializableDataResolveCheckEnable && d->resolved;
}
void ZFSerializableData::resolveMark(void) const {
    if(ZFSerializableDataResolveCheckEnable) {
        d->resolved = zftrue;
    }
}
void ZFSerializableData::resolveUnmark(void) const {
    if(ZFSerializableDataResolveCheckEnable) {
        d->resolved = zffalse;
    }
}

zfbool ZFSerializableData::resolvedAttribute(ZF_IN const zfchar *name) const {
    if(ZFSerializableDataResolveCheckEnable && name != zfnull) {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end()) {
            return it->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::resolveAttributeMark(ZF_IN const zfchar *name) const {
    if(ZFSerializableDataResolveCheckEnable && name != zfnull) {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end()) {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttributeUnmark(ZF_IN const zfchar *name) const {
    if(ZFSerializableDataResolveCheckEnable && name != zfnull) {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end()) {
            it->second.resolved = zffalse;
        }
    }
}

zfbool ZFSerializableData::resolvedAll(
        ZF_OUT_OPT const ZFSerializableData **firstNotResolvedElement /* = zfnull */
        , ZF_OUT_OPT zfstring *firstNotResolvedAttribute /* = zfnull */
        ) const {
    if(!ZFSerializableDataResolveCheckEnable) {
        return zffalse;
    }
    if(!this->resolved()) {
        if(firstNotResolvedElement != zfnull) {
            *firstNotResolvedElement = this;
        }
        return zffalse;
    }
    for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it) {
        if(!it->second.resolved) {
            if(firstNotResolvedElement != zfnull) {
                *firstNotResolvedElement = this;
            }
            if(firstNotResolvedAttribute != zfnull) {
                *firstNotResolvedAttribute += it->first;
            }
            return zffalse;
        }
    }
    for(zfindex i = 0; i < this->childCount(); ++i) {
        if(!this->childAt(i).resolvedAll(firstNotResolvedElement, firstNotResolvedAttribute)) {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFSerializableData::resolveMarkAll(void) const {
    if(ZFSerializableDataResolveCheckEnable) {
        this->resolveMark();
        this->resolveAttributeMarkAll();
        for(zfindex i = 0; i < this->childCount(); ++i) {
            this->childAt(i).resolveMarkAll();
        }
    }
}
void ZFSerializableData::resolveUnmarkAll(void) const {
    if(ZFSerializableDataResolveCheckEnable) {
        this->resolveUnmark();
        this->resolveAttributeUnmarkAll();
        for(zfindex i = 0; i < this->childCount(); ++i) {
            this->childAt(i).resolveUnmarkAll();
        }
    }
}
void ZFSerializableData::resolveAttributeMarkAll(void) const {
    if(ZFSerializableDataResolveCheckEnable) {
        for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it) {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttributeUnmarkAll(void) const {
    if(ZFSerializableDataResolveCheckEnable) {
        for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it) {
            it->second.resolved = zftrue;
        }
    }
}

// ============================================================
// other functions
void ZFSerializableData::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ZFSerializableDataToZfsd(ret, *this);
}

ZFCompareResult ZFSerializableData::objectCompare(ZF_IN const ZFSerializableData &another) const {
    if(d == another.d) {
        return ZFCompareEqual;
    }

    if(!zfstringIsEqual(this->itemClass(), another.itemClass())
            || this->attrCount() != another.attrCount()
            || this->childCount() != another.childCount()
            ) {
        return ZFCompareUncomparable;
    }

    for(zfiterator it = another.attrIter(); another.attrIterValid(it); another.attrIterNext(it)) {
        const zfchar *value = this->attr(another.attrIterKey(it));
        if(value == zfnull || !zfstringIsEqual(another.attrIterValue(it), value)) {
            return ZFCompareUncomparable;
        }
    }
    for(zfindex i = 0; i < this->childCount(); ++i) {
        if(this->childAt(i).objectCompare(another.childAt(i)) != ZFCompareEqual) {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareEqual;
}

zfbool ZFSerializableData::isEmpty(void) const {
    return (
        d->classNameFull.isEmpty()
        && d->attributes.empty()
        && d->elements.empty()
        );
}

ZF_NAMESPACE_GLOBAL_END

