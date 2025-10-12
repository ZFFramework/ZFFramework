#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAudio.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFAudio ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFAudio)
#define ZFImpl_sys_Android_JNI_NAME_ZFAudio ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFAudio)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAudio, ZFImpl_sys_Android_JNI_NAME_ZFAudio)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAudioImpl_sys_Android, ZFAudio, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:MediaPlayer")

public:
    virtual void *nativeAudioCreate(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , JNIConvertZFObjectToJNIType(jniEnv, audio)
            );
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeAudioDestroy(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject nativeAudioTmp = (jobject)audio->nativeAudio();
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId, nativeAudioTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeAudioTmp);
    }

    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const ZFInput &input
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioLoad",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            , ZFImpl_sys_Android_ZFInputWrapperFromZFInput(input)
            );
    }
    virtual void nativeAudioLoadCancel(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioLoadCancel",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            );
    }

    virtual void nativeAudioStart(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioStart",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            );
    }
    virtual void nativeAudioStop(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioStop",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            );
    }
    virtual void nativeAudioResume(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioResume",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            );
    }
    virtual void nativeAudioPause(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioPause",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            );
    }

    virtual zftimet nativeAudioDuration(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioDuration",
            JNIGetMethodSig(JNIType::S_long(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return (zftimet)JNIUtilCallStaticLongMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            );
    }
    virtual zftimet nativeAudioPosition(ZF_IN ZFAudio *audio) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioPosition",
            JNIGetMethodSig(JNIType::S_long(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return (zftimet)JNIUtilCallStaticLongMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            );
    }
    virtual void nativeAudioPosition(
            ZF_IN ZFAudio *audio
            , ZF_IN zftimet position
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioPosition",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_long())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            , (jlong)position
            );
    }

    virtual void nativeAudioVolume(
            ZF_IN ZFAudio *audio
            , ZF_IN zffloat volume
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), "native_nativeAudioVolume",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_float())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAudio(), jmId
            , (jobject)audio->nativeAudio()
            , (jfloat)volume
            );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAudioImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAudio
        , void, native_1notifyAudioOnLoad
        , JNIPointer zfjniPointerOwnerZFAudio
        , jboolean success
        , jstring errorHint
        ) {
    ZFAudio *audio = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFAudio);
    if(success || errorHint == NULL) {
        ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnLoad(audio, (zfbool)success, zfnull);
    }
    else {
        zfobj<v_zfstring> errorHintHolder;
        ZFImpl_sys_Android_zfstringFromStringT(errorHintHolder->zfv, errorHint);
        ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnLoad(audio, (zfbool)success, errorHintHolder);
    }
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAudio
        , void, native_1notifyAudioOnStop
        , JNIPointer zfjniPointerOwnerZFAudio
        , jboolean success
        , jstring errorHint
        ) {
    ZFAudio *audio = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFAudio);
    if(success || errorHint == NULL) {
        ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnStop(audio, (zfbool)success, zfnull);
    }
    else {
        zfobj<v_zfstring> errorHintHolder;
        ZFImpl_sys_Android_zfstringFromStringT(errorHintHolder->zfv, errorHint);
        ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnStop(audio, (zfbool)success, errorHintHolder);
    }
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAudio
        , void, native_1notifyAudioOnResume
        , JNIPointer zfjniPointerOwnerZFAudio
        ) {
    ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnResume(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFAudio)
        );
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAudio
        , void, native_1notifyAudioOnPause
        , JNIPointer zfjniPointerOwnerZFAudio
        ) {
    ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnPause(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFAudio)
        );
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

