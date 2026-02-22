#include "ZFImpl_sys_Android_ZFInputWrapper.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFAndroidInputWrapperBufSize 4096

// ============================================================
// ZFInputWrapper
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFInputWrapper, ZFImpl_sys_Android_JNI_NAME_ZFInputWrapper)
jobject ZFImpl_sys_Android_ZFInputWrapperFromZFInput(ZF_IN const ZFInput &input) {
    if(!input) {
        return NULL;
    }

    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFInputWrapper(), "native_nativeInputCreate",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIPointerJNIType)
            .add(JNIType::S_boolean())
        ).c_str());
    v_ZFInput *inputHolder = zfobjAlloc(v_ZFInput); // release when nativeInputClose
    inputHolder->zfv = input;
    return JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFInputWrapper(), jmId
        , JNIConvertZFObjectToJNIType(jniEnv, inputHolder)
        , (jboolean)(input.ioSize() != zfindexMax())
        );
}

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFInputWrapper
        , jlong, native_1nativeInputRead
        , JNIPointer zfjniPointerOwnerZFInput
        , jbyteArray buf
        , jlong offset
        , jlong size
        ) {
    v_ZFInput *inputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput);
    ZFInput input = inputHolder->zfv;

    jbyte rawBuf[_ZFP_ZFAndroidInputWrapperBufSize];
    zfindex total = 0;
    while(size > 0) {
        jlong toRead = size < _ZFP_ZFAndroidInputWrapperBufSize ? size : _ZFP_ZFAndroidInputWrapperBufSize;
        jlong read = (jlong)input.execute(rawBuf, toRead);
        if(read == 0) {
            break;
        }
        total += read;
        JNIUtilSetByteArrayRegion(jniEnv, buf, offset, read, rawBuf);
        offset += read;
        size -= read;
    }
    return (jlong)total;
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFInputWrapper
        , void, native_1nativeInputClose
        , JNIPointer zfjniPointerOwnerZFInput
        ) {
    v_ZFInput *inputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput);
    zfobjRelease(inputHolder);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFInputWrapper
        , jboolean, native_1nativeInputSeek
        , JNIPointer zfjniPointerOwnerZFInput
        , jlong size
        , jint seekPos
        ) {
    v_ZFInput *inputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput);
    ZFInput input = inputHolder->zfv;
    return (jboolean)input.ioSeek((zfindex)size, (ZFSeekPos)seekPos);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFInputWrapper
        , jlong, native_1nativeInputTell
        , JNIPointer zfjniPointerOwnerZFInput
        ) {
    v_ZFInput *inputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput);
    ZFInput input = inputHolder->zfv;
    zfindex pos = input.ioTell();
    if(pos == zfindexMax()) {
        return (jlong)-1;
    }
    else {
        return (jlong)pos;
    }
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFInputWrapper
        , jlong, native_1nativeInputSize
        , JNIPointer zfjniPointerOwnerZFInput
        ) {
    v_ZFInput *inputHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput);
    ZFInput input = inputHolder->zfv;
    zfindex size = input.ioSize();
    if(size == zfindexMax()) {
        return (jlong)-1;
    }
    else {
        return (jlong)size;
    }
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

