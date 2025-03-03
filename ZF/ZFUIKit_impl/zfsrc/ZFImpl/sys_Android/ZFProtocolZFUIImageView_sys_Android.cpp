#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIImageView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIImageView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImageView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIImageView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIImageView, ZFImpl_sys_Android_JNI_NAME_ZFUIImageView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageViewImpl_sys_Android, ZFUIImageView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:ImageView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeImageViewCreate(
            ZF_IN ZFUIImageView *imageView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImageView(), "native_nativeImageViewCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImageView(), jmId);
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return (void *)ret;
    }
    virtual void nativeImageViewDestroy(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN void *nativeImageView
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImageView(), "native_nativeImageViewDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject nativeImageViewTmp = (jobject)nativeImageView;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImageView(), jmId, nativeImageViewTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeImageViewTmp);
    }

    virtual void image(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN ZFUIImage *image
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIImageView(), "native_image",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
                .add(JNIType::S_float())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        if(image == zfnull) {
            JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImageView(), jmId
                , (jobject)imageView->nativeImplView()
                , (jobject)NULL
                , (jfloat)1
                , (jint)0
                , (jint)0
                , (jint)0
                , (jint)0
                );
        }
        else {
            zffloat imageScaleFixed = image->imageScaleFixed();
            ZFUIMargin imageNinePatchScaled = ZFUIMarginApplyScale(image->imageNinePatch(), image->imageScaleFixed());
            JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIImageView(), jmId
                , (jobject)imageView->nativeImplView()
                , (jobject)image->nativeImage()
                , (jfloat)imageScaleFixed
                , (jint)imageNinePatchScaled.left
                , (jint)imageNinePatchScaled.top
                , (jint)imageNinePatchScaled.right
                , (jint)imageNinePatchScaled.bottom
                );
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

