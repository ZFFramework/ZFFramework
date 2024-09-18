#include "ZFResCache.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFResCache)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFResCache, instance, ZFLevelZFFrameworkEssential)

ZFPROPERTY_ON_INIT_DEFINE(ZFResCache, zfindex, cacheMaxSize) {
    propertyValue = 100;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFResCacheAutoCleanup, ZFLevelZFFrameworkPostNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFResCacheAutoCleanup) {
    ZFResCache::instance()->cacheRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFResCacheAutoCleanup)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfauto, zfres
        , ZFMP_IN(const zfchar *, resFilePath)
        , ZFMP_IN_OPT(const ZFPathInfo &, pathInfo, zfnull)
        ) {
    ZFInput input;
    if(pathInfo == zfnull) {
        input = ZFInputForRes(resFilePath);
    }
    else {
        input = ZFInputForLocal(resFilePath, pathInfo);
    }
    return zfres(input);
}
ZFMETHOD_FUNC_DEFINE_1(zfauto, zfres
        , ZFMP_IN(const ZFInput &, input)
        ) {
    if(!input) {
        return zfnull;
    }
    if(input.callbackId() != zfnull) {
        zfauto ret = ZFResCache::instance()->cacheCheck(input.callbackId());
        if(ret != zfnull) {
            return ret;
        }
    }
    zfauto ret;
    if(!ZFObjectIOLoadT(ret, input)) {
        return zfnull;
    }
    if(ret != zfnull && input.callbackId() != zfnull) {
        ZFResCache::instance()->cacheAdd(input.callbackId(), ret);
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfauto, zfres
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_IN(const ZFListener &, loadImpl)
        ) {
    if(!input || !loadImpl) {
        return zfnull;
    }
    if(input.callbackId() != zfnull) {
        zfauto ret = ZFResCache::instance()->cacheCheck(input.callbackId());
        if(ret != zfnull) {
            return ret;
        }
    }
    zfobj<v_ZFInput> inputHolder(input);
    ZFArgs zfargs;
    zfargs.sender(inputHolder);
    loadImpl.execute(zfargs);
    if(zfargs.result() && input.callbackId() != zfnull) {
        ZFResCache::instance()->cacheAdd(input.callbackId(), zfargs.result());
    }
    return zfargs.result();
}

ZF_NAMESPACE_GLOBAL_END

