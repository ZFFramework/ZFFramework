#include "ZFThread_zfpost.h"
#include "ZFTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_zfpostTask : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_zfpostTask, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)
public:
    zfautoT<ZFThread> target;
    ZFListener callback;
public:
    zfoverride
    virtual void stop(void) {
        if(this->target) {
            this->target->taskQueueRemove(this->callback);
            this->target = zfnull;
            this->callback = zfnull;
        }
    }
};

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, zfpost
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

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, zfpostDelayed
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(ZFThread *, target, zfnull)
        ) {
    if(delay <= 0) {
        return zfpost(callback, target);
    }
    if(target == zfnull) {
        target = ZFThread::mainThread();
        if(target == zfnull) {
            return zfnull;
        }
    }
    ZFCoreAssert(target->taskQueueAvailable());

    zfobj<ZFTaskIdWrapper> wrap;

    ZFLISTENER_3(onDelay
            , zfautoT<ZFTaskIdWrapper>, wrap
            , zfweakT<ZFThread>, target
            , ZFListener, callback
            ) {
        if(target == zfnull) {
            ZFCoreCriticalMessageTrim("[zfpostDelayed] target thread has been destroyed, callback: %s", callback);
            return;
        }
        wrap->impl(zfpost(callback, target));
    } ZFLISTENER_END()
    wrap->impl(ZFTimerOnce(delay, onDelay));
    return wrap;
}

ZF_NAMESPACE_GLOBAL_END

