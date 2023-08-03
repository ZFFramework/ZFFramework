#include "ZFCoreLog_CommonLog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_zfCoreLogCriticalMessage(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
#ifdef _ZFP_I_log
    {
        _ZFP_I_log("============================================================");
        zfstring tmp;
        zfstringAppend(tmp, "| [%s %s(%s)] %s", callerInfo.callerFile(), callerInfo.callerFunc(), callerInfo.callerLine(), text);
        _ZFP_I_log("%s", tmp.cString());
        _ZFP_I_log("============================================================");
    }
#endif

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

