#include "ZFEnvInfo.h"
#include "protocol/ZFProtocolZFEnvInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummaryDataHolder, ZFLevelZFFrameworkStatic) {
}
public:
    ZFCoreMap<zfstring, ZFEnvSummaryCallback> envSummaryCallbackMap;
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummaryDataHolder)

// ============================================================
ZF_NAMESPACE_BEGIN(ZFEnvInfo)

void envSummaryCallbackRegister(
        ZF_IN const zfstring &name
        , ZF_IN ZFEnvSummaryCallback callback
        ) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEnvSummaryDataHolder)->envSummaryCallbackMap.set(name, callback);
}
void envSummaryCallbackUnregister(ZF_IN const zfstring &name) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEnvSummaryDataHolder)->envSummaryCallbackMap.remove(name);
}

ZFMETHOD_FUNC_DEFINE_0(zfstring, envSummary) {
    zfstring ret;
    zfbool first = zftrue;
    ZFCoreMap<zfstring, ZFEnvSummaryCallback> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEnvSummaryDataHolder)->envSummaryCallbackMap;
    for(zfiter it = m.iter(); it; ++it) {
        if(first) {
            first = zffalse;
        }
        else {
            ret += "\n";
        }
        ret += m.iterKey(it);
        ret += ": ";
        ret += m.iterValue(it)();
    }
    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(zfstring, systemName) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_systemInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_systemInfo);
    if(impl != zfnull) {
        return impl->systemName();
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, systemVersion) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_systemInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_systemInfo);
    if(impl != zfnull) {
        return impl->systemVersion();
    }
    else {
        return zfnull;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(zfstring, frameworkName) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_frameworkInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_frameworkInfo);
    if(impl != zfnull) {
        return impl->frameworkName();
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, frameworkVersion) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_frameworkInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_frameworkInfo);
    if(impl != zfnull) {
        return impl->frameworkVersion();
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_END(ZFEnvInfo)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummary_common, ZFLevelZFFrameworkNormal) {
    ZFEnvInfo::envSummaryCallbackRegister("systemName", ZFEnvInfo::systemName);
    ZFEnvInfo::envSummaryCallbackRegister("systemVersion", ZFEnvInfo::systemVersion);
    ZFEnvInfo::envSummaryCallbackRegister("frameworkName", ZFEnvInfo::frameworkName);
    ZFEnvInfo::envSummaryCallbackRegister("frameworkVersion", ZFEnvInfo::frameworkVersion);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFEnvSummary_common) {
    ZFEnvInfo::envSummaryCallbackUnregister("systemName");
    ZFEnvInfo::envSummaryCallbackUnregister("systemVersion");
    ZFEnvInfo::envSummaryCallbackUnregister("frameworkName");
    ZFEnvInfo::envSummaryCallbackUnregister("frameworkVersion");
}
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummary_common)

ZF_NAMESPACE_GLOBAL_END

