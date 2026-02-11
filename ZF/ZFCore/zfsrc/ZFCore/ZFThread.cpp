#include "ZFThread.h"
#include "protocol/ZFProtocolZFThread.h"
#include "ZFAutoReleasePool.h"
#include "ZFSemaphore.h"

#include "ZFCore/ZFSTLWrapper/zfstllist.h"

// #define _ZFP_ZFThread_DEBUG 1

#if _ZFP_ZFThread_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFThread_log(fmt, ...) \
        zfimplLog("%s [ZFThread] " fmt, zfimplTime(), ##__VA_ARGS__)
#else
    #define _ZFP_ZFThread_log(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static ZFCoreArray<ZFThread *> _ZFP_ZFThread_allThread;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThread_allThreadCleanup, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThread_allThreadCleanup) {
    ZFCoreMutexLock();
    while(!_ZFP_ZFThread_allThread.isEmpty()) {
        ZFThread *zfThread = _ZFP_ZFThread_allThread.getLast();
        zfobjReleaseInScope(zfobjRetain(zfThread));
        ZFCoreAssert(zfThread->threadStarted());
        ZFCoreMutexUnlock();
        zfThread->threadStop();
        zfThread->taskQueueCleanup();
        zfThread->sleepCancel();
        _ZFP_ZFThread_log("global threadWait begin: %p", zfThread);
        zfThread->threadWait();
        _ZFP_ZFThread_log("global threadWait end: %p", zfThread);
        ZFCoreMutexLock();
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
    void *sleepToken;

public:
    _ZFP_ZFThreadPrivate(void)
    : startFlag(zffalse)
    , runningFlag(zffalse)
    , stopRequestedFlag(zffalse)
    , autoReleasePool(zfobjAlloc(ZFAutoReleasePool))
    , threadWaitSema(zfobjAlloc(ZFSemaphore))
    , taskQueueInitFlag(zffalse)
    , taskQueueRunning(zffalse)
    , taskQueueSema(zfnull)
    , taskQueue()
    , threadToken(zfnull)
    , mainThreadTaskRunning(zffalse)
    , mainThreadToken(zfnull)
    , sleepToken(zfnull)
    {
    }
    ~_ZFP_ZFThreadPrivate(void) {
        zfobjRelease(this->autoReleasePool);
        zfobjRelease(this->threadWaitSema);
        zfobjRelease(this->taskQueueSema);
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
        ZFCoreLogTrim("you must not start a user registered thread");
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
        ZFCoreLogTrim("you must not stop a user registered thread");
    }
};

// ============================================================
// ZFThread
ZFOBJECT_REGISTER(ZFThread)

ZFEVENT_REGISTER(ZFThread, ThreadOnRegister)
ZFEVENT_REGISTER(ZFThread, ThreadOnUnregister)
ZFEVENT_REGISTER(ZFThread, ThreadOnStart)
ZFEVENT_REGISTER(ZFThread, ThreadOnStop)
ZFEVENT_REGISTER(ZFThread, ThreadOnStopRequested)
ZFEVENT_REGISTER(ZFThread, ThreadTaskQueueOnFinish)

ZFMETHOD_DEFINE_1(ZFThread, void *, nativeThreadRegister
        , ZFMP_IN(const zfstring &, threadName)
        ) {
    ZFCoreMutexLocker();
    ZFThread *zfThread = ZFThread::currentThread();
    ZFCoreAssert(zfThread == zfnull);

    zfThread = zfobjAlloc(_ZFP_I_ZFThreadNativeRegisterThread);
    zfThread->threadName(threadName);
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
    ZFCoreAssert(zfThread != zfnull);

    ZFCoreMutexLock();
    while(!zfThread->d->taskQueue.empty()) {
        ZFListener task = *(zfThread->d->taskQueue.begin());
        zfThread->d->taskQueue.pop_front();
        if(!task) {
            continue;
        }
        zfThread->d->taskQueueRunning = zftrue;
        ZFCoreMutexUnlock();
        task.execute(ZFArgs()
                .sender(zfThread)
            );
        ZFCoreMutexLock();
        zfThread->d->taskQueueRunning = zffalse;
        zfThread->d->autoReleasePool->poolDrain();
    }
    zfThread->taskQueueCleanup();
    ZFCoreMutexUnlock();

    zfThread->d->threadWaitSema->lockAndBroadcast();

    ZFCoreMutexLocker();
    zfThread->threadOnUnregister();
    _ZFP_ZFThread_allThread.removeElement(zfThread);
    ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadUnregister(token);
    zfunsafe_zfobjRelease(zfThread);
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, implAvailable) {
    ZFCoreMutexLocker();
    return ZFPROTOCOL_IS_AVAILABLE(ZFThread);
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, implMainThreadTaskAvailable) {
    ZFCoreMutexLocker();
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
    ZFCoreMutexLocker();
    return ZFPROTOCOL_ACCESS(ZFThread)->mainThread();
}
ZFMETHOD_DEFINE_0(ZFThread, ZFThread *, currentThread) {
    ZFCoreMutexLocker();
    return ZFPROTOCOL_ACCESS(ZFThread)->currentThread();
}

ZFMETHOD_DEFINE_1(ZFThread, zfbool, sleep
        , ZFMP_IN(zftimet, miliSecs)
        ) {
    ZFThread *cur = ZFThread::currentThread();
    ZFCoreAssertWithMessageTrim(cur, "ZFThread::sleep() depends on current thread registered");
    ZFCoreAssert(cur->d->sleepToken == zfnull);
    ZFPROTOCOL_INTERFACE_CLASS(ZFThread) *impl = ZFPROTOCOL_ACCESS(ZFThread);
    {
        ZFCoreMutexLocker();
        cur->d->sleepToken = impl->sleepTokenCreate();
        if(cur->d->sleepToken == zfnull) {
            return zffalse;
        }
    }
    zfbool ret = impl->sleep(cur->d->sleepToken, miliSecs);
    {
        ZFCoreMutexLocker();
        impl->sleepTokenDestroy(cur->d->sleepToken);
        cur->d->sleepToken = zfnull;
    }
    return !ret;
}
ZFMETHOD_DEFINE_0(ZFThread, void, sleepCancel) {
    if(d->sleepToken) {
        ZFPROTOCOL_ACCESS(ZFThread)->sleepCancel(d->sleepToken);
    }
}

ZFMETHOD_DEFINE_2(ZFThread, ZFThread *, executeInThread
        , ZFMP_IN(ZFThread *, thread)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    ZFThread *ret = (thread != zfnull && thread->taskQueueAvailable() ? thread : ZFThread::mainThread());
    ret->taskQueueAdd(callback);
    return ret;
}

// ============================================================
// thread instance method
ZFOBJECT_ON_INIT_DEFINE_1(ZFThread
        , ZFMP_IN(const ZFListener &, runnable)
        ) {
    zfself::threadRunnable(runnable);
}

void ZFThread::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFThreadPrivate);
    _ZFP_ZFThread_log("init %p", this);
}
void ZFThread::objectOnDealloc(void) {
    _ZFP_ZFThread_log("dealloc %p", this);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFThread::objectOnDeallocPrepare(void) {
    d->mainThreadTaskRunning = zffalse;
    if(d->mainThreadToken != zfnull) {
        _ZFP_ZFThread_log("executeInMainThread cleanup by dealloc %p", this);
        ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThreadCleanup(d->mainThreadToken);
        d->mainThreadToken = zfnull;
    }

    if(!this->isMainThread()) {
        this->threadWait();
    }
    this->autoReleasePoolDrain();
    zfsuper::objectOnDeallocPrepare();
}

void ZFThread::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    if(this->threadName()) {
        ret += "(";
        ret += this->threadName();
        ret += ")";
    }
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
    ZFCoreMutexLocker();
    if(d->startFlag) {
        return;
    }
    if(d->stopRequestedFlag) {
        ZFCoreLogTrim("threadStop has called, you should not start it again");
        return;
    }
    d->startFlag = zftrue;
    zfunsafe_zfobjRetain(this);
    ZFThread *zfThread = this;
    _ZFP_ZFThread_log("executeInNewThread begin %p", this);
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
    ZFCoreMutexLocker();
    if(!d->startFlag) {
        return;
    }
    d->stopRequestedFlag = zftrue;
    this->threadOnStopRequested();
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadStopRequested) {
    return d->stopRequestedFlag;
}

