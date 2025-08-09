#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIImageIO ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIImageIO)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImageIO ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIImageIO)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIImageIO, ZFImpl_sys_Android_JNI_NAME_ZFUIImageIO)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageIOImpl_sys_Android, ZFUIImageIO, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *imageApplyScale(
            ZF_IN zffloat imageScale
            , ZF_IN void *nativeImage
            , ZF_IN const ZFUISize &newSize
            , ZF_IN const ZFUIMargin &ninePatch
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImageIO(), "native_imageApplyScale",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_float())
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int()).add(JNIType::S_int())
                .add(JNIType::S_int()).add(JNIType::S_int()).add(JNIType::S_int()).add(JNIType::S_int())
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImageIO(), jmId
            , (jfloat)imageScale
            , (jobject)nativeImage
            , (jint)newSize.width, (jint)newSize.height
            , (jint)ninePatch.left, (jint)ninePatch.top, (jint)ninePatch.right, (jint)ninePatch.bottom
            );
        JNIScopeDeleteLocalRef(tmp);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
    virtual void *imageLoadInFrame(
            ZF_IN zffloat imageScale
            , ZF_IN void *nativeImage
            , ZF_IN const ZFUIRect &frameInImage
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImageIO(), "native_imageLoadInFrame",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_float())
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int()).add(JNIType::S_int()).add(JNIType::S_int()).add(JNIType::S_int())
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImageIO(), jmId
            , (jfloat)imageScale
            , (jobject)nativeImage
            , (jint)frameInImage.x, (jint)frameInImage.y, (jint)frameInImage.width, (jint)frameInImage.height
            );
        JNIScopeDeleteLocalRef(tmp);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
    virtual void *imageLoadFromColor(
            ZF_IN zffloat imageScale
            , ZF_IN const ZFUIColor &color
            , ZF_IN const ZFUISize &size
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImageIO(), "native_imageLoadFromColor",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_int())
                .add(JNIType::S_int()).add(JNIType::S_int())
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImageIO(), jmId
            , ZFImpl_sys_Android_ZFUIColorToColor(color)
            , (jint)size.width
            , (jint)size.height
            );
        JNIScopeDeleteLocalRef(tmp);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageIOImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

