#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD _ZFP_zfobjAllocCacheData {
public:
    zfbool *enableFlag;
    ZFObject **cache;
    zfindex *cacheCount;
};
static ZFCoreArray<_ZFP_zfobjAllocCacheData> &_ZFP_zfobjAllocCacheDataList(void) {
    static ZFCoreArray<_ZFP_zfobjAllocCacheData> d;
    return d;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ObjCacheDataHolder, ZFLevelZFFrameworkNormal) {
    ZFCoreArray<_ZFP_zfobjAllocCacheData> &d = _ZFP_zfobjAllocCacheDataList();
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i) {
        *(d[i].enableFlag) = zftrue;
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ObjCacheDataHolder) {
    ZFCoreArray<ZFObject *> toRelease;
    ZFCoreArray<_ZFP_zfobjAllocCacheData> &d = _ZFP_zfobjAllocCacheDataList();
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i) {
        *(d[i].enableFlag) = zffalse;
        toRelease.addFrom(d[i].cache, *(d[i].cacheCount));
        *(d[i].cacheCount) = 0;
    }
    for(zfindex i = toRelease.count() - 1; i != zfindexMax(); --i) {
        zfunsafe_zfobjRelease(toRelease[i]);
    }
}
ZF_GLOBAL_INITIALIZER_END(ObjCacheDataHolder)

void zfobjAllocCacheRemoveAll(void) {
    ZFCoreMutexLocker();
    ZFCoreArray<ZFObject *> toRelease;
    ZFCoreArray<_ZFP_zfobjAllocCacheData> &d = _ZFP_zfobjAllocCacheDataList();
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i) {
        toRelease.addFrom(d[i].cache, *(d[i].cacheCount));
        *(d[i].cacheCount) = 0;
    }
    for(zfindex i = toRelease.count() - 1; i != zfindexMax(); --i) {
        zfunsafe_zfobjRelease(toRelease[i]);
    }
}

void _ZFP_zfobjAllocCacheImplRegister(
        ZF_IN_OUT zfbool &enableFlag
        , ZF_IN_OUT ZFObject **cache
        , ZF_IN_OUT zfindex &cacheCount
        ) {
    ZFCoreArray<_ZFP_zfobjAllocCacheData> &d = _ZFP_zfobjAllocCacheDataList();
    _ZFP_zfobjAllocCacheData item;
    item.enableFlag = &enableFlag;
    item.cache = cache;
    item.cacheCount = &cacheCount;
    d.add(item);
    if(ZFFrameworkStateCheck((ZFLevel)(ZFLevelZFFrameworkNormal - 1)) == ZFFrameworkStateAvailable) {
        enableFlag = zftrue;
    }
}
void _ZFP_zfobjAllocCacheImplUnregister(ZF_IN_OUT zfbool &enableFlag) {
    ZFCoreArray<_ZFP_zfobjAllocCacheData> &d = _ZFP_zfobjAllocCacheDataList();
    for(zfindex i = 0; i < d.count(); ++i) {
        if(&enableFlag == d[i].enableFlag) {
            ZFCoreArray<ZFObject *> toRelease;
            toRelease.addFrom(d[i].cache, *(d[i].cacheCount));

            *(d[i].cacheCount) = 0;
            enableFlag = zffalse;
            d.remove(i);

            for(zfindex i = toRelease.count() - 1; i != zfindexMax(); --i) {
                zfunsafe_zfobjRelease(toRelease[i]);
            }
            break;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

