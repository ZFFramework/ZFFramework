#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFEnvInfo.h"

#if ZF_ENV_sys_Android

#include <sys/system_properties.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_systemInfoImpl_sys_Android, ZFEnvInfo_systemInfo, v_ZFProtocolLevel::e_SystemNormal)
public:
    virtual zfstring systemName(void) {
        return zftext("Android");
    }
    virtual zfstring systemVersion(void) {
        char value[PROP_VALUE_MAX];
        int success = __system_property_get("ro.build.version.sdk", value);
        if (success <= 0) {
            return zfnull;
        }
        return value;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_systemInfoImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android

