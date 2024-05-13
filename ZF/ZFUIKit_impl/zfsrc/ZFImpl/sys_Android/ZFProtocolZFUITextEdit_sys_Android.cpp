#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUITextEdit ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUITextEdit)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextEdit ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUITextEdit)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUITextEdit, ZFImpl_sys_Android_JNI_NAME_ZFUITextEdit)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_Android, ZFUITextEdit, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:EditText")

public:
    virtual void *nativeTextEditCreate(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_nativeTextEditCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            JNIConvertZFObjectToJNIType(jniEnv, textEdit));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return (void *)ret;
    }
    virtual void nativeTextEditDestroy(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN void *nativeTextEdit
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_nativeTextEditDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject nativeTextEditTmp = (jobject)nativeTextEdit;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId, nativeTextEditTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTextEditTmp);
    }

    // ============================================================
    // properties
public:
    virtual void textEditEnable(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool textEditEnable
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textEditEnable",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jboolean)textEditEnable);
    }
    virtual void textEditSecure(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool textEditSecured
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textEditSecure",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jboolean)textEditSecured);
    }
    virtual void textEditKeyboardType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textEditKeyboardType",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jint)textEditKeyboardType);
    }
    virtual void textEditKeyboardReturnType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textEditKeyboardReturnType",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jint)textEditKeyboardReturnType);
    }

    virtual void textSelectRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT ZFIndexRange &textSelectRange
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId_start = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textSelectRange_start",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        static jmethodID jmId_count = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textSelectRange_count",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        textSelectRange.start = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId_start,
            (jobject)textEdit->nativeImplView());
        textSelectRange.count = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId_count,
            (jobject)textEdit->nativeImplView());
    }
    virtual void textSelectRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFIndexRange &textSelectRange
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textSelectRange",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jint)textSelectRange.start,
            (jint)textSelectRange.count);
    }

public:
    virtual void text(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const zfchar *text
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_text",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jobject)ZFImpl_sys_Android_zfstringToString(text));
    }
    virtual void textAppearance(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextAppearanceEnum const &textAppearance
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textAppearance",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jint)textAppearance);
    }
    virtual void textAlign(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textAlign",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jint)textAlign);
    }
    virtual void textColor(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIColor const &textColor
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textColor",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            ZFImpl_sys_Android_ZFUIColorToColor(textColor));
    }
    virtual void textSize(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zffloat textSize
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textSize",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jint)textSize);
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextEdit(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_measureNativeTextEdit",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView(),
            (jint)sizeHint.width,
            (jint)sizeHint.height,
            (jint)textSize);
        jint *jarrSize = JNIUtilGetIntArrayElements(jniEnv, jobjSize, NULL);
        ZFUISize ret = ZFUISizeMake((zffloat)jarrSize[0], (zffloat)jarrSize[1]);
        JNIUtilReleaseIntArrayElements(jniEnv, jobjSize, jarrSize, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjSize);
        return ret;
    }

    // ============================================================
    // edit
public:
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textEditBegin",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView());
    }
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textEditEnd",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId,
            (jobject)textEdit->nativeImplView());
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextEditImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , jboolean, native_1notifyCheckTextShouldChange
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        , jobject jobjNewString
        ) {
    return (jboolean)ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldChange(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit),
        ZFImpl_sys_Android_zfstringFromString((jstring)jobjNewString));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , void, native_1notifyTextChange
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        , jobject jobjNewString
        ) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextChange(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit),
        ZFImpl_sys_Android_zfstringFromString((jstring)jobjNewString));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , void, native_1notifyTextSelectRangeChange
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        ) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeChange(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , void, native_1notifyTextReturnClicked
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        ) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , void, native_1notifyTextEditBegin
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        ) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , void, native_1notifyTextEditEnd
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        ) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

