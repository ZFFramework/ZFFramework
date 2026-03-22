#include "ZFOrderMap.h"
#include "ZFSTLWrapper/zfstlordermap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFOrderMapPrivate
zfclassNotPOD _ZFP_ZFOrderMapKeyHash {
public:
    inline zfstlsize operator () (ZF_IN ZFObject *obj) const {
        return ZFObjectHash(obj);
    }
};
zfclassNotPOD _ZFP_ZFOrderMapKeyEqual {
public:
    inline zfbool operator () (
            ZF_IN ZFObject *obj1
            , ZF_IN ZFObject *obj2
            ) const {
        ZFCompareResult cmp = ZFObjectCompare(obj1, obj2);
        ZFCoreAssertWithMessageTrim(cmp != ZFCompareUncomparable, "[ZFOrderMap] key must comparable: %s, %s",
            obj1,
            obj2);
        return (cmp == ZFCompareEqual);
    }
};
zfclassNotPOD _ZFP_ZFOrderMapPrivate {
public:
    typedef zfimplordermap<ZFObject *, ZFObject *, _ZFP_ZFOrderMapKeyHash, _ZFP_ZFOrderMapKeyEqual> MapType;

public:
    MapType data;

public:
    _ZFP_ZFOrderMapPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFOrderMap
ZFOBJECT_REGISTER(ZFOrderMap)

ZFOBJECT_ON_INIT_DEFINE_1(ZFOrderMap
        , ZFMP_IN(ZFKeyValueContainer *, another)
        ) {
    zfself::addFrom(another);
}
void ZFOrderMap::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFOrderMapPrivate);
}
void ZFOrderMap::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFOrderMap, zfindex, count) {
    return d->data.size();
}

ZFMETHOD_DEFINE_0(ZFOrderMap, zfbool, isEmpty) {
    return d->data.empty();
}

ZFMETHOD_DEFINE_1(ZFOrderMap, zfbool, isContain
        , ZFMP_IN(ZFObject *, key)
        ) {
    return (key != zfnull && d->data.find(key) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFOrderMap, zfany, get
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFOrderMapPrivate::MapType::iterator it = d->data.find(key);
        if(it != d->data.end()) {
            return it->second;
        }
    }
    return zfnull;
}

ZFMETHOD_DEFINE_1(ZFOrderMap, void, allKeyT
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
        ) {
    if(!this->isEmpty()) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFOrderMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it) {
            ret.add(it->first);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFOrderMap, ZFCoreArray<zfauto>, allKey) {
    ZFCoreArray<zfauto> ret;
    this->allKeyT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFOrderMap, void, allValueT
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
        ) {
    if(!this->isEmpty()) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFOrderMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it) {
            ret.add(it->second);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFOrderMap, ZFCoreArray<zfauto>, allValue) {
    ZFCoreArray<zfauto> ret;
    this->allValueT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFOrderMap, void, addFrom
        , ZFMP_IN(ZFKeyValueContainer *, another)
        ) {
    if(another == this || another == zfnull) {
        return;
    }

    ZFObject *key = zfnull;
    ZFObject *value = zfnull;
    for(zfiter it = another->iter(); it; ++it) {
        key = another->iterKey(it);
        value = another->iterValue(it);

        _ZFP_ZFOrderMapPrivate::MapType::iterator itExisting = d->data.find(key);
        if(itExisting != d->data.end()) {
            this->contentOnRemove(itExisting->first, itExisting->second);
            ZFObject *tmp = itExisting->second;
            zfobjRetain(value);
            itExisting->second = value;
            this->contentOnAdd(itExisting->first, itExisting->second);
            zfobjRelease(tmp);
        }
        else {
            zfobjRetain(key);
            zfobjRetain(value);
            d->data.insert(zfstlpair<ZFObject *, ZFObject *>(key, value));
        }
    }

    if(key != zfnull) {
        this->contentOnUpdate();
    }
}

