/**
 * @file ZFUIFlowLayout.h
 * @brief flow container view
 */

#ifndef _ZFI_ZFUIFlowLayout_h_
#define _ZFI_ZFUIFlowLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIFlowLayoutParam
/**
 * @brief layout param for #ZFUIFlowLayout
 *
 * for #ZFUIFlowLayout,
 * #ZFUILayoutParam::align and #ZFUILayoutParam::margin
 * would be relative to sibling child,
 * instead of the container view
 */
zfclass ZFLIB_ZFUIWidget ZFUIFlowLayoutParam : zfextend ZFUILayoutParam {
    ZFOBJECT_DECLARE(ZFUIFlowLayoutParam, ZFUILayoutParam)

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

    ZFPROPERTY_ON_INIT_INLINE(ZFUIAlignFlags, align) {
        propertyValue = ZFUIAlign::e_Center;
    }
};

// ============================================================
// ZFUIFlowLayout
/**
 * @brief flow container view that layout children in linear,
 *   and exceeds to next line or column if no enough space
 *
 * a #ZFUIFlowLayout is much similar to #ZFUILinearLayout,
 * except that #ZFUIFlowLayout would exceeds children to next line or column
 * if no enough space,
 * which looks like "wrap" mode for text editors
 * @note flow layout depends on expensive measure step,
 *   use only if necessary,
 *   and try to prevent deep child view tree,
 *   especially complex children
 */
zfclass ZFLIB_ZFUIWidget ZFUIFlowLayout : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIFlowLayout, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIFlowLayout)

public:
    // ============================================================
    // properties
    /**
     * @brief main direction to layout children, #ZFUIOrientation::e_Left by default
     *
     * #ZFUIOrientation::e_Left means layout children from left to right,
     * while #ZFUIOrientation::e_Bottom means layout children from bottom to top
     */
    ZFPROPERTY_ASSIGN(ZFUIOrientationEnum, orientationMain, ZFUIOrientation::e_Left)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIOrientationEnum, orientationMain)
    /**
     * @brief secondary direction to layout children, #ZFUIOrientation::e_Top by default
     *
     * #ZFUIOrientation::e_Left means layout children from left to right,
     * while #ZFUIOrientation::e_Bottom means layout children from bottom to top
     * @note this direction must differs from #orientationMain,
     *   otherwise, default value would be used\n
     *   for example, if both #orientationMain and #orientationSecondary are x direction,
     *   then #orientationSecondary would be treated as #ZFUIOrientation::e_Top,
     *   and if they are both y direction, then #orientationSecondary would be
     *   treated as #ZFUIOrientation::e_Left
     */
    ZFPROPERTY_ASSIGN(ZFUIOrientationEnum, orientationSecondary, ZFUIOrientation::e_Top)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIOrientationEnum, orientationSecondary)

    /**
     * @brief extra margin independent from children's layout param's margin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, childMargin)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIMargin, childMargin)

    /**
     * @brief extra space between each child independent from children's layout param,
     *   in x direction, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, childSpaceX)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, childSpaceX)
    /**
     * @brief extra space between each child independent from children's layout param,
     *   in y direction, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, childSpaceY)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, childSpaceY)

    // ============================================================
    // override ZFUIView
public:
    /** @brief util method for #childWithParam */
    ZFMETHOD_DECLARE_2(zfanyT<ZFUIFlowLayoutParam>, child
            , ZFMP_IN(ZFUIView *, view)
            , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
            )
protected:
    zfoverride
    virtual const ZFClass *layoutParamClass(void) {
        return ZFUIFlowLayoutParam::ClassData();
    }

    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    /**
     * @brief override ZFUIView to layout scroll view's internal view and content view
     *
     * usually you should not override this method,
     * if necessary, you must call super
     */
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIFlowLayout_h_

