#include "ZFSerializableData.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"

#include "ZFCore/ZFSTLWrapper/zfstldeque.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if ZF_ENV_DEBUG
    zfbool ZFSerializableDataResolveCheckEnable = zftrue;
#else
    zfbool ZFSerializableDataResolveCheckEnable = zffalse;
#endif
ZFEXPORT_VAR_USER_REGISTER(zfbool, ZFSerializableDataResolveCheckEnable)

// ============================================================
// _ZFP_ZFSerializableDataPrivate
zfclassNotPOD _ZFP_ZFSerializableDataAttrData {
public:
    zfstring attrValue;
    zfbool resolved;
public:
    _ZFP_ZFSerializableDataAttrData(void)
    : attrValue()
    , resolved(zffalse)
    {
    }
    _ZFP_ZFSerializableDataAttrData(
            ZF_IN const zfstring &attrValue
            , ZF_IN_OPT zfbool resolved = zffalse
            )
    : attrValue(attrValue)
    , resolved(resolved)
    {
    }
};
typedef zfimplmap<zfstring, _ZFP_ZFSerializableDataAttrData> _ZFP_ZFSerializableDataAttrMapType;
typedef zfstlmap<zfstring, zfauto> _ZFP_ZFSerializableDataTagMapType;
zfclassNotPOD _ZFP_ZFSerializableDataPrivate {
public:
    zfuint refCount;
    zfbool resolved;
    _ZFP_ZFSerializableDataPrivate *serializableDataParent;
    zfstring classNameFull;
    ZFPathInfo pathInfo;
    _ZFP_ZFSerializableDataAttrMapType attrs;
    zfstldeque<ZFSerializableData> elements;
    _ZFP_ZFSerializableDataTagMapType serializableDataTagMap;

public:
    void removeAll(void) {
        this->classNameFull.removeAll();
        this->attrs.clear();
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
    , resolved(zffalse)
    , serializableDataParent(zfnull)
    , classNameFull()
    , pathInfo(zfnull)
    , attrs()
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
    }
};

// ============================================================
// ZFSerializableData
ZFSerializableData::ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d) {
    this->d = d;
    if(d) {
        ++(d->refCount);
    }
}
ZFSerializableData::ZFSerializableData(void)
: d(zfnull)
{
}
ZFSerializableData::ZFSerializableData(ZF_IN const ZFSerializableData &ref)
: d(ref.d)
{
    if(d) {
        ++(d->refCount);
    }
}
ZFSerializableData::ZFSerializableData(ZF_IN const zfnullT &dummy)
: d(zfnull)
{
}
ZFSerializableData &ZFSerializableData::operator = (ZF_IN const zfnullT &dummy) {
    _ZFP_ZFSerializableDataPrivate *dTmp = d;
    d = zfnull;
    if(dTmp) {
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
ZFSerializableData &ZFSerializableData::operator = (ZF_IN const ZFSerializableData &ref) {
    _ZFP_ZFSerializableDataPrivate *dTmp = d;
    d = ref.d;
    if(d) {
        ++(d->refCount);
    }
    if(dTmp) {
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
zfbool ZFSerializableData::operator == (ZF_IN const ZFSerializableData &ref) const {
    return (d == ref.d);
}
ZFSerializableData::~ZFSerializableData(void) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
    }
}

void ZFSerializableData::copyFrom(ZF_IN const ZFSerializableData &ref) {
    if(d == ref.d || ref.d == zfnull) {
        return;
    }
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }

    d->classNameFull = ref.d->classNameFull;
    d->attrs = ref.d->attrs;
    d->pathInfo = ref.d->pathInfo;
    d->serializableDataTagMap = ref.d->serializableDataTagMap;

    this->childRemoveAll();
    for(zfstlsize i = 0; i < ref.d->elements.size(); ++i) {
        this->child(ref.d->elements[i].copy());
    }
}

zfindex ZFSerializableData::objectRetainCount(void) const {
    return d ? d->refCount : 0;
}

// ============================================================
// local path logic
ZFPathInfo ZFSerializableData::pathInfo(void) const {
    return d ? d->pathInfo : zfnull;
}
void ZFSerializableData::pathInfo(ZF_IN const ZFPathInfo &pathInfo) {
    if(pathInfo) {
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
        }
        d->pathInfo = pathInfo;
    }
    else {
        if(d) {
            d->pathInfo = zfnull;
        }
    }
}
ZFPathInfo ZFSerializableData::pathInfoCheck(void) const {
    _ZFP_ZFSerializableDataPrivate *check = d;
    while(check != zfnull) {
        if(check->pathInfo) {
            return check->pathInfo;
        }
        check = check->serializableDataParent;
    }
    return zfnull;
}

