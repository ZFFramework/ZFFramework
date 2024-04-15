#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFObject ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFObject)
#define ZFImpl_sys_Android_JNI_NAME_ZFObject ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFObject)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

// ============================================================
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , JNIPointer, native_1invoke
        , JNIPointer zfjniPointer
        , jstring methodName
        , jlongArray params
        ) {
    zfstring methodNameZF;
    ZFImpl_sys_Android_zfstringFromStringT(methodNameZF, methodName);

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

    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfauto ret = (obj == zfnull
            ? ZFInvokeDetail(methodNameZF, paramsZF)
            : obj->invokeDetail(methodNameZF, paramsZF)
            );
    if(ret) {
        zfautoRelease(zfRetain(ret));
    }
    return JNIConvertZFObjectToJNIType(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , JNIPointer, native_1invoke_1generic
        , JNIPointer zfjniPointer
        , jstring methodName
        , jobjectArray params
        ) {
    zfstring methodNameZF;
    ZFImpl_sys_Android_zfstringFromStringT(methodNameZF, methodName);

    ZFCoreArray<zfstring> paramsZF;
    if(params != NULL) {
        jsize count = JNIUtilGetArrayLength(jniEnv, params);
        for(jsize i = 0; i < count; ++i) {
            jobject param = JNIUtilGetObjectArrayElement(jniEnv, params, i);
            paramsZF.add(ZFImpl_sys_Android_zfstringFromString(param));
            JNIUtilDeleteLocalRef(jniEnv, param);
        }
    }

    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfauto ret = (obj == zfnull
            ? ZFInvokeDetail(methodNameZF, paramsZF)
            : obj->invokeDetail(methodNameZF, paramsZF)
            );
    if(ret) {
        zfautoRelease(zfRetain(ret));
    }
    return JNIConvertZFObjectToJNIType(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

// ============================================================
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , void, native_1zfRetain
        , JNIPointer zfjniPointer
        ) {
    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfRetain(obj);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , void, native_1zfRelease
        , JNIPointer zfjniPointer
        ) {
    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfRelease(obj);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , void, native_1zfautoRelease
        , JNIPointer zfjniPointer
        ) {
    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfautoRelease(obj);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

