/**
 * @file ZFMainEntry.h
 * @brief app's main entry wrapper
 */

#ifndef _ZFI_ZFMainEntry_h_
#define _ZFI_ZFMainEntry_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief app's main entry wrapper
 *
 * typical usage:
 * @code
 *   // in your main.cpp
 *   ZFMAIN_ENTRY() {
 *       // params hold the params from certain app routine,
 *       // it's not ensured each implementation would have same params format
 *       ZFCoreArray<zfstring> const &params = ZFApp::appParams();
 *
 *       // do your work,
 *       // which usually is creating your main window
 *
 *       // specify exit code if necessary
 *       ZFApp::appExitCode(-1);
 *   }
 * @endcode
 *
 * advanced:
 * -  #ZFMAIN_ENTRY was added to #ZFApp::E_AppParamDispatch as ZFLevelZFFrameworkPostNormal in fact,
 *   you may add your own impl to run custom code before or after #ZFMAIN_ENTRY,
 *   by specify different observer level,
 *   or ignore the default entry by #ZFArgs::eventFiltered
 */
#define ZFMAIN_ENTRY() \
    static void _ZFMain(void); \
    ZF_STATIC_REGISTER_INIT(ZFMainEntryRegister) { \
        ZFCoreAssertWithMessageTrim(_ZFP_ZFMainFunc() == zfnull, "ZFMAIN_ENTRY already registered"); \
        _ZFP_ZFMainFunc() = _ZFMain; \
    } \
    ZF_STATIC_REGISTER_END(ZFMainEntryRegister) \
    static void _ZFMain(void)

/**
 * @brief util macro to register #ZFApp::E_AppParamDispatch
 *
 * @code
 *   ZFMAIN_PARAM_DISPATCH(MyDispatch) {
 *       if(xxx) {
 *           zfargs.eventFiltered(zftrue);
 *           doSomething();
 *       }
 *   }
 * @endcode
 */
#define ZFMAIN_PARAM_DISPATCH(regSig) \
    static void _ZFP_MPD_##regSig(ZF_IN const ZFArgs &zfargs); \
    ZF_GLOBAL_INITIALIZER_INIT(MPD_##regSig) { \
        ZFGlobalObserver().observerAdd(ZFApp::E_AppParamDispatch(), ZFCallbackForFunc(_ZFP_MPD_##regSig)); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(MPD_##regSig) { \
        ZFGlobalObserver().observerRemove(ZFApp::E_AppParamDispatch(), ZFCallbackForFunc(_ZFP_MPD_##regSig)); \
    } \
    ZF_GLOBAL_INITIALIZER_END(MPD_##regSig) \
    static void _ZFP_MPD_##regSig(ZF_IN const ZFArgs &zfargs)

// ============================================================
ZF_NAMESPACE_BEGIN_REGISTER(ZFApp, ZF_NAMESPACE_GLOBAL)

/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFMainExecute, before #ZFMAIN_ENTRY and #ZFApp::E_AppEntry,
 * to let impl to supply custom #ZFApp::appParams dispatch
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, AppParamDispatch)

/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFMainExecute, after #ZFMAIN_ENTRY
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, AppEntry)

/**
 * @brief get the app params
 *
 * NOTE: the app params only avaialble after #ZFMAIN_ENTRY
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, const ZFCoreArray<zfstring> &, appParams)

/** @brief exit code for app, 0 by default */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, appExitCode
        , ZFMP_IN(zfint, exitCode)
        )
/** @brief exit code for app, 0 by default */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfint, appExitCode)

ZF_NAMESPACE_END(ZFApp)

// ============================================================
typedef void (*_ZFP_ZFMainFuncType)(void);
extern ZFLIB_ZFCore _ZFP_ZFMainFuncType &_ZFP_ZFMainFunc(void);
extern ZFLIB_ZFCore ZFCoreArray<zfstring> &_ZFP_ZFApp_appParams(void);
extern ZFLIB_ZFCore zfint &_ZFP_ZFApp_appExitCode(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMainEntry_h_

