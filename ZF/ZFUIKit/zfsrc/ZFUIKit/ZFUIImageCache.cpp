#include "ZFUIImageCache.h"
#include "ZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfauto, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    ZFLISTENER(loadImpl) {
        zfargs.result(ZFUIImageFromInput(zfargs.param0().zfv()));
    } ZFLISTENER_END()
    return ZFIOCacheLoad(src, callback, loadImpl);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFUIImageLoadCancel
        , ZFMP_IN(const zfauto &, taskId)
        ) {
    ZFIOCacheLoadCancel(taskId);
}

ZF_NAMESPACE_GLOBAL_END

