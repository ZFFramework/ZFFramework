#include "ZFImpl_sys_Android_ZFLua_impl.h"
#include "ZFLua.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFLua ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFLua)
#define ZFImpl_sys_Android_JNI_NAME_ZFLua ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFLua)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

// ============================================================
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFLua
        , JNIPointer, native_1ZFLuaExecute_1buf
        , jstring buf
        , jlongArray params
        ) {
    zfstring bufZF;
    ZFImpl_sys_Android_zfstringFromStringT(bufZF, buf);

    ZFCoreArray<zfauto> paramsZF;
    if(params != NULL) {
        jsize count = JNIUtilGetArrayLength(jniEnv, params);
        JNIPointer *paramsBuf = JNIUtilGetLongArrayElements(jniEnv, params, NULL);
        for(jsize i = 0; i < count; ++i) {
            ZFObject *paramZF = JNIConvertZFObjectFromJNIType(jniEnv, paramsBuf[i]);
            paramsZF.add(paramZF);
        }
        JNIUtilReleaseLongArrayElements(jniEnv, params, paramsBuf, JNI_ABORT);
    }

    zfauto ret = ZFLuaExecuteDetail(bufZF, paramsZF);
    if(ret) {
        zfautoRelease(zfRetain(ret));
    }
    return JNIConvertZFObjectToJNIType(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFLua
        , JNIPointer, native_1ZFLuaExecute_1input
        , jobject input
        , jlongArray params
        ) {
    ZFInput inputZF = ZFImpl_sys_Android_ZFInputFromZFAndroidInput(input);

    ZFCoreArray<zfauto> paramsZF;
    if(params != NULL) {
        jsize count = JNIUtilGetArrayLength(jniEnv, params);
        JNIPointer *paramsBuf = JNIUtilGetLongArrayElements(jniEnv, params, NULL);
        for(jsize i = 0; i < count; ++i) {
            ZFObject *paramZF = JNIConvertZFObjectFromJNIType(jniEnv, paramsBuf[i]);
            paramsZF.add(paramZF);
        }
        JNIUtilReleaseLongArrayElements(jniEnv, params, paramsBuf, JNI_ABORT);
    }

    zfauto ret = ZFLuaExecuteDetail(inputZF, paramsZF);
    if(ret) {
        zfautoRelease(zfRetain(ret));
    }
    return JNIConvertZFObjectToJNIType(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFLua
        , JNIPointer, native_1ZFLuaExecute
        , JNIPointer zfjniPointerZFInput
        , jlongArray params
        ) {
    v_ZFInput *inputZF = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerZFInput);
    if(inputZF == zfnull) {
        return JNIConvertZFObjectToJNIType(jniEnv, zfnull);
    }

    ZFCoreArray<zfauto> paramsZF;
    if(params != NULL) {
        jsize count = JNIUtilGetArrayLength(jniEnv, params);
        JNIPointer *paramsBuf = JNIUtilGetLongArrayElements(jniEnv, params, NULL);
        for(jsize i = 0; i < count; ++i) {
            ZFObject *paramZF = JNIConvertZFObjectFromJNIType(jniEnv, paramsBuf[i]);
            paramsZF.add(paramZF);
        }
        JNIUtilReleaseLongArrayElements(jniEnv, params, paramsBuf, JNI_ABORT);
    }

    zfauto ret = ZFLuaExecuteDetail(inputZF->zfv, paramsZF);
    if(ret) {
        zfautoRelease(zfRetain(ret));
    }
    return JNIConvertZFObjectToJNIType(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

