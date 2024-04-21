#include "ZFThread.h"
#include "protocol/ZFProtocolZFThread.h"
#include "ZFAutoReleasePool.h"
#include "ZFSemaphore.h"

#include "ZFCore/ZFSTLWrapper/zfstllist.h"

#if 0
    #define _ZFP_ZFThreadLog(fmt, ...) zfCoreLogTrim(fmt, ##__VA_ARGS__)
#else
    #define _ZFP_ZFThreadLog(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static ZFCoreArray<ZFThread *> _ZFP_ZFThread_allThread;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThread_allThreadCleanup, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThread_allThreadCleanup) {
    zfCoreMutexLock();
    while(!_ZFP_ZFThread_allThread.isEmpty()) {
        ZFThread *zfThread = _ZFP_ZFThread_allThread.getLast();
        zfblockedRelease(zfRetain(zfThread));
        zfCoreAssert(zfThread->threadStarted());
        zfCoreMutexUnlock();
        zfThread->taskQueueCleanup();
        zfThread->threadWait();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFThread_allThreadCleanup)

// ============================================================
// _ZFP_ZFThreadPrivate
zfclassNotPOD _ZFP_ZFThreadPrivate {
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
    ~_ZFP_ZFThreadPrivate(void) {
        zfRelease(this->autoReleasePool);
        zfRelease(this->threadWaitSema);
        zfRelease(this->taskQueueSema);
    }

public:
    static void threadCallback(
            ZF_IN ZFThread *zfThread
            , ZF_IN ZFObject *param0
            , ZF_IN ZFObject *param1
            );
    static void threadCleanupCallback(ZF_IN ZFThread *zfThread);
    static void mainThreadCallback(ZF_IN ZFThread *zfThread);
};

// ============================================================
// user registered thread
zfclass _ZFP_I_ZFThreadNativeRegisterThread : zfextend ZFThread {
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadNativeRegisterThread, ZFThread)

public:
    zfoverride
    virtual zfbool isNativeThread(void) {
        return zftrue;
    }

    zfoverride
    virtual void threadStart(
            ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            ) {
        zfCoreLogTrim("you must not start a user registered thread");
    }
    zfoverride
    virtual zfbool threadStarted(void) {
        return zftrue;
    }
    zfoverride
    virtual zfbool threadRunning(void) {
        return zftrue;
    }
    zfoverride
    virtual void threadStop(void) {
        zfCoreLogTrim("you must not stop a user registered thread");
    }
};

// ============================================================
// ZFThread
ZFOBJECT_REGISTER(ZFThread)

ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadTaskQueueOnFinish)

