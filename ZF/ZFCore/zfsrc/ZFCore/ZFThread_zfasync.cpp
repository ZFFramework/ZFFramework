#include "ZFThread_zfasync.h"
#include "ZFThreadPool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfauto, zfasync
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    return ZFThreadPool::instance()->start(callback, finishCallback);
}
ZFMETHOD_FUNC_DEFINE_1(void, zfasyncCancel
        , ZFMP_IN(const zfauto &, taskId)
        ) {
    ZFThreadPool::instance()->stop(taskId);
}

ZF_NAMESPACE_GLOBAL_END

