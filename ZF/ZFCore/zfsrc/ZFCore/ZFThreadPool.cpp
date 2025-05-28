#include "ZFThreadPool.h"

#include "ZFSTLWrapper/zfstlhashmap.h"
#include "ZFSTLWrapper/zfstllist.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFThreadPool)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFThreadPool, instance, ZFLevelZFFrameworkEssential)

zfclassFwd _ZFP_I_ZFThreadPoolTaskData;
typedef zfstlhashmap<_ZFP_I_ZFThreadPoolTaskData *, zfbool> _ZFP_ZFThreadPoolTaskMapType;
typedef zfstllist<_ZFP_I_ZFThreadPoolTaskData *> _ZFP_ZFThreadPoolTaskListType;

zfclass _ZFP_I_ZFThreadPoolTaskData : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadPoolTaskData, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)
public:
    _ZFP_ZFThreadPoolPrivate *owner;
    _ZFP_ZFThreadPoolTaskMapType::iterator mapIt; // valid only when owner not null
    _ZFP_ZFThreadPoolTaskListType::iterator listIt;
    ZFArgs zfargsForTask;
    ZFListener callback;
    zfauto result;
    ZFListener finishCallback;
    zfautoT<ZFThread> callerThread;
public:
    zfoverride
    virtual void stop(void);
};
ZFOBJECT_REGISTER(_ZFP_I_ZFThreadPoolTaskData)

zfclassNotPOD _ZFP_ZFThreadPoolPrivate {
public:
    zfuint refCount;
    ZFCoreArray<zfautoT<ZFThread> > threadPool;
    _ZFP_ZFThreadPoolTaskMapType taskMap; // manually retained _ZFP_I_ZFThreadPoolTaskData
    _ZFP_ZFThreadPoolTaskListType taskList;
    ZFListener threadOnRun; // null after removeAll, to indicate exit of thread pool
    ZFListener threadOnStopRequested;

public:
    _ZFP_ZFThreadPoolPrivate(void)
    : refCount(1)
    , threadPool()
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
    void stop(ZF_IN _ZFP_I_ZFThreadPoolTaskData *taskData) {
        if(taskData->owner) {
            taskData->owner = zfnull;
            this->taskMap.erase(taskData->mapIt);
            this->taskList.erase(taskData->listIt);
        }
        _ZFP_taskCleanup(taskData);
    }

public:
    static void _ZFP_runThread(
            ZF_IN ZFThread *ownerThread
            , ZF_IN _ZFP_ZFThreadPoolPrivate *d
            ) {
        do {
            ZFCoreMutexLock();
            ZFThread *runThread = ZFThread::currentThread();
            if(runThread == zfnull) {
                ZFCoreMutexUnlock();
                break;
            }

            _ZFP_I_ZFThreadPoolTaskData *taskData = zfnull;
            for(_ZFP_ZFThreadPoolTaskListType::iterator it = d->taskList.begin(); it != d->taskList.end(); ++it) {
                if((*it)->callerThread != runThread) {
                    taskData = *it;
                    taskData->owner = zfnull;
                    d->taskList.erase(taskData->listIt);
                    d->taskMap.erase(taskData->mapIt);
                    break;
                }
            }
            if(taskData == zfnull
                    || d->threadOnRun == zfnull
                    ) {
                ZFCoreMutexUnlock();
                break;
            }
            taskData->zfargsForTask
                .sender(taskData)
                .param0(taskData)
                ;
            zfstring runningTaskKey = zftext("_ZFP_ZFThreadPoolRunning");
            ownerThread->objectTag(runningTaskKey, taskData);
            zfunsafe_zfblockedRelease(zfunsafe_zfRetain(taskData));
            ZFCoreMutexUnlock();

            taskData->callback.execute(taskData->zfargsForTask);

            ZFCoreMutexLock();
            ownerThread->objectTag(runningTaskKey, zfnull);
            taskData->result = taskData->zfargsForTask.result();
            if(taskData->zfargsForTask.param0() == zfnull
                    || d->threadOnRun == zfnull
                    ) {
                _ZFP_taskCleanup(taskData);
                ZFCoreMutexUnlock();
                continue;
            }
            if(!taskData->finishCallback) {
                _ZFP_taskCleanup(taskData);
                ZFCoreMutexUnlock();
                continue;
            }
            ++(d->refCount);
            ZFCoreMutexUnlock();
            ZFLISTENER_2(notifyFinish
                    , _ZFP_ZFThreadPoolPrivate *, d
                    , zfautoT<_ZFP_I_ZFThreadPoolTaskData>, taskData
                    ) {
                _ZFP_notifyFinish(d, taskData);

                ZFCoreMutexLocker();
                --(d->refCount);
                if(d->refCount == 0) {
                    zfpoolDelete(d);
                }
            } ZFLISTENER_END()
            ZFThread::executeInThread(ZFThread::mainThread(), notifyFinish);
        } while(zftrue);

        ZFCoreMutexLocker();
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
    }
    static void _ZFP_notifyFinish(
            ZF_IN _ZFP_ZFThreadPoolPrivate *d
            , ZF_IN _ZFP_I_ZFThreadPoolTaskData *taskData
            ) {
        ZFCoreMutexLock();
        if(taskData->zfargsForTask.param0() == zfnull
                || d->threadOnRun == zfnull
                ) {
            _ZFP_taskCleanup(taskData);
            ZFCoreMutexUnlock();
            return;
        }
        ZFCoreMutexUnlock();

        taskData->finishCallback.execute(ZFArgs()
                .sender(taskData)
                .param0(taskData->result)
            );

        ZFCoreMutexLock();
        _ZFP_taskCleanup(taskData);
        ZFCoreMutexUnlock();
    }
    static void _ZFP_taskCleanup(ZF_IN _ZFP_I_ZFThreadPoolTaskData *taskData) {
        taskData->callback = zfnull;
        taskData->result = zfnull;
        taskData->finishCallback = zfnull;
        taskData->callerThread = zfnull;
        zfunsafe_zfRelease(taskData);
    }
    static void _ZFP_threadOnStopRequested(
            ZF_IN ZFThread *ownerThread
            , ZF_IN _ZFP_ZFThreadPoolPrivate *d
            ) {
        ZFCoreMutexLocker();
        zfstring runningTaskKey = zftext("_ZFP_ZFThreadPoolRunning");
        zfauto taskData = ownerThread->objectTagRemoveAndGet(runningTaskKey);
        if(taskData) {
            d->stop(taskData);
        }
    }
};

