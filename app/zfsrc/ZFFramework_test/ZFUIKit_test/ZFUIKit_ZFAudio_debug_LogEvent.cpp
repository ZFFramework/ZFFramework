#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUIKit_ZFAudio_debug_LogEvent) {
    this->onEventListener = ZFCallbackForFunc(zfself::onEvent);
    ZFGlobalObserver().observerAdd(ZFAudio::E_AudioOnLoad(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::E_AudioOnStart(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::E_AudioOnStop(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::E_AudioOnResume(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::E_AudioOnPause(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::E_AudioOnLoop(), this->onEventListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKit_ZFAudio_debug_LogEvent) {
    ZFGlobalObserver().observerRemove(ZFAudio::E_AudioOnLoad(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::E_AudioOnStart(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::E_AudioOnStop(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::E_AudioOnResume(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::E_AudioOnPause(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::E_AudioOnLoop(), this->onEventListener);
}
private:
    ZFListener onEventListener;
    static void onEvent(ZF_IN const ZFArgs &zfargs) {
        ZFLogTrim("%s %s %s(%s, %s)"
                , ZFLogCurTimeString()
                , zfargs.sender()
                , ZFEventNameForId(zfargs.eventId())
                , zfargs.param0()
                , zfargs.param1()
                );

        if(zfargs.eventId() == ZFAudio::E_AudioOnResume()) {
            audioOnResume(zfargs.sender());
        }
        else if(zfargs.eventId() == ZFAudio::E_AudioOnPause()) {
            audioOnPause(zfargs.sender());
        }
    }
private:
    static zfautoT<ZFTimer> &_timer(void) {
        static zfautoT<ZFTimer> d;
        return d;
    }
    static ZFCoreArray<ZFAudio *> &_playing(void) {
        static ZFCoreArray<ZFAudio *> d;
        return d;
    }
    static void audioOnResume(ZF_IN ZFAudio *audio) {
        zfautoT<ZFTimer> &timer = _timer();
        ZFCoreArray<ZFAudio *> &playing = _playing();

        playing.add(audio);
        if(playing.count() == 1) {
            ZFLISTENER_1(onTimer
                    , ZFCoreArray<ZFAudio *> const &, playing
                    ) {
                for(zfindex i = 0; i < playing.count(); ++i) {
                    ZFAudio *audio = playing[i];
                    ZFLogTrim() << audio;
                }
            } ZFLISTENER_END()
            timer = ZFTimerStart(1000, onTimer);
        }
    }
    static void audioOnPause(ZF_IN ZFAudio *audio) {
        zfautoT<ZFTimer> &timer = _timer();
        ZFCoreArray<ZFAudio *> &playing = _playing();

        playing.removeElement(audio);
        if(playing.isEmpty()) {
            timer->stop();
            timer = zfnull;
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKit_ZFAudio_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

