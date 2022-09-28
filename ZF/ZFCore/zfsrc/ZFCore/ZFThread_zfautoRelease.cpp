#include "ZFThread_zfautoRelease.h"
#include "ZFAutoReleasePool.h"
#include "protocol/ZFProtocolZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoRelease
void _ZFP_zfautoReleaseAction(ZF_IN ZFObject *obj)
{
    if(obj)
    {
        zfCoreMutexLocker();

        if(ZFPROTOCOL_IS_AVAILABLE(ZFThread))
        {
            ZFThread *threadCur = ZFThread::currentThread();
            if(threadCur == zfnull)
            {
                zfCoreCriticalMessageTrim("[zfautoRelease] current thread is null, make sure the thread is started or registered by ZFThread");
                return ;
            }
            threadCur->autoReleasePoolAdd(obj);
        }
        else
        {
            ZFAutoReleasePool::instance()->poolAdd(obj);
            zfCoreLogTrim("[zfautoRelease] zfautoRelease called while no auto drain logic support, object %s would not be released normally",
                obj->objectInfoOfInstance().cString());
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

