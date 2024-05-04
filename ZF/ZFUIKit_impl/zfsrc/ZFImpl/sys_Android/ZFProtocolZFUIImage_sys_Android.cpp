#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIImage ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIImage)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImage ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIImage)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_Android, ZFUIImage, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:BitmapDrawable")
public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIImage).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void *nativeImageFromInput(ZF_IN const ZFInput &inputCallback) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeImageFromInput",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFImpl_sys_Android_ZFInputWrapperFromZFInput(inputCallback));
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
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeImageToOutput",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        return (zfbool)JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
             , (jobject)nativeImage
             , ZFImpl_sys_Android_ZFOutputWrapperFromZFOutput(outputCallback)
             );
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeImageCopy",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, (jobject)nativeImage);
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
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeImageSize",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, (jobject)nativeImage);
        jint *jarrSize = JNIUtilGetIntArrayElements(jniEnv, jobjSize, NULL);
        ZFUISize ret = ZFUISizeMake((zffloat)jarrSize[0], (zffloat)jarrSize[1]);
        JNIUtilReleaseIntArrayElements(jniEnv, jobjSize, jarrSize, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjSize);
        return ret;
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

