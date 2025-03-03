/**
 * @file ZFUILinearLayout.h
 * @brief linear container view
 */

#ifndef _ZFI_ZFUILinearLayout_h_
#define _ZFI_ZFUILinearLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUILinearLayoutParam
/**
 * @brief layout param for #ZFUILinearLayout
 *
 * for #ZFUILinearLayout,
 * #ZFUILayoutParam::align and #ZFUILayoutParam::margin
 * would be relative to sibling child,
 * instead of the container view
 */
zfclass ZFLIB_ZFUIWidget ZFUILinearLayoutParam : zfextend ZFUILayoutParam {
    ZFOBJECT_DECLARE(ZFUILinearLayoutParam, ZFUILayoutParam)

    /**
     * @brief weight for the view, 0 by default
     *
     * if greater than 0, all children with weight would be layouted by its weight percent,
     * ignoring sizeParam\n
     * for example, assume we have 3 children with these layout weight:
     * -  child0: 2
     * -  child1: 4
     * -  child2: 3
     *
     * then the total weight of children would be (2+4+3)=9,
     * and the children's final size would be:
     * -  child0 = parentSize * 2 / 9
     * -  child1 = parentSize * 4 / 9
     * -  child2 = parentSize * 3 / 9
     */
    ZFPROPERTY_ASSIGN(zffloat, weight)

    /**
     * @brief whether reserve space if child is not visible, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, reserveSpace)

    ZFPROPERTY_ON_INIT_DECLARE(ZFUIAlignFlags, align)
public:
    /** @cond ZFPrivateDoc */
    /* ZFTAG_TRICKS: util for chained call to build view tree */
    inline zfanyT<ZFUILinearLayoutParam> child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            ) {
        return zfsuper::child(view, atIndex);
    }
    /** @endcond */
};

// ============================================================
// ZFUILinearLayout
/**
 * @brief linear container view that layout children in linear by specified orientation
 */
zfclass ZFLIB_ZFUIWidget ZFUILinearLayout : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUILinearLayout, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUILinearLayout)

public:
    // ============================================================
    // properties
    /**
     * @brief direction to layout children, #v_ZFUIOrientation::e_Left by default
     *
     * #v_ZFUIOrientation::e_Left means layout children from left to right,
     * while #v_ZFUIOrientation::e_Bottom means layout children from bottom to top
     */
    ZFPROPERTY_ASSIGN(ZFUIOrientation, orientation, v_ZFUIOrientation::e_Left)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIOrientation, orientation)

    /**
     * @brief extra margin independent from children's layout param's margin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, childMargin)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIMargin, childMargin)

    /**
     * @brief extra space between each child independent from children's layout param, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, childSpace)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, childSpace)

    // ============================================================
    // override ZFUIView
public:
    /** @cond ZFPrivateDoc */
    /* ZFTAG_TRICKS: util for chained call to build view tree */
    inline zfanyT<ZFUILinearLayoutParam> child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            ) {
        return zfsuper::child(view, atIndex);
    }
    /** @endcond */
protected:
    zfoverride
    virtual const ZFClass *layoutParamClass(void) {
        return ZFUILinearLayoutParam::ClassData();
    }

    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUILinearLayout_h_

