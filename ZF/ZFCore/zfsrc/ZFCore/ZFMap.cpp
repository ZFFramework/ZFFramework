#include "ZFMap.h"
#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFMapPrivate
zfclassNotPOD _ZFP_ZFMapKeyComparer {
public:
    inline zfbool operator () (
            ZF_IN ZFObject *obj1
            , ZF_IN ZFObject *obj2
            ) const {
        ZFCompareResult cmp = ZFObjectCompare(obj1, obj2);
        ZFCoreAssertWithMessageTrim(cmp != ZFCompareUncomparable, "[ZFMap] key must comparable: %s, %s",
            obj1,
            obj2);
        return (cmp == ZFCompareSmaller);
    }
};
zfclassNotPOD _ZFP_ZFMapPrivate {
public:
    typedef zfimplmap<ZFObject *, ZFObject *, _ZFP_ZFMapKeyComparer> MapType;

public:
    MapType data;

public:
    _ZFP_ZFMapPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFMap
ZFOBJECT_REGISTER(ZFMap)

ZFOBJECT_ON_INIT_DEFINE_1(ZFMap
        , ZFMP_IN(ZFKeyValueContainer *, another)
        ) {
    zfself::addFrom(another);
}
void ZFMap::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFMapPrivate);
}
void ZFMap::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFMap, zfindex, count) {
    return d->data.size();
}

ZFMETHOD_DEFINE_0(ZFMap, zfbool, isEmpty) {
    return d->data.empty();
}

ZFMETHOD_DEFINE_1(ZFMap, zfbool, isContain
        , ZFMP_IN(ZFObject *, key)
        ) {
    return (key != zfnull && d->data.find(key) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFMap, zfany, get
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.find(key);
        if(it != d->data.end()) {
            return it->second;
        }
    }
    return zfnull;
}

ZFMETHOD_DEFINE_1(ZFMap, void, allKeyT
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
        ) {
    if(!this->isEmpty()) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it) {
            ret.add(it->first);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFMap, ZFCoreArray<zfauto>, allKey) {
    ZFCoreArray<zfauto> ret;
    this->allKeyT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFMap, void, allValueT
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
        ) {
    if(!this->isEmpty()) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it) {
            ret.add(it->second);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFMap, ZFCoreArray<zfauto>, allValue) {
    ZFCoreArray<zfauto> ret;
    this->allValueT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFMap, void, addFrom
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

        _ZFP_ZFMapPrivate::MapType::iterator itExisting = d->data.find(key);
        if(itExisting != d->data.end()) {
            this->contentOnRemove(itExisting->first, itExisting->second);
            ZFObject *tmp = itExisting->second;
            zfRetain(value);
            itExisting->second = value;
            this->contentOnAdd(itExisting->first, itExisting->second);
            zfRelease(tmp);
        }
        else {
            zfRetain(key);
            zfRetain(value);
            (d->data)[key] = value;
        }
    }

    if(key != zfnull) {
        this->contentOnUpdate();
    }
}

ZFMETHOD_DEFINE_2(ZFMap, void, set
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

    _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(key);
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
    }

    this->contentOnUpdate();
}

ZFMETHOD_DEFINE_1(ZFMap, void, remove
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(key);
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
ZFMETHOD_DEFINE_1(ZFMap, zfauto, removeAndGet
        , ZFMP_IN(ZFObject *, key)
        ) {
    if(key != zfnull) {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(key);
        if(it != d->data.end()) {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);

            this->contentOnRemove(tmpKey, tmpValue);
            this->contentOnUpdate();

            zfRelease(tmpKey);
            zfblockedRelease(tmpValue);
            return tmpValue;
        }
    }
    return zfnull;
}
ZFMETHOD_DEFINE_0(ZFMap, void, removeAll) {
    if(!d->data.empty()) {
        _ZFP_ZFMapPrivate::MapType tmp;
        tmp.swap(d->data);
        for(_ZFP_ZFMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it) {
            this->contentOnRemove(it->first, it->second);
            zfRelease(it->first);
            zfRelease(it->second);
        }
        this->contentOnUpdate();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFMap, zfiter, iter) {
    return d->data.iter();
}
ZFMETHOD_DEFINE_1(ZFMap, zfiter, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->data.iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFMap, zfany, iterKey
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterKey(it);
}
ZFMETHOD_DEFINE_1(ZFMap, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterValue(it);
}

ZFMETHOD_DEFINE_2(ZFMap, void, iterValue
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
ZFMETHOD_DEFINE_1(ZFMap, void, iterRemove
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

ZFMETHOD_DEFINE_2(ZFMap, void, iterAdd
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

