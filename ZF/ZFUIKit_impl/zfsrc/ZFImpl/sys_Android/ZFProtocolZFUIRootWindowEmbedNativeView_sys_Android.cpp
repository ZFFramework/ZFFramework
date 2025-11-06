#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIRootWindowEmbedNativeView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIRootWindowEmbedNativeView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIRootWindowEmbedNativeView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIRootWindowEmbedNativeView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIRootWindowEmbedNativeView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIRootWindowEmbedNativeView, ZFImpl_sys_Android_JNI_NAME_ZFUIRootWindowEmbedNativeView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIRootWindowEmbedNativeViewImpl_sys_Android, ZFUIRootWindowEmbedNativeView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void nativeViewAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN_OUT void *&parent
            , ZF_IN void *child
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindowEmbedNativeView(), "native_nativeViewAdd",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        parent = (void *)JNIUtilNewGlobalRef(jniEnv, (jobject)parent);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindowEmbedNativeView(), jmId
            , (jobject)parent
            , (jobject)child
            );
    }
    virtual void nativeViewRemove(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN_OUT void *&parent
            , ZF_IN void *child
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindowEmbedNativeView(), "native_nativeViewRemove",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindowEmbedNativeView(), jmId
            , (jobject)parent
            , (jobject)child
            );
        JNIUtilDeleteGlobalRef(jniEnv, (jobject)parent);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIRootWindowEmbedNativeViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

