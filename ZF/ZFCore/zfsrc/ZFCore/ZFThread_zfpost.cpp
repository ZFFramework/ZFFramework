#include "ZFThread_zfpost.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_zfpostTask : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_zfpostTask, ZFObject)

public:
    zfautoT<ZFThread> target;
    ZFListener callback;
};

ZFMETHOD_FUNC_DEFINE_2(zfauto, zfpost
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(ZFThread *, target, zfnull)
        ) {
    if(!callback) {
        return zfnull;
    }
    if(target == zfnull || !target->taskQueueAvailable()) {
        target = ZFThread::mainThread();
    }
    zfobj<_ZFP_zfpostTask> taskId;
    taskId->target = target;
    taskId->callback = callback;
    target->taskQueueAdd(callback);
    return taskId;
}

ZFMETHOD_FUNC_DEFINE_1(void, zfpostCancel
        , ZFMP_IN(const zfauto &, taskId)
        ) {
    _ZFP_zfpostTask *task = taskId;
    if(task) {
        if(task->target) {
            task->target->taskQueueRemove(task->callback);
        }
        task->target = zfnull;
        task->callback = zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

