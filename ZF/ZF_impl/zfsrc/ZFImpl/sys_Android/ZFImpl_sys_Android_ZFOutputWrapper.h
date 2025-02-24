#ifndef _ZFI_ZFImpl_sys_Android_ZFOutputWrapper_h_
#define _ZFI_ZFImpl_sys_Android_ZFOutputWrapper_h_

#include "ZFImpl_sys_Android_def.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFOutputWrapper ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFOutputWrapper)
#define ZFImpl_sys_Android_JNI_NAME_ZFOutputWrapper ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFOutputWrapper)

extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassZFOutputWrapper(void);

// return a OutputStream, which would retain the source output
// you may explicitly call OutputStream.close() to release the source output,
// or let Java GC to handle it
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_ZFOutputWrapperFromZFOutput(ZF_IN const ZFOutput &output);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFOutputWrapper_h_

