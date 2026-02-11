#include "ZFImpl_sys_Android_ZFOutputWrapper.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOutputWrapper
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFOutputWrapper, ZFImpl_sys_Android_JNI_NAME_ZFOutputWrapper)

jobject ZFImpl_sys_Android_ZFOutputWrapperFromZFOutput(ZF_IN const ZFOutput &output) {
    if(!output) {
        return NULL;
    }

    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFOutputWrapper(), "native_nativeOutputCreate",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIPointerJNIType)
        ).c_str());
    v_ZFOutput *outputHolder = zfobjAlloc(v_ZFOutput); // release when nativeOutputClose
    outputHolder->zfv = output;
    return JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFOutputWrapper(), jmId
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
    if(buf == NULL) {
        return (jlong)0;
    }
    v_ZFOutput *outputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput);
    ZFOutput output = outputHolder->zfv;
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
    v_ZFOutput *outputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput);
    zfobjRelease(outputHolder);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper
        , jboolean, native_1nativeOutputSeek
        , JNIPointer zfjniPointerOwnerZFOutput
        , jlong size
        , jint seekPos
        ) {
    v_ZFOutput *outputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput);
    ZFOutput output = outputHolder->zfv;
    return (jboolean)output.ioSeek((zfindex)size, (ZFSeekPos)seekPos);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper
        , jlong, native_1nativeOutputTell
        , JNIPointer zfjniPointerOwnerZFOutput
        ) {
    v_ZFOutput *outputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput);
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
    v_ZFOutput *outputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFOutput);
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

