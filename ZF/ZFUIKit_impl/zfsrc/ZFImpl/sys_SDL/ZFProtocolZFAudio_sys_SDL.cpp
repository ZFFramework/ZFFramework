#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAudio.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl/_repo/SDL/SDL_mixer.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFAudioImpl_sys_SDL_ImplHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFAudioImpl_sys_SDL_ImplHolder, ZFObject)

public:
    ZFPROPERTY_RETAIN(v_zfstring *, errorHint)

public:
    void impl(ZF_IN Mix_Chunk *impl)
    {
        if(_ZFP_impl != impl)
        {
            if(_ZFP_impl != zfnull)
            {
                Mix_FreeChunk(_ZFP_impl);
            }
            _ZFP_impl = impl;
        }
    }
    Mix_Chunk *impl(void)
    {
        return _ZFP_impl;
    }

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        _ZFP_impl = zfnull;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        if(_ZFP_impl != zfnull)
        {
            Mix_FreeChunk(_ZFP_impl);
            _ZFP_impl = zfnull;
        }
        zfsuper::objectOnDealloc();
    }

private:
    Mix_Chunk *_ZFP_impl;
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAudioImpl_sys_SDL, ZFAudio, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL_mixer")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
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
    virtual void protocolOnDealloc(void)
    {
        Mix_ChannelFinished(zfnull);
        Mix_Quit();
        zfsuper::protocolOnDealloc();
    }

public:
    zfclassNotPOD NativeAudio
    {
    public:
        zfidentity loadTaskId;
        zfautoObjectT<_ZFP_ZFAudioImpl_sys_SDL_ImplHolder *> impl;
        int channel;

    public:
        NativeAudio(void)
        : loadTaskId(zfidentityInvalid())
        , impl()
        , channel(-1)
        {
        }
    };

public:
    virtual void *nativeAudioCreate(ZF_IN ZFAudio *audio)
    {
        _deviceAttach();
        return zfnew(NativeAudio);
    }
    virtual void nativeAudioDestroy(ZF_IN ZFAudio *audio)
    {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        zfdelete(nativeAudio);
        _deviceDetach();
    }

    virtual void nativeAudioLoad(ZF_IN ZFAudio *audio,
                                 ZF_IN const ZFInput &input)
    {
        zfself *owner = this;
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();

        ZFLISTENER_1(onLoad
                , ZFInput, input
                ) {
            Mix_Chunk *impl = Mix_LoadWAV_RW(ZFImpl_sys_SDL_ZFInputToSDL_RWops(input), 1);
            zfblockedAlloc(_ZFP_ZFAudioImpl_sys_SDL_ImplHolder, implHolder);
            implHolder->impl(impl);
            const char *errorHint = Mix_GetError();
            if(errorHint != zfnull && *errorHint)
            {
                implHolder->errorHint(zflineAlloc(v_zfstring, errorHint));
            }
            zfargs.result(implHolder);
        } ZFLISTENER_END()

        ZFLISTENER_2(onLoadFinish
                , zfself *, owner
                , ZFAudio *, audio
                ) {
            NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
            nativeAudio->loadTaskId = zfidentityInvalid();

            _ZFP_ZFAudioImpl_sys_SDL_ImplHolder *implHolder = zfargs.param0T();
            nativeAudio->impl = implHolder;
            owner->notifyAudioOnLoad(audio, implHolder->impl() != zfnull, implHolder->errorHint());
        } ZFLISTENER_END()

        nativeAudio->loadTaskId = zfasync(onLoad, onLoadFinish);
    }
    virtual void nativeAudioLoad(ZF_IN ZFAudio *audio,
                                 ZF_IN const zfchar *url)
    {
        zfself *owner = this;
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        const ZFMethod *loaderMethod = ZFMethodFuncForName(zfnull, "ZFInputForHttp");
        if(loaderMethod == zfnull || !ZFProtocolIsAvailable("ZFHttpRequest"))
        {
            zfblockedAlloc(v_zfstring, errorHint);
            errorHint->zfv = "net load depends on ZFHttpRequest impl";
            this->notifyAudioOnLoad(audio, zffalse, errorHint);
            return;
        }

        ZFLISTENER_1(onLoad
                , zfstring, url
                ) {
            v_zfidentity *taskId = zfargs.param0T();
            if(taskId->zfv == zfidentityInvalid()) {return;}
            zfblockedAlloc(_ZFP_ZFAudioImpl_sys_SDL_ImplHolder, implHolder);
            zfargs.result(implHolder);

            ZFInput input;
            const ZFMethod *loaderMethod = ZFMethodFuncForName(zfnull, "ZFInputForHttp");
            if(loaderMethod == zfnull || !ZFProtocolIsAvailable("ZFHttpRequest"))
            {
                implHolder->errorHint(zflineAlloc(v_zfstring, "net load depends on ZFHttpRequest impl"));
            }
            else
            {
                zfautoObject inputHolder = ZFInvoke("ZFInputForHttp", zflineAlloc(v_zfstring, url));
                v_ZFInput *inputTmp = inputHolder;
                if(inputTmp == zfnull || !inputTmp->zfv)
                {
                    implHolder->errorHint(zflineAlloc(v_zfstring, zfstringWithFormat("unable to load from url: %s", url.cString())));
                }
                else
                {
                    input = inputTmp->zfv;
                }
            }

            if(taskId->zfv == zfidentityInvalid()) {return;}
            if(input)
            {
                Mix_Chunk *impl = Mix_LoadWAV_RW(ZFImpl_sys_SDL_ZFInputToSDL_RWops(input), 1);
                implHolder->impl(impl);
                const char *errorHint = Mix_GetError();
                if(errorHint != zfnull && *errorHint)
                {
                    implHolder->errorHint(zflineAlloc(v_zfstring, errorHint));
                }
            }
        } ZFLISTENER_END()

        ZFLISTENER_2(onLoadFinish
                , zfself *, owner
                , ZFAudio *, audio
                ) {
            NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
            nativeAudio->loadTaskId = zfidentityInvalid();

            _ZFP_ZFAudioImpl_sys_SDL_ImplHolder *implHolder = zfargs.param0T();
            nativeAudio->impl = implHolder;
            owner->notifyAudioOnLoad(audio, implHolder->impl() != zfnull, implHolder->errorHint());
        } ZFLISTENER_END()

        nativeAudio->loadTaskId = zfasync(onLoad, onLoadFinish);
    }
    virtual void nativeAudioLoadCancel(ZF_IN ZFAudio *audio)
    {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->loadTaskId != zfidentityInvalid())
        {
            zfasyncCancel(nativeAudio->loadTaskId);
            nativeAudio->loadTaskId = zfidentityInvalid();
        }
    }

    virtual void nativeAudioStart(ZF_IN ZFAudio *audio)
    {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        nativeAudio->channel = Mix_PlayChannelTimed(-1, nativeAudio->impl->impl(), 0, (int)_durationForChunk(nativeAudio->impl->impl()));
        if(nativeAudio->channel == -1)
        {
            zfblockedAlloc(v_zfstring, errorHint);
            const char *implError = Mix_GetError();
            if(implError != zfnull && *implError)
            {
                errorHint->zfv = implError;
            }
            else
            {
                errorHint->zfv = "unable to play audio";
            }
            this->notifyAudioOnStop(audio, zffalse, errorHint);
        }
        else
        {
            this->_implAttach(audio);
            this->notifyAudioOnResume(audio);
        }
    }
    virtual void nativeAudioStop(ZF_IN ZFAudio *audio)
    {
        zfblockedRelease(zfRetain(audio));

        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        int channel = nativeAudio->channel;
        _implDetach(audio);
        if(channel != -1)
        {
            Mix_HaltChannel(channel);
        }
    }
    virtual void nativeAudioResume(ZF_IN ZFAudio *audio)
    {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->channel != -1)
        {
            Mix_Resume(nativeAudio->channel);
        }
        this->notifyAudioOnResume(audio);
    }
    virtual void nativeAudioPause(ZF_IN ZFAudio *audio)
    {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        Mix_Pause(nativeAudio->channel);
        this->notifyAudioOnPause(audio);
    }

    virtual zftimet nativeAudioDuration(ZF_IN ZFAudio *audio)
    {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        if(nativeAudio->impl == zfnull || nativeAudio->impl->impl() == zfnull)
        {
            return 0;
        }
        return _durationForChunk(nativeAudio->impl->impl());
    }
    virtual zftimet nativeAudioPosition(ZF_IN ZFAudio *audio)
    {
        // not supported
        return 0;
    }
    virtual void nativeAudioPosition(ZF_IN ZFAudio *audio,
                                     ZF_IN zftimet position)
    {
        // not supported
    }

    virtual void nativeAudioVolume(ZF_IN ZFAudio *audio,
                                   ZF_IN zffloat volume)
    {
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        Mix_VolumeChunk(nativeAudio->impl->impl(), (int)(volume * MIX_MAX_VOLUME));
    }

