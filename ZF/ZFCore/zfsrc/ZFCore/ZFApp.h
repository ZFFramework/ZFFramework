/**
 * @file ZFApp.h
 * @brief global app control util
 */

#ifndef _ZFI_ZFApp_h_
#define _ZFI_ZFApp_h_

#include "ZFMainEntry.h"

ZF_NAMESPACE_GLOBAL_BEGIN
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

