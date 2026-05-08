#include "ZFCoreStatistic_ZFTime.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

zfclassPOD _ZFP_ZFCoreStatisticInvokeTimeData {
public:
    zfindex invokeCount;
    ZFTimeValue invokeTotalTime;
    ZFTimeValue invokeStartTime;
    zfindex reentrantCount;
};

static zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &_ZFP_ZFCoreStatisticInvokeTimeDataMap(void) {
    static zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> m;
    return m;
}

ZFMETHOD_FUNC_DEFINE_1(void, invokeTimeLogBegin
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData &data = m[key];
    ++(data.invokeCount);
    if(data.invokeStartTime != ZFTimeValueZero()) {
        ++(data.reentrantCount);
    }
    else {
        data.invokeStartTime = ZFTime::currentTimeValue();
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, invokeTimeLogEnd
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData>::iterator it = m.find(key);
    if(it != m.end()) {
        _ZFP_ZFCoreStatisticInvokeTimeData &data = it->second;
        if(data.reentrantCount > 0) {
            --(data.reentrantCount);
        }
        else {
            data.invokeTotalTime += ZFTime::currentTimeValue() - data.invokeStartTime;
            data.invokeStartTime = ZFTimeValueZero();
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, invokeTimeRemove
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFCoreStatisticInvokeTimeDataMap().erase(key);
}
ZFMETHOD_FUNC_DEFINE_0(void, invokeTimeRemoveAll) {
    ZFCoreMutexLocker();
    _ZFP_ZFCoreStatisticInvokeTimeDataMap().clear();
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, invokeTimeGetInvokeCount
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData>::iterator it = m.find(key);
    if(it != m.end()) {
        return it->second.invokeCount;
    }
    else {
        return 0;
    }
}
ZFMETHOD_FUNC_DEFINE_1(ZFTimeValue, invokeTimeGetAverageTime
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData>::iterator it = m.find(key);
    if(it != m.end()) {
        return it->second.invokeTotalTime / it->second.invokeCount;
    }
    else {
        return ZFTimeValueZero();
    }
}
ZFMETHOD_FUNC_DEFINE_1(ZFTimeValue, invokeTimeGetTotalTime
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData>::iterator it = m.find(key);
    if(it != m.end()) {
        return it->second.invokeTotalTime;
    }
    else {
        return ZFTimeValueZero();
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, invokeTimeGetSummaryT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData>::iterator it = m.find(key);
    zfindex invokeCount = ((it != m.end()) ? it->second.invokeCount : 0);
    ZFTimeValue invokeTotalTime = ((it != m.end()) ? it->second.invokeTotalTime : ZFTimeValueZero());
    if(invokeCount > 1) {
        ZFTimeValue invokeAverageTime = ((invokeCount == 0) ? ZFTimeValueZero() : invokeTotalTime / invokeCount);
        zfstringAppend(ret, "[%s] invoke count: %s, total: %s, average: %s",
            (key == zfnull) ? ZFTOKEN_zfnull : key.cString(),
            invokeCount,
            ZFTimeValueToStringFriendly(invokeTotalTime),
            ZFTimeValueToStringFriendly(invokeAverageTime));
    }
    else {
        zfstringAppend(ret, "[%s] invoke time: %s",
            (key == zfnull) ? ZFTOKEN_zfnull : key.cString(),
            ZFTimeValueToStringFriendly(invokeTotalTime));
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, invokeTimeGetSummary
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMutexLocker();
    zfstring ret;
    invokeTimeGetSummaryT(ret, key);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<zfstring>, invokeTimeGetAllKey) {
    ZFCoreMutexLocker();
    ZFCoreArray<zfstring> ret;
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    for(zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData>::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_1(void, invokeTimeGetAllSummaryT
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData> &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    for(zfstlhashmap<zfstring, _ZFP_ZFCoreStatisticInvokeTimeData>::iterator it = m.begin(); it != m.end(); ++it) {
        invokeTimeGetSummaryT(ret, it->first);
        ret += "\n";
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, invokeTimeGetAllSummary) {
    zfstring ret;
    invokeTimeGetAllSummaryT(ret);
    return ret;
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

