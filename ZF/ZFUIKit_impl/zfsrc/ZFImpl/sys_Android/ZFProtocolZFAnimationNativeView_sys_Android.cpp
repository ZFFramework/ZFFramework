#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFAnimationNativeView)
#define ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFAnimationNativeView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAnimationNativeView, ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView)

#define ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeView_NativeAnimation ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFAnimationNativeView_00024NativeAnimation)
#define ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView_NativeAnimation ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFAnimationNativeView$NativeAnimation)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation, ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView_NativeAnimation)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAnimationNativeViewImpl_sys_Android, ZFAnimationNativeView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Animation")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeAniCreate(ZF_IN ZFAnimationNativeView *ani) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), "native_nativeAniCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), jmId,
            JNIConvertZFObjectToJNIType(jniEnv, ani));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return (void *)ret;
    }
    virtual void nativeAniDestroy(
            ZF_IN ZFAnimationNativeView *ani
            , ZF_IN void *nativeAni
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), "native_nativeAniDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject tmp = (jobject)nativeAni;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), jmId, tmp);
        JNIUtilDeleteGlobalRef(jniEnv, tmp);
    }

    virtual void nativeAniStart(
            ZF_IN ZFAnimationNativeView *ani
            , ZF_IN zffloat nativeAniScale
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), "native_nativeAniStart",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        this->nativeAniSetup(ani, nativeAniScale);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), jmId,
            (jobject)ani->nativeAnimation(),
            (jobject)ani->aniTarget().to<ZFUIView *>()->nativeView());
    }
    virtual void nativeAniStop(ZF_IN ZFAnimationNativeView *ani) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), "native_nativeAniStop",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), jmId,
            (jobject)ani->nativeAnimation(),
            (jobject)ani->aniTarget().to<ZFUIView *>()->nativeView());
    }

private:
    void nativeAniSetup(
            ZF_IN ZFAnimationNativeView *ani
            , ZF_IN zffloat nativeAniScale
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), "native_setup",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        jobject nativeAnimation = (jobject)ani->nativeAnimation();
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView(), jmId
            , nativeAnimation
            , (jint)ani->aniCurve()
            , (jint)ani->aniDurationFixed()
            );

        // ============================================================
        // alpha
        if(ani->aniAlphaFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniAlphaFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniAlphaFrom());
        }
        if(ani->aniAlphaTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniAlphaTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniAlphaTo());
        }
        // ============================================================
        // scale
        if(ani->aniScaleXFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniScaleXFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleXFrom());
        }
        if(ani->aniScaleXTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniScaleXTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleXTo());
        }
        if(ani->aniScaleYFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniScaleYFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleYFrom());
        }
        if(ani->aniScaleYTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniScaleYTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleYTo());
        }
        if(ani->aniScaleZFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniScaleZFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleZFrom());
        }
        if(ani->aniScaleZTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniScaleZTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleZTo());
        }
        // ============================================================
        // translate by view size's percent
        if(ani->aniTranslateXFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslateXFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateXFrom());
        }
        if(ani->aniTranslateXTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslateXTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateXTo());
        }
        if(ani->aniTranslateYFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslateYFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateYFrom());
        }
        if(ani->aniTranslateYTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslateYTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateYTo());
        }
        if(ani->aniTranslateZFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslateZFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateZFrom());
        }
        if(ani->aniTranslateZTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslateZTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateZTo());
        }
        // ============================================================
        // translate by pixel
        if(ani->aniTranslatePixelXFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslatePixelXFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelXFrom() * nativeAniScale));
        }
        if(ani->aniTranslatePixelXTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslatePixelXTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelXTo() * nativeAniScale));
        }
        if(ani->aniTranslatePixelYFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslatePixelYFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelYFrom() * nativeAniScale));
        }
        if(ani->aniTranslatePixelYTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslatePixelYTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelYTo() * nativeAniScale));
        }
        if(ani->aniTranslatePixelZFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslatePixelZFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelZFrom() * nativeAniScale));
        }
        if(ani->aniTranslatePixelZTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniTranslatePixelZTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelZTo() * nativeAniScale));
        }
        // ============================================================
        // rotate
        if(ani->aniRotateXFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniRotateXFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateXFrom());
        }
        if(ani->aniRotateXTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniRotateXTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateXTo());
        }
        if(ani->aniRotateYFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniRotateYFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateYFrom());
        }
        if(ani->aniRotateYTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniRotateYTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateYTo());
        }
        if(ani->aniRotateZFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniRotateZFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateZFrom());
        }
        if(ani->aniRotateZTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAnimationNativeView_NativeAnimation(), "aniRotateZTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateZTo());
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAnimationNativeViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAnimationNativeViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeView
        , void, native_1notifyAniStop
        , JNIPointer zfjniPointerOwnerZFAnimationNativeView
        ) {
    ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->notifyAniStop(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFAnimationNativeView));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

