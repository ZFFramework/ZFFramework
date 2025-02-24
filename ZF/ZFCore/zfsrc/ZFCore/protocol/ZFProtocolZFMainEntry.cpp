#include "ZFProtocolZFMainEntry.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfint ZFMainExecute(ZF_IN const ZFCoreArray<zfstring> &appParams /* = ZFCoreArray<zfstring>() */) {
    _ZFP_ZFApp_appParams().removeAll();
    _ZFP_ZFApp_appParams().addFrom(appParams);
    ZFGlobalObserver().observerNotify(ZFApp::E_AppParamDispatch());
    ZFGlobalObserver().observerNotify(ZFApp::E_AppEntry());
    return _ZFP_ZFApp_appExitCode();
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMainEntrySetup, ZFLevelZFFrameworkStatic) {
    ZFLISTENER(action) {
        _ZFP_ZFMainFuncType const &func = _ZFP_ZFMainFunc();
        if(func != zfnull) {
            func();
        }
        else {
            ZFCoreLogTrim("ZFMAIN_ENTRY not set");
        }
    } ZFLISTENER_END()
    this->callback = action;
    ZFGlobalObserver().observerAdd(
            ZFApp::E_AppParamDispatch(),
            this->callback,
            ZFLevelZFFrameworkPostNormal
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMainEntrySetup) {
    ZFGlobalObserver().observerRemove(ZFApp::E_AppParamDispatch(), this->callback);
}
private:
    ZFListener callback;
ZF_GLOBAL_INITIALIZER_END(ZFMainEntrySetup)

ZF_NAMESPACE_GLOBAL_END

