#include "ZFProtocolZFMainEntry.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfint ZFMainExecute(ZF_IN const ZFCoreArray<zfstring> &appParams /* = ZFCoreArray<zfstring>() */)
{
    _ZFP_ZFApp_appParams().removeAll();
    _ZFP_ZFApp_appParams().addFrom(appParams);

    _ZFP_ZFMainFuncType const &func = _ZFP_ZFMainFunc();
    if(func != zfnull)
    {
        func();
        if(!ZFThread::implMainThreadTaskAvailable())
        {
            ZFFrameworkCleanup();
        }
    }
    else
    {
        zfCoreLog("ZFMAIN_ENTRY not set");
    }
    return _ZFP_ZFApp_appExitCode();
}

ZF_NAMESPACE_GLOBAL_END

