#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFEnvInfo_ZFUIKit ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFEnvInfo_1ZFUIKit)
#define ZFImpl_sys_Android_JNI_NAME_ZFEnvInfo_ZFUIKit ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFEnvInfo_ZFUIKit)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFEnvInfo_ZFUIKit, ZFImpl_sys_Android_JNI_NAME_ZFEnvInfo_ZFUIKit)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_localeInfoImpl_sys_Android, ZFEnvInfo_localeInfo, v_ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual zfstring localeInfo(void) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFEnvInfo_ZFUIKit(), "native_localeInfo",
            JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
            ).c_str());
        jobject info = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFEnvInfo_ZFUIKit(), jmId);
        zfstring ret = ZFImpl_sys_Android_zfstringFromString(info);
        JNIUtilDeleteLocalRef(jniEnv, info);
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_localeInfoImpl_sys_Android)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_localeLangInfoImpl_sys_Android, ZFEnvInfo_localeLangInfo, v_ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual zfstring localeLangInfo(void) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFEnvInfo_ZFUIKit(), "native_localeLangInfo",
            JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
            ).c_str());
        jobject info = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFEnvInfo_ZFUIKit(), jmId);
        zfstring ret = ZFImpl_sys_Android_zfstringFromString(info);
        JNIUtilDeleteLocalRef(jniEnv, info);
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_localeLangInfoImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

