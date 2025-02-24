#ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidMapIter_h_
#define _ZFI_ZFImpl_sys_Android_ZFAndroidMapIter_h_

#include "ZFImpl_sys_Android_def.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidMapIter ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidMapIter)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidMapIter ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidMapIter)

extern ZFLIB_ZF_impl zfiter ZFImpl_sys_Android_MapIterFind(ZF_IN jobject nativeMap, ZF_IN jobject key);
extern ZFLIB_ZF_impl zfiter ZFImpl_sys_Android_MapIter(ZF_IN jobject nativeMap);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_MapIterKey(ZF_IN const zfiter &it);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_MapIterValue(ZF_IN const zfiter &it);
extern ZFLIB_ZF_impl void ZFImpl_sys_Android_MapIterValue(ZF_IN_OUT zfiter &it, ZF_IN jobject value);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_MapIterRemove(ZF_IN_OUT zfiter &it);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidMapIter_h_

