/**
 * @file ZFUIGridLayout.h
 * @brief flow container view
 */

#ifndef _ZFI_ZFUIGridLayout_h_
#define _ZFI_ZFUIGridLayout_h_

#include "ZFUIFlowLayout.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIGridLayoutParam
/**
 * @brief layout param for #ZFUIGridLayout
 *
 * for #ZFUIGridLayout,
 * #ZFUILayoutParam::align and #ZFUILayoutParam::margin
 * would be relative to sibling child,
 * instead of the container view
 */
zfclass ZFLIB_ZFUIWidget ZFUIGridLayoutParam : zfextend ZFUIFlowLayoutParam {
    ZFOBJECT_DECLARE(ZFUIGridLayoutParam, ZFUIFlowLayoutParam)

public:
    /** @cond ZFPrivateDoc */
    /* ZFTAG_TRICKS: util for chained call to build view tree */
    inline zfanyT<ZFUIGridLayoutParam> child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            ) {
        return zfsuper::child(view, atIndex);
    }
    /** @endcond */
};

// ============================================================
// ZFUIGridLayout
/**
 * @brief grid layout
 *
 * a #ZFUIGridLayout is much similar to #ZFUIFlowLayout,
 * except that all child cells would be measured to same size
 */
zfclass ZFLIB_ZFUIWidget ZFUIGridLayout : zfextend ZFUIFlowLayout {
    ZFOBJECT_DECLARE(ZFUIGridLayout, ZFUIFlowLayout)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIGridLayout)

public:
    ZFPROPERTY_ON_INIT_DECLARE_NO_AUTO_INIT(zfbool, gridMode)
    ZFPROPERTY_ON_INIT_DECLARE_NO_AUTO_INIT(ZFUIScaleType, childScaleType)

    // ============================================================
    // override ZFUIView
public:
    /** @cond ZFPrivateDoc */
    /* ZFTAG_TRICKS: util for chained call to build view tree */
    inline zfanyT<ZFUIGridLayoutParam> child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            ) {
        return zfsuper::child(view, atIndex);
    }
    /** @endcond */
protected:
    zfoverride
    virtual const ZFClass *layoutParamClass(void) {
        return ZFUIGridLayoutParam::ClassData();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIGridLayout_h_

