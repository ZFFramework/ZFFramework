#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIImage ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIImage)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImage ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIImage)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIImage, ZFImpl_sys_Android_JNI_NAME_ZFUIImage)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_Android, ZFUIImage, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:BitmapDrawable")
public:
    virtual void *nativeImageFromInput(ZF_IN const ZFInput &inputCallback) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), "native_nativeImageFromInput",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), jmId
            , ZFImpl_sys_Android_ZFInputWrapperFromZFInput(inputCallback)
            );
        if(tmp == NULL) {
            return zfnull;
        }
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual zfbool nativeImageToOutput(
            ZF_IN void *nativeImage
            , ZF_OUT const ZFOutput &outputCallback
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), "native_nativeImageToOutput",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        return (zfbool)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), jmId
             , (jobject)nativeImage
             , ZFImpl_sys_Android_ZFOutputWrapperFromZFOutput(outputCallback)
             );
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), "native_nativeImageCopy",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), jmId, (jobject)nativeImage);
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }

    virtual void *nativeImageRetain(ZF_IN void *nativeImage) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        return JNIUtilNewGlobalRef(jniEnv, (jobject)nativeImage);
    }
    virtual void nativeImageRelease(ZF_IN void *nativeImage) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, (jobject)nativeImage);
    }

    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), "native_nativeImageSize",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImage(), jmId, (jobject)nativeImage);
        jint *jarrSize = JNIUtilGetIntArrayElements(jniEnv, jobjSize, NULL);
        ZFUISize ret = ZFUISizeCreate((zffloat)jarrSize[0], (zffloat)jarrSize[1]);
        JNIUtilReleaseIntArrayElements(jniEnv, jobjSize, jarrSize, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjSize);
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

