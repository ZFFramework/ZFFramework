#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUIKit_ZFAudio_debug_LogEvent)
{
    this->onEventListener = ZFCallbackForFunc(zfself::onEvent);
    ZFGlobalObserver().observerAdd(ZFAudio::EventAudioOnLoad(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::EventAudioOnStart(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::EventAudioOnStop(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::EventAudioOnResume(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::EventAudioOnPause(), this->onEventListener);
    ZFGlobalObserver().observerAdd(ZFAudio::EventAudioOnLoop(), this->onEventListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKit_ZFAudio_debug_LogEvent)
{
    ZFGlobalObserver().observerRemove(ZFAudio::EventAudioOnLoad(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::EventAudioOnStart(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::EventAudioOnStop(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::EventAudioOnResume(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::EventAudioOnPause(), this->onEventListener);
    ZFGlobalObserver().observerRemove(ZFAudio::EventAudioOnLoop(), this->onEventListener);
}
private:
    ZFListener onEventListener;
    static void onEvent(ZF_IN const ZFArgs &zfargs)
    {
        zfLogTrimT() << ZFIdMapNameForId(zfargs.eventId()) << zfargs.sender()->objectInfoOfInstance();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKit_ZFAudio_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

