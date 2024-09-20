#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAniForNative.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFAniForNative ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFAniForNative)
#define ZFImpl_sys_Android_JNI_NAME_ZFAniForNative ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFAniForNative)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAniForNative, ZFImpl_sys_Android_JNI_NAME_ZFAniForNative)

#define ZFImpl_sys_Android_JNI_ID_ZFAniForNative_NativeAnimation ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFAniForNative_00024NativeAnimation)
#define ZFImpl_sys_Android_JNI_NAME_ZFAniForNative_NativeAnimation ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFAniForNative$NativeAnimation)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation, ZFImpl_sys_Android_JNI_NAME_ZFAniForNative_NativeAnimation)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAniForNativeImpl_sys_Android, ZFAniForNative, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Animation")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeAniCreate(ZF_IN ZFAniForNative *ani) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), "native_nativeAniCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), jmId
            , JNIConvertZFObjectToJNIType(jniEnv, ani)
            );
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return (void *)ret;
    }
    virtual void nativeAniDestroy(
            ZF_IN ZFAniForNative *ani
            , ZF_IN void *nativeAni
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), "native_nativeAniDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject tmp = (jobject)nativeAni;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), jmId, tmp);
        JNIUtilDeleteGlobalRef(jniEnv, tmp);
    }

    virtual void nativeAniStart(
            ZF_IN ZFAniForNative *ani
            , ZF_IN zffloat nativeAniScale
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), "native_nativeAniStart",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        this->nativeAniSetup(ani, nativeAniScale);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), jmId
            , (jobject)ani->nativeAnimation()
            , (jobject)ani->aniTarget().to<ZFUIView *>()->nativeView()
            );
    }
    virtual void nativeAniStop(ZF_IN ZFAniForNative *ani) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), "native_nativeAniStop",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), jmId
            , (jobject)ani->nativeAnimation()
            , (jobject)ani->aniTarget().to<ZFUIView *>()->nativeView()
            );
    }

private:
    void nativeAniSetup(
            ZF_IN ZFAniForNative *ani
            , ZF_IN zffloat nativeAniScale
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), "native_setup",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        jobject nativeAnimation = (jobject)ani->nativeAnimation();
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), jmId
            , nativeAnimation
            , (jint)ani->aniCurve()
            , (jint)ani->aniDurationFixed()
            );

        // ============================================================
        // alpha
        if(ani->aniAlphaFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniAlphaFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniAlphaFrom());
        }
        if(ani->aniAlphaTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniAlphaTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniAlphaTo());
        }
        // ============================================================
        // scale
        if(ani->aniScaleXFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniScaleXFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleXFrom());
        }
        if(ani->aniScaleXTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniScaleXTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleXTo());
        }
        if(ani->aniScaleYFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniScaleYFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleYFrom());
        }
        if(ani->aniScaleYTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniScaleYTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleYTo());
        }
        if(ani->aniScaleZFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniScaleZFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleZFrom());
        }
        if(ani->aniScaleZTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniScaleZTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleZTo());
        }
        // ============================================================
        // translate
        if(ani->aniTranslateXFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniTranslateXFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslateXFrom() * nativeAniScale));
        }
        if(ani->aniTranslateXTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniTranslateXTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslateXTo() * nativeAniScale));
        }
        if(ani->aniTranslateYFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniTranslateYFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslateYFrom() * nativeAniScale));
        }
        if(ani->aniTranslateYTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniTranslateYTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslateYTo() * nativeAniScale));
        }
        if(ani->aniTranslateZFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniTranslateZFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslateZFrom() * nativeAniScale));
        }
        if(ani->aniTranslateZTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniTranslateZTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslateZTo() * nativeAniScale));
        }
        // ============================================================
        // rotate
        if(ani->aniRotateXFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniRotateXFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateXFrom());
        }
        if(ani->aniRotateXTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniRotateXTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateXTo());
        }
        if(ani->aniRotateYFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniRotateYFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateYFrom());
        }
        if(ani->aniRotateYTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniRotateYTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateYTo());
        }
        if(ani->aniRotateZFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniRotateZFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateZFrom());
        }
        if(ani->aniRotateZTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "aniRotateZTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateZTo());
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAniForNativeImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAniForNative
        , void, native_1notifyAniStop
        , JNIPointer zfjniPointerOwnerZFAniForNative
        ) {
    ZFPROTOCOL_ACCESS(ZFAniForNative)->notifyAniStop(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFAniForNative));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

