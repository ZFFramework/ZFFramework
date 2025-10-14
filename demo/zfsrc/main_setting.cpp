#include "ZFCore.h"
#include "ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if ZF_ENV_DEBUG // global debug level
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(main_setting_debugLevel, ZFLevelAppEssential) {
        ZFLogLevelDefault(v_ZFLogLevel::e_Verbose);
    }
    ZF_GLOBAL_INITIALIZER_END(main_setting_debugLevel)
#endif

#if ZF_ENV_DEBUG // auto print view tree when app paused
    ZF_GLOBAL_INITIALIZER_INIT(main_setting_autoPrintViewTree) {
        if(!ZFProtocolIsAvailable("ZFUIView")) {
            return;
        }
        ZFLISTENER(windowOnPause) {
            ZFUISysWindow *sysWindow = zfargs.sender();
            ZFUIViewTreePrint(sysWindow->rootView());
        } ZFLISTENER_END()
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(main_setting_autoPrintViewTree) {
        ZFGlobalObserver().observerRemove(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(main_setting_autoPrintViewTree)
#endif

#if ZF_ENV_DEBUG // redirect to lua if start param specified
    ZFMAIN_PARAM_DISPATCH(LuaRunner) {
        if(zffalse
                || !ZFProtocolIsAvailable("ZFLua")
                || ZFApp::appParams().isEmpty()
                || !ZFRegExpMatch(ZFApp::appParams()[0], ".*\\.lua$")
                ) {
            return;
        }
        const ZFCoreArray<zfstring> &appParams = ZFApp::appParams();
        ZFPathInfo pathInfo;
        if(!ZFPathInfoFromStringT(pathInfo, appParams[0])) {
            pathInfo.pathType(ZFPathType_file());
            pathInfo.pathData(appParams[0]);
        }
        ZFCoreArray<zfauto> params;
        for(zfindex i = 1; i < appParams.count(); ++i) {
            params.add(zfobj<v_zfstring>(appParams[i]));
        }
        zfauto ret;
        if(ZFInvokeT(ret, zfnull, "ZFLuaExecuteDetail", zfobj<v_ZFCallback>(ZFInputForPathInfo(pathInfo)), zfobj<v_ZFCoreArray>(params))) {
            zfargs.eventFiltered(zftrue);
        }
    }
#endif

ZF_NAMESPACE_GLOBAL_END

