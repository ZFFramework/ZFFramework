#include "ZFUIFlowLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIFlowLayout)

// ============================================================
// ZFUIFlowLayoutParam
ZFOBJECT_REGISTER(ZFUIFlowLayoutParam)

// ============================================================
// ZFUIFlowLayout
ZFOBJECT_REGISTER(ZFUIFlowLayout)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIOrientationEnum, orientationMain) {
    if(this->orientationMain() != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIOrientationEnum, orientationSecondary) {
    if(this->orientationSecondary() != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIMargin, childMargin) {
    if(this->childMargin() != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zffloat, childSpaceX) {
    if(this->childSpaceX() != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zffloat, childSpaceY) {
    if(this->childSpaceY() != propertyValueOld) {
        this->layoutRequest();
    }
}

ZFMETHOD_DEFINE_2(ZFUIFlowLayout, zfanyT<ZFUIFlowLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        ) {
    return this->childWithParam(view, zfnull, atIndex);
}

// ============================================================
// override ZFUIView
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        );
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        );
void ZFUIFlowLayout::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    switch(this->orientationMain()) {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            ret = _ZFP_ZFUIFlowLayout_measureHorizontal(this, sizeHint);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            ret = _ZFP_ZFUIFlowLayout_measureVertical(this, sizeHint);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

static void _ZFP_ZFUIFlowLayout_layoutHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        );
static void _ZFP_ZFUIFlowLayout_layoutVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        );
void ZFUIFlowLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    switch(this->orientationMain()) {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            _ZFP_ZFUIFlowLayout_layoutHorizontal(this, ZFUIRectGetSize(bounds));
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            _ZFP_ZFUIFlowLayout_layoutVertical(this, ZFUIRectGetSize(bounds));
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

// ============================================================
// util
ZFUIOrientationEnum _ZFP_ZFUIFlowLayout_orientationSecondary(ZF_IN ZFUIFlowLayout *parent) {
    switch(parent->orientationMain()) {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            switch(parent->orientationSecondary()) {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    return ZFUIOrientation::e_Top;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    return parent->orientationSecondary();
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return ZFUIOrientation::e_Left;
            }
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            switch(parent->orientationSecondary()) {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    return parent->orientationSecondary();
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    return ZFUIOrientation::e_Left;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return ZFUIOrientation::e_Left;
            }
        default:
            ZFCoreCriticalShouldNotGoHere();
            return ZFUIOrientation::e_Left;
    }
}

// ============================================================
// measure line
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontalLine(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN zfindex childIndexStart
        , ZF_OUT zfindex &childIndexStop
        ) {
    ZFUISize lineSize = ZFUISizeZero();
    childIndexStop = parent->childCount();
    for(zfindex i = childIndexStart; i < parent->childCount(); ++i) {
        ZFUIView *child = parent->childAt(i);
        ZFUIFlowLayoutParam *layoutParam = child->layoutParam();
        if(!child->visible() && !layoutParam->reserveSpace()) {
            continue ;
        }
        zffloat prevSpace = (i > childIndexStart ? parent->childSpaceX() : (zffloat)0);

        zffloat marginX = ZFUIMarginGetWidth(layoutParam->margin());
        zffloat marginY = ZFUIMarginGetHeight(layoutParam->margin());
        child->layoutMeasure(
            ZFUISizeCreate(
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().width,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - marginX)),
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().height,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - marginY))),
            ZFUISizeParamCreate(ZFUISizeType::e_Wrap, layoutParam->sizeParam().height));
        if(sizeHint.width >= 0 && lineSize.width + prevSpace + child->layoutMeasuredSize().width + marginX > sizeHint.width) {
            childIndexStop = i;
            break;
        }
        lineSize.width += prevSpace + child->layoutMeasuredSize().width + marginX;
        lineSize.height = zfmMax<zffloat>(lineSize.height, child->layoutMeasuredSize().height + marginY);
        if(layoutParam->sizeParam().width == ZFUISizeType::e_Fill
                || (sizeHint.width >= 0 && lineSize.width == sizeHint.width)
                ) {
            childIndexStop = i + 1;
            break;
        }
    }
    return lineSize;
}
static ZFUISize _ZFP_ZFUIFlowLayout_measureVerticalLine(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN zfindex childIndexStart
        , ZF_OUT zfindex &childIndexStop
        ) {
    ZFUISize lineSize = ZFUISizeZero();
    childIndexStop = parent->childCount();
    for(zfindex i = childIndexStart; i < parent->childCount(); ++i) {
        ZFUIView *child = parent->childAt(i);
        ZFUIFlowLayoutParam *layoutParam = child->layoutParam();
        if(!child->visible() && !layoutParam->reserveSpace()) {
            continue ;
        }
        zffloat prevSpace = (i > childIndexStart ? parent->childSpaceY() : (zffloat)0);

        zffloat marginX = ZFUIMarginGetWidth(layoutParam->margin());
        zffloat marginY = ZFUIMarginGetHeight(layoutParam->margin());
        child->layoutMeasure(
            ZFUISizeCreate(
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().width,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - marginX)),
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().height,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - marginY))),
            ZFUISizeParamCreate(layoutParam->sizeParam().width, ZFUISizeType::e_Wrap));
        if(sizeHint.height >= 0 && lineSize.height + prevSpace + child->layoutMeasuredSize().height + marginY > sizeHint.height) {
            childIndexStop = i;
            break;
        }
        lineSize.width = zfmMax<zffloat>(lineSize.width, child->layoutMeasuredSize().width + marginX);
        lineSize.height += prevSpace + child->layoutMeasuredSize().height + marginY;
        if(layoutParam->sizeParam().height == ZFUISizeType::e_Fill
                || (sizeHint.height >= 0 && lineSize.height == sizeHint.height)
                ) {
            childIndexStop = i + 1;
            break;
        }
    }
    return lineSize;
}

