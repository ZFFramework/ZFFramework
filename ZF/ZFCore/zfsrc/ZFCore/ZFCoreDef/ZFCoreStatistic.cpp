#include "ZFCoreStatistic.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

static zfimplhashmap<zfstring, zfindex> &_ZFP_ZFCoreStatisticDataMap(void) {
    static zfimplhashmap<zfstring, zfindex> d;
    return d;
}

void invokeCountLog(ZF_IN const zfstring &key) {
    ZFCoreMutexLocker();
    zfimplhashmap<zfstring, zfindex> &m = _ZFP_ZFCoreStatisticDataMap();
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
    zfimplhashmap<zfstring, zfindex> &m = _ZFP_ZFCoreStatisticDataMap();
    zfimplhashmap<zfstring, zfindex>::iterator it = m.find(key);
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
    zfimplhashmap<zfstring, zfindex> &m = _ZFP_ZFCoreStatisticDataMap();
    for(zfimplhashmap<zfstring, zfindex>::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
    return ret;
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

