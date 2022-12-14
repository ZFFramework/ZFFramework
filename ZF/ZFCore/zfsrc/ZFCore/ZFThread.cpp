#include "ZFThread.h"
#include "protocol/ZFProtocolZFThread.h"
#include "ZFAutoReleasePool.h"
#include "ZFSemaphore.h"

#include "ZFCore/ZFSTLWrapper/zfstllist.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFThreadImpl ZFPROTOCOL_ACCESS(ZFThread)

static ZFCoreArrayPOD<ZFThread *> _ZFP_ZFThread_allThread;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThread_allThreadCleanup, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThread_allThreadCleanup)
{
    _ZFP_ZFThread_allThread.removeAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFThread_allThreadCleanup)

// ============================================================
// _ZFP_ZFThreadPrivate
zfclassNotPOD _ZFP_ZFThreadPrivate
{
public:
    zfbool startFlag;
    zfbool runningFlag;
    zfbool stopRequestedFlag;
    ZFAutoReleasePool *autoReleasePool;
    ZFSemaphore *threadWaitSema;

    zfbool taskQueueInitFlag;
    zfbool taskQueueRunning;
    ZFSemaphore *taskQueueSema;
    zfstllist<ZFListener> taskQueue;

    void *threadToken;
    zfbool mainThreadTaskRunning;
    void *mainThreadToken;

public:
    _ZFP_ZFThreadPrivate(void)
    : startFlag(zffalse)
    , runningFlag(zffalse)
    , stopRequestedFlag(zffalse)
    , autoReleasePool(zfAlloc(ZFAutoReleasePool))
    , threadWaitSema(zfAlloc(ZFSemaphore))
    , taskQueueInitFlag(zffalse)
    , taskQueueRunning(zffalse)
    , taskQueueSema(zfnull)
    , taskQueue()
    , threadToken(zfnull)
    , mainThreadTaskRunning(zffalse)
    , mainThreadToken(zfnull)
    {
    }
    ~_ZFP_ZFThreadPrivate(void)
    {
        zfRelease(this->autoReleasePool);
        zfRelease(this->threadWaitSema);
        zfRelease(this->taskQueueSema);
    }
};

// ============================================================
// user registered thread
zfclass _ZFP_I_ZFThreadNativeRegisterThread : zfextends ZFThread
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadNativeRegisterThread, ZFThread)

public:
    zfoverride
    virtual zfbool isNativeThread(void)
    {
        return zftrue;
    }

    zfoverride
    virtual void threadStart(ZF_IN_OPT ZFObject *param0 = zfnull, ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        zfCoreLogTrim("you must not start a user registered thread");
    }
    zfoverride
    virtual zfbool threadStarted(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool threadRunning(void)
    {
        return zftrue;
    }
    zfoverride
    virtual void threadStop(void)
    {
        zfCoreLogTrim("you must not stop a user registered thread");
    }
};

zfclass _ZFP_I_ZFThreadParamHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadParamHolder, ZFObject)
    ZFALLOC_CACHE_RELEASE({
            cache->param0 = zfnull;
            cache->param1 = zfnull;
        })

public:
    zfautoObject param0;
    zfautoObject param1;
};

// ============================================================
// ZFThread
ZFOBJECT_REGISTER(ZFThread)

ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStop)

ZFMETHOD_DEFINE_0(ZFThread, void *, nativeThreadRegister)
{
    zfCoreMutexLocker();
    ZFThread *zfThread = ZFThread::currentThread();
    zfCoreAssert(zfThread == zfnull);

    zfThread = zfAlloc(_ZFP_I_ZFThreadNativeRegisterThread);
    void *ret = _ZFP_ZFThreadImpl->nativeThreadRegister(zfThread);
    _ZFP_ZFThread_allThread.add(zfThread);
    zfThread->threadOnRegister();
    return ret;
}
ZFMETHOD_DEFINE_1(ZFThread, void, nativeThreadUnregister,
                  ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return;
    }
    ZFThread *zfThread = ZFPROTOCOL_ACCESS(ZFThread)->threadForToken(token);
    zfCoreAssert(zfThread != zfnull);

    zfCoreMutexLock();
    while(!zfThread->d->taskQueue.empty())
    {
        ZFListener task = *(zfThread->d->taskQueue.begin());
        zfThread->d->taskQueue.pop_front();
        if(!task)
        {
            continue;
        }
        zfThread->d->taskQueueRunning = zftrue;
        zfCoreMutexUnlock();
        task.execute(ZFArgs()
                .sender(zfThread)
                .userData(task.userData())
            );
        zfCoreMutexLock();
        zfThread->d->taskQueueRunning = zffalse;
        zfThread->d->autoReleasePool->poolDrain();
    }
    zfThread->taskQueueCleanup();
    zfCoreMutexUnlock();

    zfThread->d->threadWaitSema->semaphoreBroadcast();

    zfCoreMutexLocker();
    zfThread->threadOnUnregister();
    _ZFP_ZFThread_allThread.removeElement(zfThread);
    _ZFP_ZFThreadImpl->nativeThreadUnregister(token);
    zfunsafe_zfRelease(zfThread);
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, implAvailable)
{
    zfCoreMutexLocker();
    return ZFPROTOCOL_IS_AVAILABLE(ZFThread);
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, implMainThreadTaskAvailable)
{
    zfCoreMutexLocker();
    ZFPROTOCOL_INTERFACE_CLASS(ZFThread) *impl = ZFPROTOCOL_TRY_ACCESS(ZFThread);
    if(impl == zfnull)
    {
        return zffalse;
    }
    return impl->executeInMainThreadAvailable();
}

