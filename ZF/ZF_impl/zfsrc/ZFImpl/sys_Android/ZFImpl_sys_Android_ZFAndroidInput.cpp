#include "ZFImpl_sys_Android_ZFAndroidInput.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFAndroidInput
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidInput, ZFImpl_sys_Android_JNI_NAME_ZFAndroidInput)

#define _ZFP_ZFAndroidInputBufSize 4096
zfclass _ZFP_I_ZFAndroidInput : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAndroidInput, ZFObject)
    ZFOBJECT_CACHE_RELEASE({
            cache->removeAll();
        })

public:
    jobject nativeInputWrapper;
    jbyteArray nativeBuf;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->nativeInputWrapper = NULL;
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
        if(this->nativeInputWrapper != NULL) {
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidInput(), "native_nativeInputClose",
                    JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                        .add(JNIType::S_object_Object())
                        ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidInput(), jmId, this->nativeInputWrapper);

            JNIUtilDeleteGlobalRef(jniEnv, this->nativeInputWrapper);
            this->nativeInputWrapper = NULL;
        }
        if(this->nativeBuf != NULL) {
            JNIUtilDeleteGlobalRef(jniEnv, this->nativeBuf);
            this->nativeBuf = NULL;
        }
    }

    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_OUT(void *, buf)
            , ZFMP_IN(zfindex, count)
            )
};
ZFOBJECT_REGISTER(_ZFP_I_ZFAndroidInput)

ZFMETHOD_DEFINE_2(_ZFP_I_ZFAndroidInput, zfindex, onInput
        , ZFMP_OUT(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(buf == zfnull) {
        return 0;
    }

    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidInput(), "native_nativeInputRead",
        JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_array(JNIType::S_byte()))
            .add(JNIType::S_int())
        ).c_str());

    jbyte *writeBuf = (jbyte *)buf;
    zfindex read = 0;
    while(count != 0) {
        jint blockSize = (count > _ZFP_ZFAndroidInputBufSize ? _ZFP_ZFAndroidInputBufSize : (jint)count);
        jint nativeRead = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidInput(), jmId
            , this->nativeInputWrapper
            , this->nativeBuf
            , blockSize
            );
        if(nativeRead == -1) {
            return 0;
        }
        JNIUtilGetByteArrayRegion(jniEnv, this->nativeBuf, 0, nativeRead, writeBuf);
        count -= (zfindex)nativeRead;
        read += (zfindex)nativeRead;
        writeBuf += nativeRead;
        if(nativeRead != blockSize) {
            break;
        }
    }
    return read;
}

ZFInput ZFImpl_sys_Android_ZFInputFromZFAndroidInput(ZF_IN jobject nativeInputWrapper) {
    if(nativeInputWrapper == NULL) {
        return zfnull;
    }
    JNIEnv *jniEnv = JNIGetJNIEnv();

    zfobj<_ZFP_I_ZFAndroidInput> owner;
    owner->nativeInputWrapper = JNIUtilNewGlobalRef(jniEnv, nativeInputWrapper);
    owner->nativeBuf = (jbyteArray)JNIUtilNewGlobalRef(jniEnv, JNILineDeleteLocalRef(JNIUtilNewByteArray(jniEnv, _ZFP_ZFAndroidInputBufSize)));
    ZFInput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFAndroidInput, onInput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

