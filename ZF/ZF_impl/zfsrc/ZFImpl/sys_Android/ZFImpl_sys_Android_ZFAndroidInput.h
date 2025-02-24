#ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidInput_h_
#define _ZFI_ZFImpl_sys_Android_ZFAndroidInput_h_

#include "ZFImpl_sys_Android_def.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidInput ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidInput)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidInput ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidInput)

extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassZFAndroidInput(void);

// return a ZFInput from ZFAndroidInput,
// the source ZFAndroidInput would be stored as global ref until the result ZFInput destroyed
extern ZFLIB_ZF_impl ZFInput ZFImpl_sys_Android_ZFInputFromZFAndroidInput(ZF_IN jobject nativeInputWrapper);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidInput_h_

