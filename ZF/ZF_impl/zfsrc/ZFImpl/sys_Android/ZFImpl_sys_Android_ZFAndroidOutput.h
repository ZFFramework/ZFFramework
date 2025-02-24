#ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidOutput_h_
#define _ZFI_ZFImpl_sys_Android_ZFAndroidOutput_h_

#include "ZFImpl_sys_Android_def.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidOutput ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidOutput)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidOutput ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidOutput)

extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassZFAndroidOutput(void);

// return a ZFOutput from ZFAndroidOutput,
// the source ZFAndroidOutput would be stored as global ref until the result ZFOutput destroyed
extern ZFLIB_ZF_impl ZFOutput ZFImpl_sys_Android_ZFOutputFromZFAndroidOutput(ZF_IN jobject nativeOutputWrapper);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidOutput_h_

