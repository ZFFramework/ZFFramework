#include "ZFImpl_sys_Android_def.h"
#include "ZFCore/ZFObject.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFAndroid_JNIGlobalRef, JNIGlobalRef)
ZFTYPEID_ALIAS_DEFINE(ZFAndroid_JNIGlobalRef, JNIGlobalRef, ZFAndroid_jobject, jobject)

ZFCONV_REG(v_ZFAndroid_JNIGlobalRef, v_zfptr) {
    ret = zfobj<v_ZFAndroid_JNIGlobalRef>((jobject)obj->to<v_zfptr *>()->zfv);
    return zftrue;
}

ZFMETHOD_USER_REGISTER_0({
    v_ZFAndroid_JNIGlobalRef *owner = invokerObject;
    return owner->zfv;
}, v_ZFAndroid_JNIGlobalRef, zfbool, valid)

ZFOUTPUT_TYPE_DEFINE(JNIGlobalRef, {
    ZFImpl_sys_Android_objectInfoT(s, v);
})

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_jobjectHolder, ZFLevelZFFrameworkStatic) {
    jobject tmp = NULL;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    jfieldID jfId;
    JNIType jfType(JNIType::T_object, ZFImpl_sys_Android_JNI_NAME_Boolean);
    jclass jclassBoolean = ZFImpl_sys_Android_jclassBoolean();

    jfId = JNIUtilGetStaticFieldID(jniEnv, jclassBoolean, "TRUE", jfType.getId());
    this->jobjectBooleanTRUE = JNIUtilNewGlobalRef(jniEnv, JNIUtilGetStaticObjectField(jniEnv, jclassBoolean, jfId));

    jfId = JNIUtilGetStaticFieldID(jniEnv, jclassBoolean, "FALSE", jfType.getId());
    this->jobjectBooleanFALSE = JNIUtilNewGlobalRef(jniEnv, JNIUtilGetStaticObjectField(jniEnv, jclassBoolean, jfId));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_jobjectHolder) {
    JNIEnv *jniEnv = JNIGetJNIEnv();

    JNIUtilDeleteGlobalRef(jniEnv, this->jobjectBooleanTRUE);
    JNIUtilDeleteGlobalRef(jniEnv, this->jobjectBooleanFALSE);
}
public:
    jobject jobjectBooleanTRUE;
    jobject jobjectBooleanFALSE;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_jobjectHolder)

// ============================================================
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassObject, ZFImpl_sys_Android_JNI_NAME_Object)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassString, ZFImpl_sys_Android_JNI_NAME_String)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassNumber, ZFImpl_sys_Android_JNI_NAME_Number)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassBoolean, ZFImpl_sys_Android_JNI_NAME_Boolean)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassByte, ZFImpl_sys_Android_JNI_NAME_Byte)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassCharacter, ZFImpl_sys_Android_JNI_NAME_Character)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassShort, ZFImpl_sys_Android_JNI_NAME_Short)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassInteger, ZFImpl_sys_Android_JNI_NAME_Integer)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassLong, ZFImpl_sys_Android_JNI_NAME_Long)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassFloat, ZFImpl_sys_Android_JNI_NAME_Float)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassDouble, ZFImpl_sys_Android_JNI_NAME_Double)

jobject ZFImpl_sys_Android_jobjectBooleanTRUE(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jobjectHolder)->jobjectBooleanTRUE;
}
jobject ZFImpl_sys_Android_jobjectBooleanFALSE(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jobjectHolder)->jobjectBooleanFALSE;
}
jobject ZFImpl_sys_Android_newByte(ZF_IN jbyte v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassByte(), "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_byte())
        ).c_str());
    return JNIUtilNewObject(jniEnv, ZFImpl_sys_Android_jclassByte(), jmId, v);
}
jobject ZFImpl_sys_Android_newCharacter(ZF_IN jchar v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassCharacter(), "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_char())
        ).c_str());
    return JNIUtilNewObject(jniEnv, ZFImpl_sys_Android_jclassCharacter(), jmId, v);
}
jobject ZFImpl_sys_Android_newShort(ZF_IN jshort v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassShort(), "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_short())
        ).c_str());
    return JNIUtilNewObject(jniEnv, ZFImpl_sys_Android_jclassShort(), jmId, v);
}
jobject ZFImpl_sys_Android_newInteger(ZF_IN jint v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassInteger(), "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_int())
        ).c_str());
    return JNIUtilNewObject(jniEnv, ZFImpl_sys_Android_jclassInteger(), jmId, v);
}
jobject ZFImpl_sys_Android_newLong(ZF_IN jlong v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassLong(), "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_long())
        ).c_str());
    return JNIUtilNewObject(jniEnv, ZFImpl_sys_Android_jclassLong(), jmId, v);
}
jobject ZFImpl_sys_Android_newFloat(ZF_IN jfloat v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassFloat(), "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_float())
        ).c_str());
    return JNIUtilNewObject(jniEnv, ZFImpl_sys_Android_jclassFloat(), jmId, v);
}
jobject ZFImpl_sys_Android_newDouble(ZF_IN jdouble v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassDouble(), "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_double())
        ).c_str());
    return JNIUtilNewObject(jniEnv, ZFImpl_sys_Android_jclassDouble(), jmId, v);
}

// ============================================================
void ZFImpl_sys_Android_zfstringFromStringT(
        ZF_IN_OUT zfstring &s
        , ZF_IN jobject jstr
        ) {
    if(jstr == NULL) {
        return;
    }
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jstring nativeString = (jstring)jstr;
    const char *utf8 = JNIUtilGetStringUTFChars(jniEnv, nativeString, NULL);
    if(utf8 != NULL) {
        s += utf8;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, nativeString, utf8);
}
jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfchar *s) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    if(s == NULL) {
        s = "";
    }
    return JNIUtilNewStringUTF(jniEnv, s);
}

ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidLog, ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog)

zfstring ZFImpl_sys_Android_stackTrace(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidLog(), "stackTrace",
        JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
        ).c_str());
    jstring jobjString = (jstring)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidLog(), jmId);
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, jobjString, NULL);
    zfstring ret;
    if(utf != NULL) {
        ret = utf;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, jobjString, utf);
    return ret;
}
void ZFImpl_sys_Android_stackTracePrint(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidLog(), "stackTracePrint",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidLog(), jmId);
}

void ZFImpl_sys_Android_objectInfoT(
        ZF_OUT zfstring &ret
        , ZF_IN jobject nativeObject
        ) {
    if(nativeObject == NULL) {
        ret += ZFTOKEN_zfnull;
        return;
    }
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassObject(), "toString",
        JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
        ).c_str());
    jstring tmp = (jstring)JNIUtilCallObjectMethod(jniEnv, nativeObject, jmId, nativeObject);
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, tmp, NULL);
    ret += utf;
    JNIUtilReleaseStringUTFChars(jniEnv, tmp, utf);
}

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidLog
        , jstring, native_1objectInfo
        , JNIPointer zfjnipointerZFObject
        ) {
    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjnipointerZFObject);
    return jniEnv->NewStringUTF(ZFObjectInfo(obj).cString());
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

