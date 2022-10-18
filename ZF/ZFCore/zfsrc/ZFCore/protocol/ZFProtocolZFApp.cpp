#include "ZFProtocolZFApp.h"
#include "ZFProtocolZFMainEntry.h"
#include "ZFCore/ZFTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFApp)

void ZFPROTOCOL_INTERFACE_CLASS(ZFApp)::appExit(void)
{
    ZFFrameworkCleanup();
    exit(0);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFApp)::appRestart(void)
{
    ZFLISTENER(action) {
        ZFFrameworkCleanup();
        ZFFrameworkInit();
        ZFMainExecute();
    } ZFLISTENER_END(action)
    ZFTimerOnce(0, action);
}

ZF_NAMESPACE_GLOBAL_END