// ============================================================
// measure
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        ) {
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->childMargin());
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->childMargin());
    ZFUISize ret = ZFUISizeCreate(0, parentMarginY);
    zfindex childIndex = 0;
    ZFUISize sizeHintTmp = ZFUISizeCreate(
        ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - parentMarginX),
        sizeHint.height);
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        zffloat prevLineSpace = (lineIndex > 0 ? parent->childSpaceY() : (zffloat)0);
        ZFUISize lineSizeHint = ZFUISizeCreate(
            sizeHintTmp.width,
            ZFUILayoutParam::sizeHintOffset(sizeHintTmp.height, 0 - prevLineSpace - ret.height));
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureHorizontalLine(parent, lineSizeHint, childIndex, childIndex);
        ret.width = zfmMax<zffloat>(ret.width, lineSize.width + parentMarginX);
        ret.height += prevLineSpace + lineSize.height;
    }
    return ret;
}
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        ) {
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->childMargin());
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->childMargin());
    ZFUISize ret = ZFUISizeCreate(parentMarginX, 0);
    zfindex childIndex = 0;
    ZFUISize sizeHintTmp = ZFUISizeCreate(
        sizeHint.width,
        ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - parentMarginY));
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        zffloat prevLineSpace = (lineIndex > 0 ? parent->childSpaceX() : (zffloat)0);
        ZFUISize lineSizeHint = ZFUISizeCreate(
            ZFUILayoutParam::sizeHintOffset(sizeHintTmp.width, 0 - prevLineSpace - ret.width),
            sizeHintTmp.width);
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureVerticalLine(parent, lineSizeHint, childIndex, childIndex);
        ret.width += prevLineSpace + lineSize.width;
        ret.height = zfmMax<zffloat>(ret.height, lineSize.height + parentMarginY);
    }
    return ret;
}

