#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUITextView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUITextView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUITextView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUITextView, ZFImpl_sys_Android_JNI_NAME_ZFUITextView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_Android, ZFUITextView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:TextView")

public:
    virtual void *nativeTextViewCreate(
            ZF_IN ZFUITextView *textView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_nativeTextViewCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId);
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return (void *)ret;
    }
    virtual void nativeTextViewDestroy(
            ZF_IN ZFUITextView *textView
            , ZF_IN void *nativeTextView
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_nativeTextViewDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject nativeTextViewTmp = (jobject)nativeTextView;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId, nativeTextViewTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTextViewTmp);
    }

    // ============================================================
    // properties
public:
    virtual void text(
            ZF_IN ZFUITextView *textView
            , ZF_IN const zfstring &text
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_text",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , (jobject)ZFImpl_sys_Android_zfstringToString(text)
            );
    }
    virtual void textAppearance(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_textAppearance",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , (jint)textAppearance
            );
    }
    virtual void textAlign(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_textAlign",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , (jint)textAlign
            );
    }
    virtual void textColor(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIColor const &textColor
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_textColor",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , ZFImpl_sys_Android_ZFUIColorToColor(textColor)
            );
    }
    virtual void textSize(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSize
            ) {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoMin(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMin
            ) {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoMax(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMax
            ) {
        // changed during layoutNativeTextView
    }
    virtual void singleLine(
            ZF_IN ZFUITextView *textView
            , ZF_IN zfbool singleLine
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_singleLine",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , (jboolean)singleLine
            );
    }
    virtual void textTruncateMode(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextTruncateMode const &textTruncateMode
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_textTruncateMode",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , (jint)textTruncateMode
            );
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_measureNativeTextView",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , (jint)sizeHint.width
            , (jint)sizeHint.height
            , (jint)textSize
            );
        jint *jarrSize = JNIUtilGetIntArrayElements(jniEnv, jobjSize, NULL);
        ZFUISize ret = ZFUISizeCreate((zffloat)jarrSize[0], (zffloat)jarrSize[1]);
        JNIUtilReleaseIntArrayElements(jniEnv, jobjSize, jarrSize, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjSize);
        return ret;
    }

    virtual zffloat textSizeCurrent(ZF_IN ZFUITextView *textView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_textSizeCurrent",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jint ret = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            );
        return (zffloat)ret;
    }

    virtual void layoutNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &viewSize
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), "native_textSizeAuto_update",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        zffloat fixedTextSize = this->calcTextSizeAuto(textView, viewSize);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextView(), jmId
            , (jobject)textView->nativeImplView()
            , (jint)fixedTextSize
            );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

