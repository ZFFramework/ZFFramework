#include "ZFLogLevel.h"
#include "ZFLog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFLogLevel)

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(LogLevelOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

static ZFLogLevel _ZFP_ZFLogLevelGlobal = v_ZFLogLevel::EnumDefault();
ZFMETHOD_FUNC_DEFINE_1(void, ZFLogLevelDefault
        , ZFMP_IN(ZFLogLevel, level)
        ) {
    _ZFP_ZFLogLevelGlobal = level;
    ZFLogHeaderDefault_logCaller(ZFLogLevelIsActive(ZFLogLevel::e_Verbose));
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) == ZFFrameworkStateAvailable) {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_LogLevelOnUpdate());
    }
}
ZFMETHOD_FUNC_DEFINE_0(ZFLogLevel, ZFLogLevelDefault) {
    return _ZFP_ZFLogLevelGlobal;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogLevelReset, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogLevelReset) {
    _ZFP_ZFLogLevelGlobal = v_ZFLogLevel::EnumDefault();
}
ZF_GLOBAL_INITIALIZER_END(ZFLogLevelReset)

ZFMETHOD_FUNC_INLINE_DEFINE_1(zfbool, ZFLogLevelIsActive
        , ZFMP_IN(ZFLogLevel, level)
        )

ZF_NAMESPACE_GLOBAL_END