// ============================================================
// layout
static void _ZFP_ZFUIFlowLayout_layoutHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        ) {
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->childMargin());
    zfbool positiveDirectionX = (parent->orientationMain() == ZFUIOrientation::e_Left);
    zfbool positiveDirectionY = (_ZFP_ZFUIFlowLayout_orientationSecondary(parent) == ZFUIOrientation::e_Top);
    zffloat lineSizeUsed = (positiveDirectionY ? parent->childMargin().top : parent->childMargin().bottom);
    zffloat lineTailMargin = (positiveDirectionY ? parent->childMargin().bottom : parent->childMargin().top);
    zfindex childIndex = 0;
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        zffloat prevLineSpace = (lineIndex > 0 ? parent->childSpaceY() : (zffloat)0);
        zfindex childIndexStart = childIndex;
        ZFUISize lineSizeHint = ZFUISizeCreate(
            size.width - parentMarginX,
            size.height - lineSizeUsed - lineTailMargin);
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureHorizontalLine(parent, lineSizeHint, childIndexStart, childIndex);

        zffloat usedSize = (positiveDirectionX ? parent->childMargin().left : parent->childMargin().right);
        for(zfindex i = childIndexStart; i < childIndex; ++i) {
            ZFUIView *child = parent->childAt(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParam();
            if(!child->visible() && !layoutParam->reserveSpace()) {
                continue ;
            }
            zffloat prevSpace = (i > childIndexStart ? parent->childSpaceX() : (zffloat)0);
            zffloat childSize = 0;
            if(layoutParam->sizeParam().width == ZFUISizeType::e_Fill) {
                childSize = zfmMax<zffloat>(0, size.width - usedSize - prevSpace);
            }
            else {
                childSize = child->layoutMeasuredSize().width;
            }
            child->viewFrame(ZFUIAlignApply(
                layoutParam->align(),
                ZFUIRectCreate(
                    positiveDirectionX
                        ? usedSize + prevSpace
                        : size.width - usedSize - prevSpace - childSize,
                    positiveDirectionY
                        ? lineSizeUsed + prevLineSpace
                        : size.height - lineSizeUsed - prevLineSpace - lineSize.height,
                    childSize,
                    lineSize.height),
                ZFUISizeCreate(childSize, child->layoutMeasuredSize().height),
                layoutParam->margin()));
            usedSize += prevSpace + childSize;
        }

        lineSizeUsed += prevLineSpace + lineSize.height;
    }
}
static void _ZFP_ZFUIFlowLayout_layoutVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        ) {
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->childMargin());
    zfbool positiveDirectionX = (_ZFP_ZFUIFlowLayout_orientationSecondary(parent) == ZFUIOrientation::e_Left);
    zfbool positiveDirectionY = (parent->orientationMain() == ZFUIOrientation::e_Top);
    zffloat lineSizeUsed = (positiveDirectionX ? parent->childMargin().left : parent->childMargin().right);
    zffloat lineTailMargin = (positiveDirectionX ? parent->childMargin().right : parent->childMargin().left);
    zfindex childIndex = 0;
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        zffloat prevLineSpace = (lineIndex > 0 ? parent->childSpaceX() : (zffloat)0);
        zfindex childIndexStart = childIndex;
        ZFUISize lineSizeHint = ZFUISizeCreate(
            size.width - lineSizeUsed - lineTailMargin,
            size.height - parentMarginY);
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureVerticalLine(parent, lineSizeHint, childIndexStart, childIndex);

        zffloat usedSize = (positiveDirectionY ? parent->childMargin().top : parent->childMargin().bottom);
        for(zfindex i = childIndexStart; i < childIndex; ++i) {
            ZFUIView *child = parent->childAt(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParam();
            if(!child->visible() && !layoutParam->reserveSpace()) {
                continue ;
            }
            zffloat prevSpace = (i > childIndexStart ? parent->childSpaceY() : (zffloat)0);
            zffloat childSize = 0;
            if(layoutParam->sizeParam().height == ZFUISizeType::e_Fill) {
                childSize = zfmMax<zffloat>(0, size.height - usedSize - prevSpace);
            }
            else {
                childSize = child->layoutMeasuredSize().height;
            }
            child->viewFrame(ZFUIAlignApply(
                layoutParam->align(),
                ZFUIRectCreate(
                    positiveDirectionX
                        ? lineSizeUsed + prevLineSpace
                        : size.width - lineSizeUsed - prevLineSpace - lineSize.width,
                    positiveDirectionY
                        ? usedSize + prevSpace
                        : size.height - usedSize - prevSpace - childSize,
                    lineSize.width,
                    childSize),
                ZFUISizeCreate(child->layoutMeasuredSize().width, childSize),
                layoutParam->margin()));
            usedSize += prevSpace + childSize;
        }

        lineSizeUsed += prevLineSpace + lineSize.width;
    }
}

ZF_NAMESPACE_GLOBAL_END

