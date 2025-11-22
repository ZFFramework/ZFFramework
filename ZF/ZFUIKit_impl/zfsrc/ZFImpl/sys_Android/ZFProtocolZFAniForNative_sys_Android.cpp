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

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAniForNativeImpl_sys_Android, ZFAniForNative, v_ZFProtocolLevel::e_SystemNormal)
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
    virtual void nativeAniDestroy(ZF_IN ZFAniForNative *ani) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), "native_nativeAniDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject tmp = (jobject)ani->nativeAni();
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
            , (jobject)ani->nativeAni()
            , (jobject)ani->target().to<ZFUIView *>()->nativeView()
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
            , (jobject)ani->nativeAni()
            , (jobject)ani->target().to<ZFUIView *>()->nativeView()
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
        jobject nativeAni = (jobject)ani->nativeAni();
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative(), jmId
            , nativeAni
            , (jint)ani->nativeCurve()
            , (jint)ani->durationFixed()
            );

        // ============================================================
        // alpha
        if(ani->alphaFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "alphaFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->alphaFrom());
        }
        if(ani->alphaTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "alphaTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->alphaTo());
        }
        // ============================================================
        // scale
        if(ani->scaleXFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "scaleXFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->scaleXFrom());
        }
        if(ani->scaleXTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "scaleXTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->scaleXTo());
        }
        if(ani->scaleYFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "scaleYFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->scaleYFrom());
        }
        if(ani->scaleYTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "scaleYTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->scaleYTo());
        }
        if(ani->scaleZFrom() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "scaleZFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->scaleZFrom());
        }
        if(ani->scaleZTo() != 1) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "scaleZTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->scaleZTo());
        }
        // ============================================================
        // translate
        if(ani->translateXFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "translateXFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAni, jfId, (jint)(ani->translateXFrom() * nativeAniScale));
        }
        if(ani->translateXTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "translateXTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAni, jfId, (jint)(ani->translateXTo() * nativeAniScale));
        }
        if(ani->translateYFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "translateYFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAni, jfId, (jint)(ani->translateYFrom() * nativeAniScale));
        }
        if(ani->translateYTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "translateYTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAni, jfId, (jint)(ani->translateYTo() * nativeAniScale));
        }
        if(ani->translateZFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "translateZFrom", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAni, jfId, (jint)(ani->translateZFrom() * nativeAniScale));
        }
        if(ani->translateZTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "translateZTo", JNIType::S_int().getId());
            JNIUtilSetIntField(jniEnv, nativeAni, jfId, (jint)(ani->translateZTo() * nativeAniScale));
        }
        // ============================================================
        // rotate
        if(ani->rotateXFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "rotateXFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->rotateXFrom());
        }
        if(ani->rotateXTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "rotateXTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->rotateXTo());
        }
        if(ani->rotateYFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "rotateYFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->rotateYFrom());
        }
        if(ani->rotateYTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "rotateYTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->rotateYTo());
        }
        if(ani->rotateZFrom() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "rotateZFrom", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->rotateZFrom());
        }
        if(ani->rotateZTo() != 0) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAniForNative_NativeAnimation(), "rotateZTo", JNIType::S_float().getId());
            JNIUtilSetFloatField(jniEnv, nativeAni, jfId, ani->rotateZTo());
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

