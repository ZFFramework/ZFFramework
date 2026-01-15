#include "ZFHashMap.h"
#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFHashMapPrivate
zfclassNotPOD _ZFP_ZFHashMapKeyHasher {
public:
    zfstlsize operator () (ZFObject *const &v) const {
        return (zfstlsize)v->objectHash();
    }
};
zfclassNotPOD _ZFP_ZFHashMapKeyComparer {
public:
    zfbool operator () (ZFObject * const &v0, ZFObject * const &v1) const {
        return (v0->objectCompare(v1) == ZFCompareEqual);
    }
};

zfclassNotPOD _ZFP_ZFHashMapPrivate {
public:
    typedef zfimplhashmap<ZFObject *, ZFObject *, _ZFP_ZFHashMapKeyHasher, _ZFP_ZFHashMapKeyComparer> MapType;

public:
    MapType data;

public:
    _ZFP_ZFHashMapPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFHashMap
ZFOBJECT_REGISTER(ZFHashMap)

ZFOBJECT_ON_INIT_DEFINE_1(ZFHashMap
        , ZFMP_IN(ZFKeyValueContainer *, another)
        ) {
    zfself::addFrom(another);
}
void ZFHashMap::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFHashMapPrivate);
}
void ZFHashMap::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFHashMap, zfindex, count) {
    return d->data.size();
}

ZFMETHOD_DEFINE_0(ZFHashMap, zfbool, isEmpty) {
    return d->data.empty();
}

ZFMETHOD_DEFINE_1(ZFHashMap, zfbool, isContain
        , ZFMP_IN(ZFObject *, key)
        ) {
    return (key != zfnull && d->data.find(key) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFHashMap, zfany, get
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.find(key);
        if(it != d->data.end()) {
            return it->second;
        }
    }
    return zfnull;
}

ZFMETHOD_DEFINE_1(ZFHashMap, void, allKeyT
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
        ) {
    if(!this->isEmpty()) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it) {
            ret.add(it->first);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFHashMap, ZFCoreArray<zfauto>, allKey) {
    ZFCoreArray<zfauto> ret;
    this->allKeyT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFHashMap, void, allValueT
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
        ) {
    if(!this->isEmpty()) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it) {
            ret.add(it->second);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFHashMap, ZFCoreArray<zfauto>, allValue) {
    ZFCoreArray<zfauto> ret;
    this->allValueT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFHashMap, void, addFrom
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

        _ZFP_ZFHashMapPrivate::MapType::iterator itExisting = d->data.find(key);
        if(itExisting != d->data.end()) {
            this->contentOnRemove(itExisting->first, itExisting->second);
            ZFObject *tmp = itExisting->second;
            zfRetain(value);
            itExisting->second = value;
            this->contentOnAdd(key, value);
            zfRelease(tmp);
        }
        else {
            zfRetain(key);
            zfRetain(value);
            (d->data)[key] = value;
            this->contentOnAdd(key, value);
        }
    }

    if(key != zfnull) {
        this->contentOnUpdate();
    }
}

ZFMETHOD_DEFINE_2(ZFHashMap, void, set
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

    _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(key);
    if(it != d->data.end()) {
        this->contentOnRemove(it->first, it->second);
        ZFObject *tmp = it->second;
        zfRetain(value);
        it->second = value;
        this->contentOnAdd(it->first, it->second);
        zfRelease(tmp);
    }
    else {
        zfRetain(key);
        zfRetain(value);
        (d->data)[key] = value;
        this->contentOnAdd(key, value);
    }

    this->contentOnUpdate();
}

ZFMETHOD_DEFINE_1(ZFHashMap, void, remove
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(key);
        if(it != d->data.end()) {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);
            this->contentOnRemove(tmpKey, tmpValue);
            zfRelease(tmpKey);
            zfRelease(tmpValue);

            this->contentOnUpdate();
        }
    }
}
ZFMETHOD_DEFINE_1(ZFHashMap, zfauto, removeAndGet
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(key);
        if(it != d->data.end()) {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);

            this->contentOnRemove(tmpKey, tmpValue);
            this->contentOnUpdate();

            zfRelease(tmpKey);
            zfscopeRelease(tmpValue);
            return tmpValue;
        }
    }
    return zfnull;
}
ZFMETHOD_DEFINE_0(ZFHashMap, void, removeAll) {
    if(!d->data.empty()) {
        _ZFP_ZFHashMapPrivate::MapType tmp;
        tmp.swap(d->data);
        for(_ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it) {
            this->contentOnRemove(it->first, it->second);
            zfRelease(it->first);
            zfRelease(it->second);
        }
        this->contentOnUpdate();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFHashMap, zfiter, iter) {
    return d->data.iter();
}

ZFMETHOD_DEFINE_1(ZFHashMap, zfiter, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->data.iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFHashMap, zfany, iterKey
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterKey(it);
}
ZFMETHOD_DEFINE_1(ZFHashMap, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterValue(it);
}

ZFMETHOD_DEFINE_2(ZFHashMap, void, iterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *old = d->data.iterValue(it);
    zfRetain(value);
    this->contentOnRemove(key, old);
    d->data.iterValue(it, value);
    this->contentOnAdd(key, value);
    this->contentOnUpdate();
    zfRelease(old);
}
ZFMETHOD_DEFINE_1(ZFHashMap, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *value = d->data.iterValue(it);
    d->data.iterRemove(it);
    this->contentOnRemove(key, value);
    this->contentOnUpdate();
    zfRelease(key);
    zfRelease(value);
}

ZFMETHOD_DEFINE_2(ZFHashMap, void, iterAdd
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

