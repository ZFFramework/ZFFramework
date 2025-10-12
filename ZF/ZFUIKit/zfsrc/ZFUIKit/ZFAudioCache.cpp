#include "ZFAudioCache.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFAudio>, ZFAudioLoad
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    zfstring key = ZFPathInfoToString(pathInfo);
    zfautoT<ZFAudio> impl = ZFAudioCache::instance()->cacheGet(key);
    if(!impl) {
        ZFInput input = ZFInputForPathInfo(pathInfo);
        if(!input) {
            return zfnull;
        }

        impl = zfobj<ZFAudio>();
        impl->load(input);
    }
    if(!impl->startable()) {
        return zfnull;
    }
    ZFAudioCache::instance()->cacheAdd(key, impl);
    return impl;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFAudio>, ZFAudioLoad
        , ZFMP_IN(const ZFInput &, src)
        ) {
    zfautoT<ZFAudio> impl;
    if(src.callbackId()) {
        impl = ZFAudioCache::instance()->cacheGet(src.callbackId());
    }
    if(!impl) {
        impl = zfobj<ZFAudio>();
        impl->load(src);
    }
    if(!impl->startable()) {
        return zfnull;
    }
    if(src.callbackId()) {
        ZFAudioCache::instance()->cacheAdd(src.callbackId(), impl);
    }
    return impl;
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFAudio>, ZFAudioStart
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        ) {
    zfautoT<ZFAudio> impl = ZFAudioLoad(src);
    if(!impl) {
        callback.execute(ZFArgs()
                .param0(zfobj<v_ZFResultType>(v_ZFResultType::e_Fail))
                .param1(zfobj<v_zfstring>(zfstr("unable to load audio from input: %s"
                            , src
                            )))
                );
        return zfnull;
    }
    if(callback) {
        impl->observerAddForOnce(ZFAudio::E_AudioOnStop(), callback);
    }
    impl->start();
    return impl;
}
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFAudio>, ZFAudioStart
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        ) {
    zfautoT<ZFAudio> impl = ZFAudioLoad(pathInfo);
    if(!impl) {
        callback.execute(ZFArgs()
                .param0(zfobj<v_ZFResultType>(v_ZFResultType::e_Fail))
                .param1(zfobj<v_zfstring>(zfstr("unable to load audio from pathInfo: %s"
                            , pathInfo
                            )))
                );
        return zfnull;
    }
    if(callback) {
        impl->observerAddForOnce(ZFAudio::E_AudioOnStop(), callback);
    }
    impl->start();
    return impl;
}

// ============================================================
ZFOBJECT_REGISTER(ZFAudioStartTask)

ZFMETHOD_DEFINE_1(ZFAudioStartTask, void, load
        , ZFMP_IN(ZFAudio *, impl)
        ) {
    this->stop();
    _impl = impl;
}
ZFMETHOD_DEFINE_1(ZFAudioStartTask, void, load
        , ZFMP_IN(const ZFInput &, input)
        ) {
    this->stop();
    _impl = zfobj<ZFAudio>();
    _impl->load(input);
}
ZFMETHOD_DEFINE_1(ZFAudioStartTask, void, load
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    this->stop();
    _impl = zfobj<ZFAudio>();
    _impl->load(pathInfo);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAudioStartTask
        , ZFMP_IN(ZFAudio *, impl)
        ) {
    this->load(impl);
}
ZFOBJECT_ON_INIT_DEFINE_1(ZFAudioStartTask
        , ZFMP_IN(const ZFInput &, input)
        ) {
    this->load(input);
}
ZFOBJECT_ON_INIT_DEFINE_1(ZFAudioStartTask
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    this->load(pathInfo);
}

void ZFAudioStartTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(_impl && _impl->startable()) {
        _impl->loop(this->loop());

        zfweakT<zfself> owner;
        ZFLISTENER_1(onLoad
                , zfweakT<zfself>, owner
                ) {
            v_ZFResultType *resultType = zfargs.param0();
            if(resultType->zfv() != v_ZFResultType::e_Success) {
                v_zfstring *errorHint = zfargs.param1();
                if(errorHint) {
                    owner->errorHint(errorHint->zfv);
                }
                owner->stop(resultType->zfv());
            }
        } ZFLISTENER_END()
        ZFLISTENER_1(onStop
                , zfweakT<zfself>, owner
                ) {
            v_ZFResultType *resultType = zfargs.param0();
            if(resultType->zfv() != v_ZFResultType::e_Success) {
                v_zfstring *errorHint = zfargs.param1();
                if(errorHint) {
                    owner->errorHint(errorHint->zfv);
                }
                owner->stop(resultType->zfv());
            }
            else {
                owner->notifySuccess();
            }
        } ZFLISTENER_END()
        ZFObserverGroup(_holder, _impl)
            .on(ZFAudio::E_AudioOnLoad(), onLoad)
            .on(ZFAudio::E_AudioOnLoad(), onStop)
            ;

        _impl->start();
    }
    else {
        this->notifySuccess();
    }
}
void ZFAudioStartTask::taskOnStop(void) {
    ZFObserverGroupRemove(_holder);
    if(this->_impl) {
        this->_impl->stop();
    }
    zfsuper::taskOnStop();
}

// ============================================================
ZFOBJECT_REGISTER(ZFAudioCache)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFAudioCache, instance, ZFLevelZFFrameworkNormal)

ZF_NAMESPACE_GLOBAL_END

