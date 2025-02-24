#include "ZFThread_zfasync.h"
#include "ZFThreadPool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, zfasync
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    return ZFThreadPool::instance()->start(callback, finishCallback);
}

ZF_NAMESPACE_GLOBAL_END

