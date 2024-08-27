#include "ZFUIImageCache.h"
#include "ZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    ZFLISTENER(loadImpl) {
        zfargs.result(ZFUIImageFromInput(zfargs.param0().zfv()));
    } ZFLISTENER_END()
    return ZFIOCacheLoad(src, callback, loadImpl);
}

ZF_NAMESPACE_GLOBAL_END

