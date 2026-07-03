#include "ZFMap.h"
#include "ZFObjectDef/ZFObjectKeyPrivate"
#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFMapPrivate
zfclassNotPOD _ZFP_ZFMapPrivate {
public:
    typedef zfimplmap<ZFObject *, ZFObject *, _ZFP_ZFObjectKeyCompare> MapType;

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

ZFMETHOD_DEFINE_1(ZFMap, void, capacity
        , ZFMP_IN(zfindex, capacity)
        ) {
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

    this->capacity(this->count() + another->count());
    ZFObject *key = zfnull;
    ZFObject *value = zfnull;
    for(zfiter it = another->iter(); it; ++it) {
        key = another->iterKey(it);
        value = another->iterValue(it);
        zfstlpair<_ZFP_ZFMapPrivate::MapType::iterator, bool> insertResult = d->data.insert(zfstlpair<ZFObject *, ZFObject *>(key, value));
        if(insertResult.second) {
            zfobjRetain(key);
            zfobjRetain(value);
        }
        else {
            ZFObject *valueOld = insertResult.first->second;
            insertResult.first->second = value;
            zfobjRetain(value);
            zfobjRelease(valueOld);
        }
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
    zfstlpair<_ZFP_ZFMapPrivate::MapType::iterator, bool> insertResult = d->data.insert(zfstlpair<ZFObject *, ZFObject *>(key, value));
    if(insertResult.second) {
        zfobjRetain(key);
        zfobjRetain(value);
    }
    else {
        ZFObject *valueOld = insertResult.first->second;
        insertResult.first->second = value;
        zfobjRetain(value);
        zfobjRelease(valueOld);
    }
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
            zfobjRelease(tmpKey);
            zfobjRelease(tmpValue);
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
            zfobjRelease(tmpKey);
            zfobjReleaseInScope(tmpValue);
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
            zfobjRelease(it->first);
            zfobjRelease(it->second);
        }
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
    ZFObject *valueOld = d->data.iterValue(it);
    d->data.iterValue(it, value);
    zfobjRetain(value);
    zfobjRelease(valueOld);
}
ZFMETHOD_DEFINE_1(ZFMap, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *value = d->data.iterValue(it);
    d->data.iterRemove(it);
    zfobjRelease(key);
    zfobjRelease(value);
}

ZFMETHOD_DEFINE_2(ZFMap, zfiter, iterAdd
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN(ZFObject *, value)
        ) {
    if(key == zfnull || value == zfnull) {
        return zfnull;
    }
    zfstlpair<_ZFP_ZFMapPrivate::MapType::iterator, bool> insertResult = d->data.insert(zfstlpair<ZFObject *, ZFObject *>(key, value));
    if(insertResult.second) {
        zfobjRetain(key);
        zfobjRetain(value);
    }
    else {
        ZFObject *valueOld = insertResult.first->second;
        insertResult.first->second = value;
        zfobjRetain(value);
        zfobjRelease(valueOld);
    }
    return d->data.iter(insertResult.first);
}
ZFMETHOD_DEFINE_2(ZFMap, zfiter, iterAccess
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN_OPT(ZFObject *, defValue, ZFNull())
        ) {
    if(key == zfnull) {
        return zfnull;
    }
    zfstlpair<_ZFP_ZFMapPrivate::MapType::iterator, bool> insertResult = d->data.insert(zfstlpair<ZFObject *, ZFObject *>(key, defValue));
    if(insertResult.second) {
        zfobjRetain(key);
        zfobjRetain(defValue);
    }
    return d->data.iter(insertResult.first);
}

ZF_NAMESPACE_GLOBAL_END

