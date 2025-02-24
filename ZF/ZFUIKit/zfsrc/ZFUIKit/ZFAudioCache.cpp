#include "ZFAudioCache.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFAudio>, ZFAudioPlay
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
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
        callback.execute(ZFArgs()
                .param0(zfobj<v_ZFResultType>(ZFResultType::e_Fail))
                .param1(zfobj<v_zfstring>(zfstr("unable to load audio from input: %s"
                            , src
                            )))
                );
        return zfnull;
    }
    if(src.callbackId()) {
        ZFAudioCache::instance()->cacheAdd(src.callbackId(), impl);
    }
    if(callback) {
        impl->observerAddForOnce(ZFAudio::E_AudioOnStop(), callback);
    }
    impl->start();
    return impl;
}
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFAudio>, ZFAudioPlay
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        ) {
    zfautoT<ZFAudio> impl;
    if(url) {
        impl = ZFAudioCache::instance()->cacheGet(url);
        if(!impl) {
            impl = zfobj<ZFAudio>();
            impl->load(url);
        }
    }
    if(!impl->startable()) {
        callback.execute(ZFArgs()
                .param0(zfobj<v_ZFResultType>(ZFResultType::e_Fail))
                .param1(zfobj<v_zfstring>(zfstr("unable to load audio from url: %s"
                            , url
                            )))
                );
        return zfnull;
    }
    ZFAudioCache::instance()->cacheAdd(url, impl);
    if(callback) {
        impl->observerAddForOnce(ZFAudio::E_AudioOnStop(), callback);
    }
    impl->start();
    return impl;
}

// ============================================================
ZFOBJECT_REGISTER(ZFAudioPlayTask)

ZFMETHOD_DEFINE_1(ZFAudioPlayTask, void, load
        , ZFMP_IN(ZFAudio *, impl)
        ) {
    this->stop();
    _impl = impl;
}
ZFMETHOD_DEFINE_1(ZFAudioPlayTask, void, load
        , ZFMP_IN(const ZFInput &, input)
        ) {
    this->stop();
    _impl = zfobj<ZFAudio>();
    _impl->load(input);
}
ZFMETHOD_DEFINE_1(ZFAudioPlayTask, void, load
        , ZFMP_IN(const zfstring &, url)
        ) {
    this->stop();
    _impl = zfobj<ZFAudio>();
    _impl->load(url);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAudioPlayTask
        , ZFMP_IN(ZFAudio *, impl)
        ) {
    this->objectOnInit();
    this->load(impl);
}
ZFOBJECT_ON_INIT_DEFINE_1(ZFAudioPlayTask
        , ZFMP_IN(const ZFInput &, input)
        ) {
    this->objectOnInit();
    this->load(input);
}
ZFOBJECT_ON_INIT_DEFINE_1(ZFAudioPlayTask
        , ZFMP_IN(const zfstring &, url)
        ) {
    this->objectOnInit();
    this->load(url);
}

void ZFAudioPlayTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(_impl && _impl->startable()) {
        _impl->loop(this->loop());

        zfweakT<zfself> owner;
        ZFLISTENER_1(onLoad
                , zfweakT<zfself>, owner
                ) {
            v_ZFResultType *resultType = zfargs.param0();
            if(resultType->zfv() != ZFResultType::e_Success) {
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
            if(resultType->zfv() != ZFResultType::e_Success) {
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
void ZFAudioPlayTask::taskOnStop(ZF_IN ZFResultType resultType) {
    ZFObserverGroupRemove(_holder);
    if(this->_impl) {
        this->_impl->stop();
    }
    zfsuper::taskOnStop(resultType);
}

// ============================================================
ZFOBJECT_REGISTER(ZFAudioCache)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFAudioCache, instance, ZFLevelZFFrameworkNormal)

ZF_NAMESPACE_GLOBAL_END

