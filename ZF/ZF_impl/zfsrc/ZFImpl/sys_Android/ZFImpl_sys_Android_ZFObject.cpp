#include "ZFImpl_sys_Android_ZFObject.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFObject, ZFImpl_sys_Android_JNI_NAME_ZFObject)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFListener, ZFImpl_sys_Android_JNI_NAME_ZFListener)

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

    ZFCoreArray<zfauto> paramsZF;
    if(params != NULL) {
        jsize count = JNIUtilGetArrayLength(jniEnv, params);
        for(jsize i = 0; i < count; ++i) {
            jobject param = JNIUtilGetObjectArrayElement(jniEnv, params, i);
            zfobj<v_zfstring> tmp;
            ZFImpl_sys_Android_zfstringFromStringT(tmp->zfv, param) ;
            paramsZF.add(tmp);
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
        , JNIPointer, zfRetain
        , JNIPointer zfjniPointer
        ) {
    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfRetain(obj);
    return zfjniPointer;
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , void, zfRelease
        , JNIPointer zfjniPointer
        ) {
    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfRelease(obj);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , JNIPointer, zfautoRelease
        , JNIPointer zfjniPointer
        ) {
    ZFObject *obj = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer);
    zfautoRelease(obj);
    return zfjniPointer;
}
JNI_METHOD_DECLARE_END()

// ============================================================
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , jstring, toString
        , JNIPointer zfjniPointer
        ) {
    const zfchar *s = ZFDI_toString(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer));
    return JNIUtilNewStringUTF(jniEnv, s);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , jdouble, toNumber
        , JNIPointer zfjniPointer
        ) {
    return (jdouble)ZFDI_toNumber(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointer));
}
JNI_METHOD_DECLARE_END()

// ============================================================
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFObject
        , JNIPointer, native_1toListener
        , jobject listener
        ) {
    ZFLISTENER_1(wrap
            , JNIGlobalRef, listener
            ) {
        if(!listener) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, ZFImpl_sys_Android_jclassZFListener(), "run",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        zfobj<v_ZFArgs> tmp(zfargs);
        JNIUtilCallVoidMethod(jniEnv, (jobject)listener, jmId
            , JNIConvertZFObjectToJNIType(jniEnv, tmp)
            );
    } ZFLISTENER_END()
    v_ZFListener *ret = zfAlloc(v_ZFListener, wrap);
    return JNIConvertZFObjectToJNIType(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

