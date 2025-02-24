#ifndef _ZFI_ZFMainEntry_sys_iOS_h_
#define _ZFI_ZFMainEntry_sys_iOS_h_

#include "ZFCore.h"
#include "ZFImpl_sys_iOS_ZF_impl.h"

/*
 * main entry steps:
 * -# ensure your UIApplication.keyWindow has been setup properly
 *   during application:didFinishLaunchingWithOptions:
 */
#if ZF_ENV_sys_iOS

#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZFLIB_ZF_impl UIApplication *ZFImpl_sys_iOS_application(void);
extern ZFLIB_ZF_impl UIWindow *ZFImpl_sys_iOS_rootWindow(void);

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS
#endif // #ifndef _ZFI_ZFMainEntry_sys_iOS_h_

