#include "ZFHashSet.h"
#include "ZFObjectDef/ZFObjectKeyPrivate"
#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFHashSetPrivate
zfclassNotPOD _ZFP_ZFHashSetPrivate {
public:
    typedef zfimplhashmap<ZFObject *, ZFObject *, _ZFP_ZFObjectKeyHash, _ZFP_ZFObjectKeyEqual> MapType;

public:
    MapType data;

public:
    _ZFP_ZFHashSetPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFHashSet
ZFOBJECT_REGISTER(ZFHashSet)

ZFOBJECT_ON_INIT_DEFINE_1(ZFHashSet
        , ZFMP_IN(ZFContainer *, another)
        ) {
    zfself::addFrom(another);
}
void ZFHashSet::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFHashSetPrivate);
}
void ZFHashSet::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, capacity
        , ZFMP_IN(zfindex, capacity)
        ) {
    d->data.reserve((zfstlsize)capacity);
}

ZFMETHOD_DEFINE_0(ZFHashSet, zfindex, count) {
    return (zfindex)d->data.size();
}
ZFMETHOD_DEFINE_0(ZFHashSet, zfbool, isEmpty) {
    return d->data.empty();
}
ZFMETHOD_DEFINE_1(ZFHashSet, zfbool, isContain
        , ZFMP_IN(ZFObject *, obj)
        ) {
    return (obj != zfnull && d->data.find(obj) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, add
        , ZFMP_IN(ZFObject *, obj)
        ) {
    ZFCoreAssertWithMessage(obj != zfnull, "insert null object");
    zfstlpair<_ZFP_ZFHashSetPrivate::MapType::iterator, bool> insertResult = d->data.insert(_ZFP_ZFHashSetPrivate::MapType::value_type(obj, zfnull));
    if(insertResult.second) {
        zfobjRetain(obj);
    }
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, addFrom
        , ZFMP_IN(ZFContainer *, another)
        ) {
    if(another == this || another == zfnull) {
        return;
    }
    this->capacity(this->count() + another->count());
    for(zfiter it = another->iter(); it; ++it) {
        ZFObject *obj = another->iterValue(it);
        zfstlpair<_ZFP_ZFHashSetPrivate::MapType::iterator, bool> insertResult = d->data.insert(_ZFP_ZFHashSetPrivate::MapType::value_type(obj, zfnull));
        if(insertResult.second) {
            zfobjRetain(obj);
        }
    }
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, remove
        , ZFMP_IN(ZFObject *, obj)
        ) {
    _ZFP_ZFHashSetPrivate::MapType::iterator it = d->data.find(obj);
    if(it != d->data.end()) {
        ZFObject *toRelease = it->first;
        d->data.erase(it);
        zfobjRelease(toRelease);
    }
}
ZFMETHOD_DEFINE_1(ZFHashSet, zfauto, removeAndGet
        , ZFMP_IN(ZFObject *, obj)
        ) {
    _ZFP_ZFHashSetPrivate::MapType::iterator it = d->data.find(obj);
    if(it != d->data.end()) {
        zfauto ret = it->first;
        d->data.erase(it);
        zfobjRelease(ret);
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_0(ZFHashSet, void, removeAll) {
    if(!d->data.empty()) {
        _ZFP_ZFHashSetPrivate::MapType tmp;
        tmp.swap(d->data);
        for(_ZFP_ZFHashSetPrivate::MapType::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            zfobjRelease(it->first);
        }
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFHashSet, zfiter, iter) {
    return d->data.iter();
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfiter, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->data.iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterKey(it);
}

ZFMETHOD_DEFINE_2(ZFHashSet, void, iterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    zfobjRetain(value);
    d->data.iterRemove(it);
    this->add(value);
    zfobjRelease(value);
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    ZFObject *key = d->data.iterKey(it);
    d->data.iterRemove(it);
    zfobjRelease(key);
}
ZFMETHOD_DEFINE_1(ZFHashSet, zfiter, iterAdd
        , ZFMP_IN(ZFObject *, value)
        ) {
    if(value == zfnull) {
        return zfnull;
    }
    zfstlpair<_ZFP_ZFHashSetPrivate::MapType::iterator, bool> insertResult = d->data.insert(_ZFP_ZFHashSetPrivate::MapType::value_type(value, zfnull));
    if(insertResult.second) {
        zfobjRetain(value);
    }
    return d->data.iter(insertResult.first);
}
ZFMETHOD_DEFINE_2(ZFHashSet, zfiter, iterAdd
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    return this->iterAdd(value);
}

ZF_NAMESPACE_GLOBAL_END

