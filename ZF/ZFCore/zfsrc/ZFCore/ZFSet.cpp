#include "ZFSet.h"
#include "ZFObjectDef/ZFObjectKeyPrivate"
#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFSetPrivate
zfclassNotPOD _ZFP_ZFSetPrivate {
public:
    typedef zfimplmap<ZFObject *, ZFObject *, _ZFP_ZFObjectKeyCompare> MapType;

public:
    MapType data;

public:
    _ZFP_ZFSetPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFSet
ZFOBJECT_REGISTER(ZFSet)

ZFOBJECT_ON_INIT_DEFINE_1(ZFSet
        , ZFMP_IN(ZFContainer *, another)
        ) {
    zfself::addFrom(another);
}
void ZFSet::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFSetPrivate);
}
void ZFSet::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_1(ZFSet, void, capacity
        , ZFMP_IN(zfindex, capacity)
        ) {
}

ZFMETHOD_DEFINE_0(ZFSet, zfindex, count) {
    return (zfindex)d->data.size();
}
ZFMETHOD_DEFINE_0(ZFSet, zfbool, isEmpty) {
    return d->data.empty();
}
ZFMETHOD_DEFINE_1(ZFSet, zfbool, isContain
        , ZFMP_IN(ZFObject *, obj)
        ) {
    return (obj != zfnull && d->data.find(obj) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFSet, void, add
        , ZFMP_IN(ZFObject *, obj)
        ) {
    ZFCoreAssertWithMessage(obj != zfnull, "insert null object");
    zfstlpair<_ZFP_ZFSetPrivate::MapType::iterator, bool> insertResult = d->data.insert(_ZFP_ZFSetPrivate::MapType::value_type(obj, zfnull));
    if(insertResult.second) {
        zfobjRetain(obj);
    }
}
ZFMETHOD_DEFINE_1(ZFSet, void, addFrom
        , ZFMP_IN(ZFContainer *, another)
        ) {
    if(another == this || another == zfnull) {
        return;
    }
    this->capacity(this->count() + another->count());
    for(zfiter it = another->iter(); it; ++it) {
        ZFObject *obj = another->iterValue(it);
        zfstlpair<_ZFP_ZFSetPrivate::MapType::iterator, bool> insertResult = d->data.insert(_ZFP_ZFSetPrivate::MapType::value_type(obj, zfnull));
        if(insertResult.second) {
            zfobjRetain(obj);
        }
    }
}

ZFMETHOD_DEFINE_1(ZFSet, void, remove
        , ZFMP_IN(ZFObject *, obj)
        ) {
    _ZFP_ZFSetPrivate::MapType::iterator it = d->data.find(obj);
    if(it != d->data.end()) {
        ZFObject *toRelease = it->first;
        d->data.erase(it);
        zfobjRelease(toRelease);
    }
}
ZFMETHOD_DEFINE_1(ZFSet, zfauto, removeAndGet
        , ZFMP_IN(ZFObject *, obj)
        ) {
    _ZFP_ZFSetPrivate::MapType::iterator it = d->data.find(obj);
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
ZFMETHOD_DEFINE_0(ZFSet, void, removeAll) {
    if(!d->data.empty()) {
        _ZFP_ZFSetPrivate::MapType tmp;
        tmp.swap(d->data);
        for(_ZFP_ZFSetPrivate::MapType::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            zfobjRelease(it->first);
        }
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFSet, zfiter, iter) {
    return d->data.iter();
}

ZFMETHOD_DEFINE_1(ZFSet, zfiter, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->data.iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFSet, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->data.iterKey(it);
}

ZFMETHOD_DEFINE_2(ZFSet, void, iterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    zfobjRetain(value);
    d->data.iterRemove(it);
    this->add(value);
    zfobjRelease(value);
}
ZFMETHOD_DEFINE_1(ZFSet, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    ZFObject *key = d->data.iterKey(it);
    d->data.iterRemove(it);
    zfobjRelease(key);
}
ZFMETHOD_DEFINE_1(ZFSet, zfiter, iterAdd
        , ZFMP_IN(ZFObject *, value)
        ) {
    if(value == zfnull) {
        return zfnull;
    }
    zfstlpair<_ZFP_ZFSetPrivate::MapType::iterator, bool> insertResult = d->data.insert(_ZFP_ZFSetPrivate::MapType::value_type(value, zfnull));
    if(insertResult.second) {
        zfobjRetain(value);
    }
    return d->data.iter(insertResult.first);
}
ZFMETHOD_DEFINE_2(ZFSet, zfiter, iterAdd
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    return this->iterAdd(value);
}

ZF_NAMESPACE_GLOBAL_END

