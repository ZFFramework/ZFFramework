#include "ZFThread.h"
#include "protocol/ZFProtocolZFThread.h"
#include "ZFAutoReleasePool.h"
#include "ZFSemaphore.h"

#include "ZFCore/ZFSTLWrapper/zfstl_list.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFThreadImpl ZFPROTOCOL_ACCESS(ZFThread)

// ============================================================
// _ZFP_ZFThreadPrivate
zfclassLikePOD _ZFP_ZFThreadTaskData
{
public:
    ZFListener task;
    zfautoObject taskUserData;
};

zfclassNotPOD _ZFP_ZFThreadPrivate
{
public:
    zfbool startFlag;
    zfbool runningFlag;
    zfbool stopRequestedFlag;
    ZFAutoReleasePool *autoReleasePool;
    ZFSemaphore *threadWaitSema;

    zfbool taskQueueInitFlag;
    ZFSemaphore *taskQueueSema;
    zfstllist<_ZFP_ZFThreadTaskData> taskQueue;

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
    virtual void threadStart(ZF_IN_OPT ZFObject *userData = zfnull)
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

// ============================================================
// ZFThread
ZFOBJECT_REGISTER(ZFThread)

ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStop)

ZFMETHOD_DEFINE_0(ZFThread, void *, nativeThreadRegister)
{
    return _ZFP_ZFThreadImpl->nativeThreadRegister(zfAlloc(_ZFP_I_ZFThreadNativeRegisterThread));
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
        _ZFP_ZFThreadTaskData task = *(zfThread->d->taskQueue.begin());
        zfThread->d->taskQueue.pop_front();
        if(!task.task)
        {
            continue;
        }
        zfCoreMutexUnlock();
        task.task.execute(ZFListenerData(zfidentityInvalid(), zfThread), task.taskUserData);
        zfCoreMutexLock();
        zfThread->d->autoReleasePool->poolDrain();
    }
    zfCoreMutexUnlock();
    zfThread->taskQueueCleanup();

    zfThread->d->threadWaitSema->semaphoreBroadcast();
    _ZFP_ZFThreadImpl->nativeThreadUnregister(token);
    zfRelease(zfThread);
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

ZFMETHOD_DEFINE_3(ZFThread, ZFThread *, executeInThread,
                  ZFMP_IN(ZFThread *, thread),
                  ZFMP_IN(const ZFListener &, callback),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    ZFThread *ret = (thread != zfnull && thread->taskQueueAvailable() ? thread : ZFThread::mainThread());
    ret->taskQueueAdd(callback, userData);
    return ret;
}

ZFMETHOD_DEFINE_2(ZFThread, ZFThread *, post,
                  ZFMP_IN(const ZFListener &, callback),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    ZFThread *ret = ZFThread::currentThread();
    if(ret == zfnull || !ret->taskQueueAvailable())
    {
        ret = ZFThread::mainThread();
    }
    ret->taskQueueAdd(callback, userData);
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
ZFMETHOD_DEFINE_1(ZFThread, void, threadStart,
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull))
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
    zfunsafe_zfRetain(userData);
    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread begin %p", this);
    d->threadToken = _ZFP_ZFThreadImpl->executeInNewThread(
        ZFCallbackForFunc(ZFThread::_ZFP_ZFThread_threadCallback),
        ZFCallbackForFunc(ZFThread::_ZFP_ZFThread_threadCleanupCallback),
        this,
        userData);
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

ZFMETHOD_DEFINE_2(ZFThread, void, threadOnRun,
                  ZFMP_IN(const ZFListenerData &, listenerData),
                  ZFMP_IN(ZFObject *, userData))
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
ZFMETHOD_DEFINE_2(ZFThread, void, taskQueueAdd,
                  ZFMP_IN(const ZFListener &, task),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    if(!task)
    {
        return;
    }
    zfCoreMutexLocker();
    _ZFP_ZFThreadTaskData taskData;
    taskData.task = task;
    taskData.taskUserData = userData;

    if(this->isMainThread())
    {
        d->taskQueue.push_back(taskData);
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
            d->taskQueue.push_back(taskData);
            d->taskQueueSema->semaphoreBroadcast();
        }
        else
        {
            zfCoreLogTrim("taskQueueAdd() called without taskQueueInit(), thread: %s", this->objectInfo().cString());
            ZFThread::mainThread()->taskQueueAdd(task, userData);
        }
    }
}
ZFMETHOD_DEFINE_2(ZFThread, void, taskQueueRemove,
                  ZFMP_IN(const ZFListener &, task),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    zfCoreMutexLocker();
    for(zfstllist<_ZFP_ZFThreadTaskData>::iterator it = d->taskQueue.begin(); it != d->taskQueue.end(); ++it)
    {
        if(it->task == task && it->taskUserData == userData)
        {
            d->taskQueue.erase(it);
            break;
        }
    }
}