ZFMETHOD_DEFINE_0(ZFThread, void *, nativeThreadRegister) {
    zfCoreMutexLocker();
    ZFThread *zfThread = ZFThread::currentThread();
    zfCoreAssert(zfThread == zfnull);

    zfThread = zfAlloc(_ZFP_I_ZFThreadNativeRegisterThread);
    void *ret = ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadRegister(zfThread);
    _ZFP_ZFThread_allThread.add(zfThread);
    zfThread->threadOnRegister();
    return ret;
}
ZFMETHOD_DEFINE_1(ZFThread, void, nativeThreadUnregister
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return;
    }
    ZFThread *zfThread = ZFPROTOCOL_ACCESS(ZFThread)->threadForToken(token);
    zfCoreAssert(zfThread != zfnull);

    zfCoreMutexLock();
    while(!zfThread->d->taskQueue.empty()) {
        ZFListener task = *(zfThread->d->taskQueue.begin());
        zfThread->d->taskQueue.pop_front();
        if(!task) {
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

    zfThread->d->threadWaitSema->lockAndBroadcast();

    zfCoreMutexLocker();
    zfThread->threadOnUnregister();
    _ZFP_ZFThread_allThread.removeElement(zfThread);
    ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadUnregister(token);
    zfunsafe_zfRelease(zfThread);
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, implAvailable) {
    zfCoreMutexLocker();
    return ZFPROTOCOL_IS_AVAILABLE(ZFThread);
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, implMainThreadTaskAvailable) {
    zfCoreMutexLocker();
    ZFPROTOCOL_INTERFACE_CLASS(ZFThread) *impl = ZFPROTOCOL_TRY_ACCESS(ZFThread);
    if(impl == zfnull) {
        return zffalse;
    }
    return impl->executeInMainThreadAvailable();
}

ZFMETHOD_DEFINE_0(ZFThread, const ZFCoreArray<ZFThread *> &, allThread) {
    return _ZFP_ZFThread_allThread;
}
ZFMETHOD_DEFINE_0(ZFThread, ZFThread *, mainThread) {
    return ZFPROTOCOL_ACCESS(ZFThread)->mainThread();
}
ZFMETHOD_DEFINE_0(ZFThread, ZFThread *, currentThread) {
    return ZFPROTOCOL_ACCESS(ZFThread)->currentThread();
}

ZFMETHOD_DEFINE_1(ZFThread, void, sleep
        , ZFMP_IN(zftimet, miliSecs)
        ) {
    ZFPROTOCOL_ACCESS(ZFThread)->sleep(miliSecs);
}

ZFMETHOD_DEFINE_2(ZFThread, ZFThread *, executeInThread
        , ZFMP_IN(ZFThread *, thread)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    ZFThread *ret = (thread != zfnull && thread->taskQueueAvailable() ? thread : ZFThread::mainThread());
    ret->taskQueueAdd(callback);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFThread, ZFThread *, post
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    ZFThread *ret = ZFThread::currentThread();
    if(ret == zfnull || !ret->taskQueueAvailable()) {
        ret = ZFThread::mainThread();
    }
    ret->taskQueueAdd(callback);
    return ret;
}

// ============================================================
// thread instance method
ZFOBJECT_ON_INIT_DEFINE_1(ZFThread
        , ZFMP_IN(const ZFListener &, runnable)
        ) {
    this->objectOnInit();
    zfself::threadRunnable(runnable);
}

void ZFThread::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFThreadPrivate);
    _ZFP_ZFThreadLog("[ZFThread] init %s", (const void *)this);
}
void ZFThread::objectOnDealloc(void) {
    _ZFP_ZFThreadLog("[ZFThread] dealloc %s", (const void *)this);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFThread::objectOnDeallocPrepare(void) {
    d->mainThreadTaskRunning = zffalse;
    if(d->mainThreadToken != zfnull) {
        _ZFP_ZFThreadLog("[ZFThread] executeInMainThread cleanup by dealloc %s", (const void *)this);
        ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThreadCleanup(d->mainThreadToken);
        d->mainThreadToken = zfnull;
    }

    if(!this->isMainThread()) {
        this->threadWait();
    }
    this->autoReleasePoolDrain();
    zfsuper::objectOnDeallocPrepare();
}

void ZFThread::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    if(this->isMainThread()) {
        ret += " MainThread";
    }
    else if(this->isNativeThread()) {
        ret += " NativeThread";
    }
}

// ============================================================
ZFMETHOD_DEFINE_2(ZFThread, void, threadStart
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        ) {
    zfCoreMutexLocker();
    if(d->startFlag) {
        return;
    }
    if(d->stopRequestedFlag) {
        zfCoreLogTrim("threadStop has called, you should not start it again");
        return;
    }
    d->startFlag = zftrue;
    zfunsafe_zfRetain(this);
    ZFThread *zfThread = this;
    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread begin %s", (const void *)this);
    ZFLISTENER_3(threadCallback
        , ZFThread *, zfThread
        , zfauto, param0
        , zfauto, param1
        ) {
        _ZFP_ZFThreadPrivate::threadCallback(zfThread, param0, param1);
    } ZFLISTENER_END()
    ZFLISTENER_1(threadCleanupCallback
        , ZFThread *, zfThread
        ) {
        _ZFP_ZFThreadPrivate::threadCleanupCallback(zfThread);
    } ZFLISTENER_END()
    d->threadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThread(threadCallback, threadCleanupCallback);
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadStarted) {
    return d->startFlag;
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadRunning) {
    return d->runningFlag;
}
ZFMETHOD_DEFINE_0(ZFThread, void, threadStop) {
    zfCoreMutexLocker();
    if(!d->startFlag) {
        return;
    }
    d->stopRequestedFlag = zftrue;
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadStopRequested) {
    return d->stopRequestedFlag;
}

