#include "ZFUILinearLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUILinearLayoutParam
ZFOBJECT_REGISTER(ZFUILinearLayoutParam)

/* ZFTAG_TRICKS: util for chained call to build view tree */
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUILinearLayoutParam, zfanyT<ZFUILinearLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUILinearLayoutParam, zfanyT<ZFUILinearLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUILinearLayout, zfanyT<ZFUILinearLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUILinearLayout, zfanyT<ZFUILinearLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

ZFPROPERTY_ON_INIT_DEFINE(ZFUILinearLayoutParam, ZFUIAlignFlags, align) {
    propertyValue = v_ZFUIAlign::e_Center;
}

// ============================================================
// ZFUILinearLayout
ZFOBJECT_REGISTER(ZFUILinearLayout)
ZFSTYLE_DEFAULT_DEFINE(ZFUILinearLayout)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUILinearLayout, ZFUIOrientation, orientation) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUILinearLayout, ZFUIMargin, childMargin) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUILinearLayout, zffloat, childSpace) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}

// ============================================================
// override ZFUIView
static ZFUISize _ZFP_ZFUILinearLayout_measureHorizontal(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType heightParam
        , ZF_OUT_OPT zffloat *fixedSize = zfnull
        , ZF_OUT_OPT zffloat *totalWeight = zfnull
        );
static ZFUISize _ZFP_ZFUILinearLayout_measureVertical(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType widthParam
        , ZF_OUT_OPT zffloat *fixedSize = zfnull
        , ZF_OUT_OPT zffloat *totalWeight = zfnull
        );
void ZFUILinearLayout::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    switch(this->orientation()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            ret = _ZFP_ZFUILinearLayout_measureHorizontal(this, sizeHint, sizeParam.height);
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            ret = _ZFP_ZFUILinearLayout_measureVertical(this, sizeHint, sizeParam.width);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

static void _ZFP_ZFUILinearLayout_layoutHorizontal(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &size
        );
static void _ZFP_ZFUILinearLayout_layoutVertical(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &size
        );
void ZFUILinearLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    switch(this->orientation()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            _ZFP_ZFUILinearLayout_layoutHorizontal(this, ZFUIRectGetSize(bounds));
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            _ZFP_ZFUILinearLayout_layoutVertical(this, ZFUIRectGetSize(bounds));
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

// ============================================================
// measure
static ZFUISize _ZFP_ZFUILinearLayout_measureHorizontal(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType heightParam
        , ZF_OUT_OPT zffloat *fixedSize /* = zfnull */
        , ZF_OUT_OPT zffloat *totalWeight /* = zfnull */
        ) {
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->childMargin());
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->childMargin());
    ZFUISize ret = ZFUISizeCreate(parentMarginX, 0);
    if(fixedSize != zfnull) {
        *fixedSize = parentMarginX;
    }
    for(zfindex i = 0; i < parent->childCount(); ++i) {
        ZFUIView *child = parent->childAt(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam();
        if(!child->visible() && !layoutParam->reserveSpace()) {
            continue;
        }
        zffloat prevSpace = (i > 0 ? parent->childSpace() : (zffloat)0);
        if(totalWeight != zfnull) {
            *totalWeight += layoutParam->weight();
        }

        zffloat marginX = ZFUIMarginGetWidth(layoutParam->margin());
        zffloat marginY = ZFUIMarginGetHeight(layoutParam->margin());
        child->layoutMeasure(
                ZFUISizeCreate(
                    ZFUILayoutParam::sizeHintMerge(
                        layoutParam->sizeHint().width,
                        ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - parentMarginX - marginX)
                        ),
                    ZFUILayoutParam::sizeHintMerge(
                        layoutParam->sizeHint().height,
                        ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - parentMarginY - marginY)
                        )),
                ZFUISizeParamCreate(
                    v_ZFUISizeType::e_Wrap,
                    heightParam == v_ZFUISizeType::e_Fill && layoutParam->sizeParam().height == v_ZFUISizeType::e_Fill
                        ? v_ZFUISizeType::e_Fill
                        : v_ZFUISizeType::e_Wrap
                    ));
        ret.width += prevSpace + child->layoutMeasuredSize().width + marginX;
        ret.height = zfmMax<zffloat>(ret.height, child->layoutMeasuredSize().height + parentMarginY + marginY);
        if(fixedSize != zfnull) {
            *fixedSize += prevSpace + marginX;
            if(layoutParam->weight() == 0) {
                *fixedSize += child->layoutMeasuredSize().width;
            }
        }
    }
    return ret;
}
static ZFUISize _ZFP_ZFUILinearLayout_measureVertical(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType widthParam
        , ZF_OUT_OPT zffloat *fixedSize /* = zfnull */
        , ZF_OUT_OPT zffloat *totalWeight /* = zfnull */
        ) {
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->childMargin());
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->childMargin());
    ZFUISize ret = ZFUISizeCreate(0, parentMarginY);
    if(fixedSize != zfnull) {
        *fixedSize = parentMarginY;
    }
    for(zfindex i = 0; i < parent->childCount(); ++i) {
        ZFUIView *child = parent->childAt(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam();
        if(!child->visible() && !layoutParam->reserveSpace()) {
            continue;
        }
        zffloat prevSpace = (i > 0 ? parent->childSpace() : (zffloat)0);
        if(totalWeight != zfnull) {
            *totalWeight += layoutParam->weight();
        }

        zffloat marginX = ZFUIMarginGetWidth(layoutParam->margin());
        zffloat marginY = ZFUIMarginGetHeight(layoutParam->margin());
        child->layoutMeasure(
                ZFUISizeCreate(
                    ZFUILayoutParam::sizeHintMerge(
                        layoutParam->sizeHint().width,
                        ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - parentMarginX - marginX)
                        ),
                    ZFUILayoutParam::sizeHintMerge(
                        layoutParam->sizeHint().height,
                        ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - parentMarginY - marginY)
                        )),
                ZFUISizeParamCreate(
                    widthParam == v_ZFUISizeType::e_Fill && layoutParam->sizeParam().width == v_ZFUISizeType::e_Fill
                        ? v_ZFUISizeType::e_Fill
                        : v_ZFUISizeType::e_Wrap,
                    v_ZFUISizeType::e_Wrap
                    ));
        ret.width = zfmMax<zffloat>(ret.width, child->layoutMeasuredSize().width + parentMarginX + marginX);
        ret.height += prevSpace + child->layoutMeasuredSize().height + marginY;
        if(fixedSize != zfnull) {
            *fixedSize += prevSpace + marginY;
            if(layoutParam->weight() == 0) {
                *fixedSize += child->layoutMeasuredSize().height;
            }
        }
    }
    return ret;
}

