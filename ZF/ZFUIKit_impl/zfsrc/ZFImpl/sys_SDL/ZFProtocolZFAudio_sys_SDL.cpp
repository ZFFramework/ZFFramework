#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAudio.h"

#if ZF_ENV_sys_SDL

#include "SDL3_mixer/SDL_mixer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAudioImpl_sys_SDL, ZFAudio, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_mixer")

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        _implFade = 500;
        MIX_Init();
        _implMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, zfnull);
    }
    zfoverride
    virtual void protocolOnDealloc(void) {
        MIX_DestroyMixer(_implMixer);
        MIX_Quit();
        zfsuper::protocolOnDealloc();
    }

private:
    zfclassNotPOD NativeAudio {
    public:
        MIX_Track *implTrack;
        zfautoT<ZFValueHolder> implAudio; // MIX_Audio
        zfautoT<ZFTaskId> loadTaskId;
    public:
        NativeAudio(void)
        : implTrack(zfnull)
        , implAudio(zfnull)
        , loadTaskId(zfnull)
        {
        }
    };
    static void _implAudioOnDealloc(ZF_IN void *v) {
        if(v) {
            MIX_DestroyAudio((MIX_Audio *)v);
        }
    }

public:
    virtual void *nativeAudioCreate(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = zfpoolNew(NativeAudio);
        nativeAudio->implTrack = MIX_CreateTrack(_implMixer);
        return nativeAudio;
    }
    virtual void nativeAudioDestroy(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->loadTaskId) {
            nativeAudio->loadTaskId->stop();
        }
        if(nativeAudio->implTrack) {
            MIX_DestroyTrack(nativeAudio->implTrack);
        }
        zfpoolDelete(nativeAudio);
    }

    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const ZFInput &input
            ) {
        zfself *owner = this;

        ZFLISTENER_3(onLoad
                , zfself *, owner
                , zfautoT<ZFAudio>, audio
                , ZFInput, input
                ) {
            if(zfargs.param0() == zfnull) {
                return;
            }
            NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
            if(nativeAudio == zfnull) {
                return;
            }
            MIX_Audio *implAudio = MIX_LoadAudio_IO(owner->_implMixer, ZFImpl_sys_SDL_ZFInputToSDL_IOStream(input), zffalse, zftrue);
            if(implAudio == zfnull) {
                zfargs.result(zfobj<v_zfstring>(SDL_GetError()));
                return;
            }
            zfargs.result(zfobj<ZFValueHolder>(implAudio, _implAudioOnDealloc));
        } ZFLISTENER_END()

        ZFLISTENER_2(onLoadFinish
                , zfself *, owner
                , zfautoT<ZFAudio>, audio
                ) {
            NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
            nativeAudio->loadTaskId = zfnull;

            v_zfstring *errorHint = zfargs.param0();
            if(errorHint) {
                owner->notifyAudioOnLoad(audio, zffalse, errorHint);
                return;
            }

            nativeAudio->implAudio = zfargs.param0();
            if(!nativeAudio->implAudio) {
                MIX_SetTrackAudio(nativeAudio->implTrack, zfnull);
                owner->notifyAudioOnLoad(audio, zffalse, zfobj<v_zfstring>("unable to load audio"));
                return;
            }

            if(!MIX_SetTrackAudio(nativeAudio->implTrack, nativeAudio->implAudio->valueT<MIX_Audio *>())) {
                zfstring errorHint = SDL_GetError();
                MIX_SetTrackAudio(nativeAudio->implTrack, zfnull);
                owner->notifyAudioOnLoad(audio, zffalse, zfobj<v_zfstring>(errorHint));
                return;
            }

            owner->notifyAudioOnLoad(audio, zftrue, zfnull);
        } ZFLISTENER_END()

        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        nativeAudio->loadTaskId = zfasync(onLoad, onLoadFinish);
    }
    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const zfstring &url
            ) {
        zfself *owner = this;

        ZFLISTENER_1(onLoad
                , zfstring, url
                ) {
            if(zfargs.param0() == zfnull) {return;}

            const ZFMethod *loaderMethod = ZFMethodFuncForName(zfnull, "ZFInputForHttp");
            if(loaderMethod == zfnull || !ZFProtocolIsAvailable("ZFHttpRequest")) {
                zfargs.result(zfobj<v_zfstring>("net load depends on ZFHttpRequest impl"));
                return;
            }

            zfauto inputHolder = ZFInvoke("ZFInputForHttp", zfobj<v_zfstring>(url));
            v_ZFInput *inputTmp = inputHolder;
            if(inputTmp == zfnull || !inputTmp->zfv) {
                zfargs.result(zfobj<v_zfstring>(zfstr("unable to load from url: %s", url)));
                return;
            }

            zfargs.result(inputHolder);
        } ZFLISTENER_END()

        ZFLISTENER_2(onLoadFinish
                , zfself *, owner
                , zfautoT<ZFAudio>, audio
                ) {
            NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
            nativeAudio->loadTaskId = zfnull;

            v_ZFInput *input = zfargs.param0();
            if(input == zfnull || !input->zfv) {
                MIX_SetTrackAudio(nativeAudio->implTrack, zfnull);
                owner->notifyAudioOnLoad(audio, zffalse, zfobj<v_zfstring>("unable to load from url"));
                return;
            }

            if(!MIX_SetTrackIOStream(nativeAudio->implTrack, ZFImpl_sys_SDL_ZFInputToSDL_IOStream(input->zfv), zftrue)) {
                zfstring errorHint = SDL_GetError();
                MIX_SetTrackAudio(nativeAudio->implTrack, zfnull);
                owner->notifyAudioOnLoad(audio, zffalse, zfobj<v_zfstring>(errorHint));
                return;
            }

            owner->notifyAudioOnLoad(audio, zftrue, zfnull);
        } ZFLISTENER_END()

        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
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

        zfclassNotPOD Cb {
        public:
            static void a(void *userdata, MIX_Track *track) {
                ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnStop((ZFAudio *)userdata, zftrue, zfnull);
            }
        };
        MIX_SetTrackStoppedCallback(nativeAudio->implTrack, Cb::a, audio);

        MIX_PlayTrack(nativeAudio->implTrack, 0);
    }
    virtual void nativeAudioStop(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        MIX_SetTrackStoppedCallback(nativeAudio->implTrack, zfnull, zfnull);
        MIX_StopTrack(nativeAudio->implTrack, MIX_TrackMSToFrames(nativeAudio->implTrack, _implFade));
    }
    virtual void nativeAudioResume(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        MIX_ResumeTrack(nativeAudio->implTrack);
        this->notifyAudioOnResume(audio);
    }
    virtual void nativeAudioPause(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        MIX_PauseTrack(nativeAudio->implTrack);
        this->notifyAudioOnPause(audio);
    }

    virtual zftimet nativeAudioDuration(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        Sint64 played = MIX_GetTrackPlaybackPosition(nativeAudio->implTrack);
        Sint64 remain = MIX_GetTrackRemaining(nativeAudio->implTrack);
        return (zftimet)MIX_TrackFramesToMS(nativeAudio->implTrack
                , (Uint64)(0
                    + played >= 0 ? played : 0
                    + remain >= 0 ? remain : 0
                    ));
    }
    virtual zftimet nativeAudioPosition(ZF_IN ZFAudio *audio) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        Sint64 played = MIX_GetTrackPlaybackPosition(nativeAudio->implTrack);
        return (zftimet)MIX_TrackFramesToMS(nativeAudio->implTrack, (Uint64)(played >= 0 ? played : 0));
    }
    virtual void nativeAudioPosition(
            ZF_IN ZFAudio *audio
            , ZF_IN zftimet position
            ) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        MIX_SetTrackPlaybackPosition(nativeAudio->implTrack, MIX_TrackMSToFrames(nativeAudio->implTrack, (Uint64)position));
    }

    virtual void nativeAudioVolume(
            ZF_IN ZFAudio *audio
            , ZF_IN zffloat volume
            ) {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        MIX_SetTrackGain(nativeAudio->implTrack, (float)volume);
    }

private:
    Uint64 _implFade;
    MIX_Mixer *_implMixer;
ZFPROTOCOL_IMPLEMENTATION_END(ZFAudioImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

