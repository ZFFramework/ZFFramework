#include "ZFCoreOrderMap.h"

#ifndef ZF_ENV_ZFCOREORDERMAP_USE_HASHMAP
    #define ZF_ENV_ZFCOREORDERMAP_USE_HASHMAP 1
#endif

#if ZF_ENV_ZFCOREORDERMAP_USE_HASHMAP
    #include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#else
    #include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#endif

#include "ZFCore/ZFSTLWrapper/zfstllist.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFCoreOrderMapPrivate {
public:
    zfclassFwd Item;
#if ZF_ENV_ZFCOREORDERMAP_USE_HASHMAP
    typedef zfimplhashmap<zfstring, Item *, zfstring_zfstlHash, zfstring_zfstlEqual> MapType;
#else
    typedef zfimplmap<zfstring, Item *> MapType;
#endif
    typedef zfstllist<Item *> ArrayType;

public:
    zfclassNotPOD Item {
    public:
        zfstring key;
        const ZFCorePointerBase *value;
        MapType::iterator mapIt;
        ArrayType::iterator arrIt;
    public:
        ~Item(void) {
            this->value->refDelete();
        }
    };

public:
    zfuint refCount;
    MapType map;
    ArrayType arr;

public:
    void removeAll(void) {
        if(!this->map.empty()) {
            _ZFP_ZFCoreOrderMapPrivate::MapType tmp;
            tmp.swap(this->map);
            this->arr.clear();
            for(_ZFP_ZFCoreOrderMapPrivate::MapType::iterator it = tmp.begin(); it != tmp.end(); ++it) {
                zfpoolDelete(it->second);
            }
        }
    }

public:
    _ZFP_ZFCoreOrderMapPrivate(void)
    : refCount(1)
    {
    }
    ~_ZFP_ZFCoreOrderMapPrivate(void) {
        this->removeAll();
    }
};

// ============================================================
ZFCoreOrderMap::ZFCoreOrderMap(void)
: d(zfnull)
{
}
ZFCoreOrderMap::ZFCoreOrderMap(ZF_IN const ZFCoreOrderMap &ref)
: d(ref.d)
{
    if(d) {
        ++(d->refCount);
    }
}
ZFCoreOrderMap &ZFCoreOrderMap::operator = (ZF_IN const ZFCoreOrderMap &ref) {
    _ZFP_ZFCoreOrderMapPrivate *dTmp = d;
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
zfbool ZFCoreOrderMap::operator == (ZF_IN const ZFCoreOrderMap &ref) const {
    return (d == ref.d);
}
ZFCoreOrderMap::~ZFCoreOrderMap(void) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
    }
}

