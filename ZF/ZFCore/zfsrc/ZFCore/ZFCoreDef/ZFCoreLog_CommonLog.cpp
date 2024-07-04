#include "ZFCoreLog_CommonLog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_zfCoreLogCriticalMessage(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
    zfCoreLogTrim("============================================================");
    zfCoreLogDetail(callerInfo, "| %s", text);
    zfCoreLogTrim("============================================================");
}
void _ZFP_zfCoreCritical(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
    zfCoreCriticalErrorPrepareDetail(callerInfo);
    zfCoreLogCriticalMessageDetail(callerInfo, text);
    zfCoreCriticalErrorDetail(callerInfo);
}

ZF_NAMESPACE_GLOBAL_END

