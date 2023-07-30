#include "ZFLog.h"
#include "ZFTime.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFMutex *_ZFP_ZFLogMutex = zfnull;
static ZFLogFormat *_ZFP_ZFLogFormatHolder = zfnull;

// ============================================================
ZFOBJECT_REGISTER(ZFLogFormat)

void ZFLogFormat::format(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFOutputFormatStepEnum outputStep
        , ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfindex writtenLen
        , ZF_IN zfindex outputCount
        , ZF_IN_OUT_OPT void *&state
        ) {
    switch(outputStep) {
        case ZFOutputFormatStep::e_OnInit:
            if(_ZFP_ZFLogMutex != zfnull) {
                _ZFP_ZFLogMutex->mutexLock();
            }
            break;
        case ZFOutputFormatStep::e_OnDealloc:
            this->autoSpace(zftrue);
            this->autoEndl(zftrue);
            if(_ZFP_ZFLogMutex != zfnull) {
                _ZFP_ZFLogMutex->mutexUnlock();
            }
            break;
        case ZFOutputFormatStep::e_OnOutput:
            if(this->autoSpace() && outputCount > 0) {
                ret += " ";
            }
            ret.append(src, srcLen);
            break;
        case ZFOutputFormatStep::e_OnOutputEnd:
            if(this->autoEndl()) {
                ret += "\n";
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogDataHolder, ZFLevelZFFrameworkEssential) {
    _ZFP_ZFLogMutex = zfAlloc(ZFMutex);
    _ZFP_ZFLogFormatHolder = zfAlloc(ZFLogFormat);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogDataHolder) {
    zfRelease(_ZFP_ZFLogFormatHolder);
    _ZFP_ZFLogFormatHolder = zfnull;
    zfRelease(_ZFP_ZFLogMutex);
    _ZFP_ZFLogMutex = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFLogDataHolder)

ZFMETHOD_FUNC_DEFINE_0(ZFMutex *, ZFLogMutex) {
    return _ZFP_ZFLogMutex;
}

zfstring _ZFP_ZFLogHeaderString(ZF_IN const ZFCallerInfo &callerInfo) {
    zfstring ret;
    ZFTimeInfo ti = ZFTime::currentTimeInfo();
    zfstringAppend(ret,
        "%02d:%02d:%02d.%03d ",
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    callerInfo.callerInfoT(ret);
    return ret;
}

// ============================================================
static zfindex _ZFP_zfLogOnOutput(
        ZF_IN const void *src
        , ZF_IN zfindex size
        ) {
    if(size == zfindexMax()) {
        size = zfslen((const zfchar *)src) * sizeof(zfchar);
    }
    ZFOutputDefault().execute(src, size);
    return size;
}
ZFOutput _ZFP_zfLog(
        ZF_IN const zfchar *header
        , ZF_IN_OPT const zfchar *fmt /* = zfnull */
        , ...
        ) {
    ZFOutput ret;
    ret.callbackSerializeCustomDisable(zftrue);
    ZFOutputForFormatT(ret, ZFCallbackForFunc(_ZFP_zfLogOnOutput), _ZFP_ZFLogFormatHolder);

    if(header != zfnull) {
        ret << header;
    }
    if(fmt != zfnull) {
        va_list vaList;
        va_start(vaList, fmt);
        zfstring s;
        zfstringAppendV(s, fmt, vaList);
        va_end(vaList);

        ret << s;
    }

    return ret;
}

// ============================================================
// other convenient method
zfstring _ZFP_zfLogCurTimeString(void) {
    zfstring s;
    ZFTimeInfo ti = ZFTime::currentTimeInfo();
    zfstringAppend(s,
        "%02d:%02d:%02d.%03d",
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    return s;
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, zfLogCurTimeString) {
    return _ZFP_zfLogCurTimeString();
}

ZF_NAMESPACE_GLOBAL_END

