#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAudio.h"

#if ZF_ENV_sys_SDL

#include "SDL_mixer.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFAudioImpl_sys_SDL_ImplHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFAudioImpl_sys_SDL_ImplHolder, ZFObject)

public:
    ZFPROPERTY_RETAIN(zfanyT<v_zfstring>, errorHint)

public:
    void impl(ZF_IN Mix_Chunk *impl) {
        if(_ZFP_impl != impl) {
            if(_ZFP_impl != zfnull) {
                Mix_FreeChunk(_ZFP_impl);
            }
            _ZFP_impl = impl;
        }
    }
    Mix_Chunk *impl(void) {
        return _ZFP_impl;
    }

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _ZFP_impl = zfnull;
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        if(_ZFP_impl != zfnull) {
            Mix_FreeChunk(_ZFP_impl);
            _ZFP_impl = zfnull;
        }
        zfsuper::objectOnDealloc();
    }

private:
    Mix_Chunk *_ZFP_impl;
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAudioImpl_sys_SDL, ZFAudio, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_mixer")

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        _deviceCount = 0;

        Mix_Init(0
            | MIX_INIT_MP3
            | MIX_INIT_OGG
            );

        Mix_AllocateChannels(64);
        Mix_ChannelFinished(zfself::_implOnFinish);
    }
    zfoverride
    virtual void protocolOnDealloc(void) {
        Mix_ChannelFinished(zfnull);
        Mix_Quit();
        zfsuper::protocolOnDealloc();
    }

public:
    zfclassNotPOD NativeAudio {
    public:
        zfautoT<ZFTaskId> loadTaskId;
        zfautoT<_ZFP_ZFAudioImpl_sys_SDL_ImplHolder> impl;
        int channel;
        zftimet position_resumeTime;
        zftimet position_prev;

    public:
        void position_udpate(void) {
            if(this->position_resumeTime != 0) {
                zftimet curTime = ZFTime::timestamp();
                this->position_prev += curTime - this->position_resumeTime;
                this->position_resumeTime = curTime;
            }
        }

    public:
        NativeAudio(void)
        : loadTaskId(zfnull)
        , impl()
        , channel(-1)
        , position_resumeTime(0)
        , position_prev(0)
        {
        }
    };

