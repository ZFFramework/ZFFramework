#include "ZFCoreStatistic_ZFTime.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

zfclassPOD _ZFP_ZFCoreStatisticInvokeTimeData {
public:
    zfindex invokeCount;
    ZFTimeValue invokeTotalTime;
    ZFTimeValue invokeStartTime;
    zfindex reentrantCount;
};

static ZFCoreMap &_ZFP_ZFCoreStatisticInvokeTimeDataMap(void) {
    static ZFCoreMap m; // _ZFP_ZFCoreStatisticInvokeTimeData
    return m;
}

ZFMETHOD_FUNC_DEFINE_1(void, invokeTimeLogBegin
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data == zfnull) {
        data = (_ZFP_ZFCoreStatisticInvokeTimeData *)zfmalloc(sizeof(_ZFP_ZFCoreStatisticInvokeTimeData));
        zfmemset(data, 0, sizeof(_ZFP_ZFCoreStatisticInvokeTimeData));
        m.set(key, ZFCorePointerForPOD<_ZFP_ZFCoreStatisticInvokeTimeData *>(data));

        ++(data->invokeCount);
        data->invokeStartTime = ZFTime::currentTimeValue();
    }
    else {
        ++(data->invokeCount);

        if(data->invokeStartTime != ZFTimeValueZero()) {
            ++(data->reentrantCount);
        }
        else {
            data->invokeStartTime = ZFTime::currentTimeValue();
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, invokeTimeLogEnd
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull) {
        if(data->reentrantCount > 0) {
            --(data->reentrantCount);
        }
        else {
            data->invokeTotalTime += ZFTime::currentTimeValue() - data->invokeStartTime;
            data->invokeStartTime = ZFTimeValueZero();
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, invokeTimeRemove
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    m.remove(key);
}
ZFMETHOD_FUNC_DEFINE_0(void, invokeTimeRemoveAll) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    m.removeAll();
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, invokeTimeGetInvokeCount
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull) {
        return data->invokeCount;
    }
    return 0;
}
ZFMETHOD_FUNC_DEFINE_1(ZFTimeValue, invokeTimeGetAverageTime
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull && data->invokeCount > 0) {
        return data->invokeTotalTime / data->invokeCount;
    }
    return ZFTimeValueZero();
}
ZFMETHOD_FUNC_DEFINE_1(ZFTimeValue, invokeTimeGetTotalTime
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull) {
        return data->invokeTotalTime;
    }
    return ZFTimeValueZero();
}
ZFMETHOD_FUNC_DEFINE_2(void, invokeTimeGetSummaryT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, key)
        ) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    zfindex invokeCount = ((data == zfnull) ? 0 : data->invokeCount);
    ZFTimeValue invokeTotalTime = ((data == zfnull) ? ZFTimeValueZero() : data->invokeTotalTime);
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
    zfstring ret;
    invokeTimeGetSummaryT(ret, key);
    return ret;
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

