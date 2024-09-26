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
    virtual void focusable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focusable
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_focusable",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId
            , (jobject)view->nativeView()
            , (jboolean)focusable
            );
    }
    virtual zfbool focused(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_focused",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId, (jobject)view->nativeView());
    }
    virtual ZFUIView *focusFind(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_focusFind",
            JNIGetMethodSig(JNIType::S_long(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIPointer zfjniPointerFocusedChild = JNIUtilCallStaticLongMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId, (jobject)view->nativeView());
        return JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerFocusedChild);
    }
    virtual void focusRequest(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focus
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), "native_focusRequest",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewFocus(), jmId
            , (jobject)view->nativeView()
            , (jboolean)focus
            );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIViewFocus
        , void, native_1notifyViewFocusUpdate
        , JNIPointer zfjniPointerOwnerZFUIView
        ) {
    ZFUIView *view = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView);
    if(!view->objectDeallocRunning()) {
        ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusUpdate(view);
    }
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

