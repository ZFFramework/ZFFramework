#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFEnvInfo.h"

#if ZF_ENV_sys_iOS

#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_systemInfoImpl_sys_iOS, ZFEnvInfo_systemInfo, v_ZFProtocolLevel::e_SystemNormal)
public:
    virtual void systemName(ZF_IN_OUT zfstring &ret) {
        ZFImpl_sys_iOS_zfstringFromNSString(ret, [[UIDevice currentDevice] systemName]);
    }
    virtual void systemVersion(ZF_IN_OUT zfstring &ret) {
        ZFImpl_sys_iOS_zfstringFromNSString(ret, [[UIDevice currentDevice] systemVersion]);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_systemInfoImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

