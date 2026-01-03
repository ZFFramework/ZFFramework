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

ZFMETHOD_FUNC_DEFINE_1(void, envSummaryT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    zfstring tmp;

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
        it->second(ret);
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, envSummary) {
    zfstring ret;
    ZFEnvInfo::envSummaryT(ret);
    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, systemNameT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_systemInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_systemInfo);
    if(impl != zfnull) {
        impl->systemName(ret);
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, systemName) {
    zfstring ret;
    ZFEnvInfo::systemNameT(ret);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_1(void, systemVersionT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_systemInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_systemInfo);
    if(impl != zfnull) {
        impl->systemVersion(ret);
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, systemVersion) {
    zfstring ret;
    ZFEnvInfo::systemVersionT(ret);
    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, frameworkNameT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_frameworkInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_frameworkInfo);
    if(impl != zfnull) {
        impl->frameworkName(ret);
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, frameworkName) {
    zfstring ret;
    ZFEnvInfo::frameworkNameT(ret);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_1(void, frameworkVersionT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_frameworkInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_frameworkInfo);
    if(impl != zfnull) {
        impl->frameworkVersion(ret);
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, frameworkVersion) {
    zfstring ret;
    ZFEnvInfo::frameworkVersionT(ret);
    return ret;
}

ZF_NAMESPACE_END(ZFEnvInfo)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummary_common, ZFLevelZFFrameworkNormal) {
    ZFEnvInfo::envSummaryCallbackRegister("systemName", ZFEnvInfo::systemNameT);
    ZFEnvInfo::envSummaryCallbackRegister("systemVersion", ZFEnvInfo::systemVersionT);
    ZFEnvInfo::envSummaryCallbackRegister("frameworkName", ZFEnvInfo::frameworkNameT);
    ZFEnvInfo::envSummaryCallbackRegister("frameworkVersion", ZFEnvInfo::frameworkVersionT);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFEnvSummary_common) {
    ZFEnvInfo::envSummaryCallbackUnregister("systemName");
    ZFEnvInfo::envSummaryCallbackUnregister("systemVersion");
    ZFEnvInfo::envSummaryCallbackUnregister("frameworkName");
    ZFEnvInfo::envSummaryCallbackUnregister("frameworkVersion");
}
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummary_common)

ZF_NAMESPACE_GLOBAL_END

