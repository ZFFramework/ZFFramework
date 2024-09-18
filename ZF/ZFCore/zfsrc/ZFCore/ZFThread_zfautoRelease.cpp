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
            ZFCoreLogTrim("[zfautoRelease] zfautoRelease called while no auto drain logic support, object %s would not be released normally",
                obj->objectInfoOfInstance());
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

