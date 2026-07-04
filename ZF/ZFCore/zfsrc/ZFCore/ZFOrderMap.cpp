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

ZFMETHOD_DEFINE_1(ZFOrderMap, void, capacity
        , ZFMP_IN(zfindex, capacity)
        ) {
    d->data.reserve((zfstlsize)capacity);
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

    this->capacity(this->count() + another->count());
    ZFObject *key = zfnull;
    ZFObject *value = zfnull;
    for(zfiter it = another->iter(); it; ++it) {
        key = another->iterKey(it);
        value = another->iterValue(it);
        _ZFP_ZFOrderMapPrivate::MapType::iterator implIt;
        if(d->data.iterAccess(implIt, key)) {
            zfobjRetainChange(implIt->second, value);
        }
        else {
            zfobjRetain(key);
            zfobjRetain(value);
        }
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
    _ZFP_ZFOrderMapPrivate::MapType::iterator implIt;
    if(d->data.iterAccess(implIt, key)) {
        zfobjRetainChange(implIt->second, value);
    }
    else {
        zfobjRetain(key);
        zfobjRetain(value);
    }
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
            zfobjRelease(tmpKey);
            zfobjRelease(tmpValue);
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
            zfobjRelease(it->first);
            zfobjRelease(it->second);
        }
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
    ZFObject *valueOld = d->data.iterValue(it);
    d->data.iterValue(it, value);
    zfobjRetain(value);
    zfobjRelease(valueOld);
}
ZFMETHOD_DEFINE_1(ZFOrderMap, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    ZFObject *key = d->data.iterKey(it);
    ZFObject *value = d->data.iterValue(it);
    d->data.iterRemove(it);
    zfobjRelease(key);
    zfobjRelease(value);
}

ZFMETHOD_DEFINE_2(ZFOrderMap, zfiter, iterAdd
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN(ZFObject *, value)
        ) {
    return d->data.iterAdd(key, value);
}
ZFMETHOD_DEFINE_3(ZFOrderMap, zfbool, iterAccess
        , ZFMP_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, key)
        , ZFMP_IN_OPT(ZFObject *, defValue, ZFNull())
        ) {
    if(key == zfnull) {
        return zffalse;
    }
    if(defValue == zfnull) {
        defValue = ZFNull();
    }
    if(d->data.iterAccess(it, key, defValue)) {
        return zftrue;
    }
    else {
        zfobjRetain(key);
        zfobjRetain(defValue);
        return zffalse;
    }
}

// ============================================================
// order map spec
ZFMETHOD_DEFINE_2(ZFOrderMap, void, move
        , ZFMP_IN_OUT(zfiter &, from)
        , ZFMP_IN(const zfiter &, to)
        ) {
    d->data.iterMove(from, to);
}

ZF_NAMESPACE_GLOBAL_END