// ============================================================
// parent
zfbool ZFSerializableData::serializableDataParent(ZF_OUT ZFSerializableData &ret) const {
    if(d && d->serializableDataParent != zfnull) {
        ret = ZFSerializableData(d->serializableDataParent);
        return zftrue;
    }
    else {
        return zffalse;
    }
}

// ============================================================
// class
void ZFSerializableData::itemClass(ZF_IN const zfstring &classNameFull) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }
    if(classNameFull == zfnull) {
        d->classNameFull.removeAll();
    }
    else {
        d->classNameFull = classNameFull;
    }
}
const zfstring &ZFSerializableData::itemClass(void) const {
    return d ? d->classNameFull : zfstring::Empty();
}

// ============================================================
// other types
void ZFSerializableData::removeAll(void) {
    if(d) {
        d->removeAll();
    }
}

// ============================================================
void ZFSerializableData::serializableDataTag(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *tag
        ) {
    if(key == zfnull) {
        return;
    }
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
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
zfany ZFSerializableData::serializableDataTag(ZF_IN const zfstring &key) const {
    if(d && key != zfnull) {
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
    if(d) {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        allKey.capacity(allKey.count() + m.size());
        allValue.capacity(allValue.count() + m.size());
        for(_ZFP_ZFSerializableDataTagMapType::iterator it = m.begin(); it != m.end(); ++it) {
            allKey.add(it->first);
            allValue.add(it->second);
        }
    }
}
void ZFSerializableData::serializableDataTagRemove(ZF_IN const zfstring &key) {
    if(d && key != zfnull) {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end()) {
            m.erase(it);
        }
    }
}
zfauto ZFSerializableData::serializableDataTagRemoveAndGet(ZF_IN const zfstring &key) {
    if(d && key != zfnull) {
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
    if(d && !d->serializableDataTagMap.empty()) {
        _ZFP_ZFSerializableDataTagMapType tmp;
        tmp.swap(d->serializableDataTagMap);
    }
}

// ============================================================
// attribute
void ZFSerializableData::attr(
        ZF_IN const zfstring &name
        , ZF_IN const zfstring &value
        ) {
    if(name != zfnull) {
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
        }
        if(value != zfnull) {
            _ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.find(name);
            if(it != d->attrs.end()) {
                it->second = _ZFP_ZFSerializableDataAttrData(value);
            }
            else {
                d->attrs[name] = _ZFP_ZFSerializableDataAttrData(value);
            }
        }
        else {
            d->attrs.erase(name);
        }
    }
}
zfstring ZFSerializableData::attr(ZF_IN const zfstring &name) const {
    if(d && name != zfnull) {
        _ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.find(name);
        if(it != d->attrs.end()) {
            return it->second.attrValue;
        }
    }
    return zfnull;
}

zfindex ZFSerializableData::attrCount(void) const {
    return (zfindex)(d ? d->attrs.size() : 0);
}
void ZFSerializableData::attrRemove(ZF_IN const zfstring &name) {
    if(d && name != zfnull) {
        d->attrs.erase(name);
    }
}
void ZFSerializableData::attrRemoveAll(void) {
    if(d) {
        d->attrs.clear();
    }
}

zfiter ZFSerializableData::attrIterFind(ZF_IN const zfstring &name) const {
    if(d && name != zfnull) {
        return d->attrs.iterFind(name);
    }
    else {
        return zfnull;
    }
}
zfiter ZFSerializableData::attrIter(void) const {
    return d ? d->attrs.iter() : zfnull;
}
zfstring ZFSerializableData::attrIterKey(ZF_IN const zfiter &it) const {
    return d ? d->attrs.iterKey(it) : zfnull;
}
zfstring ZFSerializableData::attrIterValue(ZF_IN const zfiter &it) const {
    return d ? d->attrs.iterValue(it).attrValue : zfnull;
}
void ZFSerializableData::attrIterValue(
        ZF_IN_OUT zfiter &it
        , ZF_IN const zfstring &value
        ) {
    if(d) {
        if(value == zfnull) {
            this->attrIterRemove(it);
        }
        else {
            d->attrs.iterValue(it, value);
        }
    }
}
void ZFSerializableData::attrIterRemove(ZF_IN_OUT zfiter &it) {
    if(d) {
        d->attrs.iterRemove(it);
    }
}

zfbool ZFSerializableData::attrIterResolved(ZF_IN const zfiter &it) const {
    if(it) {
        return d->attrs.iterValue(it).resolved;
    }
    return zffalse;
}
void ZFSerializableData::attrIterResolveMark(ZF_IN const zfiter &it) const {
    if(it) {
        d->attrs.iterValue(it).resolved = zftrue;
    }
}
void ZFSerializableData::attrIterResolveUnmark(ZF_IN const zfiter &it) const {
    d->attrs.iterValue(it).resolved = zffalse;
}

// ============================================================
// element
void ZFSerializableData::child(ZF_IN const ZFSerializableData &element) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }
    ZFCoreAssertWithMessage(d != element.d, "adding self is not allowed");
    if(element.d == zfnull) {
        const_cast<ZFSerializableData &>(element).d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }
    ZFCoreAssertWithMessage(element.d->serializableDataParent == zfnull, "adding a data which already has parent");
    d->elements.push_back(element);
    element.d->serializableDataParent = d;
}
void ZFSerializableData::child(
        ZF_IN const ZFSerializableData &element
        , ZF_IN zfindex atIndex
        ) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }
    ZFCoreAssertWithMessage(d != element.d, "adding self is not allowed");
    if(element.d == zfnull) {
        const_cast<ZFSerializableData &>(element).d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }
    ZFCoreAssertWithMessage(element.d->serializableDataParent == zfnull, "adding a data which already has parent");
    if(atIndex == zfindexMax()) {
        atIndex = (zfindex)d->elements.size();
    }
    ZFCoreAssertIndexRange(atIndex, zfindex(d->elements.size() + 1));

    d->elements.insert(d->elements.begin() + atIndex, element);
    element.d->serializableDataParent = d;
}

