#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindowEmbedNativeView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUISysWindowEmbedNativeView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUISysWindowEmbedNativeView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUISysWindowEmbedNativeView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUISysWindowEmbedNativeView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUISysWindowEmbedNativeView, ZFImpl_sys_Android_JNI_NAME_ZFUISysWindowEmbedNativeView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowEmbedNativeViewImpl_sys_Android, ZFUISysWindowEmbedNativeView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void nativeViewAdd(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN_OUT void *&parent
            , ZF_IN void *child
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindowEmbedNativeView(), "native_nativeViewAdd",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        parent = (void *)JNIUtilNewGlobalRef(jniEnv, (jobject)parent);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindowEmbedNativeView(), jmId
            , (jobject)parent
            , (jobject)child
            );
    }
    virtual void nativeViewRemove(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN_OUT void *&parent
            , ZF_IN void *child
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindowEmbedNativeView(), "native_nativeViewRemove",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindowEmbedNativeView(), jmId
            , (jobject)parent
            , (jobject)child
            );
        JNIUtilDeleteGlobalRef(jniEnv, (jobject)parent);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowEmbedNativeViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

