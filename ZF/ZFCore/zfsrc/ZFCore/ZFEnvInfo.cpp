#include "ZFEnvInfo.h"
#include "protocol/ZFProtocolZFEnvInfo.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummaryDataHolder, ZFLevelZFFrameworkStatic) {
}
public:
    zfstlmap<zfstring, ZFEnvSummaryCallback> envSummaryCallbackMap;
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummaryDataHolder)

// ============================================================
ZF_NAMESPACE_BEGIN(ZFEnvInfo)

void envSummaryCallbackRegister(
        ZF_IN const zfstring &name
        , ZF_IN ZFEnvSummaryCallback callback
        ) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEnvSummaryDataHolder)->envSummaryCallbackMap[name] = callback;
}
void envSummaryCallbackUnregister(ZF_IN const zfstring &name) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEnvSummaryDataHolder)->envSummaryCallbackMap.erase(name);
}

ZFMETHOD_FUNC_DEFINE_0(zfstring, envSummary) {
    zfstring ret;
    zfbool first = zftrue;
    zfstlmap<zfstring, ZFEnvSummaryCallback> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEnvSummaryDataHolder)->envSummaryCallbackMap;
    for(zfstlmap<zfstring, ZFEnvSummaryCallback>::iterator it = m.begin(); it != m.end(); ++it) {
        if(first) {
            first = zffalse;
        }
        else {
            ret += "\n";
        }
        ret += it->first;
        ret += ": ";
        ret += it->second();
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