// ============================================================
// layout
static void _ZFP_ZFUILinearLayout_layoutHorizontal(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &size
        ) {
    zffloat fixedSize = 0;
    zffloat totalWeight = 0;
    _ZFP_ZFUILinearLayout_measureHorizontal(
        parent, size, v_ZFUISizeType::e_Fill, &fixedSize, &totalWeight);
    zffloat flexibleSize = zfmMax<zffloat>(0, size.width - fixedSize);
    zfbool positiveDirection = (parent->orientation() == v_ZFUIOrientation::e_Left);
    zffloat usedSize = (positiveDirection ? parent->childMargin().left : parent->childMargin().right);
    for(zfindex i = 0; i < parent->childCount(); ++i) {
        ZFUIView *child = parent->childAt(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam();
        if(!child->visible() && !layoutParam->reserveSpace()) {
            continue;
        }
        zffloat prevSpace = (i > 0 ? parent->childSpace() : (zffloat)0);
        zffloat childSize = 0;
        if(layoutParam->weight() > 0) {
            childSize = layoutParam->weight() * flexibleSize / totalWeight;
        }
        else if(layoutParam->sizeParam().width == v_ZFUISizeType::e_Fill && totalWeight == 0) {
            childSize = flexibleSize + child->layoutMeasuredSize().width;
            flexibleSize = 0;
        }
        else {
            childSize = child->layoutMeasuredSize().width + ZFUIMarginGetWidth(layoutParam->margin());
        }
        child->viewFrame(ZFUIAlignApply(
                    layoutParam->align(),
                    ZFUIRectApplyMargin(ZFUIRectCreate(
                            positiveDirection
                            ? usedSize + prevSpace
                            : size.width - usedSize - prevSpace - childSize,
                            parent->childMargin().top,
                            childSize,
                            size.height - ZFUIMarginGetHeight(parent->childMargin())
                            ), layoutParam->margin()),
                    ZFUISizeCreate(
                        childSize - ZFUIMarginGetWidth(layoutParam->margin()),
                        child->layoutMeasuredSize().height)
                        ));
        usedSize += prevSpace + childSize;
    }
}
static void _ZFP_ZFUILinearLayout_layoutVertical(
        ZF_IN ZFUILinearLayout *parent
        , ZF_IN const ZFUISize &size
        ) {
    zffloat fixedSize = 0;
    zffloat totalWeight = 0;
    _ZFP_ZFUILinearLayout_measureVertical(
        parent, size, v_ZFUISizeType::e_Fill, &fixedSize, &totalWeight);
    zffloat flexibleSize = zfmMax<zffloat>(0, size.height - fixedSize);
    zfbool positiveDirection = (parent->orientation() == v_ZFUIOrientation::e_Top);
    zffloat usedSize = (positiveDirection ? parent->childMargin().top : parent->childMargin().bottom);
    for(zfindex i = 0; i < parent->childCount(); ++i) {
        ZFUIView *child = parent->childAt(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam();
        if(!child->visible() && !layoutParam->reserveSpace()) {
            continue;
        }
        zffloat prevSpace = (i > 0 ? parent->childSpace() : (zffloat)0);
        zffloat childSize = 0;
        if(layoutParam->weight() > 0) {
            childSize = layoutParam->weight() * flexibleSize / totalWeight;
        }
        else if(layoutParam->sizeParam().height == v_ZFUISizeType::e_Fill && totalWeight == 0) {
            childSize = flexibleSize + child->layoutMeasuredSize().height;
            flexibleSize = 0;
        }
        else {
            childSize = child->layoutMeasuredSize().height + ZFUIMarginGetHeight(layoutParam->margin());
        }
        child->viewFrame(ZFUIAlignApply(
                    layoutParam->align(),
                    ZFUIRectApplyMargin(ZFUIRectCreate(
                            parent->childMargin().left,
                            positiveDirection
                            ? usedSize + prevSpace
                            : size.height - usedSize - prevSpace - childSize,
                            size.width - ZFUIMarginGetWidth(parent->childMargin()),
                            childSize
                            ), layoutParam->margin()),
                    ZFUISizeCreate(
                        child->layoutMeasuredSize().width,
                        childSize - ZFUIMarginGetHeight(layoutParam->margin()))
                        ));
        usedSize += prevSpace + childSize;
    }
}

ZF_NAMESPACE_GLOBAL_END

