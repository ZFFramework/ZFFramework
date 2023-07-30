#include "ZFImpl_sys_Android_ZF_impl.h"
#include "ZFCore/ZFObject.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_jclsHolder, ZFLevelZFFrameworkStatic) {
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Object).c_str());
    this->jclsObject = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_String).c_str());
    this->jclsString = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_jclsHolder) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsObject);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsString);
}
public:
    jclass jclsObject;
    jclass jclsString;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_jclsHolder)

jclass ZFImpl_sys_Android_jclassObject(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclsObject;
}

void ZFImpl_sys_Android_zfstringFromString(
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
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
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
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
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
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
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