private:
    zftimet _durationForChunk(ZF_IN Mix_Chunk *impl)
    {
        // MIX_DEFAULT_FORMAT is 16 bit, 2 bytes
        return 1000 * (zftimet)impl->alen / MIX_DEFAULT_FREQUENCY / 2 / MIX_DEFAULT_CHANNELS;
    }

private:
    zfindex _deviceCount;
    void _deviceAttach(void)
    {
        ++_deviceCount;
        if(_deviceCount == 1)
        {
            Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
        }
    }
    void _deviceDetach(void)
    {
        --_deviceCount;
        if(_deviceCount == 0)
        {
            Mix_CloseAudio();
        }
    }

private:
    static void _implOnFinish(int channel)
    {
        zfCoreMutexLock();
        zfself *d = (zfself *)ZFPROTOCOL_ACCESS(ZFAudio);
        zfstlmap<zfidentity, zfautoObjectT<ZFAudio *> >::iterator it = d->_implPlaying.find(channel);
        if(it == d->_implPlaying.end())
        {
            zfCoreMutexUnlock();
            return;
        }
        zfautoObjectT<ZFAudio *> audio = it->second;
        d->_implPlaying.erase(it);
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        nativeAudio->channel = -1;
        zfCoreMutexUnlock();

        d->notifyAudioOnStop(audio, zftrue, zfnull);
    }
    void _implAttach(ZF_IN ZFAudio *audio)
    {
        zfCoreMutexLocker();
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        zfCoreAssert(nativeAudio->channel != -1);
        _implPlaying[nativeAudio->channel] = audio;
    }
    void _implDetach(ZF_IN ZFAudio *audio)
    {
        zfCoreMutexLocker();
        NativeAudio *nativeAudio = (NativeAudio *)audio->nativeAudio();
        _implPlaying.erase(nativeAudio->channel);
    }

private:
    zfstlmap<zfidentity, zfautoObjectT<ZFAudio *> > _implPlaying;
ZFPROTOCOL_IMPLEMENTATION_END(ZFAudioImpl_sys_SDL)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAudioImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL
