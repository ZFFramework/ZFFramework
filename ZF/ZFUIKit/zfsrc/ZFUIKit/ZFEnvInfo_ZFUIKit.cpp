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
ZFMETHOD_FUNC_DEFINE_1(void, localeInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_localeInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_localeInfo);
    if(impl != zfnull) {
        impl->localeInfo(ret);
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, localeInfo) {
    zfstring ret;
    ZFEnvInfo::localeInfoT(ret);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_1(void, localeLangInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_localeLangInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_localeLangInfo);
    if(impl != zfnull) {
        impl->localeLangInfo(ret);
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, localeLangInfo) {
    zfstring ret;
    ZFEnvInfo::localeLangInfoT(ret);
    return ret;
}

ZF_NAMESPACE_END(ZFEnvInfo)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummary_ZFUIKit, ZFLevelZFFrameworkNormal) {
    ZFEnvInfo::envSummaryCallbackRegister("deviceUIType", zfself::deviceUITypeSummary);
    ZFEnvInfo::envSummaryCallbackRegister("localeInfo", ZFEnvInfo::localeInfoT);
    ZFEnvInfo::envSummaryCallbackRegister("localeLangInfo", ZFEnvInfo::localeLangInfoT);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFEnvSummary_ZFUIKit) {
    ZFEnvInfo::envSummaryCallbackUnregister("deviceUIType");
    ZFEnvInfo::envSummaryCallbackUnregister("localeInfo");
    ZFEnvInfo::envSummaryCallbackUnregister("localeLangInfo");
}
public:
    static void deviceUITypeSummary(ZF_IN_OUT zfstring &ret) {
        ret += v_ZFEnvDeviceUIType::EnumNameForValue(ZFEnvInfo::deviceUIType());
    }
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummary_ZFUIKit)

ZF_NAMESPACE_GLOBAL_END

