#include "ZFThread_zfasync.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_zfasyncTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_zfasyncTaskData, ZFObject)
public:
    zfidentity taskId;
    ZFListener callback;
    zfautoObject result;
    ZFListener finishCallback;
    zfautoObjectT<ZFThread *> callerThread;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfasyncDataHolder, ZFLevelZFFrameworkHigh)
{
    this->maxThread = 8;
    this->threadPoolIndex = 0;
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfasyncDataHolder)
{
    do {
        zfCoreMutexLock();
        if(this->threadPool.isEmpty())
        {
            zfCoreMutexUnlock();
            break;
        }
        zfautoObjectT<ZFThread *> thread = this->threadPool.removeAndGet(0);
        zfCoreMutexUnlock();

        thread->taskQueueCleanup();
        thread->threadWait();
    } while(zftrue);
}
public:
    zfuint maxThread;
    ZFCoreArray<zfautoObjectT<ZFThread *> > threadPool;
    zfindex threadPoolIndex;
    ZFIdentityGenerator taskIdGenerator;
public:
    static void _ZFP_runThread(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_zfasyncTaskData *taskData)
    {
        zfCoreMutexLock();
        ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
        if(!d->taskIdGenerator.idExist(taskData->taskId))
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        ZFArgs zfargsTmp;
        zfargsTmp.resultEnable(zftrue);
        taskData->callback.execute(zfargsTmp
                .userData(taskData->callback.userData())
            );

        zfCoreMutexLock();
        taskData->result = zfargsTmp.result();
        if(!d->taskIdGenerator.idExist(taskData->taskId))
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        if(!taskData->finishCallback)
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        ZFLISTENER_1(callerThread
                , zfautoObjectT<_ZFP_I_zfasyncTaskData *>, taskData
                ) {
            ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder)::_ZFP_callerThread(zfargs, taskData);
        } ZFLISTENER_END()
        ZFThread::executeInThread(taskData->callerThread, callerThread);
    }
    static void _ZFP_callerThread(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_zfasyncTaskData *taskData)
    {
        zfCoreMutexLock();
        ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
        if(!d->taskIdGenerator.idExist(taskData->taskId))
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        taskData->finishCallback.execute(ZFArgs()
                .param0(taskData->result)
                .userData(taskData->finishCallback.userData())
            );

        zfCoreMutexLock();
        _ZFP_taskDataCleanup(d, taskData);
        zfCoreMutexUnlock();
    }
private:
    static void _ZFP_taskDataCleanup(ZF_IN ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d, ZF_IN _ZFP_I_zfasyncTaskData *taskData)
    {
        d->taskIdGenerator.idRelease(taskData->taskId);
        taskData->taskId = zfidentityInvalid();
        taskData->callback = zfnull;
        taskData->result = zfnull;
        taskData->finishCallback = zfnull;
        taskData->callerThread = zfnull;
    }
ZF_GLOBAL_INITIALIZER_END(zfasyncDataHolder)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfidentity, zfasync,
                       ZFMP_IN(const ZFListener &, callback),
                       ZFMP_IN_OPT(const ZFListener &, finishCallback, ZFCallback()))
{
    if(!callback)
    {
        return zfidentityInvalid();
    }

    zfCoreMutexLock();
    ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
    zfautoObjectT<ZFThread *> thread;
    for(zfindex i = 0; i < d->threadPool.count(); ++i)
    {
        zfautoObjectT<ZFThread *> &tmp = d->threadPool[i];
        if(tmp->taskQueueCount() == 0 && !tmp->taskQueueRunning())
        {
            thread = tmp;
            break;
        }
    }
    if(thread == zfnull)
    {
        if(d->threadPool.count() < d->maxThread)
        {
            thread = zflineAlloc(ZFThread);
            thread->taskQueueInit();
            thread->threadStart();
            d->threadPool.add(thread);
        }
        else
        {
            d->threadPoolIndex = ((d->threadPoolIndex + 1) % d->threadPool.count());
            thread = d->threadPool[d->threadPoolIndex];
        }
    }
    zfCoreAssert(thread != zfnull);
    if(thread == ZFThread::currentThread())
    {
        if(d->threadPool.count() < d->maxThread)
        {
            thread = zflineAlloc(ZFThread);
            thread->taskQueueInit();
            thread->threadStart();
            d->threadPool.add(thread);
        }
        else
        {
            d->threadPoolIndex = ((d->threadPoolIndex + 1) % d->threadPool.count());
            thread = d->threadPool[d->threadPoolIndex];
        }
    }

    zfblockedAlloc(_ZFP_I_zfasyncTaskData, taskData);
    taskData->taskId = d->taskIdGenerator.idAcquire();
    taskData->callback = callback;
    taskData->finishCallback = finishCallback;
    if(finishCallback)
    {
        taskData->callerThread = ZFThread::currentThread();
    }
    zfCoreMutexUnlock();

    ZFLISTENER_1(taskImpl
            , zfautoObjectT<_ZFP_I_zfasyncTaskData *>, taskData
            ) {
        ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder)::_ZFP_runThread(zfargs, taskData);
    } ZFLISTENER_END()
    thread->taskQueueAdd(taskImpl);
    return taskData->taskId;
}

ZFMETHOD_FUNC_DEFINE_1(void, zfasyncCancel,
                       ZFMP_IN(zfidentity, taskId))
{
    zfCoreMutexLock();
    ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder)->taskIdGenerator.idRelease(taskId);
    zfCoreMutexUnlock();
}

ZF_NAMESPACE_GLOBAL_END

