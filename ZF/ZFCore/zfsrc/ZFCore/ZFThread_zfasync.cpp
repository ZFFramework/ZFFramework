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
    ZFListener threadScheduleCallback;

public:
    _ZFP_ZFThreadPoolPrivate(void)
    : refCount(1)
    , threadPool()
    , taskIdCur(0)
    , taskMap()
    , taskList()
    , threadScheduleCallback()
    {
        _ZFP_ZFThreadPoolPrivate *d = this;
        ZFLISTENER_1(wrap
                , _ZFP_ZFThreadPoolPrivate *, d
                ) {
            _ZFP_runThread(d);
        }
        ZFLISTENER_END()
        this->threadScheduleCallback = wrap;
    }

public:
    static void _ZFP_runThread(ZF_IN _ZFP_ZFThreadPoolPrivate *d) {
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
            if(taskData == zfnull) {
                zfCoreMutexUnlock();
                break;
            }
            zfCoreMutexUnlock();

            taskData->zfargs.param0(taskData);
            taskData->callback.execute(taskData->zfargs);

            zfCoreMutexLock();
            taskData->result = taskData->zfargs.result();
            if(taskData->zfargs.param0() == zfnull) {
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
        if(taskData->zfargs.param0() == zfnull) {
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
};

void ZFThreadPool::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFThreadPoolPrivate);
}
void ZFThreadPool::objectOnDealloc(void) {
    this->removeAllAndWait();
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_2(ZFThreadPool, zfauto, start
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN(const ZFListener &, finishCallback)
        ) {
    if(!callback) {
        return zfnull;
    }

    zfCoreMutexLocker();
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
            threadPool->taskQueueAdd(d->threadScheduleCallback);
        }
    }
    while(d->threadPool.count() < this->maxThread() && taskCount > 0) {
        --taskCount;
        zfobj<ZFThread> threadPool;
        d->threadPool.add(threadPool);
        threadPool->threadName(zfstr("ZFThreadPool:%s:%s", (const void *)this, (const void *)threadPool.toObject()));
        threadPool->taskQueueInit();
        threadPool->threadStart();
        threadPool->taskQueueAdd(d->threadScheduleCallback);
    }

    return taskData;
}
ZFMETHOD_DEFINE_1(ZFThreadPool, void, stop
        , ZFMP_IN(const zfauto &, taskId)
        ) {
    zfCoreMutexLocker();
    _ZFP_I_ZFThreadPoolTaskData *taskData = taskId;
    if(taskData == zfnull || taskData->taskId == zfidentityInvalid()) {
        return;
    }
    d->taskMap.erase(taskData->taskId);
    d->taskList.removeElement(taskData);

    taskData->zfargs.param0(zfnull);
    taskData->taskId = zfidentityInvalid();
}

ZFMETHOD_DEFINE_0(ZFThreadPool, void, removeAllAndWait) {
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
        runThread->threadWait();
    } while(zftrue);
}

ZF_NAMESPACE_GLOBAL_END