void ZFCoreOrderMap::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    this->objectInfoOfContentT(ret, 5);
}
void ZFCoreOrderMap::objectInfoOfContentT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */
        ) const {
    zfindex count = 0;
    ret += token.tokenLeft;
    if(!this->isEmpty()) {
        for(zfiter it = this->iter(); it && count < maxCount; ++it, ++count) {
            if(count > 0) {
                ret += token.tokenSeparator;
            }

            ret += token.tokenPairLeft;
            {
                ret += token.tokenKeyLeft;
                ret += this->iterKey(it);
                ret += token.tokenKeyRight;
            }
            ret += token.tokenPairSeparator;
            {
                ret += token.tokenValueLeft;
                const ZFCorePointerBase *value = this->iterValue(it);
                if(value == zfnull) {
                    ret += ZFTOKEN_zfnull;
                }
                else {
                    value->objectInfoOfContentT(ret);
                }
                ret += token.tokenValueRight;
            }
            ret += token.tokenPairRight;
        }
    }
    if(count < this->count()) {
        if(count > 0) {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}

void ZFCoreOrderMap::swap(ZF_IN_OUT ZFCoreOrderMap &ref) {
    _ZFP_ZFCoreOrderMapPrivate *dTmp = d;
    d = ref.d;
    ref.d = dTmp;
}

void ZFCoreOrderMap::copyFrom(ZF_IN const ZFCoreOrderMap &ref) {
    if(d != ref.d) {
        this->removeAll();
        this->addFrom(ref);
    }
}

zfindex ZFCoreOrderMap::objectRetainCount(void) const {
    return (zfindex)(d ? d->refCount : 0);
}

zfindex ZFCoreOrderMap::count(void) const {
    return (zfindex)(d ? d->map.size() : 0);
}

zfbool ZFCoreOrderMap::isEmpty(void) const {
    return d == zfnull || d->map.empty();
}

zfbool ZFCoreOrderMap::isContain(ZF_IN const zfstring &key) const {
    return (d && d->map.find(key) != d->map.end());
}

void ZFCoreOrderMap::addFrom(ZF_IN const ZFCoreOrderMap &ref) {
    if(d != ref.d && !ref.isEmpty()) {
        for(zfiter it = ref.iter(); it; ++it) {
            this->set(this->iterKey(it), *(this->iterValue(it)));
        }
    }
}

void ZFCoreOrderMap::set(
        ZF_IN const zfstring &key
        , ZF_IN const ZFCorePointerBase &value
        ) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFCoreOrderMapPrivate);
    }

    _ZFP_ZFCoreOrderMapPrivate::MapType::iterator it = d->map.find(key);
    if(it == d->map.end()) {
        _ZFP_ZFCoreOrderMapPrivate::Item *item = zfpoolNew(_ZFP_ZFCoreOrderMapPrivate::Item);
        item->key = key;
        item->value = value.refNew();
        d->map[key] = item;
        item->mapIt = d->map.find(key);
        d->arr.push_back(item);
        item->arrIt = d->arr.end();
        --(item->arrIt);
    }
    else {
        _ZFP_ZFCoreOrderMapPrivate::Item *item = it->second;
        const ZFCorePointerBase *toDelete = item->value;
        item->value = value.refNew();
        d->arr.erase(item->arrIt);
        d->arr.push_back(item);
        item->arrIt = d->arr.end();
        --(item->arrIt);
        toDelete->refDelete();
    }
}
const ZFCorePointerBase *ZFCoreOrderMap::get(ZF_IN const zfstring &key) const {
    if(d) {
        _ZFP_ZFCoreOrderMapPrivate::MapType::iterator it = d->map.find(key);
        if(it != d->map.end()) {
            return it->second->value;
        }
    }
    return zfnull;
}

void ZFCoreOrderMap::allKeyT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) const {
    if(d) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFCoreOrderMapPrivate::MapType::const_iterator it = d->map.begin();
                it != d->map.end();
                ++it
                ) {
            ret.add(it->first);
        }
    }
}
void ZFCoreOrderMap::allValueT(ZF_IN_OUT ZFCoreArray<const ZFCorePointerBase *> &ret) const {
    if(d) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFCoreOrderMapPrivate::MapType::const_iterator it = d->map.begin();
                it != d->map.end();
                ++it
                ) {
            ret.add(it->second->value);
        }
    }
}

void ZFCoreOrderMap::remove(ZF_IN const zfstring &key) {
    if(d) {
        _ZFP_ZFCoreOrderMapPrivate::MapType::iterator it = d->map.find(key);
        if(it != d->map.end()) {
            _ZFP_ZFCoreOrderMapPrivate::Item *item = it->second;
            const ZFCorePointerBase *savedValue = item->value;
            d->map.erase(it);
            d->arr.erase(item->arrIt);
            savedValue->refDelete();
            zfpoolDelete(item);
        }
    }
}

void ZFCoreOrderMap::removeAll(void) {
    if(d) {
        d->removeAll();
    }
}

