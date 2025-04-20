#include "ZFUIImageCache.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    ZFLISTENER(loadImpl) {
        zfargs.result(ZFUIImageFromInput(zfargs.param0().to<v_ZFInput *>()->zfv));
    } ZFLISTENER_END()
    return ZFIOCacheLoad(src, callback, loadImpl);
}

// ============================================================
ZFOBJECT_REGISTER(ZFUIImageLoadTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFUIImageLoadTask
        , ZFMP_IN(const ZFInput &, input)
        ) {
    this->input(input);
}

void ZFUIImageLoadTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->input()) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implOnStop
                , zfweakT<zfself>, owner
                ) {
            owner->_implTaskId = zfnull;
            v_ZFResultType *resultType = zfargs.param1();
            if(resultType->enumValue() == v_ZFResultType::e_Success) {
                owner->notifySuccess(zfargs.param0());
            }
            else {
                owner->notifyFail(zfstr("io cache load fail: %s"
                            , owner->input()
                            ), zfargs.param0());
            }
        } ZFLISTENER_END()
        this->_implTaskId = ZFIOCacheLoad(this->input(), implOnStop);
    }
    else {
        this->notifySuccess();
    }
}
void ZFUIImageLoadTask::taskOnStop(ZF_IN ZFResultType resultType) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop(resultType);
}

ZF_NAMESPACE_GLOBAL_END

