#include "ZFImpl_sys_Android_ZF_impl.h"
#include "ZFCore/ZFObject.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_jclsHolder, ZFLevelZFFrameworkStatic) {
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Object).c_str());
    this->jclassObject = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_String).c_str());
    this->jclassString = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Boolean).c_str());
    this->jclassBoolean = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Byte).c_str());
    this->jclassByte = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Character).c_str());
    this->jclassCharacter = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Short).c_str());
    this->jclassShort = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Integer).c_str());
    this->jclassInteger = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Long).c_str());
    this->jclassLong = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Float).c_str());
    this->jclassFloat = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Double).c_str());
    this->jclassDouble = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    jfieldID jfId;
    JNIType jfType(JNIType::T_object, ZFImpl_sys_Android_JNI_NAME_Boolean);

    jfId = JNIUtilGetStaticFieldID(jniEnv, this->jclassBoolean, "TRUE", jfType.getId());
    this->jobjectBooleanTRUE = JNIUtilNewGlobalRef(jniEnv, JNIUtilGetStaticObjectField(jniEnv, this->jclassBoolean, jfId));

    jfId = JNIUtilGetStaticFieldID(jniEnv, this->jclassBoolean, "FALSE", jfType.getId());
    this->jobjectBooleanFALSE = JNIUtilNewGlobalRef(jniEnv, JNIUtilGetStaticObjectField(jniEnv, this->jclassBoolean, jfId));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_jclsHolder) {
    JNIEnv *jniEnv = JNIGetJNIEnv();

    JNIUtilDeleteGlobalRef(jniEnv, this->jclassObject);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassString);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassBoolean);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassByte);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassCharacter);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassShort);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassInteger);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassLong);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassFloat);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclassDouble);

    JNIUtilDeleteGlobalRef(jniEnv, this->jobjectBooleanTRUE);
    JNIUtilDeleteGlobalRef(jniEnv, this->jobjectBooleanFALSE);
}
public:
    jclass jclassObject;
    jclass jclassString;
    jclass jclassBoolean;
    jclass jclassByte;
    jclass jclassCharacter;
    jclass jclassShort;
    jclass jclassInteger;
    jclass jclassLong;
    jclass jclassFloat;
    jclass jclassDouble;

    jobject jobjectBooleanTRUE;
    jobject jobjectBooleanFALSE;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_jclsHolder)

// ============================================================
jclass ZFImpl_sys_Android_jclassObject(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassObject;
}

jclass ZFImpl_sys_Android_jclassString(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassString;
}

jclass ZFImpl_sys_Android_jclassBoolean(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassBoolean;
}
jobject ZFImpl_sys_Android_jobjectBooleanTRUE(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jobjectBooleanTRUE;
}
jobject ZFImpl_sys_Android_jobjectBooleanFALSE(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jobjectBooleanFALSE;
}

jclass ZFImpl_sys_Android_jclassByte(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassByte;
}
jobject ZFImpl_sys_Android_newByte(ZF_IN jbyte v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassByte;
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jcls, "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_byte())
        ).c_str());
    return JNIUtilNewObject(jniEnv, jcls, jmId, v);
}

jclass ZFImpl_sys_Android_jclassCharacter(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassCharacter;
}
jobject ZFImpl_sys_Android_newCharacter(ZF_IN jchar v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassCharacter;
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jcls, "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_char())
        ).c_str());
    return JNIUtilNewObject(jniEnv, jcls, jmId, v);
}

jclass ZFImpl_sys_Android_jclassShort(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassShort;
}
jobject ZFImpl_sys_Android_newShort(ZF_IN jshort v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassShort;
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jcls, "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_short())
        ).c_str());
    return JNIUtilNewObject(jniEnv, jcls, jmId, v);
}

jclass ZFImpl_sys_Android_jclassInteger(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassInteger;
}
jobject ZFImpl_sys_Android_newInteger(ZF_IN jint v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassInteger;
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jcls, "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_int())
        ).c_str());
    return JNIUtilNewObject(jniEnv, jcls, jmId, v);
}

jclass ZFImpl_sys_Android_jclassLong(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassLong;
}
jobject ZFImpl_sys_Android_newLong(ZF_IN jlong v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassLong;
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jcls, "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_long())
        ).c_str());
    return JNIUtilNewObject(jniEnv, jcls, jmId, v);
}

jclass ZFImpl_sys_Android_jclassFloat(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassFloat;
}
jobject ZFImpl_sys_Android_newFloat(ZF_IN jfloat v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassFloat;
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jcls, "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_float())
        ).c_str());
    return JNIUtilNewObject(jniEnv, jcls, jmId, v);
}

jclass ZFImpl_sys_Android_jclassDouble(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassDouble;
}
jobject ZFImpl_sys_Android_newDouble(ZF_IN jdouble v) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclassDouble;
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jcls, "<init>",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_double())
        ).c_str());
    return JNIUtilNewObject(jniEnv, jcls, jmId, v);
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
    jstring nativeString = ZFCastStatic(jstring, jstr);
    const zfchar *utf8 = JNIUtilGetStringUTFChars(jniEnv, nativeString, zfnull);
    if(utf8 != zfnull) {
        s += utf8;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, nativeString, utf8);
}
jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfchar *s) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    if(s == zfnull) {
        s = "";
    }
    return JNIUtilNewStringUTF(jniEnv, s);
}

zfstring ZFImpl_sys_Android_stackTrace(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "stackTrace",
        JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
        ).c_str());
    jstring jobjString = ZFCastStatic(jstring, JNIUtilCallStaticObjectMethod(jniEnv, jcls, jmId));
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, jobjString, zfnull);
    zfstring ret;
    if(utf != zfnull) {
        ret = utf;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, jobjString, utf);
    return ret;
}
void ZFImpl_sys_Android_stackTracePrint(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "stackTracePrint",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, jcls, jmId);
}

void ZFImpl_sys_Android_objectInfoT(
        ZF_OUT zfstring &ret
        , ZF_IN jobject nativeObject
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "objectInfo",
        JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
        ).c_str());
    jstring tmp = ZFCastStatic(jstring, JNIUtilCallStaticObjectMethod(jniEnv, jcls, jmId, nativeObject));
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, tmp, zfnull);
    ret += utf;
    JNIUtilReleaseStringUTFChars(jniEnv, tmp, utf);
}

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidLog
        , jstring, native_1objectInfo
        , JNIPointer zfjnipointerZFObject
        ) {
    ZFObject *obj = ZFCastZFObject(ZFObject *, JNIConvertZFObjectFromJNIType(jniEnv, zfjnipointerZFObject));
    return jniEnv->NewStringUTF(ZFObjectInfo(obj).cString());
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

