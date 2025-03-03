#include "ZFImpl_sys_Android_ZFAndroidOutput.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFAndroidOutput
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidOutput, ZFImpl_sys_Android_JNI_NAME_ZFAndroidOutput)

#define _ZFP_ZFAndroidOutputBufSize 1024
zfclass _ZFP_I_ZFAndroidOutput : zfextend ZFObject {
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
        JNIEnv *jniEnv = JNIGetJNIEnv();
        if(this->nativeOutputWrapper != NULL) {
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidOutput(), "native_nativeOutputClose",
                    JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                        .add(JNIType::S_object_Object())
                        ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidOutput(), jmId, this->nativeOutputWrapper);

            JNIUtilDeleteGlobalRef(jniEnv, this->nativeOutputWrapper);
            this->nativeOutputWrapper = NULL;
        }
        if(this->nativeBuf != NULL) {
            JNIUtilDeleteGlobalRef(jniEnv, this->nativeBuf);
            this->nativeBuf = NULL;
        }
    }

    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_OUT(const void *, buf)
            , ZFMP_IN_OPT(zfindex, count, zfindexMax())
            )
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
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidOutput(), "native_nativeOutputWrite",
        JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_array(JNIType::S_byte()))
            .add(JNIType::S_int())
        ).c_str());

    const jbyte *src = (const jbyte *)buf;
    jint toWrite = (jint)count;
    while(toWrite > 0) {
        jint blockSize = toWrite > _ZFP_ZFAndroidOutputBufSize ? _ZFP_ZFAndroidOutputBufSize : toWrite;
        JNIUtilSetByteArrayRegion(jniEnv, this->nativeBuf, 0, blockSize, src);
        jint nativeWritten = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidOutput(), jmId
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

    zfobj<_ZFP_I_ZFAndroidOutput> owner;
    owner->nativeOutputWrapper = JNIUtilNewGlobalRef(jniEnv, nativeOutputWrapper);
    owner->nativeBuf = (jbyteArray)JNIUtilNewGlobalRef(jniEnv, JNILineDeleteLocalRef(JNIUtilNewByteArray(jniEnv, _ZFP_ZFAndroidOutputBufSize)));
    ZFOutput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFAndroidOutput, onOutput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