// ============================================================
// iterator
zfclassNotPOD _ZFP_ZFCoreOrderMapIter : zfextend zfiter::Impl {
public:
    _ZFP_ZFCoreOrderMapPrivate::ArrayType *owner;
    _ZFP_ZFCoreOrderMapPrivate::ArrayType::iterator impl;
public:
    zfoverride
    virtual zfbool valid(void) {
        return impl != owner->end();
    }
    zfoverride
    virtual void next(void) {
        ++impl;
    }
    zfoverride
    virtual Impl *copy(void) {
        _ZFP_ZFCoreOrderMapIter *ret = zfpoolNew(_ZFP_ZFCoreOrderMapIter);
        ret->owner = owner;
        ret->impl = impl;
        return ret;
    }
    zfoverride
    virtual void destroy(void) {
        zfpoolDelete(this);
    }
    zfoverride
    virtual zfbool isEqual(ZF_IN Impl *d) {
        _ZFP_ZFCoreOrderMapIter *t = (_ZFP_ZFCoreOrderMapIter *)d;
        return (owner == t->owner && impl == t->impl);
    }
};
zfiter ZFCoreOrderMap::iter(void) const {
    if(d) {
        _ZFP_ZFCoreOrderMapIter *impl = zfpoolNew(_ZFP_ZFCoreOrderMapIter);
        impl->owner = &(d->arr);
        impl->impl = d->arr.begin();
        return zfiter(impl);
    }
    else {
        return zfnull;
    }
}

zfiter ZFCoreOrderMap::iterFind(ZF_IN const zfstring &key) const {
    if(d) {
        _ZFP_ZFCoreOrderMapPrivate::MapType::iterator mapIt = d->map.find(key);
        if(mapIt != d->map.end()) {
            _ZFP_ZFCoreOrderMapIter *impl = zfpoolNew(_ZFP_ZFCoreOrderMapIter);
            impl->owner = &(d->arr);
            impl->impl = mapIt->second->arrIt;
            return zfiter(impl);
        }
    }
    return zfnull;
}

zfstring ZFCoreOrderMap::iterKey(ZF_IN const zfiter &it) const {
    if(it) {
        return (*(it.impl<_ZFP_ZFCoreOrderMapIter *>()->impl))->key;
    }
    else {
        return zfnull;
    }
}
const ZFCorePointerBase *ZFCoreOrderMap::iterValue(ZF_IN const zfiter &it) const {
    if(it) {
        return (*(it.impl<_ZFP_ZFCoreOrderMapIter *>()->impl))->value;
    }
    else {
        return zfnull;
    }
}

void ZFCoreOrderMap::iterValue(
        ZF_IN_OUT zfiter &it
        , ZF_IN const ZFCorePointerBase &newValue
        ) {
    if(it) {
        const ZFCorePointerBase *&ref = (*(it.impl<_ZFP_ZFCoreOrderMapIter *>()->impl))->value;
        const ZFCorePointerBase *old = ref;
        ref = newValue.refNew();
        old->refDelete();
    }
}
void ZFCoreOrderMap::iterRemove(ZF_IN_OUT zfiter &it) {
    if(it) {
        _ZFP_ZFCoreOrderMapPrivate::Item *item = *(it.impl<_ZFP_ZFCoreOrderMapIter *>()->impl);
        d->map.erase(item->mapIt);
        d->arr.erase(item->arrIt);
        item->value->refDelete();
        zfpoolDelete(item);
    }
}

void ZFCoreOrderMap::iterAdd(
        ZF_IN const zfstring &key
        , ZF_IN const ZFCorePointerBase &value
        ) {
    this->set(key, value);
}

// ============================================================
// order map spec
const ZFCorePointerBase *ZFCoreOrderMap::update(ZF_IN const zfstring &key) const {
    _ZFP_ZFCoreOrderMapPrivate::MapType::iterator mapIt = d->map.find(key);
    if(mapIt == d->map.end()) {
        return zfnull;
    }
    _ZFP_ZFCoreOrderMapPrivate::Item *item = mapIt->second;
    d->arr.erase(item->arrIt);
    d->arr.push_back(item);
    item->arrIt = d->arr.end();
    --(item->arrIt);
    return item->value;
}

ZF_NAMESPACE_GLOBAL_END

