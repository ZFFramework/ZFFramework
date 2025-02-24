#include "ZFApp.h"
#include "protocol/ZFProtocolZFApp.h"
#include "protocol/ZFProtocolZFMainEntry.h"
#include "ZFTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(AppExit)
ZFEVENT_GLOBAL_REGISTER(AppRestart)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
ZF_NAMESPACE_BEGIN(ZFApp)

ZFMETHOD_FUNC_DEFINE_1(void, appExit
        , ZFMP_IN_OPT(zfint, appExitCode, ZFApp::appExitCode())
        ) {
    zfobj<v_zfbool> flag;
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppExit(), zfobj<v_zfint>(appExitCode), flag);
    if(flag->zfv) {
        return;
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFApp) *impl = ZFPROTOCOL_TRY_ACCESS(ZFApp);
    if(impl != zfnull) {
        impl->appExit(appExitCode);
    }
    else {
        ZFFrameworkCleanup();
        exit(0);
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, appRestart) {
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppRestart());
    ZFPROTOCOL_INTERFACE_CLASS(ZFApp) *impl = ZFPROTOCOL_TRY_ACCESS(ZFApp);
    if(impl != zfnull) {
        impl->appRestart();
    }
    else {
        ZFLISTENER(action) {
            ZFCoreArray<zfstring> appParamsSaved;
            appParamsSaved.addFrom(ZFApp::appParams());
            ZFFrameworkCleanup();
            ZFFrameworkInit();
            ZFMainExecute(appParamsSaved);
        } ZFLISTENER_END()
        ZFTimerOnce(100, action);
    }
}

ZF_NAMESPACE_END(ZFApp)
ZF_NAMESPACE_GLOBAL_END

