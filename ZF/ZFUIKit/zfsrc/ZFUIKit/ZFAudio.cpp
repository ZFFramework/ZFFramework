#include "ZFAudio.h"
#include "protocol/ZFProtocolZFAudio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFAudioPrivate
{
public:
    typedef enum {
        LoadFlag = 1 << 1,
        StartFlag = 1 << 2,
        PauseFlag = 1 << 3,
        ImplLoaded = 1 << 4,
        ImplPlaying = 1 << 5,
    } StateFlag;

public:
    void *nativeAudio;
    zftimet positionToUpdate;
    zfuint state; // StateFlag
    zfuint looped;

public:
    _ZFP_ZFAudioPrivate(void)
    : nativeAudio(zfnull)
    , positionToUpdate(-1)
    , state(0)
    , looped(0)
    {
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAudio)

ZFOBSERVER_EVENT_REGISTER(ZFAudio, AudioOnLoad)
ZFOBSERVER_EVENT_REGISTER(ZFAudio, AudioOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFAudio, AudioOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFAudio, AudioOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFAudio, AudioOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFAudio, AudioOnLoop)

ZFMETHOD_DEFINE_0(ZFAudio, zfbool, implAvailable)
{
    return ZFPROTOCOL_IS_AVAILABLE(ZFAudio);
}

ZFMETHOD_DEFINE_0(ZFAudio, void *, nativeAudio)
{
    return d->nativeAudio;
}

ZFMETHOD_DEFINE_0(ZFAudio, zfbool, loading)
{
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, loaded)
{
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, started)
{
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, playing)
{
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, paused)
{
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
}

ZFMETHOD_DEFINE_0(ZFAudio, zfindex, looped)
{
    return (zfindex)d->looped;
}

ZFMETHOD_DEFINE_1(ZFAudio, void, load,
                  ZFMP_IN(const ZFInput &, input))
{
    this->stop();

    zfRetain(this); // release when OnLoad

    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplLoaded);
    ZFBitSet(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
    ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioLoad(this, input);
}

ZFMETHOD_DEFINE_1(ZFAudio, void, load,
                  ZFMP_IN(const zfchar *, url))
{
    this->stop();

    zfRetain(this); // release when OnLoad

    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplLoaded);
    ZFBitSet(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
    ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioLoad(this, url);
}

ZFMETHOD_DEFINE_0(ZFAudio, void, start)
{
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag))
    {
        return;
    }
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag))
    {
        zfRetain(this); // release when OnStop

        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        d->looped = 0;
        this->audioOnStart();
    }
    else if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded))
    {
        zfRetain(this); // release when OnStop

        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        d->looped = 0;
        this->audioOnStart();
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStart(this);
    }
    else
    {
        zfLogTrimT() << this << "start called before load";
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, void, stop)
{
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag) && !ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag))
    {
        return;
    }
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag))
    {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioLoadCancel(this);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
    }
    else if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag))
    {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStop(this);
    }
    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::StartFlag);
    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
    this->audioOnStop();
}

ZFMETHOD_DEFINE_0(ZFAudio, void, resume)
{
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag) && !ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag))
    {
        return;
    }
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag))
    {
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioResume(this);
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, void, pause)
{
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag))
    {
        return;
    }
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag))
    {
        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPause(this);
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, zftimet, duration)
{
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded))
    {
        return ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioDuration(this);
    }
    else
    {
        return 0;
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, zftimet, position)
{
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded))
    {
        return ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPosition(this);
    }
    else
    {
        return 0;
    }
}
ZFMETHOD_DEFINE_1(ZFAudio, void, position,
                  ZFMP_IN(zftimet, position))
{
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag))
    {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPosition(this, position);
    }
    else if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag))
    {
        d->positionToUpdate = position;
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFAudio, zffloat, volume)
{
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded))
    {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioVolume(this, propertyValue);
    }
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAudio,
                          ZFMP_IN(const ZFInput &, input))
{
    this->objectOnInit();
    this->load(input);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAudio,
                          ZFMP_IN(const zfchar *, url))
{
    this->objectOnInit();
    this->load(url);
}

ZFMETHOD_DEFINE_0(ZFAudio, const zfchar *, stateHint)
{
    if(this->loading())
    {
        return "loading";
    }
    else if(this->playing())
    {
        return "playing";
    }
    else if(this->paused())
    {
        return "paused";
    }
    else if(this->started())
    {
        return "waiting";
    }
    else if(this->loaded())
    {
        return "loaded";
    }
    else
    {
        return "idle";
    }
}

// ============================================================
void ZFAudio::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAudioPrivate);
    d->nativeAudio = ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioCreate(this);
}
void ZFAudio::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioDestroy(this);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFAudio::_ZFP_ZFAudio_OnLoad(ZF_IN zfbool success)
{
    zfCoreAssert(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag));
    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
    if(success)
    {
        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::ImplLoaded);
    }

    this->audioOnLoad();

    if(success)
    {
        if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag))
        {
            ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStart(this);
            ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioVolume(this, this->volume());
        }
        if(d->positionToUpdate != -1)
        {
            ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPosition(this, d->positionToUpdate);
            d->positionToUpdate = -1;
        }
    }

    zfRelease(this); // retained when load
}
void ZFAudio::_ZFP_ZFAudio_OnStop(ZF_IN zfbool success)
{
    zfCoreAssert(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag));
    ++d->looped;
    if(this->loop() == zfindexMax() || d->looped <= this->loop())
    {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStart(this);
        this->audioOnLoop();
    }
    else
    {
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
        this->audioOnStop();

        zfRelease(this); // retained when start
    }
}
void ZFAudio::_ZFP_ZFAudio_OnResume(void)
{
    zfCoreAssert(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag));
    ZFBitSet(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
    this->audioOnResume();

    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag))
    {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPause(this);
    }
}
void ZFAudio::_ZFP_ZFAudio_OnPause(void)
{
    zfCoreAssert(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag));
    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
    this->audioOnPause();
}

ZF_NAMESPACE_GLOBAL_END

