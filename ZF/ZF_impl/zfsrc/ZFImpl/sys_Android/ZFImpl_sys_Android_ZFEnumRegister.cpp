#include "ZFImpl_sys_Android_ZF_impl.h"
#include "ZFCore.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFEnum ZFImpl_sys_Android_JNI_ID(ZF_1impl_ZFEnum)
#define ZFImpl_sys_Android_JNI_NAME_ZFEnum ZFImpl_sys_Android_JNI_NAME(ZF_impl.ZFEnum)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum
        , void, native_1enumInvalid
        , jstring rawEnumNamespace
        , jstring rawEnumValueName
        ) {
    const char *rawEnumNamespaceT = rawEnumNamespace ? JNIUtilGetStringUTFChars(jniEnv, rawEnumNamespace, NULL) : NULL;
    const char *rawEnumValueNameT = JNIUtilGetStringUTFChars(jniEnv, rawEnumValueName, NULL);
    ZFCoreCriticalMessageTrim("[ZFEnum] invalid enum: %s::%s",
        rawEnumNamespaceT,
        rawEnumValueNameT);
    if(rawEnumNamespaceT != NULL) {
        JNIUtilReleaseStringUTFChars(jniEnv, rawEnumNamespace, rawEnumNamespaceT);
    }
    JNIUtilReleaseStringUTFChars(jniEnv, rawEnumValueName, rawEnumValueNameT);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum
        , jint, native_1rawEnumValue
        , jstring rawEnumNamespace
        , jstring rawEnumValueName
        ) {
    const char *rawEnumNamespaceT = rawEnumNamespace ? JNIUtilGetStringUTFChars(jniEnv, rawEnumNamespace, NULL) : NULL;
    const char *rawEnumValueNameT = JNIUtilGetStringUTFChars(jniEnv, rawEnumValueName, NULL);
    zfuint ret = ZFEnumInvalid();
    do {
        const ZFMethod *method = ZFMethodForName(rawEnumNamespaceT, rawEnumValueNameT);
        if(method == zfnull) {break;}
        ret = *(const zfuint *)method->methodInvoke().to<ZFTypeIdWrapper *>()->wrappedValue();
    } while(zffalse);
    if(rawEnumNamespaceT != NULL) {
        JNIUtilReleaseStringUTFChars(jniEnv, rawEnumNamespace, rawEnumNamespaceT);
    }
    JNIUtilReleaseStringUTFChars(jniEnv, rawEnumValueName, rawEnumValueNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum
        , jint, native_1enumDefault
        , jstring enumClassName
        ) {
    const char *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, NULL);
    zfuint ret = ZFEnumInvalid();
    do {
        const ZFClass *cls = ZFClass::classForName(enumClassNameT);
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName("EnumDefault");
        if(method == zfnull) {break;}
        ret = method->methodInvoke().to<v_zfuint *>()->zfv;
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum
        , jint, native_1enumValue
        , jstring enumClassName
        , jstring enumValueName
        ) {
    const char *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, NULL);
    const char *enumValueNameT = JNIUtilGetStringUTFChars(jniEnv, enumValueName, NULL);
    zfuint ret = ZFEnumInvalid();
    do {
        const ZFClass *cls = ZFClass::classForName(enumClassNameT);
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName("EnumValueForName");
        if(method == zfnull) {break;}
        ret = method->methodInvoke(zfnull, zfobj<v_zfstring>(enumValueNameT)).to<v_zfuint *>()->zfv;
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    JNIUtilReleaseStringUTFChars(jniEnv, enumValueName, enumValueNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum
        , jstring, native_1enumName
        , jstring enumClassName
        , jint enumValue
        ) {
    const char *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, NULL);
    zfstring ret = ZFEnumNameInvalid();
    do {
        const ZFClass *cls = ZFClass::classForName(enumClassNameT);
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName("EnumNameForValue");
        if(method == zfnull) {break;}
        ret = method->methodInvoke(zfnull, zfobj<v_zfuint>(enumValue)).to<v_zfstring *>()->zfv;
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    return JNIUtilNewStringUTF(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