void ZFSerializableData::childReplace(
        ZF_IN zfindex index
        , ZF_IN const ZFSerializableData &element
        ) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements[(zfstlsize)index] = element;
    if(element.d == zfnull) {
        const_cast<ZFSerializableData &>(element).d = zfpoolNew(_ZFP_ZFSerializableDataPrivate);
    }
    element.d->serializableDataParent = d;
}

zfindex ZFSerializableData::childForName(ZF_IN const zfstring &name) const {
    if(d && name != zfnull && *name != '\0') {
        for(zfstlsize i = 0; i < d->elements.size(); ++i) {
            if(zfstringIsEqual(d->elements[i].propertyName(), name)) {
                return i;
            }
        }
    }
    return zfindexMax();
}
zfindex ZFSerializableData::childForCategory(ZF_IN const zfstring &category) const {
    if(d && category != zfnull && *category != '\0') {
        for(zfstlsize i = 0; i < d->elements.size(); ++i) {
            if(zfstringIsEqual(d->elements[i].category(), category)) {
                return i;
            }
        }
    }
    return zfindexMax();
}

zfindex ZFSerializableData::childCount(void) const {
    return (zfindex)(d ? d->elements.size() : 0);
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
    if(d && !d->elements.empty()) {
        for(zfstlsize i = 0; i < d->elements.size(); ++i) {
            d->elements[i].d->serializableDataParent = zfnull;
        }
        d->elements.clear();
    }
}

