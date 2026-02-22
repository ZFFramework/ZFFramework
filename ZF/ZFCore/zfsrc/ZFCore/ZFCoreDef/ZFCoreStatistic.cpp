#include "ZFCoreStatistic.h"
#include "ZFCoreMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

static ZFCoreMap &_ZFP_ZFCoreStatisticDataMap(void) {
    static ZFCoreMap d;
    return d;
}

void invokeCountLog(ZF_IN const zfstring &key) {
    ZFCoreMap &m = _ZFP_ZFCoreStatisticDataMap();
    zfindex *count = m.get<zfindex *>(key);
    if(count != zfnull) {
        ++(*count);
    }
    else {
        m.set(key, ZFCorePointerForPoolObject<zfindex *>(zfpoolNew(zfindex, 1)));
    }
}
void invokeCountRemove(ZF_IN const zfstring &key) {
    _ZFP_ZFCoreStatisticDataMap().remove(key);
}
void invokeCountRemoveAll(void) {
    _ZFP_ZFCoreStatisticDataMap().removeAll();
}
zfindex invokeCountGet(ZF_IN const zfstring &key) {
    zfindex *count = _ZFP_ZFCoreStatisticDataMap().get<zfindex *>(key);
    return ((count != zfnull) ? *count : 0);
}

ZFCoreArray<zfstring> invokeCountGetAllKey(void) {
    return _ZFP_ZFCoreStatisticDataMap().allKey();
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

