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
        zfCoreAssertWithMessageTrim(cmp != ZFCompareUncomparable, "[ZFMap] key must comparable: %s, %s",
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
    this->objectOnInit();
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
        , ZFMP_IN(ZFObject *, pKey)
        ) {
    return (pKey != zfnull && d->data.find(pKey) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFMap, zfany, get
        , ZFMP_IN(ZFObject *, pKey)
        ) {
    if(pKey != zfnull) {
        _ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.find(pKey);
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
    for(zfiterator it = another->iter(); another->iterValid(it); another->iterNext(it)) {
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
        this->contentOnChange();
    }
}

ZFMETHOD_DEFINE_2(ZFMap, void, set
        , ZFMP_IN(ZFObject *, pKey)
        , ZFMP_IN(ZFObject *, pValue)
        ) {
    if(pKey == zfnull) {
        return;
    }
    if(pValue == zfnull) {
        this->remove(pKey);
        return;
    }

    _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
    if(it != d->data.end()) {
        this->contentOnRemove(it->first, it->second);
        ZFObject *tmp = it->second;
        zfRetain(pValue);
        it->second = pValue;
        this->contentOnAdd(it->first, it->second);
        zfRelease(tmp);
    }
    else {
        zfRetain(pKey);
        zfRetain(pValue);
        (d->data)[pKey] = pValue;
    }

    this->contentOnChange();
}

ZFMETHOD_DEFINE_1(ZFMap, void, remove
        , ZFMP_IN(ZFObject *, pKey)
        ) {
    if(pKey != zfnull) {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end()) {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);
            this->contentOnRemove(tmpKey, tmpValue);
            zfRelease(tmpKey);
            zfRelease(tmpValue);

            this->contentOnChange();
        }
    }
}
ZFMETHOD_DEFINE_1(ZFMap, zfauto, removeAndGet
        , ZFMP_IN(ZFObject *, pKey)
        ) {
    if(pKey != zfnull) {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end()) {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);

            this->contentOnRemove(tmpKey, tmpValue);
            this->contentOnChange();

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
        this->contentOnChange();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFMap, zfiterator, iter) {
    return d->data.iter();
}
ZFMETHOD_DEFINE_1(ZFMap, zfiterator, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->data.iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFMap, zfbool, iterValid
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return d->data.iterValid(it);
}

ZFMETHOD_DEFINE_1(ZFMap, void, iterNext
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    d->data.iterNext(it);
}
ZFMETHOD_DEFINE_1(ZFMap, zfany, iterKey
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return d->data.iterKey(it);
}
ZFMETHOD_DEFINE_1(ZFMap, zfany, iterValue
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return d->data.iterValue(it);
}

ZFMETHOD_DEFINE_2(ZFMap, void, iterValue
        , ZFMP_IN_OUT(zfiterator &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *old = d->data.iterValue(it);
    zfRetain(value);
    this->contentOnRemove(key, old);
    d->data.iterValue(it, value);
    this->contentOnAdd(key, value);
    this->contentOnChange();
    zfRelease(old);
}
ZFMETHOD_DEFINE_1(ZFMap, void, iterRemove
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *value = d->data.iterValue(it);
    d->data.iterRemove(it);
    this->contentOnRemove(key, value);
    this->contentOnChange();
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

