#include "ZFProtocolZFApp.h"
#include "ZFProtocolZFMainEntry.h"
#include "ZFCore/ZFTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFApp)

void ZFPROTOCOL_INTERFACE_CLASS(ZFApp)::appExit(ZF_IN zfint appExitCode)
{
    ZFFrameworkCleanup();
    exit(appExitCode);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFApp)::appRestart(void)
{
    ZFLISTENER(action) {
        ZFCoreArray<zfstring> appParamsSaved;
        appParamsSaved.addFrom(ZFApp::appParams());
        ZFFrameworkCleanup();
        ZFFrameworkInit();
        ZFMainExecute(appParamsSaved);
    } ZFLISTENER_END()
    ZFTimerOnce(100, action);
}

ZF_NAMESPACE_GLOBAL_END

