#include "ZFImpl_sys_Android_ZFOutputWrapper.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOutputWrapper
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFOutputWrapper_jclsHolder, ZFLevelZFFrameworkStatic) {
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFOutputWrapper).c_str());
    this->jclsZFOutputWrapper = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFOutputWrapper_jclsHolder) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFOutputWrapper);
}
public:
    jclass jclsZFOutputWrapper;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFOutputWrapper_jclsHolder)

jclass ZFImpl_sys_Android_jclassZFOutputWrapper(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFOutputWrapper_jclsHolder)->jclsZFOutputWrapper;
}
jobject ZFImpl_sys_Android_ZFOutputWrapperFromZFOutput(ZF_IN const ZFOutput &output) {
    if(!output) {
        return NULL;
    }

    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsZFOutputWrapper = ZFImpl_sys_Android_jclassZFOutputWrapper();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsZFOutputWrapper, "native_nativeOutputCreate",
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            .add(JNIPointerJNIType)
        ).c_str());
    v_ZFOutput *outputHolder = zfAlloc(v_ZFOutput); // release when nativeOutputClose
    outputHolder->zfv = output;
    return JNIUtilCallStaticObjectMethod(jniEnv, jclsZFOutputWrapper, jmId
        , JNIConvertZFObjectToJNIType(jniEnv, outputHolder)
        );
}

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper
        , jlong, native_1nativeOutputWrite
        , JNIPointer zfjniPointerOwnerZFOutput
        , jbyteArray buf
        , jlong offset
        , jlong size
        ) {
    v_ZFOutput *outputHolder = ZFCastZFObject(v_ZFOutput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput));
    ZFOutput output = outputHolder->zfv;

    if(buf == NULL) {
        zfindex ret = output.execute(zfnull, zfindexMax());
        if(ret == zfindexMax()) {
            return -1;
        }
        else {
            return (jlong)ret;
        }
    }

    jbyte *rawBuf = JNIUtilGetByteArrayElements(jniEnv, buf, NULL);
    zfindex written = output.execute(rawBuf + offset, size);
    JNIUtilReleaseByteArrayElements(jniEnv, buf, rawBuf, 0);
    return (jlong)written;
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper
        , void, native_1nativeOutputClose
        , JNIPointer zfjniPointerOwnerZFOutput
        ) {
    v_ZFOutput *outputHolder = ZFCastZFObject(v_ZFOutput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput));
    zfRelease(outputHolder);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper
        , jboolean, native_1nativeOutputSeek
        , JNIPointer zfjniPointerOwnerZFOutput
        , jlong size
        , jint pos
        ) {
    v_ZFOutput *outputHolder = ZFCastZFObject(v_ZFOutput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput));
    ZFOutput output = outputHolder->zfv;
    return (jboolean)output.ioSeek((zfindex)size, (ZFSeekPos)pos);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper
        , jlong, native_1nativeOutputTell
        , JNIPointer zfjniPointerOwnerZFOutput
        ) {
    v_ZFOutput *outputHolder = ZFCastZFObject(v_ZFOutput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput));
    ZFOutput output = outputHolder->zfv;
    zfindex pos = output.ioTell();
    if(pos == zfindexMax()) {
        return (jlong)-1;
    }
    else {
        return (jlong)pos;
    }
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper
        , jlong, native_1nativeOutputSize
        , JNIPointer zfjniPointerOwnerZFOutput
        ) {
    v_ZFOutput *outputHolder = ZFCastZFObject(v_ZFOutput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput));
    ZFOutput output = outputHolder->zfv;
    zfindex size = output.ioSize();
    if(size == zfindexMax()) {
        return (jlong)-1;
    }
    else {
        return (jlong)size;
    }
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

