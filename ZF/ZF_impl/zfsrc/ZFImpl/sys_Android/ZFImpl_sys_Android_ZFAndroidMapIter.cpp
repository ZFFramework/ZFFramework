#include "ZFImpl_sys_Android_ZFAndroidMapIter.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidMapIter, ZFImpl_sys_Android_JNI_NAME_ZFAndroidMapIter)

zfclassNotPOD _ZFP_ZFImpl_sys_Android_ZFAndroidMapIter : zfextend zfiter::Impl {
public:
    JNIGlobalRef it;
public:
    zfoverride
    virtual zfbool valid(void) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterValid",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return it && (zfbool)JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
                , (jobject)it
                );
    }
    zfoverride
    virtual void next(void) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterNext",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
                , (jobject)it
                );
    }
    zfoverride
    virtual Impl *copy(void) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterCopy",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIGlobalRef itNew = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
                , (jobject)it
                );
        if(itNew) {
            _ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *ret = zfpoolNew(_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter);
            ret->it = itNew;
            return ret;
        }
        else {
            return zfnull;
        }
    }
    zfoverride
    virtual void destroy(void) {
        zfpoolDelete(this);
    }
    zfoverride
    virtual zfbool isEqual(ZF_IN Impl *d) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterIsEqual",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        _ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *t = (_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *)d;
        return (zfbool)JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
                , (jobject)it
                , (jobject)t->it
                );
    }
};

zfiter ZFImpl_sys_Android_MapIterFind(ZF_IN jobject nativeMap, ZF_IN jobject key) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterFind",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_object_Object())
        ).c_str());
    JNIGlobalRef it = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
            , nativeMap
            , key
            );
    if(it) {
        _ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *impl = zfpoolNew(_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter);
        impl->it = it;
        return zfiter(impl);
    }
    else {
        return zfnull;
    }
}
zfiter ZFImpl_sys_Android_MapIter(ZF_IN jobject nativeMap) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iter",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
        ).c_str());
    JNIGlobalRef it = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
            , nativeMap
            );
    if(it) {
        _ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *impl = zfpoolNew(_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter);
        impl->it = it;
        return zfiter(impl);
    }
    else {
        return zfnull;
    }
}
jobject ZFImpl_sys_Android_MapIterKey(ZF_IN const zfiter &it) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterKey",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
        ).c_str());
    return JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
            , (jobject)it.impl<_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *>()->it
            );
}
jobject ZFImpl_sys_Android_MapIterValue(ZF_IN const zfiter &it) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterValue",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
        ).c_str());
    return JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
            , (jobject)it.impl<_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *>()->it
            );
}
void ZFImpl_sys_Android_MapIterValue(ZF_IN_OUT zfiter &it, ZF_IN jobject value) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterValue",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_object_Object())
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
            , (jobject)it.impl<_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *>()->it
            , value
            );
}
jobject ZFImpl_sys_Android_MapIterRemove(ZF_IN_OUT zfiter &it) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), "native_iterRemove",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
        ).c_str());
    return JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidMapIter(), jmId
            , (jobject)it.impl<_ZFP_ZFImpl_sys_Android_ZFAndroidMapIter *>()->it
            );
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

