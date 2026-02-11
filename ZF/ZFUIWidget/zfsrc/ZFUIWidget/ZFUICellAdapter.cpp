#include "ZFUICellAdapter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUICellAdapter)

ZFEVENT_REGISTER(ZFUICellAdapter, CellAdapterOnReload)
ZFEVENT_REGISTER(ZFUICellAdapter, CellOnUpdate)
ZFEVENT_REGISTER(ZFUICellAdapter, CellCacheOnRecycle)

// ============================================================
ZFMETHOD_DEFINE_0(ZFUICellAdapter, ZFUIOrientation, orientation) {
    return this->_ZFP_ZFUICellAdapter_orientation;
}
ZFMETHOD_DEFINE_0(ZFUICellAdapter, const ZFUISize &, containerSize) {
    return this->_ZFP_ZFUICellAdapter_containerSize;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUICellAdapter, zffloat, cellSizeHint) {
    this->cellAdapterNotifyReload();
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUICellAdapter, zfbool, cellSizeFill) {
    this->cellAdapterNotifyReload();
}

ZFMETHOD_DEFINE_1(ZFUICellAdapter, void, cellAdapterNotifyReload
        , ZFMP_IN_OPT(zfindex, atIndexOrMax, zfindexMax())
        ) {
    this->cellAdapterOnReload(atIndexOrMax);
}

ZFMETHOD_DEFINE_2(ZFUICellAdapter, zffloat, cellSizeAt
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(ZFUICell *, cell)
        ) {
    if(this->cellSizeFill()) {
        switch(this->orientation()) {
            case v_ZFUIOrientation::e_Left:
            case v_ZFUIOrientation::e_Right:
                return this->containerSize().width;
            case v_ZFUIOrientation::e_Top:
            case v_ZFUIOrientation::e_Bottom:
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
#define _ZFP_ZFUICellAdapter_cacheKey(cacheKey, key) \
    zfstring cacheKey = zfstr("_ZFP_ZFUICellAdapter_cacheKey_%s", key)
ZFMETHOD_DEFINE_1(ZFUICellAdapter, zfautoT<ZFUICell>, cellCacheDefaultAccess
        , ZFMP_IN(const zfstring &, key)
        ) {
    _ZFP_ZFUICellAdapter_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty()) {
        return cacheList->removeLastAndGet();
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFUICellAdapter, void, cellCacheDefaultRecycle
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFUICell *, cell)
        ) {
    _ZFP_ZFUICellAdapter_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag(cacheKey);
    if(cacheList == zfnull) {
        cacheList = zfobjAlloc(ZFArray);
        this->toObject()->objectTag(cacheKey, cacheList);
        zfobjRelease(cacheList);
    }
    cacheList->add(cell);
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUICellAdapter, zfindex, cellCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFUICellAdapter, zfautoT<ZFUICell>, cellAt
        , ZFMP_IN(zfindex, index)
        )

ZF_NAMESPACE_GLOBAL_END

