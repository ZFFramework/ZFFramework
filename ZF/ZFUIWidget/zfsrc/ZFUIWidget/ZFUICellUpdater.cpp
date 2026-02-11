#include "ZFUICellUpdater.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUICellUpdater)

ZFEVENT_REGISTER(ZFUICellUpdater, CellOnUpdate)
ZFEVENT_REGISTER(ZFUICellUpdater, CellOnRecycle)

#define _ZFP_ZFUICellUpdater_cacheKey(cacheKey, key) \
    zfstring cacheKey = zfstr("_ZFP_ZFUICellUpdater_cacheKey_%s", key)
ZFMETHOD_DEFINE_1(ZFUICellUpdater, zfauto, itemCacheAccess
        , ZFMP_IN(const zfstring &, key)
        ) {
    _ZFP_ZFUICellUpdater_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty()) {
        return cacheList->removeLastAndGet();
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFUICellUpdater, void, itemCacheRecycle
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, cache)
        ) {
    _ZFP_ZFUICellUpdater_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList == zfnull) {
        cacheList = zfobjAlloc(ZFArray);
        this->toObject()->objectTag(cacheKey, cacheList);
        zfobjRelease(cacheList);
    }
    cacheList->add(cache);
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUICellUpdaterParam, ZFUICellUpdaterParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUICellUpdaterParam, zfanyT<ZFUICell>, cell)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUICellUpdaterParam, zfindex, cellIndex)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUICellUpdaterParam, zfindex, cellCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUICellUpdaterParam, ZFUIOrientation, orientation)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUICellUpdaterParam, ZFUISize, containerSize)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUICellUpdaterParam, zffloat, cellSizeHint)

ZFOUTPUT_TYPE_DEFINE(ZFUICellUpdaterParam, {
    zfstringAppend(s, "<ZFUICellUpdaterParam cell:%s cellIndex:%s cellCount:%s orientation:%s containerSize:%s cellSizeHint:%s>"
            , v.cell
            , v.cellIndex
            , v.cellCount
            , v.orientation
            , v.containerSize
            , v.cellSizeHint
            );
})

ZF_NAMESPACE_GLOBAL_END

