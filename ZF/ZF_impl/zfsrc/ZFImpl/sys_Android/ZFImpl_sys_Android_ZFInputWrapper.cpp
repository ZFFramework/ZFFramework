#include "ZFImpl_sys_Android_ZFInputWrapper.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFInputWrapper
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFInputWrapper_jclsHolder, ZFLevelZFFrameworkStatic) {
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFInputWrapper).c_str());
    this->jclsZFInputWrapper = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFInputWrapper_jclsHolder) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFInputWrapper);
}
public:
    jclass jclsZFInputWrapper;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFInputWrapper_jclsHolder)

jclass ZFImpl_sys_Android_jclassZFInputWrapper(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFInputWrapper_jclsHolder)->jclsZFInputWrapper;
}
jobject ZFImpl_sys_Android_ZFInputWrapperFromZFInput(ZF_IN const ZFInput &input) {
    if(!input) {
        return NULL;
    }

    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsZFInputWrapper = ZFImpl_sys_Android_jclassZFInputWrapper();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsZFInputWrapper, "native_nativeInputCreate",
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            .add(JNIPointerJNIType)
            .add(JNIType::S_boolean)
        ).c_str());
    v_ZFInput *inputHolder = zfAlloc(v_ZFInput); // release when nativeInputClose
    inputHolder->zfv = input;
    return JNIUtilCallStaticObjectMethod(jniEnv, jclsZFInputWrapper, jmId
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
    v_ZFInput *inputHolder = ZFCastZFObject(v_ZFInput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput));
    ZFInput input = inputHolder->zfv;

    jbyte rawBuf[64];
    zfindex total = 0;
    while(size > 0) {
        jlong toRead = size < 64 ? size : 64;
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
    v_ZFInput *inputHolder = ZFCastZFObject(v_ZFInput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput));
    zfRelease(inputHolder);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFInputWrapper
        , jboolean, native_1nativeInputSeek
        , JNIPointer zfjniPointerOwnerZFInput
        , jlong size
        , jint pos
        ) {
    v_ZFInput *inputHolder = ZFCastZFObject(v_ZFInput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput));
    ZFInput input = inputHolder->zfv;
    return (jboolean)input.ioSeek((zfindex)size, (ZFSeekPos)pos);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFInputWrapper
        , jlong, native_1nativeInputTell
        , JNIPointer zfjniPointerOwnerZFInput
        ) {
    v_ZFInput *inputHolder = ZFCastZFObject(v_ZFInput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput));
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
    v_ZFInput *inputHolder = ZFCastZFObject(v_ZFInput *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFInput));
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

