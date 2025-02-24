#include "ZFLog.h"
#include "ZFLog_StackTrace.h"
#include "ZFImplOutput.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFLog_ZFCoreLogExt(ZF_IN const ZFCallerInfo &callerInfo) {
    if(ZFLogStackTraceAvailable()) {
        ZFCoreLogTrim(ZFLogStackTrace(zfnull, 2));
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLog_ZFCoreLogExt, ZFLevelZFFrameworkEssential) {
    ZFCoreCriticalErrorCallbackAdd(_ZFP_ZFLog_ZFCoreLogExt);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLog_ZFCoreLogExt) {
    ZFCoreCriticalErrorCallbackRemove(_ZFP_ZFLog_ZFCoreLogExt);
}
ZF_GLOBAL_INITIALIZER_END(ZFLog_ZFCoreLogExt)

ZF_NAMESPACE_GLOBAL_END