// ============================================================
void ZFThread::_ZFP_ZFThread_threadCallback(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfCoreMutexLock();
    ZFThread *zfThread = listenerData.param0<ZFThread *>();
    _ZFP_ZFThreadPrivate *d = zfThread->d;
    ZFObject *zfThreadUserData = listenerData.param1();
    d->runningFlag = zftrue;

    _ZFP_ZFThreadImpl->nativeThreadRegister(zfThread);
    zfCoreMutexUnlock();

    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread enter %p", zfThread);

    zfThread->threadOnStart();

    if(!d->stopRequestedFlag)
    {
        zfThread->threadOnRun(ZFListenerData(zfidentityInvalid(), zfThread), zfThreadUserData);
    }
    zfCoreMutexLock();
    if(zfThread->threadRunnable())
    {
        ZFListener tmp = zfThread->threadRunnable();
        zfCoreMutexUnlock();
        tmp.execute(ZFListenerData(zfidentityInvalid(), zfThread), zfThreadUserData);
    }
    else
    {
        zfCoreMutexUnlock();
    }

    do {
        d->autoReleasePool->poolDrain();

        zfCoreMutexLock();
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

        ZFListener task = d->taskQueue.begin()->task;
        zfautoObject taskUserData = d->taskQueue.begin()->taskUserData;
        d->taskQueue.pop_front();

        zfCoreMutexUnlock();

        if(task)
        {
            task.execute(ZFListenerData(zfidentityInvalid(), zfThread), taskUserData);
        }
    } while(zftrue);

    zfThread->threadOnStop();

    zfCoreMutexLock();
    _ZFP_ZFThreadImpl->nativeThreadUnregister(zfThread);
    d->runningFlag = zffalse;
    d->startFlag = zffalse;
    zfCoreMutexUnlock();
}
void ZFThread::_ZFP_ZFThread_threadCleanupCallback(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfCoreMutexLocker();

    ZFThread *zfThread = listenerData.param0<ZFThread *>();
    _ZFP_ZFThreadPrivate *d = zfThread->d;
    ZFObject *zfThreadUserData = listenerData.param1();

    if(d->threadToken != zfnull)
    {
        _ZFP_ZFThreadLog("[ZFThread] executeInMainThread cleanup by run %p", zfThread);
        _ZFP_ZFThreadImpl->executeInNewThreadCleanup(d->threadToken);
        d->threadToken = zfnull;
    }

    d->threadWaitSema->semaphoreBroadcast();

    zfunsafe_zfRelease(zfThread);
    zfunsafe_zfRelease(zfThreadUserData);

    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread exit %p", zfThread);
}

void ZFThread::_ZFP_ZFThread_mainThreadCallback(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFThread *zfThread = listenerData.param0<ZFThread *>();
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
        d->autoReleasePool->poolDrain();

        zfCoreMutexLock();
        if(d->taskQueue.empty())
        {
            zfCoreMutexUnlock();
            _ZFP_ZFThreadLog("[ZFThread] executeInMainThread exit %p", zfThread);
            break;
        }

        ZFListener task = d->taskQueue.begin()->task;
        zfautoObject taskUserData = d->taskQueue.begin()->taskUserData;
        d->taskQueue.pop_front();

        zfCoreMutexUnlock();

        if(task)
        {
            task.execute(ZFListenerData(zfidentityInvalid(), zfThread), taskUserData);
        }
    } while(zftrue);
}

ZF_NAMESPACE_GLOBAL_END

