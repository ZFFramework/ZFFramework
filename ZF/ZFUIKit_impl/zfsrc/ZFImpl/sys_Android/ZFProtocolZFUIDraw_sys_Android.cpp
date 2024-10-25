#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIDraw.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIDrawableView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIDrawableView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIDrawableView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIDrawableView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIDrawableView, ZFImpl_sys_Android_JNI_NAME_ZFUIDrawableView)

#define ZFImpl_sys_Android_JNI_ID_ZFUIDraw ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIDraw)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIDraw ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIDraw)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIDraw, ZFImpl_sys_Android_JNI_NAME_ZFUIDraw)

#define ZFImpl_sys_Android_JNI_ID_ZFUIDrawNativeToken ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIDraw_00024NativeToken)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIDrawNativeToken ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIDraw$NativeToken)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIDrawNativeToken, ZFImpl_sys_Android_JNI_NAME_ZFUIDrawNativeToken)

// ============================================================
zfclassNotPOD _ZFP_ZFUIDrawImpl_sys_Android_Token {
public:
    jobject canvas;
    jobject image; // for beginForImage only
};

// ============================================================
// ZFUIDrawForView
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForViewImpl_sys_Android, ZFUIDrawForView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")

public:
    virtual void *nativeDrawableViewCreate(
            ZF_IN ZFUIDrawableView *drawableView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawableView(), "native_nativeDrawableViewCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawableView(), jmId
            , JNIConvertZFObjectToJNIType(jniEnv, drawableView)
            );
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return (void *)ret;
    }
    virtual void nativeDrawableViewDestroy(
            ZF_IN ZFUIDrawableView *drawableView
            , ZF_IN void *nativeDrawableView
            ) {
        // for performance, we won't have JNI call to destroy, simply delete the global ref

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeDrawableViewTmp = (jobject)nativeDrawableView;
        JNIUtilDeleteGlobalRef(jniEnv, nativeDrawableViewTmp);
    }

    virtual void drawRequest(ZF_IN ZFUIDrawableView *drawableView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawableView(), "native_drawRequest",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawableView(), jmId
            , (jobject)drawableView->nativeImplView()
            );
    }

public:
    virtual zfbool beginForView(ZF_IN_OUT ZFUIDrawToken &token) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawableView(), "native_beginForView",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject canvas = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawableView(), jmId
            , (jobject)token.target.to<ZFUIDrawableView *>()->nativeImplView()
            );
        if(canvas == NULL) {
            return zffalse;
        }
        _ZFP_ZFUIDrawImpl_sys_Android_Token *nativeToken = zfpoolNew(_ZFP_ZFUIDrawImpl_sys_Android_Token);
        token.impl = nativeToken;
        nativeToken->canvas = JNIUtilNewGlobalRef(jniEnv, canvas);
        JNIUtilDeleteLocalRef(jniEnv, canvas);
        return zftrue;
    }
    virtual void endForView(ZF_IN_OUT ZFUIDrawToken &token) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFUIDrawImpl_sys_Android_Token *nativeToken = (_ZFP_ZFUIDrawImpl_sys_Android_Token *)token.impl;
        JNIUtilDeleteGlobalRef(jniEnv, nativeToken->canvas);
        zfpoolDelete(nativeToken);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIDrawableView
        , void, native_1notifyOnDraw
        , JNIPointer zfjniPointerOwnerZFUIDrawableView
        ) {
    ZFPROTOCOL_ACCESS(ZFUIDrawForView)->notifyOnDraw(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIDrawableView));
}
JNI_METHOD_DECLARE_END()
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDrawForImage
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForImageImpl_sys_Android, ZFUIDrawForImage, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual zfbool beginForImage(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN const ZFUISize &imageSizePixel
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), "native_beginForImage",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_ZFUIDrawNativeToken), JNIParamTypeContainer()
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        jobject implToken = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), jmId
            , (jint)imageSizePixel.width
            , (jint)imageSizePixel.height
            );
        JNIBlockedDeleteLocalRef(implToken);

        _ZFP_ZFUIDrawImpl_sys_Android_Token *nativeToken = zfpoolNew(_ZFP_ZFUIDrawImpl_sys_Android_Token);
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawNativeToken(), "canvas", JNIType::S_object_Object().getId());
            jobject tmp = JNIUtilGetObjectField(jniEnv, implToken, jfId);
            nativeToken->canvas = JNIUtilNewGlobalRef(jniEnv, tmp);
            JNIUtilDeleteLocalRef(jniEnv, tmp);
        }
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFUIDrawNativeToken(), "image", JNIType::S_object_Object().getId());
            jobject tmp = JNIUtilGetObjectField(jniEnv, implToken, jfId);
            nativeToken->image = JNIUtilNewGlobalRef(jniEnv, tmp);
            JNIUtilDeleteLocalRef(jniEnv, tmp);
        }

        token.impl = nativeToken;
        return zftrue;
    }
    virtual void *endForImage(ZF_IN_OUT ZFUIDrawToken &token) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFUIDrawImpl_sys_Android_Token *nativeToken = (_ZFP_ZFUIDrawImpl_sys_Android_Token *)token.impl;
        void *ret = (void *)nativeToken->image;
        JNIUtilDeleteGlobalRef(jniEnv, nativeToken->canvas);
        // delete later: JNIUtilDeleteGlobalRef(jniEnv, nativeToken->image);
        zfpoolDelete(nativeToken);
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForImageImpl_sys_Android)

// ============================================================
// ZFUIDraw
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawImpl_sys_Android, ZFUIDraw, ZFProtocolLevel::e_SystemNormal)
    // ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Canvas")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForImage, "Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void drawClear(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), "native_drawClear",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        _ZFP_ZFUIDrawImpl_sys_Android_Token *nativeToken = (_ZFP_ZFUIDrawImpl_sys_Android_Token *)token.impl;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), jmId
            , nativeToken->canvas
            , (jint)targetFramePixel.x
            , (jint)targetFramePixel.y
            , (jint)targetFramePixel.width
            , (jint)targetFramePixel.height
            );
    }
    virtual void drawColor(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN const ZFUIColor &color
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), "native_drawColor",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        _ZFP_ZFUIDrawImpl_sys_Android_Token *nativeToken = (_ZFP_ZFUIDrawImpl_sys_Android_Token *)token.impl;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), jmId
            , nativeToken->canvas
            , ZFImpl_sys_Android_ZFUIColorToColor(color)
            , (jint)targetFramePixel.x
            , (jint)targetFramePixel.y
            , (jint)targetFramePixel.width
            , (jint)targetFramePixel.height
            );
    }
    virtual void drawImage(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN ZFUIImage *image
            , ZF_IN const ZFUIRect &imageFramePixel
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), "native_drawImage",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        _ZFP_ZFUIDrawImpl_sys_Android_Token *nativeToken = (_ZFP_ZFUIDrawImpl_sys_Android_Token *)token.impl;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIDraw(), jmId
            , nativeToken->canvas
            , (jobject)image->nativeImage()
            , (jint)imageFramePixel.x
            , (jint)imageFramePixel.y
            , (jint)imageFramePixel.width
            , (jint)imageFramePixel.height
            , (jint)targetFramePixel.x
            , (jint)targetFramePixel.y
            , (jint)targetFramePixel.width
            , (jint)targetFramePixel.height
            );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

