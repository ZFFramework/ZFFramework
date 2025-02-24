#ifndef _ZFI_ZFImpl_sys_Android_ZFInputWrapper_h_
#define _ZFI_ZFImpl_sys_Android_ZFInputWrapper_h_

#include "ZFImpl_sys_Android_def.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFInputWrapper ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFInputWrapper)
#define ZFImpl_sys_Android_JNI_NAME_ZFInputWrapper ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFInputWrapper)

extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassZFInputWrapper(void);

// return a InputStream, which would retain the source input
// you may explicitly call InputStream.close() to release the source input,
// or let Java GC to handle it
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_ZFInputWrapperFromZFInput(ZF_IN const ZFInput &input);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFInputWrapper_h_

