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
    zfCoreAssertWithMessage(obj != zfnull, "insert null object");
    if(!d->isContain(obj)) {
        d->set(obj, ZFNull());
        this->contentOnAdd(obj);
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_1(ZFSet, void, addFrom
        , ZFMP_IN(ZFContainer *, another)
        ) {
    if(another == this || another == zfnull) {
        return;
    }

    zfbool changed = zffalse;
    for(zfiterator it = another->iter(); another->iterValid(it); another->iterNext(it)) {
        ZFObject *obj = another->iterValue(it);
        if(!d->isContain(obj)) {
            changed = zftrue;
            d->set(obj, ZFNull());
            this->contentOnAdd(obj);
        }
    }

    if(changed) {
        this->contentOnChange();
    }
}

ZFMETHOD_DEFINE_1(ZFSet, void, removeElement
        , ZFMP_IN(ZFObject *, obj)
        ) {
    zfiterator it = d->iterFind(obj);
    if(d->iterValid(it)) {
        zfautoObject key = d->iterKey(it);
        d->iterRemove(it);
        this->contentOnRemove(key);
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_0(ZFSet, void, removeAll) {
    if(!d->isEmpty()) {
        ZFCoreArray<zfautoObject> tmp;
        tmp.capacity(d->count());
        for(zfiterator it = d->iter(); d->iterValid(it); d->iterNext(it)) {
            tmp.add(d->iterValue(it));
        }
        d->removeAll();

        for(zfindex i = 0; i < tmp.count(); ++i) {
            this->contentOnRemove(tmp[i]);
        }
        this->contentOnChange();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFSet, zfiterator, iter) {
    return d->iter();
}

ZFMETHOD_DEFINE_1(ZFSet, zfiterator, iterFind
        , ZFMP_IN(ZFObject *, key)
        ) {
    return d->iterFind(key);
}

ZFMETHOD_DEFINE_1(ZFSet, zfbool, iterValid
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return d->iterValid(it);
}

ZFMETHOD_DEFINE_1(ZFSet, void, iterNext
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    d->iterNext(it);
}

ZFMETHOD_DEFINE_1(ZFSet, ZFObject *, iterValue
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return d->iterValue(it);
}

ZFMETHOD_DEFINE_2(ZFSet, void, iterValue
        , ZFMP_IN_OUT(zfiterator &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    zfRetain(value);
    this->iterRemove(it);
    this->add(value);
    zfRelease(value);
}
ZFMETHOD_DEFINE_1(ZFSet, void, iterRemove
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    zfautoObject key = d->iterKey(it);
    if(key != zfnull) {
        d->iterRemove(it);
        this->contentOnRemove(key);
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_1(ZFSet, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->add(value);
}
ZFMETHOD_DEFINE_2(ZFSet, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

