#include "ZFEnvInfo_ZFUIKit.h"
#include "protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFEnvDeviceUIType)

ZF_NAMESPACE_BEGIN(ZFEnvInfo)

ZFMETHOD_FUNC_DEFINE_1(ZFEnvDeviceUIType, deviceUIType
        , ZFMP_IN_OPT(ZFEnvDeviceUIType, defaultValue, v_ZFEnvDeviceUIType::e_Desktop)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_deviceUIInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_deviceUIInfo);
    if(impl != zfnull) {
        return impl->deviceUIType();
    }
    else {
        return defaultValue;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(zfstring, localeId) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_localeInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_localeInfo);
    if(impl != zfnull) {
        return impl->localeId();
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, localeLangId) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_localeInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_localeInfo);
    if(impl != zfnull) {
        return impl->localeLangId();
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_END(ZFEnvInfo)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummary_ZFUIKit, ZFLevelZFFrameworkNormal) {
    ZFEnvInfo::envSummaryCallbackRegister("deviceUIType", zfself::deviceUITypeSummary);
    ZFEnvInfo::envSummaryCallbackRegister("localeId", ZFEnvInfo::localeId);
    ZFEnvInfo::envSummaryCallbackRegister("localeLangId", ZFEnvInfo::localeLangId);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFEnvSummary_ZFUIKit) {
    ZFEnvInfo::envSummaryCallbackUnregister("deviceUIType");
    ZFEnvInfo::envSummaryCallbackUnregister("localeId");
    ZFEnvInfo::envSummaryCallbackUnregister("localeLangId");
}
public:
    static zfstring deviceUITypeSummary(void) {
        return v_ZFEnvDeviceUIType::EnumNameForValue(ZFEnvInfo::deviceUIType());
    }
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummary_ZFUIKit)

ZF_NAMESPACE_GLOBAL_END

