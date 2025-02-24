#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIViewTransform ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIViewTransform)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIViewTransform ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIViewTransform)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIViewTransform, ZFImpl_sys_Android_JNI_NAME_ZFUIViewTransform)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_Android, ZFUIViewTransform, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual ZFUITransformFlags transformAvailable(void) {
        return ZFUITransform::e_Transform2D;
    }

    virtual void viewTransform(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewTransform(), "native_viewTransform",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewTransform(), jmId
            , (jobject)view->nativeView()
            , (jfloat)(view->translateX() * view->UIScaleFixed())
            , (jfloat)(view->translateY() * view->UIScaleFixed())
            , (jfloat)view->scaleX()
            , (jfloat)view->scaleY()
            , (jfloat)view->rotateZ()
            );
    }
    virtual void viewTransformReset(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewTransform(), "native_viewTransform",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
                .add(JNIType::S_float())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewTransform(), jmId
            , (jobject)view->nativeView()
            , (jfloat)0
            , (jfloat)0
            , (jfloat)1
            , (jfloat)1
            , (jfloat)0
            );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewTransformImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

