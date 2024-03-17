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

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidReflect_h_

#include "ZFImpl_sys_Android_ZFAndroidInput.h"
#include "ZFImpl_sys_Android_ZFAndroidOutput.h"
#include "ZFImpl_sys_Android_ZFInputWrapper.h"
#include "ZFImpl_sys_Android_ZFOutputWrapper.h"