ZFMETHOD_DEFINE_0(ZFThread, const ZFCoreArrayPOD<ZFThread *> &, allThread)
{
    return _ZFP_ZFThread_allThread;
}
ZFMETHOD_DEFINE_0(ZFThread, ZFThread *, mainThread)
{
    return _ZFP_ZFThreadImpl->mainThread();
}
ZFMETHOD_DEFINE_0(ZFThread, ZFThread *, currentThread)
{
    return _ZFP_ZFThreadImpl->currentThread();
}

ZFMETHOD_DEFINE_1(ZFThread, void, sleep,
                  ZFMP_IN(zftimet, miliSecs))
{
    _ZFP_ZFThreadImpl->sleep(miliSecs);
}

ZFMETHOD_DEFINE_2(ZFThread, ZFThread *, executeInThread,
                  ZFMP_IN(ZFThread *, thread),
                  ZFMP_IN(const ZFListener &, callback))
{
    ZFThread *ret = (thread != zfnull && thread->taskQueueAvailable() ? thread : ZFThread::mainThread());
    ret->taskQueueAdd(callback);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFThread, ZFThread *, post,
                  ZFMP_IN(const ZFListener &, callback))
{
    ZFThread *ret = ZFThread::currentThread();
    if(ret == zfnull || !ret->taskQueueAvailable())
    {
        ret = ZFThread::mainThread();
    }
    ret->taskQueueAdd(callback);
    return ret;
}

// ============================================================
// thread instance method
ZFOBJECT_ON_INIT_DEFINE_1(ZFThread,
                          ZFMP_IN(const ZFListener &, runnable))
{
    this->objectOnInit();
    zfself::threadRunnable(runnable);
}

void ZFThread::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFThreadPrivate);
    _ZFP_ZFThreadLog("[ZFThread] init %p", this);
}
void ZFThread::objectOnDealloc(void)
{
    _ZFP_ZFThreadLog("[ZFThread] dealloc %p", this);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFThread::objectOnDeallocPrepare(void)
{
    d->mainThreadTaskRunning = zffalse;
    if(d->mainThreadToken != zfnull)
    {
        _ZFP_ZFThreadLog("[ZFThread] executeInMainThread cleanup by dealloc %p", this);
        _ZFP_ZFThreadImpl->executeInMainThreadCleanup(d->mainThreadToken);
        d->mainThreadToken = zfnull;
    }

    if(!this->isMainThread())
    {
        this->threadWait();
    }
    this->autoReleasePoolDrain();
    zfsuper::objectOnDeallocPrepare();
}

void ZFThread::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->isMainThread())
    {
        ret += " MainThread";
    }
    else if(this->isNativeThread())
    {
        ret += " NativeThread";
    }
}

// ============================================================
ZFMETHOD_DEFINE_2(ZFThread, void, threadStart,
                  ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                  ZFMP_IN_OPT(ZFObject *, param1, zfnull))
{
    zfCoreMutexLocker();
    if(d->startFlag)
    {
        return;
    }
    if(d->stopRequestedFlag)
    {
        zfCoreLogTrim("threadStop has called, you should not start it again");
        return;
    }
    d->startFlag = zftrue;
    zfunsafe_zfRetain(this);
    _ZFP_I_ZFThreadParamHolder *paramHolder = zfunsafe_zfAlloc(_ZFP_I_ZFThreadParamHolder);
    paramHolder->param0 = param0;
    paramHolder->param1 = param1;
    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread begin %p", this);
    d->threadToken = _ZFP_ZFThreadImpl->executeInNewThread(
        ZFCallbackForFunc(ZFThread::_ZFP_ZFThread_threadCallback),
        ZFCallbackForFunc(ZFThread::_ZFP_ZFThread_threadCleanupCallback),
        this,
        paramHolder);
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadStarted)
{
    return d->startFlag;
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadRunning)
{
    return d->runningFlag;
}
ZFMETHOD_DEFINE_0(ZFThread, void, threadStop)
{
    zfCoreMutexLocker();
    if(!d->startFlag)
    {
        return;
    }
    d->stopRequestedFlag = zftrue;
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadStopRequested)
{
    return d->stopRequestedFlag;
}

