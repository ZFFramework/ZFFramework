#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIOnScreenKeyboardState.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIOnScreenKeyboardState ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIOnScreenKeyboardState)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIOnScreenKeyboardState ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIOnScreenKeyboardState)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState, ZFImpl_sys_Android_JNI_NAME_ZFUIOnScreenKeyboardState)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIOnScreenKeyboardStateImpl_sys_Android, ZFUIOnScreenKeyboardState, v_ZFProtocolLevel::e_SystemNormal)
public:
    virtual zfbool keyboardShowing(ZF_IN ZFUIOnScreenKeyboardState *keyboardState) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), "native_keyboardShowing",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), jmId);
    }
    virtual ZFUIRect keyboardFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), "native_keyboardFrame",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
            ).c_str());
        jintArray jobjRect = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), jmId);
        jint *jarrRect = JNIUtilGetIntArrayElements(jniEnv, jobjRect, NULL);
        ZFUIRect ret = ZFUIRectCreate((zffloat)jarrRect[0], (zffloat)jarrRect[1], (zffloat)jarrRect[2], (zffloat)jarrRect[3]);
        JNIUtilReleaseIntArrayElements(jniEnv, jobjRect, jarrRect, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjRect);
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIOnScreenKeyboardStateImpl_sys_Android)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardStateImpl_sys_Android_init, ZFLevelZFFrameworkPostNormal) {
    JNIEnv *jniEnv = JNIGetJNIEnv();

    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), "native_keyboardStaticInit",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), jmId);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardStateImpl_sys_Android_init) {
    JNIEnv *jniEnv = JNIGetJNIEnv();

    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), "native_keyboardStaticCleanup",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIOnScreenKeyboardState(), jmId);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardStateImpl_sys_Android_init)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIOnScreenKeyboardState
        , void, native_1notifyKeyboardStateOnUpdate
        ) {
    if(ZFFrameworkStateCheck() != ZFFrameworkStateAvailable) {
        return;
    }
    ZFPROTOCOL_ACCESS(ZFUIOnScreenKeyboardState)->notifyKeyboardStateOnUpdate(
        ZFUIOnScreenKeyboardState::instanceForRootWindow());
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

