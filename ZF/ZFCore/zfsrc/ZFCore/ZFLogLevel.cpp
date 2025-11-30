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
    ZFLogHeaderDefault_logCaller(ZFLogV());
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

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFLogV) {return ZFLogV();}
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFLogD) {return ZFLogD();}
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFLogI) {return ZFLogI();}
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFLogW) {return ZFLogW();}
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFLogE) {return ZFLogE();}
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFLogA) {return ZFLogA();}

ZF_NAMESPACE_GLOBAL_END