ZFMETHOD_DEFINE_0(ZFThread, void, threadWait)
{
    zfCoreMutexLock();
    if(!d->startFlag)
    {
        zfCoreMutexUnlock();
        return;
    }
    zfCoreMutexUnlock();
    _ZFP_ZFThreadLog("[ZFThread] threadWait begin %p", this);
    d->threadWaitSema->semaphoreWait();
    _ZFP_ZFThreadLog("[ZFThread] threadWait end %p", this);
}
ZFMETHOD_DEFINE_1(ZFThread, zfbool, threadWait,
                  ZFMP_IN(zftimet, miliSecs))
{
    zfCoreMutexLock();
    if(!d->startFlag)
    {
        zfCoreMutexUnlock();
        return zftrue;
    }
    zfCoreMutexUnlock();
    return d->threadWaitSema->semaphoreWait(miliSecs);
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, isMainThread)
{
    return zffalse;
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, isNativeThread)
{
    return zffalse;
}

void ZFThread::autoReleasePoolAdd(ZF_IN ZFObject *obj)
{
    d->autoReleasePool->poolAdd(obj);
}

void ZFThread::autoReleasePoolDrain()
{
    d->autoReleasePool->poolDrain();
}

ZFMETHOD_DEFINE_1(ZFThread, void, threadOnRun,
                  ZFMP_IN(const ZFArgs &, zfargs))
{
    // nothing to do
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFThread, void, taskQueueInit)
{
    if(this->isMainThread())
    {
        return;
    }
    zfCoreMutexLocker();
    if(d->taskQueueInitFlag)
    {
        return;
    }
    d->taskQueueInitFlag = zftrue;
    d->taskQueueSema = zfunsafe_zfAlloc(ZFSemaphore);
    zfunsafe_zfRetain(this);
}
ZFMETHOD_DEFINE_0(ZFThread, void, taskQueueCleanup)
{
    if(this->isMainThread())
    {
        return;
    }
    zfCoreMutexLock();
    if(!d->taskQueueInitFlag)
    {
        zfCoreMutexUnlock();
        return;
    }
    zfCoreMutexUnlock();
    d->taskQueueInitFlag = zffalse;
    d->taskQueueSema->semaphoreBroadcast();
    zfunsafe_zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, taskQueueAvailable)
{
    return (this->isMainThread() || d->taskQueueInitFlag);
}
ZFMETHOD_DEFINE_0(ZFThread, zfindex, taskQueueCount)
{
    zfCoreMutexLocker();
    return (zfindex)d->taskQueue.size();
}
ZFMETHOD_DEFINE_1(ZFThread, void, taskQueueAdd,
                  ZFMP_IN(const ZFListener &, task))
{
    if(!task)
    {
        return;
    }
    zfCoreMutexLocker();

    if(this->isMainThread())
    {
        d->taskQueue.push_back(task);
        if(!d->mainThreadTaskRunning)
        {
            d->mainThreadTaskRunning = zftrue;
            _ZFP_ZFThreadLog("[ZFThread] executeInMainThread begin %p", this);
            d->mainThreadToken = _ZFP_ZFThreadImpl->executeInMainThread(
                ZFCallbackForFunc(ZFThread::_ZFP_ZFThread_mainThreadCallback),
                this,
                zfnull);
        }
    }
    else
    {
        if(d->taskQueueInitFlag)
        {
            d->taskQueue.push_back(task);
            d->taskQueueSema->semaphoreBroadcast();
        }
        else
        {
            zfCoreLogTrim("taskQueueAdd() called without taskQueueInit(), thread: %s", this->objectInfo().cString());
            ZFThread::mainThread()->taskQueueAdd(task);
        }
    }
}
ZFMETHOD_DEFINE_1(ZFThread, void, taskQueueRemove,
                  ZFMP_IN(const ZFListener &, task))
{
    zfCoreMutexLocker();
    for(zfstllist<ZFListener>::iterator it = d->taskQueue.begin(); it != d->taskQueue.end(); ++it)
    {
        if(*it == task)
        {
            d->taskQueue.erase(it);
            break;
        }
    }
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, taskQueueRunning)
{
    return d->taskQueueRunning;
}

