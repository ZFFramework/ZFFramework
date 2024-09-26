#include "ZFUIListCellUpdater.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListCellUpdater)

ZFEVENT_REGISTER(ZFUIListCellUpdater, CellOnUpdate)
ZFEVENT_REGISTER(ZFUIListCellUpdater, CellOnRecycle)

#define _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key) \
    zfstring cacheKey = zfstr("_ZFP_ZFUIListCellUpdater_cacheKey_%s", key)
ZFMETHOD_DEFINE_1(ZFUIListCellUpdater, zfauto, itemCacheAccess
        , ZFMP_IN(const zfstring &, key)
        ) {
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty()) {
        zfauto ret = cacheList->getLast();
        cacheList->removeLast();
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFUIListCellUpdater, void, itemCacheRecycle
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, cache)
        ) {
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList == zfnull) {
        cacheList = zfAlloc(ZFArray);
        this->toObject()->objectTag(cacheKey, cacheList);
        zfRelease(cacheList);
    }
    cacheList->add(cache);
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIListCellUpdaterParam, ZFUIListCellUpdaterParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, zfanyT<ZFUIListCell>, cell)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, zfindex, cellIndex)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, zfindex, cellCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, ZFUIOrientationEnum, orientation)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, ZFUISize, containerSize)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIListCellUpdaterParam, zffloat, cellSizeHint)

ZFOUTPUT_TYPE_DEFINE(ZFUIListCellUpdaterParam, {
    zfstringAppend(s, "<ZFUIListCellUpdaterParam cell:%s cellIndex:%s cellCount:%s orientation:%s containerSize:%s cellSizeHint:%s>"
            , v.cell
            , v.cellIndex
            , v.cellCount
            , v.orientation
            , v.containerSize
            , v.cellSizeHint
            );
})

ZF_NAMESPACE_GLOBAL_END

