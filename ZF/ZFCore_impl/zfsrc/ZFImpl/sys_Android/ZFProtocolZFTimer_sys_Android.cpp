#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFTimer ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFTimer)
#define ZFImpl_sys_Android_JNI_NAME_ZFTimer ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFTimer)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFTimer, ZFImpl_sys_Android_JNI_NAME_ZFTimer)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_Android, ZFTimer, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Object:Timer")

public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFTimer(), "native_nativeTimerCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFTimer(), jmId);
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeTimer = (jobject)timer->nativeTimer();
        JNIUtilDeleteGlobalRef(jniEnv, nativeTimer);
    }

    virtual void start(
            ZF_IN ZFTimer *timer
            , ZF_IN zfidentity timerImplId
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFTimer(), "native_start",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIPointerJNIType)
                .add(JNIType::S_long())
                .add(JNIType::S_long())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFTimer(), jmId
            , (jobject)timer->nativeTimer()
            , JNIConvertZFObjectToJNIType(jniEnv, timer)
            , (jlong)timer->interval()
            , (jlong)timerImplId
            );
    }
    virtual void stop(ZF_IN ZFTimer *timer) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFTimer(), "native_stop",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFTimer(), jmId, (jobject)timer->nativeTimer());
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFTimer
        , void, native_1notifyTimerActivate
        , JNIPointer zfjniPointerToken
        , jlong timerImplId
        ) {
    ZFTimer *timer = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerToken);
    ZFPROTOCOL_ACCESS(ZFTimer)->notifyTimerActivate(timer, (zfidentity)timerImplId);
}
JNI_METHOD_DECLARE_END()
#endif // #if ZF_ENV_sys_Android

