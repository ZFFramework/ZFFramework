#include "ZFProtocolZFAudio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFAudio)

void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnLoad(
        ZF_IN ZFAudio *audio
        , ZF_IN zfbool success
        , ZF_IN v_zfstring *errorHint
        ) {
    ZFLISTENER_3(action
            , zfautoT<ZFAudio>, audio
            , zfbool, success
            , zfautoT<v_zfstring>, errorHint
            ) {
        audio->_ZFP_ZFAudio_OnLoad(success ? v_ZFResultType::e_Success : v_ZFResultType::e_Fail, errorHint);
    } ZFLISTENER_END()
    ZFThread::mainThread()->taskQueueAdd(action);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnStop(
        ZF_IN ZFAudio *audio
        , ZF_IN zfbool success
        , ZF_IN v_zfstring *errorHint
        ) {
    ZFLISTENER_3(action
            , zfautoT<ZFAudio>, audio
            , zfbool, success
            , zfautoT<v_zfstring>, errorHint
            ) {
        audio->_ZFP_ZFAudio_OnStop(success ? v_ZFResultType::e_Success : v_ZFResultType::e_Fail, errorHint);
    } ZFLISTENER_END()
    ZFThread::mainThread()->taskQueueAdd(action);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnResume(ZF_IN ZFAudio *audio) {
    ZFLISTENER_1(action
            , zfautoT<ZFAudio>, audio
            ) {
        audio->_ZFP_ZFAudio_OnResume();
    } ZFLISTENER_END()
    ZFThread::mainThread()->taskQueueAdd(action);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnPause(ZF_IN ZFAudio *audio) {
    ZFLISTENER_1(action
            , zfautoT<ZFAudio>, audio
            ) {
        audio->_ZFP_ZFAudio_OnPause();
    } ZFLISTENER_END()
    ZFThread::mainThread()->taskQueueAdd(action);
}

ZF_NAMESPACE_GLOBAL_END

