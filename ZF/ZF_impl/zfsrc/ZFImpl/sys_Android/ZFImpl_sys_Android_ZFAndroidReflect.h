#ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidReflect_h_
#define _ZFI_ZFImpl_sys_Android_ZFAndroidReflect_h_

#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief util for ZFAndroidReflect.java */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZF_impl, const ZFClass *, ZFAndroidReflect_classForNameInJava
        , ZFMP_IN(const zfchar *, clsNameInJava)
        )

/** @brief util for ZFAndroidReflect.java */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZF_impl, const ZFClass *, ZFAndroidReflect_registerClass
        , ZFMP_IN(const zfchar *, clsNameInJava)
        , ZFMP_IN_OPT(const zfchar *, clsNameInZF, zfnull)
        )
/** @brief util for ZFAndroidReflect.java */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZF_impl, void, ZFAndroidReflect_unregisterClass
        , ZFMP_IN(const zfchar *, clsNameInJava)
        )

/** @brief util for ZFAndroidReflect.java */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZF_impl, void, ZFAndroidReflect_registerClassContents
        , ZFMP_IN(const zfchar *, clsNameInJava)
        )

// ============================================================
/** @brief util for ZFAndroidReflect.java */
#define ZFAndroidReflect_ToJNIType_DEFINE(regSig) \
    static zfbool _ZFP_ZFAndroidReflect_ToJNIType_##regSig( \
            ZF_OUT jobject &jobj \
            , ZF_IN zfany const &obj \
            , ZF_IN const JNIType &jniType \
            ); \
    ZF_STATIC_REGISTER_INIT(_ZFP_ZFAndroidReflect_ToJNIType_##regSig) { \
        _ZFP_ZFAndroidReflect_ToJNITypeRegister(_ZFP_ZFAndroidReflect_ToJNIType_##regSig); \
    } \
    ZF_STATIC_REGISTER_DESTROY(_ZFP_ZFAndroidReflect_ToJNIType_##regSig) { \
        _ZFP_ZFAndroidReflect_ToJNITypeUnregister(_ZFP_ZFAndroidReflect_ToJNIType_##regSig); \
    } \
    ZF_STATIC_REGISTER_END(_ZFP_ZFAndroidReflect_ToJNIType_##regSig) \
    static zfbool _ZFP_ZFAndroidReflect_ToJNIType_##regSig( \
            ZF_OUT jobject &jobj \
            , ZF_IN zfany const &obj \
            , ZF_IN const JNIType &jniType \
            )

/** @brief util for ZFAndroidReflect.java */
#define ZFAndroidReflect_FromJNIType_DEFINE(regSig) \
    static zfbool _ZFP_ZFAndroidReflect_FromJNIType_##regSig( \
            ZF_OUT zfauto &obj \
            , ZF_IN jobject jobj \
            , ZF_IN const JNIType &jniType \
            ); \
    ZF_STATIC_REGISTER_INIT(_ZFP_ZFAndroidReflect_FromJNIType_##regSig) { \
        _ZFP_ZFAndroidReflect_FromJNITypeRegister(_ZFP_ZFAndroidReflect_FromJNIType_##regSig); \
    } \
    ZF_STATIC_REGISTER_DESTROY(_ZFP_ZFAndroidReflect_FromJNIType_##regSig) { \
        _ZFP_ZFAndroidReflect_FromJNITypeUnregister(_ZFP_ZFAndroidReflect_FromJNIType_##regSig); \
    } \
    ZF_STATIC_REGISTER_END(_ZFP_ZFAndroidReflect_FromJNIType_##regSig) \
    static zfbool _ZFP_ZFAndroidReflect_FromJNIType_##regSig( \
            ZF_OUT zfauto &obj \
            , ZF_IN jobject jobj \
            , ZF_IN const JNIType &jniType \
            )

typedef zfbool (*_ZFP_ZFAndroidReflect_ToJNITypeCallback)(
        ZF_OUT jobject &jobj
        , ZF_IN zfany const &obj
        , ZF_IN const JNIType &jniType
        );
typedef zfbool (*_ZFP_ZFAndroidReflect_FromJNITypeCallback)(
        ZF_OUT zfauto &obj
        , ZF_IN jobject jobj
        , ZF_IN const JNIType &jniType
        );
extern ZFLIB_ZF_impl void _ZFP_ZFAndroidReflect_ToJNITypeRegister(
        ZF_IN _ZFP_ZFAndroidReflect_ToJNITypeCallback callback
        );
extern ZFLIB_ZF_impl void _ZFP_ZFAndroidReflect_ToJNITypeUnregister(
        ZF_IN _ZFP_ZFAndroidReflect_ToJNITypeCallback callback
        );
extern ZFLIB_ZF_impl void _ZFP_ZFAndroidReflect_FromJNITypeRegister(
        ZF_IN _ZFP_ZFAndroidReflect_FromJNITypeCallback callback
        );
extern ZFLIB_ZF_impl void _ZFP_ZFAndroidReflect_FromJNITypeUnregister(
        ZF_IN _ZFP_ZFAndroidReflect_FromJNITypeCallback callback
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidReflect_h_

#include "ZFImpl_sys_Android_ZFAndroidInput.h"
#include "ZFImpl_sys_Android_ZFAndroidOutput.h"
#include "ZFImpl_sys_Android_ZFInputWrapper.h"
#include "ZFImpl_sys_Android_ZFOutputWrapper.h"

