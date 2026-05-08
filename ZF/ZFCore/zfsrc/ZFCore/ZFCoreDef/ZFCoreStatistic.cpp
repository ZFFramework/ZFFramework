#include "ZFCoreStatistic.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

static zfstlhashmap<zfstring, zfindex> &_ZFP_ZFCoreStatisticDataMap(void) {
    static zfstlhashmap<zfstring, zfindex> d;
    return d;
}

void invokeCountLog(ZF_IN const zfstring &key) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, zfindex> &m = _ZFP_ZFCoreStatisticDataMap();
    ++(m[key]);
}
void invokeCountRemove(ZF_IN const zfstring &key) {
    ZFCoreMutexLocker();
    _ZFP_ZFCoreStatisticDataMap().erase(key);
}
void invokeCountRemoveAll(void) {
    ZFCoreMutexLocker();
    _ZFP_ZFCoreStatisticDataMap().clear();
}
zfindex invokeCountGet(ZF_IN const zfstring &key) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfstring, zfindex> &m = _ZFP_ZFCoreStatisticDataMap();
    zfstlhashmap<zfstring, zfindex>::iterator it = m.find(key);
    if(it != m.end()) {
        return it->second;
    }
    else {
        return 0;
    }
}

ZFCoreArray<zfstring> invokeCountGetAllKey(void) {
    ZFCoreMutexLocker();
    ZFCoreArray<zfstring> ret;
    zfstlhashmap<zfstring, zfindex> &m = _ZFP_ZFCoreStatisticDataMap();
    for(zfstlhashmap<zfstring, zfindex>::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
    return ret;
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

