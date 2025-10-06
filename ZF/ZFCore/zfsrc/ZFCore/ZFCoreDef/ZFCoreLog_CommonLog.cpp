#include "ZFCoreLog_CommonLog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCoreLogCriticalMessage(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
    ZFCoreLogTrim("====================== critical error ======================");
    if(callerInfo) {
        ZFCoreLogTrim("| %s %s", callerInfo.callerInfo(), text);
    }
    else {
        ZFCoreLogTrim("| %s", text);
    }
    ZFCoreLogTrim("---------------------- critical error ----------------------");
}
void _ZFP_ZFCoreCritical(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
    ZFCoreLogCriticalMessageDetail(callerInfo, text);
    ZFCoreCriticalErrorDetail(callerInfo, text);
}

ZF_NAMESPACE_GLOBAL_END

