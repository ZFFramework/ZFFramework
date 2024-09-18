#include "ZFCoreLog_CommonLog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCoreLogCriticalMessage(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
    ZFCoreLogTrim("============================================================");
    ZFCoreLogDetail(callerInfo, "| %s", text);
    ZFCoreLogTrim("============================================================");
}
void _ZFP_ZFCoreCritical(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
    ZFCoreCriticalErrorPrepareDetail(callerInfo);
    ZFCoreLogCriticalMessageDetail(callerInfo, text);
    ZFCoreCriticalErrorDetail(callerInfo);
}

ZF_NAMESPACE_GLOBAL_END

