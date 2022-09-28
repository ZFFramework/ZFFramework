#include "ZFThread_zfasync.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_zfasyncTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_zfasyncTaskData, ZFObject)
public:
    zftaskid taskId;
    ZFListener callback;
    zfautoObject userData;
    ZFListener finishCallback;
    zfautoObject finishCallbackUserData;
    zfautoObjectT<ZFThread *> callerThread;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfasyncDataHolder, ZFLevelZFFrameworkEssential)
{
    this->maxThread = 8;
    this->threadPoolIndex = 0;
    this->runThreadListener = ZFCallbackForFunc(zfself::_ZFP_runThread);
    this->callerThreadListener = ZFCallbackForFunc(zfself::_ZFP_callerThread);
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
    ZFTaskIdGenerator taskIdGenerator;

    ZFListener runThreadListener;
    ZFListener callerThreadListener;
public:
    static void _ZFP_runThread(ZF_IN const ZFListener &listenerData, ZF_IN ZFObject *userData)
    {
        zfCoreMutexLock();
        ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
        _ZFP_I_zfasyncTaskData *taskData = userData->to<_ZFP_I_zfasyncTaskData *>();
        if(!d->taskIdGenerator.idExist(taskData->taskId))
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        taskData->callback.execute(ZFListenerData(), taskData->userData);

        zfCoreMutexLock();
        if(!d->taskIdGenerator.idExist(taskData->taskId))
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        if(!taskData->finishCallback.callbackIsValid())
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        ZFThread::executeInThread(taskData->callerThread, d->callerThreadListener, taskData);
    }
    static void _ZFP_callerThread(ZF_IN const ZFListener &listenerData, ZF_IN ZFObject *userData)
    {
        zfCoreMutexLock();
        ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
        _ZFP_I_zfasyncTaskData *taskData = userData->to<_ZFP_I_zfasyncTaskData *>();
        if(!d->taskIdGenerator.idExist(taskData->taskId))
        {
            _ZFP_taskDataCleanup(d, taskData);
            zfCoreMutexUnlock();
            return;
        }
        zfCoreMutexUnlock();

        taskData->finishCallback.execute(ZFListenerData(), taskData->finishCallbackUserData);

        zfCoreMutexLock();
        _ZFP_taskDataCleanup(d, taskData);
        zfCoreMutexUnlock();
    }
private:
    static void _ZFP_taskDataCleanup(ZF_IN ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d, ZF_IN _ZFP_I_zfasyncTaskData *taskData)
    {
        d->taskIdGenerator.idRelease(taskData->taskId);
        taskData->callback.callbackClear();
        taskData->userData = zfnull;
        taskData->finishCallback.callbackClear();
        taskData->finishCallbackUserData = zfnull;
        taskData->callerThread = zfnull;
    }
ZF_GLOBAL_INITIALIZER_END(zfasyncDataHolder)

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zftaskid, zfasync,
                       ZFMP_IN(const ZFListener &, callback),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                       ZFMP_IN_OPT(const ZFListener &, finishCallback, ZFCallback()),
                       ZFMP_IN_OPT(ZFObject *, finishCallbackUserData, zfnull))
{
    if(!callback.callbackIsValid())
    {
        return zftaskidInvalid();
    }

    zfCoreMutexLock();
    ZF_GLOBAL_INITIALIZER_CLASS(zfasyncDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder);
    zfautoObjectT<ZFThread *> thread;
    for(zfindex i = 0; i < d->threadPool.count(); ++i)
    {
        if(d->threadPool[i]->taskQueueCount() == 0)
        {
            thread = d->threadPool[i];
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

    zfblockedAlloc(_ZFP_I_zfasyncTaskData, taskData);
    taskData->taskId = d->taskIdGenerator.idAcquire();
    taskData->callback = callback;
    taskData->userData = userData;
    taskData->finishCallback = finishCallback;
    taskData->finishCallbackUserData = finishCallbackUserData;
    if(finishCallback.callbackIsValid())
    {
        taskData->callerThread = ZFThread::currentThread();
    }
    zfCoreMutexUnlock();

    thread->taskQueueAdd(d->runThreadListener, taskData);
    return taskData->taskId;
}

ZFMETHOD_FUNC_DEFINE_1(void, zfasyncCancel,
                       ZFMP_IN_OUT(zftaskid &, taskid))
{
    zfCoreMutexLock();
    ZF_GLOBAL_INITIALIZER_INSTANCE(zfasyncDataHolder)->taskIdGenerator.idRelease(taskid);
    zfCoreMutexUnlock();
}

ZF_NAMESPACE_GLOBAL_END

