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
void ZFPROTOCOL_INTERFACE_CLASS(ZFApp)::appRestart(ZF_IN zftimet delay)
{
    ZFLISTENER(action) {
        ZFFrameworkCleanup();
        ZFFrameworkInit();
        ZFMainExecute();
    } ZFLISTENER_END()
    ZFTimerOnce(delay, action);
}

ZF_NAMESPACE_GLOBAL_END

