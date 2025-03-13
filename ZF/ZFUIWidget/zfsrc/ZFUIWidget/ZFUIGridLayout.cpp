#include "ZFUIGridLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIGridLayout)

// ============================================================
// ZFUIGridLayoutParam
ZFOBJECT_REGISTER(ZFUIGridLayoutParam)

/* ZFTAG_TRICKS: util for chained call to build view tree */
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIGridLayoutParam, zfanyT<ZFUIGridLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIGridLayoutParam, zfanyT<ZFUIGridLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIGridLayout, zfanyT<ZFUIGridLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIGridLayout, zfanyT<ZFUIGridLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

// ============================================================
// ZFUIGridLayout
ZFOBJECT_REGISTER(ZFUIGridLayout)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIGridLayout, zfbool, gridMode) {
    propertyValue = zftrue;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIGridLayout, ZFUIScaleType, childScaleType) {
    propertyValue = v_ZFUIScaleType::e_FillCenter;
}

ZF_NAMESPACE_GLOBAL_END

