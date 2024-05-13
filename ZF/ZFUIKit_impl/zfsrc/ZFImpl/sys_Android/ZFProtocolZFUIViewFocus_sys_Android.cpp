#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIViewFocus ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIViewFocus)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIViewFocus ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIViewFocus)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIViewFocus, ZFImpl_sys_Android_JNI_NAME_ZFUIViewFocus)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_Android, ZFUIViewFocus, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void viewFocusable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewFocusable
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_viewFocusable",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId,
            (jobject)view->nativeView(),
            viewFocusable);
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_viewFocused",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId, (jobject)view->nativeView());
    }
    virtual ZFUIView *viewFocusFind(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_viewFocusFind",
            JNIGetMethodSig(JNIType::S_long(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIPointer zfjniPointerFocusedChild = JNIUtilCallStaticLongMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId, (jobject)view->nativeView());
        return JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerFocusedChild);
    }
    virtual void viewFocusRequest(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewFocus
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_viewFocusRequest",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId,
            (jobject)view->nativeView(),
            viewFocus);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIViewFocus
        , void, native_1notifyViewFocusChanged
        , JNIPointer zfjniPointerOwnerZFUIView
        ) {
    ZFUIView *view = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView);
    if(!ZFBitTest(view->objectInstanceState(), ZFObjectInstanceStateOnDealloc)) {
        ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusChanged(view);
    }
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

