#include "ZFLog.h"
#include "ZFTime.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFObject *_ZFP_ZFLogMutex = zfnull;
static ZFLogFormat *_ZFP_ZFLogFormatHolder = zfnull;

// ============================================================
ZFOBJECT_REGISTER(ZFLogFormat)

void ZFLogFormat::format(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFOutputFormatStep outputStep
        , ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        ) {
    switch(outputStep) {
        case v_ZFOutputFormatStep::e_OnInit:
            if(_ZFP_ZFLogMutex != zfnull) {
                ZFObjectLock(_ZFP_ZFLogMutex);
            }
            break;
        case v_ZFOutputFormatStep::e_OnDealloc:
            this->autoEndl(zftrue);
            if(_ZFP_ZFLogMutex != zfnull) {
                ZFObjectUnlock(_ZFP_ZFLogMutex);
            }
            break;
        case v_ZFOutputFormatStep::e_OnOutput:
            ret.append(src, srcLen);
            break;
        case v_ZFOutputFormatStep::e_OnOutputEnd:
            if(this->autoEndl()) {
                ret += "\n";
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogDataHolder, ZFLevelZFFrameworkEssential) {
    _ZFP_ZFLogMutex = zfAlloc(ZFObject);
    _ZFP_ZFLogFormatHolder = zfAlloc(ZFLogFormat);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogDataHolder) {
    zfRelease(_ZFP_ZFLogFormatHolder);
    _ZFP_ZFLogFormatHolder = zfnull;
    zfRelease(_ZFP_ZFLogMutex);
    _ZFP_ZFLogMutex = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFLogDataHolder)

ZFMETHOD_FUNC_DEFINE_0(ZFObject *, ZFLogMutex) {
    return _ZFP_ZFLogMutex;
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLogFormatDefault
        , ZFMP_IN(ZFLogFormat *, fmt)
        ) {
    if(fmt != zfnull)
    {
        zfRetainChange(_ZFP_ZFLogFormatHolder, fmt);
    }
}
ZFMETHOD_FUNC_DEFINE_0(ZFLogFormat *, ZFLogFormatDefault) {
    return _ZFP_ZFLogFormatHolder;
}

ZFLogHeaderFunc ZFLogHeader = zfnull;

ZFEXPORT_VAR_DEFINE(zfbool, ZFLogHeaderDefault_logTime, zftrue)
ZFEXPORT_VAR_DEFINE(zfbool, ZFLogHeaderDefault_logCaller, zftrue)

zfstring ZFLogHeaderDefault(ZF_IN const ZFCallerInfo &callerInfo) {
    zfstring ret;
    if(ZFLogHeaderDefault_logTime()) {
        ZFTimeInfo ti = ZFTime::currentTimeInfo();
        zfstringAppend(ret,
            "%02s:%02s:%02s.%03s ",
            ti.hour,
            ti.minute,
            ti.second,
            ti.miliSecond);
    }
    if(ZFLogHeaderDefault_logCaller()) {
        callerInfo.callerInfoT(ret);
        ret += " ";
    }
    return ret;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogHeaderInit, ZFLevelZFFrameworkStatic) {
    ZFLogHeader = ZFLogHeaderDefault;
    ZFLogHeaderDefault_logTime(zftrue);
    ZFLogHeaderDefault_logCaller(ZFLogV());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogHeaderInit) {
    ZFLogHeader = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFLogHeaderInit)

// ============================================================
static zfindex _ZFP_ZFLogOnOutput(
        ZF_IN const void *src
        , ZF_IN zfindex size
        ) {
    if(size == zfindexMax()) {
        size = zfslen((const zfchar *)src);
    }
    ZFOutputDefault().execute(src, size);
    return size;
}
ZFOutput _ZFP_ZFLog(
        ZF_IN const zfchar *header
        , ZF_IN_OPT const zfchar *text /* = zfnull */
        ) {
    ZFOutput ret;
    ret.callbackSerializeDisable(zftrue);
    ZFOutputForFormatT(ret, ZFCallbackForFunc(_ZFP_ZFLogOnOutput), _ZFP_ZFLogFormatHolder);

    if(header != zfnull) {
        ret << header;
    }
    if(text != zfnull) {
        ret << text;
    }

    return ret;
}

// ============================================================
// other convenient method
zfstring _ZFP_ZFLogCurTimeString(void) {
    zfstring s;
    ZFTimeInfo ti = ZFTime::currentTimeInfo();
    zfstringAppend(s,
        "%02s:%02s:%02s.%03s",
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    return s;
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFLogCurTimeString) {
    return _ZFP_ZFLogCurTimeString();
}

ZF_NAMESPACE_GLOBAL_END

