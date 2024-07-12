#include "ZFThread_zfasync.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_zfasyncTaskData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_zfasyncTaskData, ZFObject)
public:
    ZFArgs zfargs;
    zfidentity taskId;
    ZFListener callback;
    zfauto result;
    ZFListener finishCallback;
    zfautoT<ZFThread> callerThread;
};

typedef zfstlmap<zfidentity, zfautoT<_ZFP_I_zfasyncTaskData> > _ZFP_zfasyncTaskMap;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfasyncDataHolder, ZFLevelZFFrameworkHigh) {
    this->maxThread = 8;
    this->threadScheduleCallback = ZFCallbackForFunc(zfself::_ZFP_runThread);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfasyncDataHolder) {
    do {
        zfCoreMutexLock();
        while(!this->taskList.isEmpty()) {
            zfasyncCancel(this->taskList.getLast());
        }
        if(this->threadPool.isEmpty()) {
            zfCoreMutexUnlock();
            break;
        }
        zfautoT<ZFThread> runThread = this->threadPool.removeAndGet(0);
        zfCoreMutexUnlock();

        runThread->threadStop();
        runThread->taskQueueCleanup();
        runThread->sleepCancel();
        runThread->threadWait();
    } while(zftrue);
}
public:
    zfuint maxThread;
    ZFCoreArray<zfautoT<ZFThread> > threadPool;
    zfidentity taskIdCur;
    _ZFP_zfasyncTaskMap taskMap;
    ZFCoreArray<_ZFP_I_zfasyncTaskData *> taskList;
    ZFListener threadScheduleCallback;
public:
    static void _ZFP_runThread(ZF_IN const ZFArgs &zfargs) {
        do {
            zfCoreMutexLock();
            ZFThread *runThread = ZFThread::currentThread();
            if(runThread == zfnull) {
                zfCoreMutexUnlock();
                return;
            }

            ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
            zfautoT<_ZFP_I_zfasyncTaskData> taskData;
            for(zfindex i = 0; i < d->taskList.count(); ++i) {
                if(d->taskList[i]->callerThread != runThread) {
                    taskData = d->taskList.removeAndGet(i);
                    d->taskMap.erase(taskData->taskId);
                    break;
                }
            }
            if(taskData == zfnull) {
                zfCoreMutexUnlock();
                return;
            }
            zfCoreMutexUnlock();

            taskData->zfargs.param0(taskData);
            taskData->callback.execute(taskData->zfargs);

            zfCoreMutexLock();
            taskData->result = taskData->zfargs.result();
            if(taskData->zfargs.param0() == zfnull) {
                d->taskCleanup(taskData);
                zfCoreMutexUnlock();
                continue;
            }
            if(!taskData->finishCallback) {
                d->taskCleanup(taskData);
                zfCoreMutexUnlock();
                continue;
            }
            zfCoreMutexUnlock();

            ZFLISTENER_1(notifyFinish
                    , zfautoT<_ZFP_I_zfasyncTaskData>, taskData
                    ) {
                ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder)::_ZFP_notifyFinish(zfargs, taskData);
            } ZFLISTENER_END()
            ZFThread::executeInThread(ZFThread::mainThread(), notifyFinish);
        } while(zftrue);
    }
    static void _ZFP_notifyFinish(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN _ZFP_I_zfasyncTaskData *taskData
            ) {
        zfCoreMutexLock();
        ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
        if(taskData->zfargs.param0() == zfnull) {
            d->taskCleanup(taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        taskData->finishCallback.execute(ZFArgs()
                .result(taskData->result)
            );

        zfCoreMutexLock();
        d->taskCleanup(taskData);
        zfCoreMutexUnlock();
    }
    void taskCleanup(ZF_IN _ZFP_I_zfasyncTaskData *taskData) {
        taskData->taskId = zfidentityInvalid();
        taskData->callback = zfnull;
        taskData->result = zfnull;
        taskData->finishCallback = zfnull;
        taskData->callerThread = zfnull;
    }
ZF_GLOBAL_INITIALIZER_END(zfasyncDataHolder)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfauto, zfasync
            , ZFMP_IN(const ZFListener &, callback)
            , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
            ) {
    if(!callback) {
        return zfnull;
    }

    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
    do {
        ++(d->taskIdCur);
        if(d->taskIdCur == zfidentityInvalid()) {
            d->taskIdCur = 0;
        }
    } while(d->taskMap.find(d->taskIdCur) != d->taskMap.end());
    zfobj<_ZFP_I_zfasyncTaskData> taskData;
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
    while(d->threadPool.count() < d->maxThread && taskCount > 0) {
        --taskCount;
        zfobj<ZFThread> threadPool;
        d->threadPool.add(threadPool);
        threadPool->threadName("zfasync pool");
        threadPool->taskQueueInit();
        threadPool->threadStart();
        threadPool->taskQueueAdd(d->threadScheduleCallback);
    }

    return taskData;
}

ZFMETHOD_FUNC_DEFINE_1(void, zfasyncCancel
            , ZFMP_IN(const zfauto &, taskId)
            ) {
    zfCoreMutexLocker();
    _ZFP_I_zfasyncTaskData *taskData = taskId;
    if(taskData == zfnull || taskData->taskId == zfidentityInvalid()) {
        return;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
    d->taskMap.erase(taskData->taskId);
    d->taskList.removeElement(taskData);

    taskData->zfargs.param0(zfnull);
    taskData->taskId = zfidentityInvalid();
}

ZF_NAMESPACE_GLOBAL_END

