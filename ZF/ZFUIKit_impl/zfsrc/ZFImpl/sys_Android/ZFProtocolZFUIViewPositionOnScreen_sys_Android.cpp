#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionOnScreen.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIViewPositionOnScreen ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIViewPositionOnScreen)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIViewPositionOnScreen ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIViewPositionOnScreen)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIViewPositionOnScreen, ZFImpl_sys_Android_JNI_NAME_ZFUIViewPositionOnScreen)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionOnScreenImpl_sys_Android, ZFUIViewPositionOnScreen, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void viewPositionOnScreen(
            ZF_IN ZFUIView *view
            , ZF_OUT ZFUIRect &rect
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewPositionOnScreen(), "native_viewPositionOnScreen",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jintArray jobjRect = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewPositionOnScreen(), jmId
            , (jobject)view->nativeView()
            );
        jint *jarrRect = JNIUtilGetIntArrayElements(jniEnv, jobjRect, NULL);
        rect = ZFUIRectCreate((zffloat)jarrRect[0], (zffloat)jarrRect[1], (zffloat)jarrRect[2], (zffloat)jarrRect[3]);
        JNIUtilReleaseIntArrayElements(jniEnv, jobjRect, jarrRect, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjRect);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionOnScreenImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android

