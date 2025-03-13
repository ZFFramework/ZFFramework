#include "ZFLog_StackTrace.h"
#include "protocol/ZFProtocolZFLogStackTrace.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFLogStackTraceAvailable) {
    return (ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull);
}

ZFMETHOD_FUNC_DEFINE_4(void, ZFLogStackTrace
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(const zfchar *, prefix, zfnull)
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        , ZFMP_IN_OPT(zfindex, maxLevel, 20)
        ) {
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull) {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->stackTrace(ret, prefix, ignoreLevel + 2, maxLevel);
    }
    else {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? "" : prefix);

        ret += fixedPrefix;
        ret += "======================== stack trace =======================";
        ret += '\n';

        ret += fixedPrefix;
        ret += "| ZFLogStackTrace is currently unsupported";
        ret += '\n';

        ret += fixedPrefix;
        ret += "------------------------ stack trace -----------------------";
        ret += '\n';
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfstring, ZFLogStackTrace
        , ZFMP_IN_OPT(const zfchar *, prefix, zfnull)
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        , ZFMP_IN_OPT(zfindex, maxLevel, 20)
        ) {
    zfstring ret;
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull) {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->stackTrace(ret, prefix, ignoreLevel + 2, maxLevel);
    }
    else {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? "" : prefix);

        ret += fixedPrefix;
        ret += "======================== stack trace =======================";
        ret += '\n';

        ret += fixedPrefix;
        ret += "| ZFLogStackTrace is currently unsupported";
        ret += '\n';

        ret += fixedPrefix;
        ret += "------------------------ stack trace -----------------------";
        ret += '\n';
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_2(void, ZFLogCallerInfo
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        ) {
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull) {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->callerInfo(ret, ignoreLevel + 2);
    }
    else {
        ret += "<ZFLogCallerInfo is currently unsupported>";
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFLogCallerInfo
        , ZFMP_IN_OPT(zfindex, ignoreLevel, 0)
        ) {
    zfstring ret;
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull) {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->callerInfo(ret, ignoreLevel + 2);
    }
    else {
        ret += "<ZFLogCallerInfo is currently unsupported>";
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

