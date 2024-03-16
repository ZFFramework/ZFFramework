#ifndef _ZFI_ZFMainEntry_sys_Android_h_
#define _ZFI_ZFMainEntry_sys_Android_h_

#include "ZFCore.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

/*
 * main entry steps:
 * -# prepare your loader Activity
 * -# load all so files in your Activity as static init step
 *   @code
 *     public class Loader extends Activity {
 *         static {
 *             System.loadLibrary("ZF_lib_name");
 *         }
 *     }
 *   @endcode
 * in your loader activity, use ZFMainEntry.java to setup
 */
#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// return android.app.Application
extern ZFLIB_ZF_impl JNIGlobalRef ZFImpl_sys_Android_app(void);
// return android.content.Context
extern ZFLIB_ZF_impl JNIGlobalRef ZFImpl_sys_Android_appContext(void);
// return android.content.res.AssetManager
extern ZFLIB_ZF_impl JNIGlobalRef ZFImpl_sys_Android_assetManager(void);
// return android.app.Activity, may be null if embeded to native UI
extern ZFLIB_ZF_impl JNIGlobalRef ZFImpl_sys_Android_mainEntryActivity(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFMainEntry_sys_Android_h_