void _ZFP_I_ZFThreadPoolTaskData::stop(void) {
    if(this->owner != zfnull) {
        this->owner->stop(this);
    }
    this->zfargsForTask.param0(zfnull);
}

void ZFThreadPool::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfnull;
}
void ZFThreadPool::objectOnDeallocPrepare(void) {
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_2(ZFThreadPool, zfautoT<ZFTaskId>, start
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN(const ZFListener &, finishCallback)
        ) {
    if(!callback) {
        return zfnull;
    }

    ZFCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFThreadPoolPrivate);
    }

    _ZFP_I_ZFThreadPoolTaskData *taskData = zfunsafe_zfAlloc(_ZFP_I_ZFThreadPoolTaskData);
    taskData->owner = d;
    taskData->callback = callback;
    taskData->finishCallback = finishCallback;
    taskData->callerThread = ZFThread::currentThread();
    taskData->mapIt = d->taskMap.insert(zfstlpair<_ZFP_I_ZFThreadPoolTaskData *, zfbool>(taskData, zftrue)).first;
    d->taskList.push_back(taskData);
    --(taskData->listIt = d->taskList.end());

    zfindex taskCount = (zfindex)d->taskList.size();
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
        threadPool->threadName(this->threadPoolName());
        threadPool->observerAdd(ZFThread::E_ThreadOnStopRequested(), d->threadOnStopRequested);
        threadPool->taskQueueInit();
        threadPool->threadStart();
        ++(d->refCount);
        threadPool->taskQueueAdd(d->threadOnRun);
    }

    return taskData;
}

ZFMETHOD_DEFINE_0(ZFThreadPool, void, removeAll) {
    if(d == zfnull) {
        return;
    }
    do {
        ZFCoreMutexLock();
        while(!d->taskList.empty()) {
            (*(d->taskList.begin()))->stop();
        }
        if(d->threadPool.isEmpty()) {
            ZFCoreMutexUnlock();
            break;
        }
        zfautoT<ZFThread> runThread = d->threadPool.removeAndGet(0);
        ZFCoreMutexUnlock();

        runThread->threadStop();
        runThread->taskQueueCleanup();
        runThread->sleepCancel();
    } while(zftrue);

    ZFCoreMutexLocker();
    d->threadOnRun = zfnull;
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
        d = zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