ZFMETHOD_DEFINE_0(ZFThread, void, threadWait) {
    ZFCoreMutexLock();
    d->threadWaitSema->semaphoreLock();
    if(!d->startFlag) {
        d->threadWaitSema->semaphoreUnlock();
        ZFCoreMutexUnlock();
        return;
    }
    ZFCoreMutexUnlock();
    _ZFP_ZFThread_log("threadWait begin %p", this);
    d->threadWaitSema->semaphoreWait();
    d->threadWaitSema->semaphoreUnlock();
    _ZFP_ZFThread_log("threadWait end %p", this);
}
ZFMETHOD_DEFINE_1(ZFThread, zfbool, threadWait
        , ZFMP_IN(zftimet, miliSecs)
        ) {
    ZFCoreMutexLock();
    d->threadWaitSema->semaphoreLock();
    if(!d->startFlag) {
        d->threadWaitSema->semaphoreUnlock();
        ZFCoreMutexUnlock();
        return zftrue;
    }
    ZFCoreMutexUnlock();
    _ZFP_ZFThread_log("threadWait begin %p", this);
    zfbool ret = d->threadWaitSema->semaphoreWait(miliSecs);
    d->threadWaitSema->semaphoreUnlock();
    _ZFP_ZFThread_log("threadWait end %p", this);
    return ret;
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
    ZFCoreMutexLocker();
    if(d->taskQueueInitFlag) {
        return;
    }
    d->taskQueueInitFlag = zftrue;
    d->taskQueueSema = zfunsafe_zfobjAlloc(ZFSemaphore);
    zfunsafe_zfobjRetain(this);
}
ZFMETHOD_DEFINE_0(ZFThread, void, taskQueueCleanup) {
    if(this->isMainThread()) {
        return;
    }
    ZFCoreMutexLock();
    if(!d->taskQueueInitFlag) {
        ZFCoreMutexUnlock();
        return;
    }
    ZFCoreMutexUnlock();
    d->taskQueueInitFlag = zffalse;
    d->taskQueueSema->lockAndBroadcast();
    zfunsafe_zfobjRelease(this);
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, taskQueueAvailable) {
    return (this->isMainThread() || d->taskQueueInitFlag);
}
ZFMETHOD_DEFINE_0(ZFThread, zfindex, taskQueueCount) {
    ZFCoreMutexLocker();
    return (zfindex)d->taskQueue.size();
}
ZFMETHOD_DEFINE_1(ZFThread, void, taskQueueAdd
        , ZFMP_IN(const ZFListener &, task)
        ) {
    if(!task) {
        return;
    }
    ZFCoreMutexLocker();

    if(this->isMainThread()) {
        d->taskQueue.push_back(task);
        if(!d->mainThreadTaskRunning) {
            d->mainThreadTaskRunning = zftrue;
            _ZFP_ZFThread_log("executeInMainThread begin %p", this);
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
            ZFCoreLogTrim("taskQueueAdd() called without taskQueueInit(), thread: %s", this);
            ZFThread::mainThread()->taskQueueAdd(task);
        }
    }
}
ZFMETHOD_DEFINE_1(ZFThread, void, taskQueueRemove
        , ZFMP_IN(const ZFListener &, task)
        ) {
    ZFCoreMutexLocker();
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
    ZFCoreMutexLock();
    _ZFP_ZFThreadPrivate *d = zfThread->d;
    d->runningFlag = zftrue;

    void *nativeThreadToken = ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadRegister(zfThread);
    _ZFP_ZFThread_allThread.add(zfThread);
    zfThread->threadOnRegister();
    ZFCoreMutexUnlock();

    _ZFP_ZFThread_log("executeInNewThread enter %p", zfThread);

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
    ZFCoreMutexLock();
    if(zfThread->threadRunnable()) {
        ZFListener tmp = zfThread->threadRunnable();
        ZFCoreMutexUnlock();
        tmp.execute(zfargsTmp);
    }
    else {
        ZFCoreMutexUnlock();
    }

    do {
        ZFCoreMutexLock();
        d->taskQueueRunning = zffalse;
        d->autoReleasePool->poolDrain();

        if(!d->taskQueueInitFlag) {
            ZFCoreMutexUnlock();
            break;
        }
        if(d->taskQueue.empty()) {
            zfThread->threadTaskQueueOnFinish();
            d->taskQueueSema->semaphoreLock();
            if(!d->taskQueue.empty()) {
                d->taskQueueSema->semaphoreUnlock();
                ZFCoreMutexUnlock();
                continue;
            }
            ZFCoreMutexUnlock();
            _ZFP_ZFThread_log("taskWait begin: %p", zfThread);
            d->taskQueueSema->semaphoreWait();
            _ZFP_ZFThread_log("taskWait end: %p", zfThread);
            d->taskQueueSema->semaphoreUnlock();
            continue;
        }

        ZFListener task = *(zfThread->d->taskQueue.begin());
        d->taskQueue.pop_front();

        d->taskQueueRunning = zftrue;
        ZFCoreMutexUnlock();
        if(task) {
            task.execute(ZFArgs()
                    .sender(zfThread)
                );
        }
    } while(!d->stopRequestedFlag);

    zfThread->threadOnStop(zfargsTmp);

    ZFCoreMutexLock();
    zfThread->threadOnUnregister();
    _ZFP_ZFThread_allThread.removeElement(zfThread);
    ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadUnregister(nativeThreadToken);
    d->runningFlag = zffalse;
    d->startFlag = zffalse;
    ZFCoreMutexUnlock();
}
void _ZFP_ZFThreadPrivate::threadCleanupCallback(ZF_IN ZFThread *zfThread) {
    ZFCoreMutexLocker();

    _ZFP_ZFThreadPrivate *d = zfThread->d;

    if(d->threadToken != zfnull) {
        _ZFP_ZFThread_log("executeInMainThread cleanup by run %p", zfThread);
        ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThreadCleanup(d->threadToken);
        d->threadToken = zfnull;
    }

    d->threadWaitSema->lockAndBroadcast();

    _ZFP_ZFThread_log("executeInNewThread exit %p", zfThread);
    zfunsafe_zfobjRelease(zfThread);
}

