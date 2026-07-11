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
    ZFArgs zfargs;
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppExit(), zfargs.param0(zfobj<v_zfint>(appExitCode)));
    v_zfbool *bypass = zfargs.result();
    if(bypass && bypass->zfv) {
        return;
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFApp_appExit) *impl = ZFPROTOCOL_TRY_ACCESS(ZFApp_appExit);
    if(impl != zfnull) {
        impl->appExit(appExitCode);
    }
    else {
        ZFFrameworkCleanup();
        exit(0);
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, appRestart) {
    ZFArgs zfargs;
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppRestart(), zfargs);
    v_zfbool *bypass = zfargs.result();
    if(bypass && bypass->zfv) {
        return;
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFApp_appRestart) *impl = ZFPROTOCOL_TRY_ACCESS(ZFApp_appRestart);
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

