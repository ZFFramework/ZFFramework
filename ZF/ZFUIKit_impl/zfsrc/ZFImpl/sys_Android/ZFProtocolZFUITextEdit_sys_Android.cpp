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
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , JNIConvertZFObjectToJNIType(jniEnv, textEdit)
            );
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
    virtual void editEnable(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editEnable
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_editEnable",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jboolean)editEnable
            );
    }
    virtual void textEditSecure(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editSecured
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textEditSecure",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jboolean)editSecured
            );
    }
    virtual void keyboardType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardType keyboardType
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_keyboardType",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jint)keyboardType
            );
    }
    virtual void keyboardReturnType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardReturnType keyboardReturnType
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_keyboardReturnType",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jint)keyboardReturnType
            );
    }

    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT ZFIndexRange &selectedRange
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId_start = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_selectedRange_start",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        static jmethodID jmId_count = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_selectedRange_count",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        selectedRange.start = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId_start
            , (jobject)textEdit->nativeImplView()
            );
        selectedRange.count = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId_count
            , (jobject)textEdit->nativeImplView()
            );
    }
    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFIndexRange &selectedRange
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_selectedRange",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jint)selectedRange.start
            , (jint)selectedRange.count
            );
    }

public:
    virtual void text(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const zfstring &text
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_text",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jobject)ZFImpl_sys_Android_zfstringToString(text)
            );
    }
    virtual void textAppearance(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_textAppearance",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jint)textAppearance
            );
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
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jint)textAlign
            );
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
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , ZFImpl_sys_Android_ZFUIColorToColor(textColor)
            );
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
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            , (jint)textSize
            );
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
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
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

    // ============================================================
    // edit
public:
    virtual void editBegin(ZF_IN ZFUITextEdit *textEdit) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_editBegin",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            );
    }
    virtual void editEnd(ZF_IN ZFUITextEdit *textEdit) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), "native_editEnd",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUITextEdit(), jmId
            , (jobject)textEdit->nativeImplView()
            );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , jboolean, native_1notifyCheckTextShouldUpdate
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        , jobject jobjNewString
        ) {
    return (jboolean)ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldUpdate(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit),
        ZFImpl_sys_Android_zfstringFromString((jstring)jobjNewString));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , void, native_1notifyTextUpdate
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        , jobject jobjNewString
        ) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextUpdate(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit),
        ZFImpl_sys_Android_zfstringFromString((jstring)jobjNewString));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUITextEdit
        , void, native_1notifyTextSelectRangeOnUpdate
        , JNIPointer zfjniPointerOwnerZFUITextEdit
        ) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeOnUpdate(
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

