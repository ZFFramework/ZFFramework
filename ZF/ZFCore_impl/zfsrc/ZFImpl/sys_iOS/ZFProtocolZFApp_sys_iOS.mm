#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFApp.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAppImpl_sys_iOS, ZFApp, v_ZFProtocolLevel::e_SystemNormal)
public:
    virtual void appRestart(void) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)((float)100 / 1000 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            ZFCoreArray<zfstring> appParamsSaved;
            appParamsSaved.copyFrom(ZFApp::appParams());
            ZFFrameworkCleanup();
            ZFFrameworkInit();
            ZFMainExecute(appParamsSaved);
        });
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAppImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

