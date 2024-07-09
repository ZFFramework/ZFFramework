#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFApp.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#if ZF_ENV_sys_Qt

#include <QTimer>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAppImpl_sys_Qt, ZFApp, ZFProtocolLevel::e_SystemHigh)
public:
    virtual void appRestart(void) {
        QTimer::singleShot(100, _ZFP_appRestart);
    }
    static void _ZFP_appRestart(void) {
        ZFCoreArray<zfstring> appParamsSaved;
        appParamsSaved.copyFrom(ZFApp::appParams());
        ZFFrameworkCleanup();
        ZFFrameworkInit();
        ZFMainExecute(appParamsSaved);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAppImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt

