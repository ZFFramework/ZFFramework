/**
 * @file ZFApp.h
 * @brief global app control util
 */

#ifndef _ZFI_ZFApp_h_
#define _ZFI_ZFApp_h_

#include "ZFMainEntry.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief called when #ZFApp::appExit
 *
 * param0 is a #v_zfint contains appExitCode\n
 * param1 is a #v_zfbool, set to true to override and ignore default impl
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, AppExit)
/**
 * @brief called when #ZFApp::appRestart
 *
 * param0 is a #v_zfbool, set to true to override and ignore default impl
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, AppRestart)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
ZF_NAMESPACE_BEGIN(ZFApp)

/**
 * @brief exit the app
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, appExit
        , ZFMP_IN_OPT(zfint, appExitCode, ZFApp::appExitCode())
        )
/**
 * @brief restart the app
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, void, appRestart)

ZF_NAMESPACE_END(ZFApp)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFApp_h_

