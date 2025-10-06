#include "ZFProtocolZFAudio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFAudio)

void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnLoad(
        ZF_IN ZFAudio *audio
        , ZF_IN zfbool success
        , ZF_IN v_zfstring *errorHint
        ) {
    zfidentity audioTaskId = audio->audioTaskId();
    ZFLISTENER_4(action
            , zfidentity, audioTaskId
            , zfautoT<ZFAudio>, audio
            , zfbool, success
            , zfautoT<v_zfstring>, errorHint
            ) {
        if(audioTaskId == audio->audioTaskId()) {
            audio->_ZFP_ZFAudio_OnLoad(success ? v_ZFResultType::e_Success : v_ZFResultType::e_Fail, errorHint);
        }
    } ZFLISTENER_END()
    zfpost(action);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnStop(
        ZF_IN ZFAudio *audio
        , ZF_IN zfbool success
        , ZF_IN v_zfstring *errorHint
        ) {
    zfidentity audioTaskId = audio->audioTaskId();
    ZFLISTENER_4(action
            , zfidentity, audioTaskId
            , zfautoT<ZFAudio>, audio
            , zfbool, success
            , zfautoT<v_zfstring>, errorHint
            ) {
        if(audioTaskId == audio->audioTaskId()) {
            audio->_ZFP_ZFAudio_OnStop(success ? v_ZFResultType::e_Success : v_ZFResultType::e_Fail, errorHint);
        }
    } ZFLISTENER_END()
    zfpost(action);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnResume(ZF_IN ZFAudio *audio) {
    zfidentity audioTaskId = audio->audioTaskId();
    ZFLISTENER_2(action
            , zfidentity, audioTaskId
            , zfautoT<ZFAudio>, audio
            ) {
        if(audioTaskId == audio->audioTaskId()) {
            audio->_ZFP_ZFAudio_OnResume();
        }
    } ZFLISTENER_END()
    zfpost(action);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFAudio)::notifyAudioOnPause(ZF_IN ZFAudio *audio) {
    zfidentity audioTaskId = audio->audioTaskId();
    ZFLISTENER_2(action
            , zfidentity, audioTaskId
            , zfautoT<ZFAudio>, audio
            ) {
        if(audioTaskId == audio->audioTaskId()) {
            audio->_ZFP_ZFAudio_OnPause();
        }
    } ZFLISTENER_END()
    zfpost(action);
}

ZF_NAMESPACE_GLOBAL_END

