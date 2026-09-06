#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionMap.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIViewPositionMap ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIViewPositionMap)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIViewPositionMap ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIViewPositionMap)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIViewPositionMap, ZFImpl_sys_Android_JNI_NAME_ZFUIViewPositionMap)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionMapImpl_sys_Android, ZFUIViewPositionMap, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void viewPositionMap(
            ZF_OUT ZFUIPoint &ret
            , ZF_IN ZFUIView *view
            , ZF_IN_OPT const ZFUIPoint &localPos = ZFUIPointZero()
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIViewPositionMap(), "native_viewPositionMap",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        jintArray jobjRect = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIViewPositionMap(), jmId
            , (jobject)view->nativeView()
            , (jint)localPos.x
            , (jint)localPos.y
            );
        jint *jarrRet = JNIUtilGetIntArrayElements(jniEnv, jobjRect, NULL);
        ret.x = (zffloat)jarrRet[0];
        ret.y = (zffloat)jarrRet[1];
        JNIUtilReleaseIntArrayElements(jniEnv, jobjRect, jarrRet, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjRect);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionMapImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android

