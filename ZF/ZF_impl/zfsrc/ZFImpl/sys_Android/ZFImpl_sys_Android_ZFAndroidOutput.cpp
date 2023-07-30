#include "ZFImpl_sys_Android_ZFAndroidOutput.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFAndroidOutput
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFAndroidOutput_jclsHolder, ZFLevelZFFrameworkStatic) {
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidOutput).c_str());
    this->jclsZFAndroidOutput = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFAndroidOutput_jclsHolder) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFAndroidOutput);
}
public:
    jclass jclsZFAndroidOutput;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFAndroidOutput_jclsHolder)

jclass ZFImpl_sys_Android_jclassZFAndroidOutput(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFAndroidOutput_jclsHolder)->jclsZFAndroidOutput;
}

#define _ZFP_ZFAndroidOutputBufSize 64
zfclass _ZFP_I_ZFAndroidOutput : zfextends ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAndroidOutput, ZFObject)
    ZFALLOC_CACHE_RELEASE({
            cache->removeAll();
        })

public:
    jobject nativeOutputWrapper;
    jbyteArray nativeBuf;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->nativeOutputWrapper = NULL;
        this->nativeBuf = NULL;
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        this->removeAll();
        zfsuper::objectOnDealloc();
    }

public:
    void removeAll(void) {
        if(this->nativeOutputWrapper != NULL) {
            JNIUtilDeleteGlobalRef(JNIGetJNIEnv(), this->nativeOutputWrapper);
            this->nativeOutputWrapper = zfnull;
        }
        if(this->nativeBuf != NULL) {
            JNIUtilDeleteGlobalRef(JNIGetJNIEnv(), this->nativeBuf);
            this->nativeBuf = NULL;
        }
    }

    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_OUT(const void *, buf)
            , ZFMP_IN_OPT(zfindex, count, zfindexMax())
            );
};
ZFOBJECT_REGISTER(_ZFP_I_ZFAndroidOutput)

ZFMETHOD_DEFINE_2(_ZFP_I_ZFAndroidOutput, zfindex, onOutput
        , ZFMP_OUT(const void *, buf)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    if(buf == zfnull) {
        return 0;
    }
    if(count == zfindexMax()) {
        count = zfslen((const zfchar *)buf);
    }

    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsZFAndroidOutput = ZFImpl_sys_Android_jclassZFAndroidOutput();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsZFAndroidOutput, "native_nativeOutputWrite",
        JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            .add(JNIType::S_array(JNIType::S_byte))
            .add(JNIType::S_int)
        ).c_str());

    const jbyte *src = (const jbyte *)buf;
    jint toWrite = (jint)count;
    while(toWrite > 0) {
        jint blockSize = toWrite > _ZFP_ZFAndroidOutputBufSize ? _ZFP_ZFAndroidOutputBufSize : toWrite;
        JNIUtilSetByteArrayRegion(jniEnv, this->nativeBuf, 0, blockSize, src);
        jint nativeWritten = JNIUtilCallStaticIntMethod(jniEnv, jclsZFAndroidOutput, jmId
            , this->nativeOutputWrapper
            , this->nativeBuf
            , blockSize
            );
        if(nativeWritten != blockSize) {
            break;
        }
        toWrite -= blockSize;
        src += blockSize;
    }
    return count - toWrite;
}

ZFOutput ZFImpl_sys_Android_ZFOutputFromZFAndroidOutput(ZF_IN jobject nativeOutputWrapper) {
    if(nativeOutputWrapper == NULL) {
        return zfnull;
    }
    JNIEnv *jniEnv = JNIGetJNIEnv();

    zfblockedAlloc(_ZFP_I_ZFAndroidOutput, owner);
    owner->nativeOutputWrapper = JNIUtilNewGlobalRef(jniEnv, nativeOutputWrapper);
    owner->nativeBuf = (jbyteArray)JNIUtilNewGlobalRef(jniEnv, JNILineDeleteLocalRefWithEnv(JNIUtilNewByteArray(jniEnv, _ZFP_ZFAndroidOutputBufSize), jniEnv));
    ZFOutput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFAndroidOutput, onOutput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

