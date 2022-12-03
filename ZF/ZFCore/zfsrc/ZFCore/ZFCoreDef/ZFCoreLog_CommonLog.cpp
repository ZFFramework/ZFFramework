#include "ZFCoreLog_CommonLog.h"
#include "ZFCoreSPrintf.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_zfCoreLogCriticalMessage(ZF_IN const ZFCallerInfo &callerInfo,
                                   ZF_IN const zfchar *fmt,
                                   ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    _ZFP_zfCoreLogCriticalMessageV(callerInfo, fmt, vaList);
    va_end(vaList);
}
void _ZFP_zfCoreLogCriticalMessageV(ZF_IN const ZFCallerInfo &callerInfo,
                                    ZF_IN const zfchar *fmt,
                                    ZF_IN va_list vaList)
{
#ifdef _ZFP_I_log
    {
        _ZFP_I_log("============================================================");
        zfstring tmp;
        tmp += "| ";
        zfstringAppend(tmp, "[%s %s(%u)] ", callerInfo.callerFile(), callerInfo.callerFunc(), callerInfo.callerLine());
        zfstringAppendV(tmp, fmt, vaList);
        _ZFP_I_log("%s", tmp.cString());
        _ZFP_I_log("============================================================");
    }
#endif

    zfCoreLogTrim("============================================================");
    zfCoreLogTrimNoEndl("| ");
    zfCoreLogDetailV(callerInfo, fmt, vaList);
    zfCoreLogTrim("============================================================");
}
void _ZFP_zfCoreCritical(ZF_IN const ZFCallerInfo &callerInfo,
                         ZF_IN const zfchar *fmt,
                         ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    _ZFP_zfCoreCriticalV(callerInfo, fmt, vaList);
    va_end(vaList);
}
void _ZFP_zfCoreCriticalV(ZF_IN const ZFCallerInfo &callerInfo,
                          ZF_IN const zfchar *fmt,
                          ZF_IN va_list vaList)
{
    zfCoreCriticalErrorPrepareDetail(callerInfo);
    zfCoreLogCriticalMessageDetailV(callerInfo, fmt, vaList);
    zfCoreCriticalErrorDetail(callerInfo);
}

ZF_NAMESPACE_GLOBAL_END