// ============================================================
void ZFThread::_ZFP_ZFThread_threadCallback(ZF_IN const ZFArgs &zfargs)
{
    zfCoreMutexLock();
    ZFThread *zfThread = zfargs.param0T();
    _ZFP_I_ZFThreadParamHolder *paramHolder = zfargs.param1T();
    _ZFP_ZFThreadPrivate *d = zfThread->d;
    d->runningFlag = zftrue;

    void *nativeThreadToken = _ZFP_ZFThreadImpl->nativeThreadRegister(zfThread);
    _ZFP_ZFThread_allThread.add(zfThread);
    zfThread->threadOnRegister();
    zfCoreMutexUnlock();

    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread enter %p", zfThread);

    ZFArgs zfargsTmp;
    zfargsTmp
        .sender(zfThread)
        .param0(paramHolder->param0)
        .param1(paramHolder->param1)
        ;

    zfThread->threadOnStart(zfargsTmp);

    if(!d->stopRequestedFlag)
    {
        zfThread->threadOnRun(zfargsTmp);
    }
    zfCoreMutexLock();
    if(zfThread->threadRunnable())
    {
        ZFListener tmp = zfThread->threadRunnable();
        zfCoreMutexUnlock();
        tmp.execute(zfargsTmp
                .userData(tmp.userData())
            );
        zfargsTmp.userData(zfnull);
    }
    else
    {
        zfCoreMutexUnlock();
    }

    do {
        zfCoreMutexLock();
        d->taskQueueRunning = zffalse;
        d->autoReleasePool->poolDrain();

        if(!d->taskQueueInitFlag)
        {
            zfCoreMutexUnlock();
            break;
        }
        if(d->taskQueue.empty())
        {
            zfCoreMutexUnlock();
            d->taskQueueSema->semaphoreWait();
            continue;
        }

        ZFListener task = *(zfThread->d->taskQueue.begin());
        d->taskQueue.pop_front();

        d->taskQueueRunning = zftrue;
        zfCoreMutexUnlock();
        if(task)
        {
            task.execute(ZFArgs()
                    .sender(zfThread)
                    .userData(task.userData())
                );
        }
    } while(zftrue);

    zfThread->threadOnStop(zfargsTmp);

    zfCoreMutexLock();
    zfThread->threadOnUnregister();
    _ZFP_ZFThread_allThread.removeElement(zfThread);
    _ZFP_ZFThreadImpl->nativeThreadUnregister(nativeThreadToken);
    d->runningFlag = zffalse;
    d->startFlag = zffalse;
    zfCoreMutexUnlock();
}
void ZFThread::_ZFP_ZFThread_threadCleanupCallback(ZF_IN const ZFArgs &zfargs)
{
    zfCoreMutexLocker();

    ZFThread *zfThread = zfargs.param0T();
    _ZFP_I_ZFThreadParamHolder *paramHolder = zfargs.param1T();
    _ZFP_ZFThreadPrivate *d = zfThread->d;

    if(d->threadToken != zfnull)
    {
        _ZFP_ZFThreadLog("[ZFThread] executeInMainThread cleanup by run %p", zfThread);
        _ZFP_ZFThreadImpl->executeInNewThreadCleanup(d->threadToken);
        d->threadToken = zfnull;
    }

    d->threadWaitSema->semaphoreBroadcast();

    zfunsafe_zfRelease(zfThread);
    zfunsafe_zfRelease(paramHolder);

    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread exit %p", zfThread);
}

void ZFThread::_ZFP_ZFThread_mainThreadCallback(ZF_IN const ZFArgs &zfargs)
{
    ZFThread *zfThread = zfargs.param0T();
    _ZFP_ZFThreadPrivate *d = zfThread->d;

    _ZFP_ZFThreadLog("[ZFThread] executeInMainThread enter %p", zfThread);
    zfCoreMutexLock();
    d->mainThreadTaskRunning = zffalse;
    if(d->mainThreadToken != zfnull)
    {
        _ZFP_ZFThreadLog("[ZFThread] executeInMainThread cleanup by run %p", zfThread);
        _ZFP_ZFThreadImpl->executeInMainThreadCleanup(d->mainThreadToken);
        d->mainThreadToken = zfnull;
    }
    zfCoreMutexUnlock();

    do {
        zfCoreMutexLock();
        d->taskQueueRunning = zffalse;
        d->autoReleasePool->poolDrain();

        if(d->taskQueue.empty())
        {
            zfCoreMutexUnlock();
            _ZFP_ZFThreadLog("[ZFThread] executeInMainThread exit %p", zfThread);
            break;
        }

        ZFListener task = *(zfThread->d->taskQueue.begin());
        d->taskQueue.pop_front();

        d->taskQueueRunning = zftrue;
        zfCoreMutexUnlock();

        if(task)
        {
            task.execute(ZFArgs()
                    .sender(zfThread)
                    .userData(task.userData())
                );
        }
    } while(zftrue);
}

ZF_NAMESPACE_GLOBAL_END

