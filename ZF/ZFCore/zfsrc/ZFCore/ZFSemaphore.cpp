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
    d->impl->nativeSemaphoreDestroy(this);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void *, nativeSemaphore) {
    return d->nativeSemaphore;
}

ZFMETHOD_DEFINE_0(ZFSemaphore, void, semaphoreLock) {
    d->impl->semaphoreLock(this);
}
ZFMETHOD_DEFINE_0(ZFSemaphore, void, semaphoreUnlock) {
    d->impl->semaphoreUnlock(this);
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
    d->impl->semaphoreLock(this);
    d->impl->semaphoreSignal(this);
    d->impl->semaphoreUnlock(this);
}
ZFMETHOD_DEFINE_0(ZFSemaphore, void, lockAndBroadcast) {
    d->impl->semaphoreLock(this);
    d->impl->semaphoreBroadcast(this);
    d->impl->semaphoreUnlock(this);
}
ZFMETHOD_DEFINE_0(ZFSemaphore, void, lockAndWait) {
    d->impl->semaphoreLock(this);
    d->impl->semaphoreWait(this);
    d->impl->semaphoreUnlock(this);
}
ZFMETHOD_DEFINE_1(ZFSemaphore, zfbool, lockAndWait
        , ZFMP_IN(zftimet, miliSecs)
        ) {
    if(miliSecs == zftimetInvalid()) {
        d->impl->semaphoreLock(this);
        d->impl->semaphoreWait(this);
        d->impl->semaphoreUnlock(this);
        return zftrue;
    }
    else {
        d->impl->semaphoreLock(this);
        zfbool ret = d->impl->semaphoreWait(this, miliSecs);
        d->impl->semaphoreUnlock(this);
        return ret;
    }
}

ZF_NAMESPACE_GLOBAL_END

