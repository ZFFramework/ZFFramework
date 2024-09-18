#include "ZFHashSet.h"
#include "ZFHashMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFHashSet
ZFOBJECT_REGISTER(ZFHashSet)

ZFOBJECT_ON_INIT_DEFINE_1(ZFHashSet
        , ZFMP_IN(ZFContainer *, another)
        ) {
    this->objectOnInit();
    zfself::addFrom(another);
}
void ZFHashSet::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfAlloc(ZFHashMap);
}
void ZFHashSet::objectOnDealloc(void) {
    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFHashSet, zfindex, count) {
    return d->count();
}
ZFMETHOD_DEFINE_0(ZFHashSet, zfbool, isEmpty) {
    return d->isEmpty();
}
ZFMETHOD_DEFINE_1(ZFHashSet, zfbool, isContain
        , ZFMP_IN(ZFObject *, obj)
        ) {
    return d->isContain(obj);
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, add
        , ZFMP_IN(ZFObject *, obj)
        ) {
    ZFCoreAssertWithMessage(obj != zfnull, "insert null object");
    if(!d->isContain(obj)) {
        d->set(obj, ZFNull());
        this->contentOnAdd(obj);
        this->contentOnUpdate();
    }
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, addFrom
        , ZFMP_IN(ZFContainer *, another)
        ) {
    if(another == this || another == zfnull) {
        return;
    }

    zfbool changed = zffalse;
    for(zfiter it = another->iter(); it; ++it) {
        ZFObject *obj = another->iterValue(it);
        if(!d->isContain(obj)) {
            changed = zftrue;
            d->set(obj, ZFNull());
            this->contentOnAdd(obj);
        }
    }

    if(changed) {
        this->contentOnUpdate();
    }
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, removeElement
        , ZFMP_IN(ZFObject *, obj)
        ) {
    zfiter it = d->iterFind(obj);
    if(it) {
        zfauto key = d->iterKey(it);
        d->iterRemove(it);
        this->contentOnRemove(key);
        this->contentOnUpdate();
    }
}
ZFMETHOD_DEFINE_0(ZFHashSet, void, removeAll) {
    if(!d->isEmpty()) {
        ZFCoreArray<zfauto> tmp;
        tmp.capacity(d->count());
        for(zfiter it = d->iter(); it; ++it) {
            tmp.add(d->iterValue(it));
        }
        d->removeAll();

        for(zfindex i = 0; i < tmp.count(); ++i) {
            this->contentOnRemove(tmp[i]);
        }
        this->contentOnUpdate();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFHashSet, zfiter, iter) {
    return d->iter();
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfiter, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->iterValue(it);
}

ZFMETHOD_DEFINE_2(ZFHashSet, void, iterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    zfRetain(value);
    this->iterRemove(it);
    this->add(value);
    zfRelease(value);
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    zfauto key = d->iterKey(it);
    if(key != zfnull) {
        d->iterRemove(it);
        this->contentOnRemove(key);
        this->contentOnUpdate();
    }
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->add(value);
}
ZFMETHOD_DEFINE_2(ZFHashSet, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

