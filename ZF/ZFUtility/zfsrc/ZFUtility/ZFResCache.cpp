#include "ZFResCache.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFResCache)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFResCache, instance, ZFLevelZFFrameworkEssential)

ZFPROPERTY_ON_INIT_DEFINE(ZFResCache, zfindex, cacheMaxSize) {
    propertyValue = 100;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFResCacheAutoCleanup, ZFLevelZFFrameworkPostNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFResCacheAutoCleanup) {
    ZFResCache::instance()->cacheMaxSize(0);
    ZFResCache::instance()->cacheRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFResCacheAutoCleanup)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfautoObject, zfRes
        , ZFMP_IN(const zfchar *, resFilePath)
        , ZFMP_IN_OPT(const ZFPathInfo *, pathInfo, zfnull)
        ) {
    ZFInput input;
    if(pathInfo == zfnull || pathInfo->isEmpty()) {
        input = ZFInputForRes(resFilePath);
    }
    else {
        input = ZFInputForLocal(resFilePath, pathInfo);
    }
    if(!input) {
        return zfnull;
    }
    zfCoreMutexLocker();
    if(input.callbackId() != zfnull) {
        zfautoObject ret = ZFResCache::instance()->cacheGet(input.callbackId());
        if(ret != zfnull) {
            return ret;
        }
    }
    zfautoObject ret;
    if(!ZFObjectIOLoadT(ret, input)) {
        return zfnull;
    }
    if(ret != zfnull && input.callbackId() != zfnull) {
        ZFResCache::instance()->cacheAdd(input.callbackId(), ret);
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

