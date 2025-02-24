#include "ZFSemaphore.h"
#include "protocol/ZFProtocolZFSemaphore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFSemaphorePrivate {
public:
    void *nativeSemaphore;
    ZFPROTOCOL_INTERFACE_CLASS(ZFSemaphore) *impl;
};

// ============================================================
// ZFSemaphore
ZFOBJECT_REGISTER(ZFSemaphore)
void ZFSemaphore::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFSemaphorePrivate);
    d->impl = ZFPROTOCOL_ACCESS(ZFSemaphore);
    d->nativeSemaphore = d->impl->nativeSemaphoreCreate(this);
}
void ZFSemaphore::objectOnDealloc(void) {
    d->impl->nativeSemaphoreDestroy(this, d->nativeSemaphore);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void *, nativeSemaphore) {
    return d->nativeSemaphore;
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void, semaphoreLock) {
    zfRetain(this);
    d->impl->semaphoreLock(this);
}
ZFMETHOD_DEFINE_0(ZFSemaphore, void, semaphoreUnlock) {
    d->impl->semaphoreUnlock(this);
    zfRelease(this);
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void, semaphoreSignal) {
    d->impl->semaphoreSignal(this);
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void, semaphoreBroadcast) {
    d->impl->semaphoreBroadcast(this);
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void, semaphoreWait) {
    d->impl->semaphoreWait(this);
}

ZFMETHOD_DEFINE_1(ZFSemaphore, zfbool, semaphoreWait
        , ZFMP_IN(zftimet, miliSecs)
        ) {
    return d->impl->semaphoreWait(this, miliSecs);
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void, lockAndSignal) {
    zfRetain(this);
    d->impl->semaphoreLock(this);
    d->impl->semaphoreSignal(this);
    d->impl->semaphoreUnlock(this);
    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFSemaphore, void, lockAndBroadcast) {
    zfRetain(this);
    d->impl->semaphoreLock(this);
    d->impl->semaphoreBroadcast(this);
    d->impl->semaphoreUnlock(this);
    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFSemaphore, void, lockAndWait) {
    zfRetain(this);
    d->impl->semaphoreLock(this);
    d->impl->semaphoreWait(this);
    d->impl->semaphoreUnlock(this);
    zfRelease(this);
}
ZFMETHOD_DEFINE_1(ZFSemaphore, zfbool, lockAndWait
        , ZFMP_IN(zftimet, miliSecs)
        ) {
    if(miliSecs == zftimetInvalid()) {
        zfRetain(this);
        d->impl->semaphoreLock(this);
        d->impl->semaphoreWait(this);
        d->impl->semaphoreUnlock(this);
        zfRelease(this);
        return zftrue;
    }
    else {
        zfRetain(this);
        d->impl->semaphoreLock(this);
        zfbool ret = d->impl->semaphoreWait(this, miliSecs);
        d->impl->semaphoreUnlock(this);
        zfRelease(this);
        return ret;
    }
}

ZF_NAMESPACE_GLOBAL_END

