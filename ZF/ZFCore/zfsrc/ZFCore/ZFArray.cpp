#include "ZFArray.h"
#include "ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFArrayPrivate
zfclassNotPOD _ZFP_ZFArrayPrivate {
public:
    zfstldeque<ZFObject *> data;
};

// ============================================================
// ZFArray
ZFOBJECT_REGISTER(ZFArray)

ZFOBJECT_ON_INIT_DEFINE_1(ZFArray
        , ZFMP_IN(ZFContainer *, another)
        ) {
    this->objectOnInit();
    zfself::addFrom(another);
}
void ZFArray::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFArrayPrivate);
}
void ZFArray::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFArray, zfindex, count) {
    return (zfindex)(d->data.size());
}
ZFMETHOD_DEFINE_0(ZFArray, zfbool, isEmpty) {
    return d->data.empty();
}
ZFMETHOD_DEFINE_2(ZFArray, zfbool, isContain
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerDefault)
        ) {
    return (this->find(obj, comparer) != zfindexMax());
}
ZFMETHOD_DEFINE_1(ZFArray, zfany, get
        , ZFMP_IN(zfindex, index)
        ) {
    ZFCoreAssertIndexRange(index, this->count());
    return d->data[index];
}
ZFMETHOD_DEFINE_0(ZFArray, zfany, getFirst) {
    if(d->data.empty()) {
        return zfnull;
    }
    return d->data[0];
}
ZFMETHOD_DEFINE_0(ZFArray, zfany, getLast) {
    if(d->data.empty()) {
        return zfnull;
    }
    return d->data[d->data.size() - 1];
}
ZFMETHOD_DEFINE_2(ZFArray, zfindex, find
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerDefault)
        ) {
    if(comparer == zfnull) {
        return zfindexMax();
    }
    for(zfstlsize i = 0; i < d->data.size(); ++i) {
        if(comparer(d->data[i], obj) == ZFCompareEqual) {
            return (zfindex)i;
        }
    }
    return zfindexMax();
}
ZFMETHOD_DEFINE_2(ZFArray, zfindex, findReversely
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerDefault)
        ) {
    if(comparer == zfnull) {
        return zfindexMax();
    }
    for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i) {
        if(comparer(d->data[i], obj) == ZFCompareEqual) {
            return (zfindex)i;
        }
    }
    return zfindexMax();
}
ZFMETHOD_DEFINE_2(ZFArray, void, add
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN(zfindex, indexAddTo)
        ) {
    if(indexAddTo == zfindexMax()) {
        indexAddTo = this->count();;
    }
    else {
        ZFCoreAssertIndexRange(indexAddTo, this->count() + 1);
    }
    ZFCoreAssertWithMessage(obj != zfnull, "insert null object");
    zfRetain(obj);
    d->data.insert(d->data.begin() + indexAddTo, obj);

    this->contentOnAdd(obj);
    this->contentOnUpdate();
}
ZFMETHOD_DEFINE_1(ZFArray, void, add
        , ZFMP_IN(ZFObject *, obj)
        ) {
    ZFCoreAssertWithMessage(obj != zfnull, "insert null object");
    zfRetain(obj);
    d->data.push_back(obj);

    this->contentOnAdd(obj);
    this->contentOnUpdate();
}
ZFMETHOD_DEFINE_1(ZFArray, void, addFrom
        , ZFMP_IN(ZFContainer *, another)
        ) {
    if(another == this || another == zfnull) {
        return;
    }
    ZFObject *obj = zfnull;
    for(zfiter it = another->iter(); it; ++it) {
        obj = another->iterValue(it);
        zfRetain(obj);
        d->data.push_back(obj);
        this->contentOnAdd(obj);
    }
    if(obj != zfnull) {
        this->contentOnUpdate();
    }
}

ZFMETHOD_DEFINE_2(ZFArray, void, set
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(ZFObject *, obj)
        ) {
    ZFCoreAssertIndexRange(index, this->count());
    ZFCoreAssertWithMessage(obj != zfnull, "set null object");
    zfRetain(obj);

    ZFObject *old = d->data[index];
    d->data[index] = obj;

    this->contentOnRemove(old);
    this->contentOnAdd(obj);
    this->contentOnUpdate();

    zfRelease(old);
}

