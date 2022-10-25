#include "ZFApp.h"
#include "protocol/ZFProtocolZFApp.h"
#include "protocol/ZFProtocolZFMainEntry.h"
#include "ZFTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFApp)

ZFMETHOD_FUNC_DEFINE_0(void, appExit)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFApp) *impl = ZFPROTOCOL_TRY_ACCESS(ZFApp);
    if(impl != zfnull)
    {
        impl->appExit();
    }
    else
    {
        ZFFrameworkCleanup();
        exit(0);
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, appRestart)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFApp) *impl = ZFPROTOCOL_TRY_ACCESS(ZFApp);
    if(impl != zfnull)
    {
        impl->appRestart(1000);
    }
    else
    {
        ZFLISTENER(action) {
            ZFFrameworkCleanup();
            ZFFrameworkInit();
            ZFMainExecute();
        } ZFLISTENER_END(action)
        ZFTimerOnce(0, action);
    }
}

ZF_NAMESPACE_END_WITH_REGISTER(ZFApp, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_GLOBAL_END

