#include "ZFThread_zfasync.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_zfasyncTaskData {
public:
    zfautoT<v_zfidentity *> taskId;
    ZFListener callback;
    zfauto result;
    ZFListener finishCallback;
    zfautoT<ZFThread *> callerThread;
};

typedef zfstlmap<zfidentity, _ZFP_zfasyncTaskData *> _ZFP_zfasyncTaskMap;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfasyncDataHolder, ZFLevelZFFrameworkHigh) {
    this->maxThread = 8;
    this->threadScheduleCallback = ZFCallbackForFunc(zfself::_ZFP_runThread);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfasyncDataHolder) {
    do {
        zfCoreMutexLock();
        while(!this->taskList.isEmpty()) {
            _ZFP_zfasyncTaskData *taskData = this->taskList.removeAndGet(this->taskList.count() - 1);
            this->taskMap.erase(taskData->taskId->zfv);
            this->taskCleanup(taskData);
        }
        if(this->threadPool.isEmpty()) {
            zfCoreMutexUnlock();
            break;
        }
        zfautoT<ZFThread *> runThread = this->threadPool.removeAndGet(0);
        zfCoreMutexUnlock();

        runThread->taskQueueCleanup();
        runThread->threadWait();
    } while(zftrue);
}
public:
    zfuint maxThread;
    ZFCoreArray<zfautoT<ZFThread *> > threadPool;
    zfidentity taskIdCur;
    _ZFP_zfasyncTaskMap taskMap;
    ZFCoreArray<_ZFP_zfasyncTaskData *> taskList;
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
            _ZFP_zfasyncTaskData *taskData = zfnull;
            for(zfindex i = 0; i < d->taskList.count(); ++i) {
                if(d->taskList[i]->callerThread != runThread) {
                    taskData = d->taskList.removeAndGet(i);
                    d->taskMap.erase(taskData->taskId->zfv);
                    if(taskData->taskId->zfv == zfidentityInvalid()) {
                        d->taskCleanup(taskData);
                        taskData = zfnull;
                        --i;
                        continue;
                    }
                    break;
                }
            }
            if(taskData == zfnull) {
                zfCoreMutexUnlock();
                return;
            }
            zfCoreMutexUnlock();

            ZFArgs zfargsTmp;
            zfargsTmp.resultEnable(zftrue);
            taskData->callback.execute(zfargsTmp
                    .param0(taskData->taskId)
                    .userData(taskData->callback.userData())
                );

            zfCoreMutexLock();
            taskData->result = zfargsTmp.result();
            if(taskData->taskId->zfv == zfidentityInvalid()) {
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

            ZFLISTENER_1(callerThread
                    , _ZFP_zfasyncTaskData *, taskData
                    ) {
                ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder)::_ZFP_callerThread(zfargs, taskData);
            } ZFLISTENER_END()
            ZFThread::executeInThread(taskData->callerThread, callerThread);
        } while(zftrue);
    }
    static void _ZFP_callerThread(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN _ZFP_zfasyncTaskData *taskData
            ) {
        zfCoreMutexLock();
        ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
        if(taskData->taskId->zfv == zfidentityInvalid()) {
            d->taskCleanup(taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        taskData->finishCallback.execute(ZFArgs()
                .param0(taskData->result)
                .userData(taskData->finishCallback.userData())
            );

        zfCoreMutexLock();
        d->taskCleanup(taskData);
        zfCoreMutexUnlock();
    }
    void taskCleanup(ZF_IN _ZFP_zfasyncTaskData *taskData) {
        zfidentity taskId = taskData->taskId->zfv;
        taskData->taskId->zfv = zfidentityInvalid();
        taskData->callback = zfnull;
        taskData->result = zfnull;
        taskData->finishCallback = zfnull;
        taskData->callerThread = zfnull;
        zfdelete(taskData);
    }
ZF_GLOBAL_INITIALIZER_END(zfasyncDataHolder)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfidentity, zfasync
            , ZFMP_IN(const ZFListener &, callback)
            , ZFMP_IN_OPT(const ZFListener &, finishCallback, ZFCallback())
            ) {
    if(!callback) {
        return zfidentityInvalid();
    }

    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
    do {
        ++(d->taskIdCur);
        if(d->taskIdCur == zfidentityInvalid()) {
            d->taskIdCur = 0;
        }
    } while(d->taskMap.find(d->taskIdCur) != d->taskMap.end());
    _ZFP_zfasyncTaskData *taskData = zfnew(_ZFP_zfasyncTaskData);
    taskData->taskId = zflineAlloc(v_zfidentity, d->taskIdCur);
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
        zfblockedAlloc(ZFThread, threadPool);
        d->threadPool.add(threadPool);
        threadPool->taskQueueInit();
        threadPool->threadStart();
        threadPool->taskQueueAdd(d->threadScheduleCallback);
    }

    return taskData->taskId->zfv;
}

ZFMETHOD_FUNC_DEFINE_1(void, zfasyncCancel
            , ZFMP_IN(zfidentity, taskId)
            ) {
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
    _ZFP_zfasyncTaskMap::iterator it = d->taskMap.find(taskId);
    if(it == d->taskMap.end()) {
        return;
    }
    _ZFP_zfasyncTaskData *taskData = it->second;
    taskData->taskId->zfv = zfidentityInvalid();
    d->taskMap.erase(it);
    d->taskList.removeElement(taskData);
    d->taskCleanup(taskData);
}

ZF_NAMESPACE_GLOBAL_END

