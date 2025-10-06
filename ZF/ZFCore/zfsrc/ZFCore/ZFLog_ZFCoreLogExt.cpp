#include "ZFLog.h"
#include "ZFLog_StackTrace.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLog_ZFCoreLogExt, ZFLevelZFFrameworkStatic) {
    ZFCoreCriticalErrorCallbackAdd(zfself::impl);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLog_ZFCoreLogExt) {
    ZFCoreCriticalErrorCallbackRemove(zfself::impl);
}
private:
    static void impl(ZF_IN const ZFCallerInfo &callerInfo, ZF_IN_OUT zfstring &errorHint) {
        if(ZFLogStackTraceAvailable()) {
            zfstring t = ZFLogStackTrace(zfnull, 2);
            ZFCoreLogTrim(t);

            errorHint += "\n";
            errorHint += t;
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFLog_ZFCoreLogExt)

ZF_NAMESPACE_GLOBAL_END