ZFMETHOD_DEFINE_2(ZFOrderMap, void, set
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN(ZFObject *, value)
        ) {
    if(key == zfnull) {
        return;
    }
    if(value == zfnull) {
        this->remove(key);
        return;
    }

    _ZFP_ZFOrderMapPrivate::MapType::iterator it = d->data.find(key);
    if(it != d->data.end()) {
        this->contentOnRemove(it->first, it->second);
        ZFObject *tmp = it->second;
        zfobjRetain(value);
        it->second = value;
        this->contentOnAdd(it->first, it->second);
        zfobjRelease(tmp);
    }
    else {
        zfobjRetain(key);
        zfobjRetain(value);
        d->data.insert(zfstlpair<ZFObject *, ZFObject *>(key, value));
    }

    this->contentOnUpdate();
}

ZFMETHOD_DEFINE_1(ZFOrderMap, void, remove
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFOrderMapPrivate::MapType::iterator it = d->data.find(key);
        if(it != d->data.end()) {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);
            this->contentOnRemove(tmpKey, tmpValue);
            zfobjRelease(tmpKey);
            zfobjRelease(tmpValue);

            this->contentOnUpdate();
        }
    }
}
ZFMETHOD_DEFINE_1(ZFOrderMap, zfauto, removeAndGet
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFOrderMapPrivate::MapType::iterator it = d->data.find(key);
        if(it != d->data.end()) {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);

            this->contentOnRemove(tmpKey, tmpValue);
            this->contentOnUpdate();

            zfobjRelease(tmpKey);
            zfobjReleaseInScope(tmpValue);
            return tmpValue;
        }
    }
    return zfnull;
}
ZFMETHOD_DEFINE_0(ZFOrderMap, void, removeAll) {
    if(!d->data.empty()) {
        _ZFP_ZFOrderMapPrivate::MapType tmp;
        tmp.swap(d->data);
        for(_ZFP_ZFOrderMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it) {
            this->contentOnRemove(it->first, it->second);
            zfobjRelease(it->first);
            zfobjRelease(it->second);
        }
        this->contentOnUpdate();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFOrderMap, zfiter, iter) {
    return d->data.iter();
}
ZFMETHOD_DEFINE_1(ZFOrderMap, zfiter, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->data.iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFOrderMap, zfany, iterKey
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterKey(it);
}
ZFMETHOD_DEFINE_1(ZFOrderMap, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterValue(it);
}

ZFMETHOD_DEFINE_2(ZFOrderMap, void, iterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *old = d->data.iterValue(it);
    zfobjRetain(value);
    this->contentOnRemove(key, old);
    d->data.iterValue(it, value);
    this->contentOnAdd(key, value);
    this->contentOnUpdate();
    zfobjRelease(old);
}
ZFMETHOD_DEFINE_1(ZFOrderMap, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *value = d->data.iterValue(it);
    d->data.iterRemove(it);
    this->contentOnRemove(key, value);
    this->contentOnUpdate();
    zfobjRelease(key);
    zfobjRelease(value);
}

ZFMETHOD_DEFINE_2(ZFOrderMap, void, iterAdd
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->set(key, value);
}

// ============================================================
// order map spec
ZFMETHOD_DEFINE_2(ZFOrderMap, void, move
        , ZFMP_IN(zfindex, from)
        , ZFMP_IN(zfindex, to)
        ) {
    d->data.move((zfstlsize)from, (zfstlsize)to);
}
ZFMETHOD_DEFINE_1(ZFOrderMap, zfany, keyAt
        , ZFMP_IN(zfindex, index)
        ) {
    ZFCoreAssertIndexRange(index, this->count());
    return (d->data.begin() + index)->first;
}
ZFMETHOD_DEFINE_1(ZFOrderMap, zfany, valueAt
        , ZFMP_IN(zfindex, index)
        ) {
    ZFCoreAssertIndexRange(index, this->count());
    return (d->data.begin() + index)->second;
}
ZFMETHOD_DEFINE_1(ZFOrderMap, void, removeAt
        , ZFMP_IN(zfindex, index)
        ) {
    ZFCoreAssertIndexRange(index, this->count());
    d->data.erase(d->data.begin());
}
ZFMETHOD_DEFINE_1(ZFOrderMap, zfiter, iterAt
        , ZFMP_IN(zfindex, index)
        ) {
    if(index >= this->count()) {
        return zfnull;
    }
    return d->data.iterAt((zfstlsize)index);
}
ZFMETHOD_DEFINE_1(ZFOrderMap, zfindex, iterIndex
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterIndex(it);
}

ZF_NAMESPACE_GLOBAL_END