ZFMETHOD_DEFINE_0(ZFThread, void, threadWait) {
    zfCoreMutexLock();
    if(!d->startFlag) {
        zfCoreMutexUnlock();
        return;
    }
    zfCoreMutexUnlock();
    _ZFP_ZFThreadLog("[ZFThread] threadWait begin %s", (const void *)this);
    d->threadWaitSema->lockAndWait();
    _ZFP_ZFThreadLog("[ZFThread] threadWait end %s", (const void *)this);
}
ZFMETHOD_DEFINE_1(ZFThread, zfbool, threadWait
        , ZFMP_IN(zftimet, miliSecs)
        ) {
    zfCoreMutexLock();
    if(!d->startFlag) {
        zfCoreMutexUnlock();
        return zftrue;
    }
    zfCoreMutexUnlock();
    return d->threadWaitSema->lockAndWait(miliSecs);
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, isMainThread) {
    return zffalse;
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, isNativeThread) {
    return zffalse;
}

void ZFThread::autoReleasePoolAdd(ZF_IN ZFObject *obj) {
    d->autoReleasePool->poolAdd(obj);
}

void ZFThread::autoReleasePoolDrain() {
    d->autoReleasePool->poolDrain();
}

ZFMETHOD_DEFINE_1(ZFThread, void, threadOnRun
        , ZFMP_IN(const ZFArgs &, zfargs)
        ) {
    // nothing to do
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFThread, void, taskQueueInit) {
    if(this->isMainThread()) {
        return;
    }
    zfCoreMutexLocker();
    if(d->taskQueueInitFlag) {
        return;
    }
    d->taskQueueInitFlag = zftrue;
    d->taskQueueSema = zfunsafe_zfAlloc(ZFSemaphore);
    zfunsafe_zfRetain(this);
}
ZFMETHOD_DEFINE_0(ZFThread, void, taskQueueCleanup) {
    if(this->isMainThread()) {
        return;
    }
    zfCoreMutexLock();
    if(!d->taskQueueInitFlag) {
        zfCoreMutexUnlock();
        return;
    }
    zfCoreMutexUnlock();
    d->taskQueueInitFlag = zffalse;
    d->taskQueueSema->lockAndBroadcast();
    zfunsafe_zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, taskQueueAvailable) {
    return (this->isMainThread() || d->taskQueueInitFlag);
}
ZFMETHOD_DEFINE_0(ZFThread, zfindex, taskQueueCount) {
    zfCoreMutexLocker();
    return (zfindex)d->taskQueue.size();
}
ZFMETHOD_DEFINE_1(ZFThread, void, taskQueueAdd
        , ZFMP_IN(const ZFListener &, task)
        ) {
    if(!task) {
        return;
    }
    zfCoreMutexLocker();

    if(this->isMainThread()) {
        d->taskQueue.push_back(task);
        if(!d->mainThreadTaskRunning) {
            d->mainThreadTaskRunning = zftrue;
            _ZFP_ZFThreadLog("[ZFThread] executeInMainThread begin %s", (const void *)this);
            ZFThread *zfThread = this;
            ZFLISTENER_1(mainThreadCallback
                , ZFThread *, zfThread
                ) {
                _ZFP_ZFThreadPrivate::mainThreadCallback(zfThread);
            } ZFLISTENER_END()
            d->mainThreadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThread(mainThreadCallback);
        }
    }
    else {
        if(d->taskQueueInitFlag) {
            d->taskQueue.push_back(task);
            d->taskQueueSema->lockAndBroadcast();
        }
        else {
            zfCoreLogTrim("taskQueueAdd() called without taskQueueInit(), thread: %s", this);
            ZFThread::mainThread()->taskQueueAdd(task);
        }
    }
}
ZFMETHOD_DEFINE_1(ZFThread, void, taskQueueRemove
        , ZFMP_IN(const ZFListener &, task)
        ) {
    zfCoreMutexLocker();
    for(zfstllist<ZFListener>::iterator it = d->taskQueue.begin(); it != d->taskQueue.end(); ++it) {
        if(*it == task) {
            d->taskQueue.erase(it);
            break;
        }
    }
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, taskQueueRunning) {
    return d->taskQueueRunning;
}

