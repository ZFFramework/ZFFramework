#include "ZFImpl_default_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

#include "ZFImpl/ZFImpl_env.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_deviceUIInfoImpl_default, ZFEnvInfo_deviceUIInfo, v_ZFProtocolLevel::e_Default)
public:
    virtual ZFEnvDeviceUIType deviceUIType(ZF_IN_OPT ZFEnvDeviceUIType defaultValue = v_ZFEnvDeviceUIType::e_Desktop) {
        #if ZF_ENV_sys_Qt
            return v_ZFEnvDeviceUIType::e_Desktop;
        #else // #if ZF_ENV_sys_Qt
            return v_ZFEnvDeviceUIType::e_Handheld;
        #endif // #if ZF_ENV_sys_Qt #else
        return defaultValue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_deviceUIInfoImpl_default)

ZF_NAMESPACE_GLOBAL_END