void _ZFP_ZFThreadPrivate::mainThreadCallback(ZF_IN ZFThread *zfThread) {
    _ZFP_ZFThreadPrivate *d = zfThread->d;

    _ZFP_ZFThread_log("executeInMainThread enter %p", zfThread);
    ZFCoreMutexLock();
    d->mainThreadTaskRunning = zffalse;
    if(d->mainThreadToken != zfnull) {
        _ZFP_ZFThread_log("executeInMainThread cleanup by run %p", zfThread);
        ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThreadCleanup(d->mainThreadToken);
        d->mainThreadToken = zfnull;
    }
    ZFCoreMutexUnlock();

    do {
        ZFCoreMutexLock();
        d->taskQueueRunning = zffalse;
        d->autoReleasePool->poolDrain();

        if(d->taskQueue.empty()) {
            ZFCoreMutexUnlock();
            _ZFP_ZFThread_log("executeInMainThread exit %p", zfThread);
            break;
        }

        ZFListener task = *(zfThread->d->taskQueue.begin());
        d->taskQueue.pop_front();

        d->taskQueueRunning = zftrue;
        ZFCoreMutexUnlock();

        if(task) {
            task.execute(ZFArgs()
                    .sender(zfThread)
                );
        }
    } while(zftrue);
}

ZF_NAMESPACE_GLOBAL_END

