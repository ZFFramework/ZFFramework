#include "ZFSet.h"
#include "ZFMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFSet
ZFOBJECT_REGISTER(ZFSet)

ZFOBJECT_ON_INIT_DEFINE_1(ZFSet
        , ZFMP_IN(ZFContainer *, another)
        ) {
    this->objectOnInit();
    zfself::addFrom(another);
}
void ZFSet::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfAlloc(ZFMap);
}
void ZFSet::objectOnDealloc(void) {
    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFSet, zfindex, count) {
    return d->count();
}
ZFMETHOD_DEFINE_0(ZFSet, zfbool, isEmpty) {
    return d->isEmpty();
}
ZFMETHOD_DEFINE_1(ZFSet, zfbool, isContain
        , ZFMP_IN(ZFObject *, obj)
        ) {
    return d->isContain(obj);
}

ZFMETHOD_DEFINE_1(ZFSet, void, add
        , ZFMP_IN(ZFObject *, obj)
        ) {
    ZFCoreAssertWithMessage(obj != zfnull, "insert null object");
    if(!d->isContain(obj)) {
        d->set(obj, ZFNull());
        this->contentOnAdd(obj);
        this->contentOnUpdate();
    }
}
ZFMETHOD_DEFINE_1(ZFSet, void, addFrom
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

ZFMETHOD_DEFINE_1(ZFSet, void, removeElement
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
ZFMETHOD_DEFINE_0(ZFSet, void, removeAll) {
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
ZFMETHOD_DEFINE_0(ZFSet, zfiter, iter) {
    return d->iter();
}

ZFMETHOD_DEFINE_1(ZFSet, zfiter, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFSet, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return d->iterValue(it);
}

ZFMETHOD_DEFINE_2(ZFSet, void, iterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    zfRetain(value);
    this->iterRemove(it);
    this->add(value);
    zfRelease(value);
}
ZFMETHOD_DEFINE_1(ZFSet, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    zfauto key = d->iterKey(it);
    if(key != zfnull) {
        d->iterRemove(it);
        this->contentOnRemove(key);
        this->contentOnUpdate();
    }
}
ZFMETHOD_DEFINE_1(ZFSet, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->add(value);
}
ZFMETHOD_DEFINE_2(ZFSet, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