// ============================================================
void _ZFP_ZFThreadPrivate::threadCallback(
        ZF_IN ZFThread *zfThread
        , ZF_IN ZFObject *param0
        , ZF_IN ZFObject *param1
        ) {
    zfCoreMutexLock();
    _ZFP_ZFThreadPrivate *d = zfThread->d;
    d->runningFlag = zftrue;

    void *nativeThreadToken = ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadRegister(zfThread);
    _ZFP_ZFThread_allThread.add(zfThread);
    zfThread->threadOnRegister();
    zfCoreMutexUnlock();

    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread enter %s", (const void *)zfThread);

    ZFArgs zfargsTmp;
    zfargsTmp
        .sender(zfThread)
        .param0(param0)
        .param1(param1)
        ;

    zfThread->threadOnStart(zfargsTmp);

    if(!d->stopRequestedFlag) {
        zfThread->threadOnRun(zfargsTmp);
    }
    zfCoreMutexLock();
    if(zfThread->threadRunnable()) {
        ZFListener tmp = zfThread->threadRunnable();
        zfCoreMutexUnlock();
        tmp.execute(zfargsTmp
                .userData(tmp.userData())
            );
        zfargsTmp.userData(zfnull);
    }
    else {
        zfCoreMutexUnlock();
    }

    do {
        zfCoreMutexLock();
        d->taskQueueRunning = zffalse;
        d->autoReleasePool->poolDrain();

        if(!d->taskQueueInitFlag) {
            zfCoreMutexUnlock();
            break;
        }
        if(d->taskQueue.empty()) {
            zfThread->ThreadTaskQueueOnFinish();
            if(!d->taskQueue.empty()) {
                zfCoreMutexUnlock();
                continue;
            }
            zfCoreMutexUnlock();
            d->taskQueueSema->lockAndWait();
            continue;
        }

        ZFListener task = *(zfThread->d->taskQueue.begin());
        d->taskQueue.pop_front();

        d->taskQueueRunning = zftrue;
        zfCoreMutexUnlock();
        if(task) {
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
    ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadUnregister(nativeThreadToken);
    d->runningFlag = zffalse;
    d->startFlag = zffalse;
    zfCoreMutexUnlock();
}
void _ZFP_ZFThreadPrivate::threadCleanupCallback(ZF_IN ZFThread *zfThread) {
    zfCoreMutexLocker();

    _ZFP_ZFThreadPrivate *d = zfThread->d;

    if(d->threadToken != zfnull) {
        _ZFP_ZFThreadLog("[ZFThread] executeInMainThread cleanup by run %s", (const void *)zfThread);
        ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThreadCleanup(d->threadToken);
        d->threadToken = zfnull;
    }

    d->threadWaitSema->lockAndBroadcast();

    zfunsafe_zfRelease(zfThread);

    _ZFP_ZFThreadLog("[ZFThread] executeInNewThread exit %s", (const void *)zfThread);
}

void _ZFP_ZFThreadPrivate::mainThreadCallback(ZF_IN ZFThread *zfThread) {
    _ZFP_ZFThreadPrivate *d = zfThread->d;

    _ZFP_ZFThreadLog("[ZFThread] executeInMainThread enter %s", (const void *)zfThread);
    zfCoreMutexLock();
    d->mainThreadTaskRunning = zffalse;
    if(d->mainThreadToken != zfnull) {
        _ZFP_ZFThreadLog("[ZFThread] executeInMainThread cleanup by run %s", (const void *)zfThread);
        ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThreadCleanup(d->mainThreadToken);
        d->mainThreadToken = zfnull;
    }
    zfCoreMutexUnlock();

    do {
        zfCoreMutexLock();
        d->taskQueueRunning = zffalse;
        d->autoReleasePool->poolDrain();

        if(d->taskQueue.empty()) {
            zfCoreMutexUnlock();
            _ZFP_ZFThreadLog("[ZFThread] executeInMainThread exit %s", (const void *)zfThread);
            break;
        }

        ZFListener task = *(zfThread->d->taskQueue.begin());
        d->taskQueue.pop_front();

        d->taskQueueRunning = zftrue;
        zfCoreMutexUnlock();

        if(task) {
            task.execute(ZFArgs()
                    .sender(zfThread)
                    .userData(task.userData())
                );
        }
    } while(zftrue);
}

ZF_NAMESPACE_GLOBAL_END

