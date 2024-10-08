#include "ZFUIListAdapter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIListAdapter)

ZFEVENT_REGISTER(ZFUIListAdapter, ListAdapterOnReload)
ZFEVENT_REGISTER(ZFUIListAdapter, ListCellOnUpdate)
ZFEVENT_REGISTER(ZFUIListAdapter, ListCellCacheOnRecycle)

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIListAdapter, ZFUIOrientationEnum, orientation) {
    return this->_ZFP_ZFUIListAdapter_orientation;
}
ZFMETHOD_DEFINE_0(ZFUIListAdapter, const ZFUISize &, containerSize) {
    return this->_ZFP_ZFUIListAdapter_containerSize;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListAdapter, zffloat, cellSizeHint) {
    this->listAdapterNotifyReload();
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListAdapter, zfbool, cellSizeFill) {
    this->listAdapterNotifyReload();
}

ZFMETHOD_DEFINE_1(ZFUIListAdapter, void, listAdapterNotifyReload
        , ZFMP_IN_OPT(zfindex, atIndexOrMax, zfindexMax())
        ) {
    this->listAdapterOnReload(atIndexOrMax);
}

ZFMETHOD_DEFINE_2(ZFUIListAdapter, zffloat, cellSizeAt
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(ZFUIListCell *, cell)
        ) {
    if(this->cellSizeFill()) {
        switch(this->orientation()) {
            case ZFUIOrientation::e_Left:
            case ZFUIOrientation::e_Right:
                return this->containerSize().width;
            case ZFUIOrientation::e_Top:
            case ZFUIOrientation::e_Bottom:
                return this->containerSize().height;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return -1;
        }
    }
    else {
        return this->cellSizeHint();
    }
}

// ============================================================
#define _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key) \
    zfstring cacheKey = zfstr("_ZFP_ZFUIListAdapter_cacheKey_%s", key)
ZFMETHOD_DEFINE_1(ZFUIListAdapter, zfautoT<ZFUIListCell>, cellCacheDefaultAccess
        , ZFMP_IN(const zfstring &, key)
        ) {
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty()) {
        zfautoT<ZFUIListCell> ret = cacheList->getLast();
        cacheList->removeLast();
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFUIListAdapter, void, cellCacheDefaultRecycle
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFUIListCell *, cell)
        ) {
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList == zfnull) {
        cacheList = zfAlloc(ZFArray);
        this->toObject()->objectTag(cacheKey, cacheList);
        zfRelease(cacheList);
    }
    cacheList->add(cell);
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIListAdapter, zfindex, cellCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFUIListAdapter, zfautoT<ZFUIListCell>, cellAt
        , ZFMP_IN(zfindex, index)
        )

ZF_NAMESPACE_GLOBAL_END

