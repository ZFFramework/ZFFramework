#include "ZFAutoReleasePool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFAutoReleasePoolPrivate {
public:
    zfindex maxSize;
    ZFCoreArray<ZFObject *> array;
public:
    _ZFP_ZFAutoReleasePoolPrivate(void)
    : maxSize(zfindexMax())
    , array()
    {
    }
};

ZFOBJECT_REGISTER(ZFAutoReleasePool)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFAutoReleasePool, instance, ZFLevelZFFrameworkHigh)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAutoReleasePoolCleanup, ZFLevelZFFrameworkNormal) {
    ZFAutoReleasePool::instance()->poolDrain();
}
ZF_GLOBAL_INITIALIZER_END(ZFAutoReleasePoolCleanup)

ZFOBJECT_ON_INIT_DEFINE_1(ZFAutoReleasePool
        , ZFMP_IN(zfindex, maxSize)
        ) {
    d->maxSize = maxSize;
}
void ZFAutoReleasePool::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAutoReleasePoolPrivate);
}
void ZFAutoReleasePool::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAutoReleasePool::objectOnDeallocPrepare(void) {
    this->poolDrain();
    zfsuper::objectOnDeallocPrepare();
}
void ZFAutoReleasePool::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ret += ", count: ";
    zfsFromIntT(ret, d->array.count());
}
void ZFAutoReleasePool::poolAdd(ZF_IN ZFObject *obj) {
    ZFCoreMutexLocker();
    ZFCoreAssertWithMessage(obj != this, "add autorelease pool to itself isn't allowed");

    if(obj != zfnull) {
        if(d->array.count() >= d->maxSize) {
            ZFCoreLogTrim("warning, auto release pool full, trying to release old object");
            ZFObject *data = d->array.get(0);
            d->array.remove(0);
            zfobjRelease(data);
        }
        d->array.add(obj);
    }
}
void ZFAutoReleasePool::poolDrain() {
    if(!d->array.isEmpty()) {
        ZFCoreMutexLock();
        ZFCoreArray<ZFObject *> tmp;
        tmp.swap(d->array);
        ZFCoreMutexUnlock();

        for(zfindex i = 0; i < tmp.count(); ++i) {
            zfobjRelease(tmp[i]);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