ZFMETHOD_DEFINE_1(ZFArray, zfbool, removeElement
        , ZFMP_IN(ZFObject *, obj)
        ) {
    if(obj) {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); ) {
            if((*it)->objectCompare(obj) == ZFCompareEqual) {
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnUpdate();
                return zftrue;
            }
            else {
                ++it;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_2(ZFArray, zfbool, removeElement
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer)
        ) {
    if(obj && comparer) {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); ) {
            if(comparer(*it, obj) == ZFCompareEqual) {
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnUpdate();
                return zftrue;
            }
            else {
                ++it;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_1(ZFArray, zfbool, removeElementRevsersely
        , ZFMP_IN(ZFObject *, obj)
        ) {
    if(obj) {
        for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i) {
            if(d->data[i]->objectCompare(obj) == ZFCompareEqual) {
                ZFObject *toRelease = d->data[i];
                d->data.erase(d->data.begin() + i);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnUpdate();
                return zftrue;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_2(ZFArray, zfbool, removeElementRevsersely
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer)
        ) {
    if(obj && comparer) {
        for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i) {
            if(comparer(d->data[i], obj) == ZFCompareEqual) {
                ZFObject *toRelease = d->data[i];
                d->data.erase(d->data.begin() + i);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnUpdate();
                return zftrue;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_1(ZFArray, zfindex, removeElementAll
        , ZFMP_IN(ZFObject *, obj)
        ) {
    zfindex removedCount = 0;
    if(obj) {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); ) {
            if((*it)->objectCompare(obj) == ZFCompareEqual) {
                ++removedCount;
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
            }
            else {
                ++it;
            }
        }
    }
    if(removedCount > 0) {
        this->contentOnUpdate();
    }
    return removedCount;
}
ZFMETHOD_DEFINE_2(ZFArray, zfindex, removeElementAll
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer)
        ) {
    zfindex removedCount = 0;
    if(obj && comparer) {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); ) {
            if(comparer(*it, obj) == ZFCompareEqual) {
                ++removedCount;
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
            }
            else {
                ++it;
            }
        }
    }
    if(removedCount > 0) {
        this->contentOnUpdate();
    }
    return removedCount;
}

ZFMETHOD_DEFINE_2(ZFArray, void, remove
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN_OPT(zfindex, count, 1)
        ) {
    ZFCoreAssertIndexRange(index, this->count());
    if(count == 1) {
        ZFObject *tmp = d->data[index];
        zfblockedRelease(tmp);
        d->data.erase(d->data.begin() + index);

        this->contentOnRemove(tmp);
        this->contentOnUpdate();
    }
    else if(count > 1) {
        if(count > this->count() - index) {
            count = this->count() - index;
        }
        zfstldeque<ZFObject *> tmp(
            d->data.begin() + index,
            d->data.begin() + (index + count));
        d->data.erase(d->data.begin() + index, d->data.begin() + (index + count));
        for(zfstldeque<ZFObject *>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            this->contentOnRemove(*it);
            zfRelease(*it);
        }

        if(!tmp.empty()) {
            this->contentOnUpdate();
        }
    }
}
ZFMETHOD_DEFINE_1(ZFArray, zfauto, removeAndGet
        , ZFMP_IN(zfindex, index)
        ) {
    zfauto t = this->get(index);
    this->remove(index);
    return t;
}
ZFMETHOD_DEFINE_0(ZFArray, void, removeFirst) {
    this->remove(0);
}
ZFMETHOD_DEFINE_0(ZFArray, zfauto, removeFirstAndGet) {
    zfauto t = this->getFirst();
    this->removeFirst();
    return t;
}
ZFMETHOD_DEFINE_0(ZFArray, void, removeLast) {
    this->remove(this->count() - 1);
}
ZFMETHOD_DEFINE_0(ZFArray, zfauto, removeLastAndGet) {
    zfauto t = this->getLast();
    this->removeLast();
    return t;
}
ZFMETHOD_DEFINE_0(ZFArray, void, removeAll) {
    if(!d->data.empty()) {
        zfstldeque<ZFObject *> tmp;
        tmp.swap(d->data);

        this->contentOnUpdate();

        for(zfstldeque<ZFObject *>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            this->contentOnRemove(*it);
            zfRelease(*it);
        }
    }
}

ZFMETHOD_DEFINE_2(ZFArray, void, move
        , ZFMP_IN(zfindex, fromIndex)
        , ZFMP_IN(zfindex, toIndexOrIndexMax)
        ) {
    ZFCoreAssertIndexRange(fromIndex, this->count());
    if(toIndexOrIndexMax == zfindexMax()) {
        toIndexOrIndexMax = this->count() - 1;
    }
    else {
        ZFCoreAssertIndexRange(toIndexOrIndexMax, this->count());
    }
    if(fromIndex == toIndexOrIndexMax) {
        return;
    }
    ZFObject *t = d->data[fromIndex];
    if(fromIndex < toIndexOrIndexMax) {
        for(zfindex i = fromIndex; i < toIndexOrIndexMax; ++i) {
            d->data[i] = d->data[i + 1];
        }
    }
    else {
        for(zfindex i = fromIndex; i > toIndexOrIndexMax; --i) {
            d->data[i] = d->data[i - 1];
        }
    }
    d->data[toIndexOrIndexMax] = t;

    this->contentOnUpdate();
}

ZFMETHOD_DEFINE_3(ZFArray, void, sort
        , ZFMP_IN_OPT(zfindex, start, 0)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        , ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerDefault)
        ) {
    if(d->data.size() > 0 && start + 1 < d->data.size() && count > 1) {
        zfmSort<ZFObject *>(
            d->data,
            start,
            (count > d->data.size() - start) ? (d->data.size() - 1) : (start + count - 1),
            comparer);

        this->contentOnUpdate();
    }
}
ZFMETHOD_DEFINE_3(ZFArray, void, sortReversely
        , ZFMP_IN_OPT(zfindex, start, 0)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        , ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerDefault)
        ) {
    if(d->data.size() > 0 && start + 1 < d->data.size() && count > 1) {
        zfmSortReversely<ZFObject *>(
            d->data,
            start,
            (count > d->data.size() - start) ? (d->data.size() - 1) : (start + count - 1),
            comparer);

        this->contentOnUpdate();
    }
}

// ============================================================
zfclassNotPOD _ZFP_ZFArrayIter : zfextend zfiter::Impl {
public:
    zfstldeque<ZFObject *> *owner;
    zfstldeque<ZFObject *>::iterator impl;
public:
    zfoverride
    virtual zfbool valid(void) {
        return impl != owner->end();
    }
    zfoverride
    virtual void next(void) {
        ++impl;
    }
    zfoverride
    virtual Impl *copy(void) {
        _ZFP_ZFArrayIter *ret = zfpoolNew(_ZFP_ZFArrayIter);
        ret->owner = owner;
        ret->impl = impl;
        return ret;
    }
    zfoverride
    virtual void destroy(void) {
        zfpoolDelete(this);
    }
    zfoverride
    virtual zfbool isEqual(ZF_IN Impl *d) {
        _ZFP_ZFArrayIter *t = (_ZFP_ZFArrayIter *)d;
        return (owner == t->owner && impl == t->impl);
    }
};
ZFMETHOD_DEFINE_1(ZFArray, zfiter, iterForIndex
        , ZFMP_IN(zfindex, index)
        ) {
    ZFCoreAssertIndexRange(index, this->count());
    _ZFP_ZFArrayIter *impl = zfpoolNew(_ZFP_ZFArrayIter);
    impl->owner = &(d->data);
    impl->impl = d->data.begin() + index;
    return zfiter(impl);
}

ZFMETHOD_DEFINE_0(ZFArray, zfiter, iter) {
    _ZFP_ZFArrayIter *impl = zfpoolNew(_ZFP_ZFArrayIter);
    impl->owner = &(d->data);
    impl->impl = d->data.begin();
    return zfiter(impl);
}

ZFMETHOD_DEFINE_1(ZFArray, zfiter, iterFind
        , ZFMP_IN(ZFObject *, element)
        ) {
    return this->iterForIndex(this->find(element));
}

ZFMETHOD_DEFINE_1(ZFArray, zfany, iterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return *(it.impl<_ZFP_ZFArrayIter *>()->impl);
}

ZFMETHOD_DEFINE_2(ZFArray, void, iterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->set(it.impl<_ZFP_ZFArrayIter *>()->impl - d->data.begin(), value);
}
ZFMETHOD_DEFINE_1(ZFArray, void, iterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    this->remove(it.impl<_ZFP_ZFArrayIter *>()->impl - d->data.begin());
}
ZFMETHOD_DEFINE_1(ZFArray, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        ) {
    this->add(value);
}
ZFMETHOD_DEFINE_2(ZFArray, void, iterAdd
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    if(it) {
        _ZFP_ZFArrayIter *impl = it.impl<_ZFP_ZFArrayIter *>();
        this->add(value, impl->impl - d->data.begin());
    }
    else {
        this->add(value);
    }
}

ZF_NAMESPACE_GLOBAL_END

