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
    }
    else
    {
        zfCoreLog("ZFMAIN_ENTRY not set");
    }
    for(zfindex i = 0; i < ZFMainExtraImpl().count(); ++i)
    {
        ZFMainExtraImpl()[i]();
    }
    return _ZFP_ZFApp_appExitCode();
}

ZFCoreArrayPOD<ZFFuncAddrType> &ZFMainExtraImpl(void)
{
    static ZFCoreArrayPOD<ZFFuncAddrType> d;
    return d;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMainExtraImplCleanup, ZFLevelZFFrameworkStatic)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMainExtraImplCleanup)
{
    ZFMainExtraImpl().removeAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFMainExtraImplCleanup)

ZF_NAMESPACE_GLOBAL_END

