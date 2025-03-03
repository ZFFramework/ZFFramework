#include "ZFImpl_sys_Android_ZF_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFMainEntry ZFImpl_sys_Android_JNI_ID(ZF_1impl_ZFMainEntry)
#define ZFImpl_sys_Android_JNI_NAME_ZFMainEntry ZFImpl_sys_Android_JNI_NAME(ZF_impl.ZFMainEntry)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFMainEntry, ZFImpl_sys_Android_JNI_NAME_ZFMainEntry)

// ============================================================
// main entry
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry
        , void, native_1ZFFrameworkInit
        ) {
    ZFFrameworkInit();
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry
        , void, native_1ZFFrameworkCleanup
        ) {
    ZFFrameworkCleanup();
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry
        , jint, native_1ZFMainExecute
        , jobjectArray params
        ) {
    ZFCoreArray<zfstring> paramsTmp;
    if(params != NULL) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jsize count = JNIUtilGetArrayLength(jniEnv, params);
        for(jsize i = 0; i < count; ++i) {
            jobject e = JNIUtilGetObjectArrayElement(jniEnv, params, i);
            paramsTmp.add(ZFImpl_sys_Android_zfstringFromString(e));
            JNIUtilDeleteLocalRef(jniEnv, e);
        }
    }
    return ZFMainExecute(paramsTmp);
}
JNI_METHOD_DECLARE_END()

JNI_ONLOAD_ENTRY(vm, reserved) {
    if(!JNIInit(vm, JNI_VERSION_1_6)) {
        return -1;
    }

    return JNIGetDesiredVersion();
}

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
JNIGlobalRef ZFImpl_sys_Android_app(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), "native_app",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), jmId);
    JNIGlobalRef ret(tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
    return ret;
}
JNIGlobalRef ZFImpl_sys_Android_appContext(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), "native_appContext",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), jmId);
    JNIGlobalRef ret(tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
    return ret;
}
JNIGlobalRef ZFImpl_sys_Android_assetManager(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), "native_assetManager",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), jmId);
    JNIGlobalRef ret(tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
    return ret;
}
JNIGlobalRef ZFImpl_sys_Android_mainEntryActivity(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), "native_mainEntryActivity",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFMainEntry(), jmId);
    JNIGlobalRef ret(tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
    return ret;
}
ZF_NAMESPACE_GLOBAL_END

#include "ZFCore/ZFLogLevel.h"
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFMainEntry
        , void, native_1debugMode
        , jboolean value
        ) {
    if(value) {
        ZFLogLevelDefault(v_ZFLogLevel::e_Verbose);
    }
    else {
        ZFLogLevelDefault(v_ZFLogLevel::EnumDefault());
    }
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

