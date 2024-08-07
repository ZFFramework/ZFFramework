#include "ZFThread_zfasync.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfauto, zfasync
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    return ZFThreadPool::instance()->start(callback, finishCallback);
}
ZFMETHOD_FUNC_DEFINE_1(void, zfasyncCancel
        , ZFMP_IN(const zfauto &, taskId)
        ) {
    ZFThreadPool::instance()->stop(taskId);
}

// ============================================================
ZFOBJECT_REGISTER(ZFThreadPool)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFThreadPool, instance, ZFLevelZFFrameworkEssential)

zfclass _ZFP_I_ZFThreadPoolTaskData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadPoolTaskData, ZFObject)
public:
    ZFArgs zfargs;
    zfidentity taskId;
    ZFListener callback;
    zfauto result;
    ZFListener finishCallback;
    zfautoT<ZFThread> callerThread;
};
ZFOBJECT_REGISTER(_ZFP_I_ZFThreadPoolTaskData)

typedef zfstlmap<zfidentity, zfautoT<_ZFP_I_ZFThreadPoolTaskData> > _ZFP_ZFThreadPoolTaskMap;

zfclassNotPOD _ZFP_ZFThreadPoolPrivate {
public:
    zfuint refCount;
    ZFCoreArray<zfautoT<ZFThread> > threadPool;
    zfidentity taskIdCur;
    _ZFP_ZFThreadPoolTaskMap taskMap;
    ZFCoreArray<_ZFP_I_ZFThreadPoolTaskData *> taskList;
    ZFListener threadOnRun; // null after removeAll, to indicate exit of thread pool
    ZFListener threadOnStopRequested;

public:
    _ZFP_ZFThreadPoolPrivate(void)
    : refCount(1)
    , threadPool()
    , taskIdCur(0)
    , taskMap()
    , taskList()
    , threadOnRun()
    , threadOnStopRequested()
    {
        _ZFP_ZFThreadPoolPrivate *d = this;
        ZFLISTENER_1(threadOnRun
                , _ZFP_ZFThreadPoolPrivate *, d
                ) {
            _ZFP_runThread(zfargs.sender(), d);
        }
        ZFLISTENER_END()
        this->threadOnRun = threadOnRun;

        ZFLISTENER_1(threadOnStopRequested
                , _ZFP_ZFThreadPoolPrivate *, d
                ) {
            _ZFP_threadOnStopRequested(zfargs.sender(), d);
        }
        ZFLISTENER_END()
        this->threadOnStopRequested = threadOnStopRequested;
    }

public:
    void stop(ZF_IN const zfauto &taskId) {
        _ZFP_I_ZFThreadPoolTaskData *taskData = taskId;
        if(taskData == zfnull || taskData->taskId == zfidentityInvalid()) {
            return;
        }
        this->taskMap.erase(taskData->taskId);
        this->taskList.removeElement(taskData);

        taskData->zfargs.param0(zfnull);
        taskData->taskId = zfidentityInvalid();
    }

public:
    static void _ZFP_runThread(
            ZF_IN ZFThread *ownerThread
            , ZF_IN _ZFP_ZFThreadPoolPrivate *d
            ) {
        do {
            zfCoreMutexLock();
            ZFThread *runThread = ZFThread::currentThread();
            if(runThread == zfnull) {
                zfCoreMutexUnlock();
                break;
            }

            zfautoT<_ZFP_I_ZFThreadPoolTaskData> taskData;
            for(zfindex i = 0; i < d->taskList.count(); ++i) {
                if(d->taskList[i]->callerThread != runThread) {
                    taskData = d->taskList.removeAndGet(i);
                    d->taskMap.erase(taskData->taskId);
                    break;
                }
            }
            if(taskData == zfnull
                    || d->threadOnRun == zfnull
                    ) {
                zfCoreMutexUnlock();
                break;
            }
            taskData->zfargs.param0(taskData);
            zfstring runningTaskKey = zftext("_ZFP_ZFThreadPoolRunning");
            ownerThread->objectTag(runningTaskKey, taskData);
            zfCoreMutexUnlock();

            taskData->callback.execute(taskData->zfargs);

            zfCoreMutexLock();
            ownerThread->objectTag(runningTaskKey, zfnull);
            taskData->result = taskData->zfargs.result();
            if(taskData->zfargs.param0() == zfnull
                    || d->threadOnRun == zfnull
                    ) {
                _ZFP_taskCleanup(taskData);
                zfCoreMutexUnlock();
                continue;
            }
            if(!taskData->finishCallback) {
                _ZFP_taskCleanup(taskData);
                zfCoreMutexUnlock();
                continue;
            }
            ++(d->refCount);
            zfCoreMutexUnlock();
            ZFLISTENER_2(notifyFinish
                    , _ZFP_ZFThreadPoolPrivate *, d
                    , zfautoT<_ZFP_I_ZFThreadPoolTaskData>, taskData
                    ) {
                _ZFP_notifyFinish(d, taskData);

                zfCoreMutexLocker();
                --(d->refCount);
                if(d->refCount == 0) {
                    zfpoolDelete(d);
                }
            } ZFLISTENER_END()
            ZFThread::executeInThread(ZFThread::mainThread(), notifyFinish);
        } while(zftrue);

        zfCoreMutexLocker();
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
    }
    static void _ZFP_notifyFinish(
            ZF_IN _ZFP_ZFThreadPoolPrivate *d
            , ZF_IN _ZFP_I_ZFThreadPoolTaskData *taskData
            ) {
        zfCoreMutexLock();
        if(taskData->zfargs.param0() == zfnull
                || d->threadOnRun == zfnull
                ) {
            _ZFP_taskCleanup(taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        taskData->finishCallback.execute(ZFArgs()
                .result(taskData->result)
            );

        zfCoreMutexLock();
        _ZFP_taskCleanup(taskData);
        zfCoreMutexUnlock();
    }
    static void _ZFP_taskCleanup(ZF_IN _ZFP_I_ZFThreadPoolTaskData *taskData) {
        taskData->taskId = zfidentityInvalid();
        taskData->callback = zfnull;
        taskData->result = zfnull;
        taskData->finishCallback = zfnull;
        taskData->callerThread = zfnull;
    }
    static void _ZFP_threadOnStopRequested(
            ZF_IN ZFThread *ownerThread
            , ZF_IN _ZFP_ZFThreadPoolPrivate *d
            ) {
        zfCoreMutexLocker();
        zfstring runningTaskKey = zftext("_ZFP_ZFThreadPoolRunning");
        zfauto taskData = ownerThread->objectTagRemoveAndGet(runningTaskKey);
        if(taskData) {
            d->stop(taskData);
        }
    }
};

void ZFThreadPool::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfnull;
}
void ZFThreadPool::objectOnDeallocPrepare(void) {
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_2(ZFThreadPool, zfauto, start
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN(const ZFListener &, finishCallback)
        ) {
    if(!callback) {
        return zfnull;
    }

    zfCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFThreadPoolPrivate);
    }

    do {
        ++(d->taskIdCur);
        if(d->taskIdCur == zfidentityInvalid()) {
            d->taskIdCur = 0;
        }
    } while(d->taskMap.find(d->taskIdCur) != d->taskMap.end());
    zfobj<_ZFP_I_ZFThreadPoolTaskData> taskData;
    taskData->taskId = d->taskIdCur;
    taskData->callback = callback;
    taskData->finishCallback = finishCallback;
    taskData->callerThread = ZFThread::currentThread();
    d->taskMap[d->taskIdCur] = taskData;
    d->taskList.add(taskData);

    zfindex taskCount = d->taskList.count();
    for(zfindex i = 0; i < d->threadPool.count() && taskCount > 0; ++i) {
        ZFThread *threadPool = d->threadPool[i];
        if(threadPool->taskQueueCount() == 0 && !threadPool->taskQueueRunning()) {
            --taskCount;
            ++(d->refCount);
            threadPool->taskQueueAdd(d->threadOnRun);
        }
    }
    while(d->threadPool.count() < this->maxThread() && taskCount > 0) {
        --taskCount;
        zfobj<ZFThread> threadPool;
        d->threadPool.add(threadPool);
        threadPool->threadName(zfstr("ZFThreadPool:%s:%s", (const void *)this, (const void *)threadPool.toObject()));
        threadPool->observerAdd(ZFThread::EventThreadOnStopRequested(), d->threadOnStopRequested);
        threadPool->taskQueueInit();
        threadPool->threadStart();
        ++(d->refCount);
        threadPool->taskQueueAdd(d->threadOnRun);
    }

    return taskData;
}
ZFMETHOD_DEFINE_1(ZFThreadPool, void, stop
        , ZFMP_IN(const zfauto &, taskId)
        ) {
    zfCoreMutexLocker();
    if(d != zfnull) {
        return d->stop(taskId);
    }
}

ZFMETHOD_DEFINE_0(ZFThreadPool, void, removeAll) {
    if(d == zfnull) {
        return;
    }
    do {
        zfCoreMutexLock();
        while(!d->taskList.isEmpty()) {
            this->stop(d->taskList.getLast());
        }
        if(d->threadPool.isEmpty()) {
            zfCoreMutexUnlock();
            break;
        }
        zfautoT<ZFThread> runThread = d->threadPool.removeAndGet(0);
        zfCoreMutexUnlock();

        runThread->threadStop();
        runThread->taskQueueCleanup();
        runThread->sleepCancel();
    } while(zftrue);

    zfCoreMutexLocker();
    d->threadOnRun = zfnull;
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
        d = zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

