/**
 * @file ZFAudio.h
 * @brief audio player
 */

#ifndef _ZFI_ZFAudio_h_
#define _ZFI_ZFAudio_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFAudioPrivate;
/**
 * @brief audio player
 */
zfclass ZFLIB_ZFUIKit ZFAudio : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFAudio, ZFObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when audio load finished, ensured called in ZFThread::mainThread\n
     * param0 is a #ZFResultType shows the load result,
     * param1 is a #v_zfstring shows the error hint if load fail
     */
    ZFOBSERVER_EVENT(AudioOnLoad)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when audio started, ensured called in ZFThread::mainThread
     */
    ZFOBSERVER_EVENT(AudioOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when audio stopped, ensured called in ZFThread::mainThread\n
     * param0 is a #ZFResultType shows the play result,
     * param1 is a #v_zfstring shows the error hint if play fail
     */
    ZFOBSERVER_EVENT(AudioOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when audio start success and about to play,
     * or resume from pause, ensured called in ZFThread::mainThread
     */
    ZFOBSERVER_EVENT(AudioOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when audio pause, ensured called in ZFThread::mainThread
     */
    ZFOBSERVER_EVENT(AudioOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when audio looped, ensured called in ZFThread::mainThread
     */
    ZFOBSERVER_EVENT(AudioOnLoop)

    // ============================================================
public:
    /** @brief whether impl is available */
    ZFMETHOD_DECLARE_STATIC_0(zfbool, implAvailable)

    /** @brief the native audio */
    ZFMETHOD_DECLARE_0(void *, nativeAudio)

public:
    /** @brief whether loading */
    ZFMETHOD_DECLARE_0(zfbool, loading)

    /** @brief whether loaded */
    ZFMETHOD_DECLARE_0(zfbool, loaded)

    /** @brief whether started */
    ZFMETHOD_DECLARE_0(zfbool, started)

    /** @brief whether start success and playing, false when loading or still preparing to play or paused */
    ZFMETHOD_DECLARE_0(zfbool, playing)

    /** @brief whether explicitly paused by user */
    ZFMETHOD_DECLARE_0(zfbool, paused)

public:
    /** @brief load from input */
    ZFMETHOD_DECLARE_1(void, load,
                       ZFMP_IN(const ZFInput &, input))

    /** @brief load from url */
    ZFMETHOD_DECLARE_1(void, load,
                       ZFMP_IN(const zfchar *, url))

    /**
     * @brief start to play
     *
     * it's ensured safe to call start immediately after #load,
     * but #EventAudioOnLoad may called after #EventAudioOnStart for this case\n
     * once loaded, it's ensured able to start and stop more than one time
     */
    ZFMETHOD_DECLARE_0(void, start)

    /** @brief stop play */
    ZFMETHOD_DECLARE_0(void, stop)

    /** @brief resume play */
    ZFMETHOD_DECLARE_0(void, resume)

    /** @brief pause play */
    ZFMETHOD_DECLARE_0(void, pause)

    /**
     * @brief current looped count
     *
     * reset when #start, but keep previous value when #stop
     */
    ZFMETHOD_DECLARE_0(zfindex, looped)

    /** @brief duration, valid only when loaded, may be 0 if impl not supported */
    ZFMETHOD_DECLARE_0(zftimet, duration)

    /** @brief current position, valid only when started */
    ZFMETHOD_DECLARE_0(zftimet, position)
    /**
     * @brief change current position
     *
     * note, change position may or may not work,
     * depends on impl
     */
    ZFMETHOD_DECLARE_1(void, position,
                       ZFMP_IN(zftimet, position))

    /** @brief current volume */
    ZFPROPERTY_ASSIGN(zffloat, volume)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, volume)

    /**
     * @brief loop count of audio, 0 means play once,
     *   and zfindexMax means loop infinite until stop called
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, loop, 0)

    /** @brief return a state hint for debug */
    ZFMETHOD_DECLARE_0(const zfchar *, stateHint)

protected:
    /** @brief load from input */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(const ZFInput &, input))
    /** @brief load from url */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(const zfchar *, url))
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ret += " ";
        ret += this->stateHint();
        if(this->started())
        {
            ret += " ";
            zftimetToString(ret, this->position() / 1000);
            ret += "/";
            zftimetToString(ret, this->duration() / 1000);
        }
    }

protected:
    /** @brief see #EventAudioOnLoad */
    virtual void audioOnLoad(ZF_IN ZFResultTypeEnum result, ZF_IN v_zfstring *errorHint)
    {
        zfblockedAlloc(ZFResultType, resultHolder);
        resultHolder->enumValue(result);
        this->observerNotify(ZFAudio::EventAudioOnLoad(), resultHolder, errorHint);
    }
    /** @brief see #EventAudioOnStart */
    virtual void audioOnStart(void)
    {
        this->observerNotify(ZFAudio::EventAudioOnStart());
    }
    /** @brief see #EventAudioOnStop */
    virtual void audioOnStop(ZF_IN ZFResultTypeEnum result, ZF_IN v_zfstring *errorHint)
    {
        zfblockedAlloc(ZFResultType, resultHolder);
        resultHolder->enumValue(result);
        this->observerNotify(ZFAudio::EventAudioOnStop(), resultHolder, errorHint);
    }
    /** @brief see #EventAudioOnResume */
    virtual void audioOnResume(void)
    {
        this->observerNotify(ZFAudio::EventAudioOnResume());
    }
    /** @brief see #EventAudioOnPause */
    virtual void audioOnPause(void)
    {
        this->observerNotify(ZFAudio::EventAudioOnPause());
    }
    /** @brief see #EventAudioOnLoop */
    virtual void audioOnLoop(void)
    {
        this->observerNotify(ZFAudio::EventAudioOnLoop());
    }

public:
    zffinal void _ZFP_ZFAudio_OnLoad(ZF_IN ZFResultTypeEnum result, ZF_IN v_zfstring *errorHint);
    zffinal void _ZFP_ZFAudio_OnStop(ZF_IN ZFResultTypeEnum result, ZF_IN v_zfstring *errorHint);
    zffinal void _ZFP_ZFAudio_OnResume(void);
    zffinal void _ZFP_ZFAudio_OnPause(void);
private:
    _ZFP_ZFAudioPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAudio_h_