// ============================================================
// resolved state
zfbool ZFSerializableData::resolved(void) const {
    return (d == zfnull || d->resolved);
}
void ZFSerializableData::resolveMark(void) const {
    if(d) {
        d->resolved = zftrue;
    }
}
void ZFSerializableData::resolveUnmark(void) const {
    if(d) {
        d->resolved = zffalse;
    }
}

zfbool ZFSerializableData::resolvedAttr(ZF_IN const zfstring &name) const {
    if(d && name != zfnull) {
        _ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.find(name);
        if(it != d->attrs.end()) {
            return it->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::resolveAttrMark(ZF_IN const zfstring &name) const {
    if(d && name != zfnull) {
        _ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.find(name);
        if(it != d->attrs.end()) {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttrUnmark(ZF_IN const zfstring &name) const {
    if(d && name != zfnull) {
        _ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.find(name);
        if(it != d->attrs.end()) {
            it->second.resolved = zffalse;
        }
    }
}

zfbool ZFSerializableData::resolvedAll(
        ZF_OUT_OPT ZFSerializableData *firstNotResolvedElement /* = zfnull */
        , ZF_OUT_OPT zfstring *firstNotResolvedAttr /* = zfnull */
        ) const {
    if(d == zfnull) {
        return zftrue;
    }
    if(!this->resolved()) {
        if(firstNotResolvedElement != zfnull) {
            *firstNotResolvedElement = *this;
        }
        return zffalse;
    }
    for(_ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.begin(); it != d->attrs.end(); ++it) {
        if(!it->second.resolved) {
            if(firstNotResolvedElement != zfnull) {
                *firstNotResolvedElement = *this;
            }
            if(firstNotResolvedAttr != zfnull) {
                *firstNotResolvedAttr += it->first;
            }
            return zffalse;
        }
    }
    for(zfindex i = 0; i < this->childCount(); ++i) {
        if(!this->childAt(i).resolvedAll(firstNotResolvedElement, firstNotResolvedAttr)) {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFSerializableData::resolveMarkAll(void) const {
    if(d) {
        this->resolveMark();
        this->resolveAttrMarkAll();
        for(zfindex i = 0; i < this->childCount(); ++i) {
            this->childAt(i).resolveMarkAll();
        }
    }
}
void ZFSerializableData::resolveUnmarkAll(void) const {
    if(d) {
        this->resolveUnmark();
        this->resolveAttrUnmarkAll();
        for(zfindex i = 0; i < this->childCount(); ++i) {
            this->childAt(i).resolveUnmarkAll();
        }
    }
}
void ZFSerializableData::resolveAttrMarkAll(void) const {
    if(d) {
        for(_ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.begin(); it != d->attrs.end(); ++it) {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttrUnmarkAll(void) const {
    if(d) {
        for(_ZFP_ZFSerializableDataAttrMapType::iterator it = d->attrs.begin(); it != d->attrs.end(); ++it) {
            it->second.resolved = zftrue;
        }
    }
}

// ============================================================
// other functions
void ZFSerializableData::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ZFSerializableDataToZFSD(ret, *this);
}

ZFCompareResult ZFSerializableData::objectCompareValue(ZF_IN const ZFSerializableData &another) const {
    if(d == another.d) {
        return ZFCompareEqual;
    }

    if(d == zfnull || another.d == zfnull
            || !zfstringIsEqual(this->itemClass(), another.itemClass())
            || this->attrCount() != another.attrCount()
            || this->childCount() != another.childCount()
            ) {
        return ZFCompareUncomparable;
    }

    for(zfiter it = another.attrIter(); it; ++it) {
        zfstring value = this->attr(another.attrIterKey(it));
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

ZF_NAMESPACE_GLOBAL_END

