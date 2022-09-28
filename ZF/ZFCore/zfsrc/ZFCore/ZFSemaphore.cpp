#include "ZFSemaphore.h"
#include "protocol/ZFProtocolZFSemaphore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFSemaphorePrivate
{
public:
    void *nativeSemaphore;
    ZFPROTOCOL_INTERFACE_CLASS(ZFSemaphore) *impl;
public:
    void lock(ZF_IN ZFSemaphore *owner)
    {
        zfRetain(owner);
        this->impl->semaphoreLock(owner);
    }
    void unlock(ZF_IN ZFSemaphore *owner)
    {
        this->impl->semaphoreUnlock(owner);
        zfRelease(owner);
    }
};

// ============================================================
// ZFSemaphore
ZFOBJECT_REGISTER(ZFSemaphore)
void ZFSemaphore::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFSemaphorePrivate);
    d->impl = ZFPROTOCOL_ACCESS(ZFSemaphore);
    d->nativeSemaphore = d->impl->nativeSemaphoreCreate(this);
}
void ZFSemaphore::objectOnDealloc(void)
{
    d->impl->nativeSemaphoreDestroy(this, d->nativeSemaphore);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void *ZFSemaphore::nativeSemaphore(void)
{
    return d->nativeSemaphore;
}

void ZFSemaphore::semaphoreSignal(void)
{
    d->lock(this);
    d->impl->semaphoreSignal(this);
    d->unlock(this);
}

void ZFSemaphore::semaphoreBroadcast(void)
{
    d->lock(this);
    d->impl->semaphoreBroadcast(this);
    d->unlock(this);
}

void ZFSemaphore::semaphoreWait(void)
{
    d->lock(this);
    d->impl->semaphoreWait(this);
    d->unlock(this);
}

zfbool ZFSemaphore::semaphoreWait(ZF_IN zftimet miliSecs)
{
    d->lock(this);
    zfbool ret = d->impl->semaphoreWait(this, miliSecs);
    d->unlock(this);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

