#include "ZFImpl_sys_Android_ZFAndroidInput.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFAndroidInput
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFAndroidInput_jclsHolder, ZFLevelZFFrameworkStatic) {
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidInput).c_str());
    this->jclsZFAndroidInput = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFAndroidInput_jclsHolder) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFAndroidInput);
}
public:
    jclass jclsZFAndroidInput;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFAndroidInput_jclsHolder)

jclass ZFImpl_sys_Android_jclassZFAndroidInput(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFAndroidInput_jclsHolder)->jclsZFAndroidInput;
}

#define _ZFP_ZFAndroidInputBufSize 1024
zfclass _ZFP_I_ZFAndroidInput : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAndroidInput, ZFObject)
    ZFALLOC_CACHE_RELEASE({
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
        if(this->nativeInputWrapper != NULL) {
            JNIUtilDeleteGlobalRef(JNIGetJNIEnv(), this->nativeInputWrapper);
            this->nativeInputWrapper = zfnull;
        }
        if(this->nativeBuf != NULL) {
            JNIUtilDeleteGlobalRef(JNIGetJNIEnv(), this->nativeBuf);
            this->nativeBuf = NULL;
        }
    }

    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_OUT(void *, buf)
            , ZFMP_IN(zfindex, count)
            );
};
ZFOBJECT_REGISTER(_ZFP_I_ZFAndroidInput)

ZFMETHOD_DEFINE_2(_ZFP_I_ZFAndroidInput, zfindex, onInput
        , ZFMP_OUT(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(buf == zfnull) {
        return zfindexMax(); // not supported
    }
    if(count == zfindexMax()) {
        return 0;
    }

    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsZFAndroidInput = ZFImpl_sys_Android_jclassZFAndroidInput();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsZFAndroidInput, "native_nativeInputRead",
        JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            .add(JNIType::S_array(JNIType::S_byte))
            .add(JNIType::S_int)
        ).c_str());

    jbyte *writeBuf = (jbyte *)buf;
    jint toRead = (jint)count;
    while(toRead > 0) {
        jint blockSize = (toRead > _ZFP_ZFAndroidInputBufSize ? _ZFP_ZFAndroidInputBufSize : toRead);
        jint nativeRead = JNIUtilCallStaticIntMethod(jniEnv, jclsZFAndroidInput, jmId
            , this->nativeInputWrapper
            , this->nativeBuf
            , blockSize
            );
        if(nativeRead == -1) {
            break;
        }
        JNIUtilGetByteArrayRegion(jniEnv, this->nativeBuf, 0, nativeRead, writeBuf);
        toRead -= blockSize;
        writeBuf += blockSize;
        if(nativeRead != blockSize) {
            break;
        }
    }
    return count - toRead;
}

ZFInput ZFImpl_sys_Android_ZFInputFromZFAndroidInput(ZF_IN jobject nativeInputWrapper) {
    if(nativeInputWrapper == NULL) {
        return zfnull;
    }
    JNIEnv *jniEnv = JNIGetJNIEnv();

    zfblockedAlloc(_ZFP_I_ZFAndroidInput, owner);
    owner->nativeInputWrapper = JNIUtilNewGlobalRef(jniEnv, nativeInputWrapper);
    owner->nativeBuf = (jbyteArray)JNIUtilNewGlobalRef(jniEnv, JNILineDeleteLocalRef(JNIUtilNewByteArray(jniEnv, _ZFP_ZFAndroidInputBufSize)));
    ZFInput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFAndroidInput, onInput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