public:
    virtual void *nativeAudioCreate(ZF_IN ZFAudio *audio) {
        _deviceAttach();
        return zfnew(NativeAudio);
    }
    virtual void nativeAudioDestroy(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        zfdelete(nativeAudio);
        _deviceDetach();
    }

    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const ZFInput &input
            ) {
        zfself *owner = this;
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();

        ZFLISTENER_1(onLoad
                , ZFInput, input
                ) {
            Mix_Chunk *impl = Mix_LoadWAV_RW(ZFImpl_sys_SDL_ZFInputToSDL_RWops(input), 1);
            zfobj<_ZFP_ZFAudioImpl_sys_SDL_ImplHolder> implHolder;
            implHolder->impl(impl);
            const char *errorHint = Mix_GetError();
            if(errorHint != zfnull && *errorHint) {
                implHolder->errorHint(zfobj<v_zfstring>(errorHint));
            }
            zfargs.result(implHolder);
        } ZFLISTENER_END()

        ZFLISTENER_2(onLoadFinish
                , zfself *, owner
                , ZFAudio *, audio
                ) {
            NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
            nativeAudio->loadTaskId = zfnull;

            _ZFP_ZFAudioImpl_sys_SDL_ImplHolder *implHolder = zfargs.param0();
            nativeAudio->impl = implHolder;
            owner->notifyAudioOnLoad(audio, implHolder->impl() != zfnull, implHolder->errorHint());
        } ZFLISTENER_END()

        nativeAudio->loadTaskId = zfasync(onLoad, onLoadFinish);
    }
    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const zfstring &url
            ) {
        zfself *owner = this;
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        const ZFMethod *loaderMethod = ZFMethodFuncForName(zfnull, "ZFInputForHttp");
        if(loaderMethod == zfnull || !ZFProtocolIsAvailable("ZFHttpRequest")) {
            zfobj<v_zfstring> errorHint;
            errorHint->zfv = "net load depends on ZFHttpRequest impl";
            this->notifyAudioOnLoad(audio, zffalse, errorHint);
            return;
        }

        ZFLISTENER_1(onLoad
                , zfstring, url
                ) {
            v_zfidentity *taskId = zfargs.param0();
            if(taskId->zfv == zfidentityInvalid()) {return;}
            zfobj<_ZFP_ZFAudioImpl_sys_SDL_ImplHolder> implHolder;
            zfargs.result(implHolder);

            ZFInput input;
            const ZFMethod *loaderMethod = ZFMethodFuncForName(zfnull, "ZFInputForHttp");
            if(loaderMethod == zfnull || !ZFProtocolIsAvailable("ZFHttpRequest")) {
                implHolder->errorHint(zfobj<v_zfstring>("net load depends on ZFHttpRequest impl"));
            }
            else {
                zfauto inputHolder = ZFInvoke("ZFInputForHttp", zfobj<v_zfstring>(url));
                v_ZFInput *inputTmp = inputHolder;
                if(inputTmp == zfnull || !inputTmp->zfv) {
                    implHolder->errorHint(zfobj<v_zfstring>(zfstr("unable to load from url: %s", url)));
                }
                else {
                    input = inputTmp->zfv;
                }
            }

            if(taskId->zfv == zfidentityInvalid()) {return;}
            if(input) {
                Mix_Chunk *impl = Mix_LoadWAV_RW(ZFImpl_sys_SDL_ZFInputToSDL_RWops(input), 1);
                implHolder->impl(impl);
                const char *errorHint = Mix_GetError();
                if(errorHint != zfnull && *errorHint) {
                    implHolder->errorHint(zfobj<v_zfstring>(errorHint));
                }
            }
        } ZFLISTENER_END()

        ZFLISTENER_2(onLoadFinish
                , zfself *, owner
                , ZFAudio *, audio
                ) {
            NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
            nativeAudio->loadTaskId = zfnull;

            _ZFP_ZFAudioImpl_sys_SDL_ImplHolder *implHolder = zfargs.param0();
            nativeAudio->impl = implHolder;
            owner->notifyAudioOnLoad(audio, implHolder->impl() != zfnull, implHolder->errorHint());
        } ZFLISTENER_END()

        nativeAudio->loadTaskId = zfasync(onLoad, onLoadFinish);
    }
    virtual void nativeAudioLoadCancel(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->loadTaskId) {
            nativeAudio->loadTaskId->stop();
            nativeAudio->loadTaskId = zfnull;
        }
    }

    virtual void nativeAudioStart(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        nativeAudio->channel = Mix_PlayChannelTimed(-1, nativeAudio->impl->impl(), 0, (int)_durationForChunk(nativeAudio->impl->impl()));
        if(nativeAudio->channel == -1) {
            zfobj<v_zfstring> errorHint;
            const char *implError = Mix_GetError();
            if(implError != zfnull && *implError) {
                errorHint->zfv = implError;
            }
            else {
                errorHint->zfv = "unable to play audio";
            }
            this->notifyAudioOnStop(audio, zffalse, errorHint);
        }
        else {
            this->_implAttach(audio);
            nativeAudio->position_resumeTime = ZFTime::timestamp();
            nativeAudio->position_prev = 0;
            this->notifyAudioOnResume(audio);
        }
    }
    virtual void nativeAudioStop(ZF_IN ZFAudio *audio) {
        zfblockedRelease(zfRetain(audio));

        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        int channel = nativeAudio->channel;
        _implDetach(audio);
        if(channel != -1) {
            Mix_HaltChannel(channel);
        }
        nativeAudio->position_udpate();
    }
    virtual void nativeAudioResume(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->channel != -1) {
            Mix_Resume(nativeAudio->channel);
        }
        nativeAudio->position_resumeTime = ZFTime::timestamp();
        this->notifyAudioOnResume(audio);
    }
    virtual void nativeAudioPause(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        Mix_Pause(nativeAudio->channel);
        nativeAudio->position_udpate();
        this->notifyAudioOnPause(audio);
    }

    virtual zftimet nativeAudioDuration(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->impl == zfnull || nativeAudio->impl->impl() == zfnull) {
            return 0;
        }
        return _durationForChunk(nativeAudio->impl->impl());
    }
    virtual zftimet nativeAudioPosition(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->position_resumeTime == 0) {
            return nativeAudio->position_prev;
        }
        else {
            zftimet curTime = ZFTime::timestamp();
            return nativeAudio->position_prev + (curTime - nativeAudio->position_resumeTime);
        }
    }
    virtual void nativeAudioPosition(
            ZF_IN ZFAudio *audio
            , ZF_IN zftimet position
            ) {
        // not supported
    }

    virtual void nativeAudioVolume(
            ZF_IN ZFAudio *audio
            , ZF_IN zffloat volume
            ) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        Mix_VolumeChunk(nativeAudio->impl->impl(), (int)(volume * MIX_MAX_VOLUME));
    }

private:
    zftimet _durationForChunk(ZF_IN Mix_Chunk *impl) {
        // MIX_DEFAULT_FORMAT is 16 bit, 2 bytes
        return 1000 * (zftimet)impl->alen / MIX_DEFAULT_FREQUENCY / 2 / MIX_DEFAULT_CHANNELS;
    }

private:
    zfindex _deviceCount;
    void _deviceAttach(void) {
        ++_deviceCount;
        if(_deviceCount == 1) {
            Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
        }
    }
    void _deviceDetach(void) {
        --_deviceCount;
        if(_deviceCount == 0) {
            Mix_CloseAudio();
        }
    }

private:
    static void _implOnFinish(int channel) {
        ZFCoreMutexLock();
        zfself *d = (zfself *)ZFPROTOCOL_ACCESS(ZFAudio);
        zfstlhashmap<zfidentity, zfautoT<ZFAudio> >::iterator it = d->_implPlaying.find(channel);
        if(it == d->_implPlaying.end()) {
            ZFCoreMutexUnlock();
            return;
        }
        zfautoT<ZFAudio> audio = it->second;
        d->_implPlaying.erase(it);
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        nativeAudio->channel = -1;
        ZFCoreMutexUnlock();

        d->notifyAudioOnStop(audio, zftrue, zfnull);
    }
    void _implAttach(ZF_IN ZFAudio *audio) {
        ZFCoreMutexLocker();
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        ZFCoreAssert(nativeAudio->channel != -1);
        _implPlaying[nativeAudio->channel] = audio;
    }
    void _implDetach(ZF_IN ZFAudio *audio) {
        ZFCoreMutexLocker();
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        _implPlaying.erase(nativeAudio->channel);
    }

private:
    zfstlhashmap<zfidentity, zfautoT<ZFAudio> > _implPlaying;
ZFPROTOCOL_IMPLEMENTATION_END(ZFAudioImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

