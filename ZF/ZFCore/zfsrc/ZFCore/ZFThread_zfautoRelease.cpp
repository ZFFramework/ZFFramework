#include "ZFThread_zfautoRelease.h"
#include "ZFAutoReleasePool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoRelease
void _ZFP_zfautoReleaseAction(ZF_IN ZFObject *obj) {
    if(obj) {
        ZFCoreMutexLocker();

        if(ZFThread::implAvailable()) {
            ZFThread *threadCur = ZFThread::currentThread();
            if(threadCur == zfnull) {
                ZFCoreCriticalMessageTrim("[zfautoRelease] current thread is null, make sure the thread is started or registered by ZFThread");
                return;
            }
            threadCur->autoReleasePoolAdd(obj);
        }
        else {
            ZFAutoReleasePool::instance()->poolAdd(obj);
        }
    }
}

ZF_STATIC_INITIALIZER_INIT(zfvAccessFinishDelayImpl) {
    _ZFP_zfvAccessFinishDelayImpl() = zfself::a;
}
private:
    static void a(ZF_IN const zfauto &obj) {
        zfautoRelease(zfRetain(obj));
    }
ZF_STATIC_INITIALIZER_END(zfvAccessFinishDelayImpl)

ZF_NAMESPACE_GLOBAL_END

