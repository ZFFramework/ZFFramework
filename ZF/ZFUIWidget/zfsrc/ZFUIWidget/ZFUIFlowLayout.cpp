#include "ZFUIFlowLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIFlowLayout)

// ============================================================
// ZFUIFlowLayoutParam
ZFOBJECT_REGISTER(ZFUIFlowLayoutParam)

/* ZFTAG_TRICKS: util for chained call to build view tree */
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIFlowLayoutParam, zfanyT<ZFUIFlowLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIFlowLayoutParam, zfanyT<ZFUIFlowLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIFlowLayout, zfanyT<ZFUIFlowLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIFlowLayout, zfanyT<ZFUIFlowLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

// ============================================================
// ZFUIFlowLayout
ZFOBJECT_REGISTER(ZFUIFlowLayout)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIOrientation, orientation) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIOrientation, orientationSecondary) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zfbool, gridMode) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIContentScaleType, childScaleType) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zfindex, childCountPerLine) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIMargin, childMargin) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zffloat, childSpaceX) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zffloat, childSpaceY) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}

// ============================================================
// override ZFUIView
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType heightParam
        , ZF_OUT ZFUISize &maxCellSize
        );
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType widthParam
        , ZF_OUT ZFUISize &maxCellSize
        );

static void _ZFP_ZFUIFlowLayout_layoutHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        );
static void _ZFP_ZFUIFlowLayout_layoutVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        );
static void _ZFP_ZFUIFlowLayout_layoutGridHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        , ZF_IN_OUT ZFUISize &maxCellSize
        );
static void _ZFP_ZFUIFlowLayout_layoutGridVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        , ZF_IN_OUT ZFUISize &maxCellSize
        );

void ZFUIFlowLayout::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    if(this->childCount() <= 0) {
        ret = ZFUISizeZero();
        return;
    }

    ZFUISize maxCellSize = ZFUISizeZero();
    switch(this->orientation()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            ret = _ZFP_ZFUIFlowLayout_measureHorizontal(this, sizeHint, sizeParam.height, maxCellSize);
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            ret = _ZFP_ZFUIFlowLayout_measureVertical(this, sizeHint, sizeParam.width, maxCellSize);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}
void ZFUIFlowLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    if(this->childCount() <= 0) {
        return;
    }

    ZFUISize maxCellSize = ZFUISizeZero();
    switch(this->orientation()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            if(this->gridMode()) {
                _ZFP_ZFUIFlowLayout_measureHorizontal(this, ZFUIRectGetSize(bounds), v_ZFUISizeType::e_Wrap, maxCellSize);
                _ZFP_ZFUIFlowLayout_layoutGridHorizontal(this, ZFUIRectGetSize(bounds), maxCellSize);
            }
            else {
                _ZFP_ZFUIFlowLayout_layoutHorizontal(this, ZFUIRectGetSize(bounds));
            }
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            if(this->gridMode()) {
                _ZFP_ZFUIFlowLayout_measureVertical(this, ZFUIRectGetSize(bounds), v_ZFUISizeType::e_Wrap, maxCellSize);
                _ZFP_ZFUIFlowLayout_layoutGridVertical(this, ZFUIRectGetSize(bounds), maxCellSize);
            }
            else {
                _ZFP_ZFUIFlowLayout_layoutVertical(this, ZFUIRectGetSize(bounds));
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

// ============================================================
// util
ZFUIOrientation _ZFP_ZFUIFlowLayout_orientationSecondary(ZF_IN ZFUIFlowLayout *parent) {
    switch(parent->orientation()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            switch(parent->orientationSecondary()) {
                case v_ZFUIOrientation::e_Left:
                case v_ZFUIOrientation::e_Right:
                    return v_ZFUIOrientation::e_Top;
                case v_ZFUIOrientation::e_Top:
                case v_ZFUIOrientation::e_Bottom:
                    return parent->orientationSecondary();
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return v_ZFUIOrientation::e_Left;
            }
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            switch(parent->orientationSecondary()) {
                case v_ZFUIOrientation::e_Left:
                case v_ZFUIOrientation::e_Right:
                    return parent->orientationSecondary();
                case v_ZFUIOrientation::e_Top:
                case v_ZFUIOrientation::e_Bottom:
                    return v_ZFUIOrientation::e_Left;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return v_ZFUIOrientation::e_Left;
            }
        default:
            ZFCoreCriticalShouldNotGoHere();
            return v_ZFUIOrientation::e_Left;
    }
}

// ============================================================
// measure line
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontalLine(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType heightParam
        , ZF_IN zfindex childIndexStart
        , ZF_OUT zfindex &childIndexStop
        , ZF_OUT ZFUISize &maxCellSize
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
                        ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - marginY))
                    ),
                ZFUISizeParamCreate(
                    v_ZFUISizeType::e_Wrap,
                    heightParam == v_ZFUISizeType::e_Fill && layoutParam->sizeParam().height == v_ZFUISizeType::e_Fill
                        ? v_ZFUISizeType::e_Fill
                        : v_ZFUISizeType::e_Wrap
                    ));
        if(parent->gridMode()) {
            maxCellSize.width = zfmMax<zffloat>(
                    maxCellSize.width
                    , child->layoutMeasuredSize().width + ZFUIMarginGetWidth(child->layoutParam()->margin())
                    );
            maxCellSize.height = zfmMax<zffloat>(
                    maxCellSize.height
                    , child->layoutMeasuredSize().height + ZFUIMarginGetHeight(child->layoutParam()->margin())
                    );
        }
        if(i > childIndexStart
                && sizeHint.width >= 0
                && lineSize.width + prevSpace + child->layoutMeasuredSize().width + marginX > sizeHint.width
                ) {
            childIndexStop = i;
            break;
        }
        lineSize.width += prevSpace + child->layoutMeasuredSize().width + marginX;
        lineSize.height = zfmMax<zffloat>(lineSize.height, child->layoutMeasuredSize().height + marginY);
        if(layoutParam->sizeParam().width == v_ZFUISizeType::e_Fill
                || (sizeHint.width >= 0 && lineSize.width >= sizeHint.width)
                || (parent->childCountPerLine() > 0 && i >= parent->childCountPerLine())
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
        , ZF_IN ZFUISizeType widthParam
        , ZF_IN zfindex childIndexStart
        , ZF_OUT zfindex &childIndexStop
        , ZF_OUT ZFUISize &maxCellSize
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
                        ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - marginY))
                        ),
                ZFUISizeParamCreate(
                    widthParam == v_ZFUISizeType::e_Fill && layoutParam->sizeParam().width == v_ZFUISizeType::e_Fill
                        ? v_ZFUISizeType::e_Fill
                        : v_ZFUISizeType::e_Wrap,
                    v_ZFUISizeType::e_Wrap
                    ));
        if(parent->gridMode()) {
            maxCellSize.width = zfmMax<zffloat>(
                    maxCellSize.width
                    , child->layoutMeasuredSize().width + ZFUIMarginGetWidth(child->layoutParam()->margin())
                    );
            maxCellSize.height = zfmMax<zffloat>(
                    maxCellSize.height
                    , child->layoutMeasuredSize().height + ZFUIMarginGetHeight(child->layoutParam()->margin())
                    );
        }
        if(i > childIndexStart
                && sizeHint.height >= 0
                && lineSize.height + prevSpace + child->layoutMeasuredSize().height + marginY > sizeHint.height
                ) {
            childIndexStop = i;
            break;
        }
        lineSize.width = zfmMax<zffloat>(lineSize.width, child->layoutMeasuredSize().width + marginX);
        lineSize.height += prevSpace + child->layoutMeasuredSize().height + marginY;
        if(layoutParam->sizeParam().height == v_ZFUISizeType::e_Fill
                || (sizeHint.height >= 0 && lineSize.height >= sizeHint.height)
                || (parent->childCountPerLine() > 0 && i >= parent->childCountPerLine())
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
        , ZF_IN ZFUISizeType heightParam
        , ZF_OUT ZFUISize &maxCellSize
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
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureHorizontalLine(parent, lineSizeHint, heightParam, childIndex, childIndex, maxCellSize);
        ret.width = zfmMax<zffloat>(ret.width, lineSize.width + parentMarginX);
        ret.height += prevLineSpace + lineSize.height;
    }

    if(parent->gridMode()) {
        ZFUISize contentSize = ZFUISizeCreate(
                zfmMax<zffloat>(
                    (zffloat)(sizeHint.width - ZFUIMarginGetWidth(parent->childMargin()))
                    , maxCellSize.width
                    ),
                zfmMax<zffloat>(
                    (zffloat)(sizeHint.height - ZFUIMarginGetHeight(parent->childMargin()))
                    , maxCellSize.height
                    ));
        zfindex childCountPerLine = zfmMax<zfindex>(1, maxCellSize.width > 0
                ? (zfindex)((contentSize.width + parent->childSpaceX()) / (maxCellSize.width + parent->childSpaceX()))
                : 0
                );
        if(childCountPerLine > parent->childCount()) {
            childCountPerLine = parent->childCount();
        }
        if(parent->childCountPerLine() > 0 && childCountPerLine > parent->childCountPerLine()) {
            childCountPerLine = parent->childCountPerLine();
        }
        zfindex lineCount = (parent->childCount() + childCountPerLine - 1) / childCountPerLine;
        ret.width = (maxCellSize.width + parent->childSpaceX()) * childCountPerLine - parent->childSpaceX();
        ret.height = (maxCellSize.height + parent->childSpaceY()) * lineCount - parent->childSpaceY();
    }

    ret.width += parentMarginX;
    ret.height += parentMarginY;
    return ret;
}
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN ZFUISizeType widthParam
        , ZF_OUT ZFUISize &maxCellSize
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
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureVerticalLine(parent, lineSizeHint, widthParam, childIndex, childIndex, maxCellSize);
        ret.width += prevLineSpace + lineSize.width;
        ret.height = zfmMax<zffloat>(ret.height, lineSize.height + parentMarginY);
    }

    if(parent->gridMode()) {
        ZFUISize contentSize = ZFUISizeCreate(
                zfmMax<zffloat>(
                    (zffloat)(sizeHint.width - ZFUIMarginGetWidth(parent->childMargin()))
                    , maxCellSize.width
                    ),
                zfmMax<zffloat>(
                    (zffloat)(sizeHint.height - ZFUIMarginGetHeight(parent->childMargin()))
                    , maxCellSize.height
                    ));
        zfindex childCountPerLine = zfmMax<zfindex>(1, maxCellSize.height > 0
                ? (zfindex)((contentSize.height + parent->childSpaceY()) / (maxCellSize.height + parent->childSpaceY()))
                : 0
                );
        if(childCountPerLine > parent->childCount()) {
            childCountPerLine = parent->childCount();
        }
        if(parent->childCountPerLine() > 0 && childCountPerLine > parent->childCountPerLine()) {
            childCountPerLine = parent->childCountPerLine();
        }
        zfindex lineCount = (parent->childCount() + childCountPerLine - 1) / childCountPerLine;
        ret.width = (maxCellSize.width + parent->childSpaceX()) * lineCount - parent->childSpaceX();
        ret.height = (maxCellSize.height + parent->childSpaceY()) * childCountPerLine - parent->childSpaceY();
    }

    ret.width += parentMarginX;
    ret.height += parentMarginY;
    return ret;
}

// ============================================================
// layout
static void _ZFP_ZFUIFlowLayout_layoutHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        ) {
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->childMargin());
    zfbool positiveDirectionX = (parent->orientation() == v_ZFUIOrientation::e_Left);
    zfbool positiveDirectionY = (_ZFP_ZFUIFlowLayout_orientationSecondary(parent) == v_ZFUIOrientation::e_Top);
    zffloat lineSizeUsed = (positiveDirectionY ? parent->childMargin().top : parent->childMargin().bottom);
    zffloat lineTailMargin = (positiveDirectionY ? parent->childMargin().bottom : parent->childMargin().top);
    zfindex childIndex = 0;
    ZFUISize lineSizeHint = ZFUISizeCreate(size.width - parentMarginX, 0);
    ZFUISize maxCellSize = ZFUISizeZero();
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        zffloat prevLineSpace = (lineIndex > 0 ? parent->childSpaceY() : (zffloat)0);
        zfindex childIndexStart = childIndex;
        lineSizeHint.height = size.height - lineSizeUsed - lineTailMargin;
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureHorizontalLine(parent, lineSizeHint, v_ZFUISizeType::e_Fill, childIndexStart, childIndex, maxCellSize);

        zffloat usedSize = (positiveDirectionX ? parent->childMargin().left : parent->childMargin().right);
        for(zfindex i = childIndexStart; i < childIndex; ++i) {
            ZFUIView *child = parent->childAt(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParam();
            if(!child->visible() && !layoutParam->reserveSpace()) {
                continue ;
            }
            zffloat prevSpace = (i > childIndexStart ? parent->childSpaceX() : (zffloat)0);
            zffloat childSize = 0;
            if(layoutParam->sizeParam().width == v_ZFUISizeType::e_Fill) {
                childSize = zfmMax<zffloat>(0, size.width - usedSize - prevSpace);
            }
            else {
                childSize = child->layoutMeasuredSize().width;
            }
            child->viewFrame(ZFUIContentScaleTypeApply(
                        parent->childScaleType(),
                        ZFUIRectApplyMargin(ZFUIRectCreate(
                                positiveDirectionX
                                ? usedSize + prevSpace
                                : size.width - usedSize - prevSpace - childSize,
                                positiveDirectionY
                                ? lineSizeUsed + prevLineSpace
                                : size.height - lineSizeUsed - prevLineSpace - lineSize.height,
                                childSize,
                                lineSize.height
                                ), layoutParam->margin()),
                        ZFUISizeCreate(childSize, child->layoutMeasuredSize().height),
                        layoutParam->align()
                        ));
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
    zfbool positiveDirectionX = (_ZFP_ZFUIFlowLayout_orientationSecondary(parent) == v_ZFUIOrientation::e_Left);
    zfbool positiveDirectionY = (parent->orientation() == v_ZFUIOrientation::e_Top);
    zffloat lineSizeUsed = (positiveDirectionX ? parent->childMargin().left : parent->childMargin().right);
    zffloat lineTailMargin = (positiveDirectionX ? parent->childMargin().right : parent->childMargin().left);
    zfindex childIndex = 0;
    ZFUISize lineSizeHint = ZFUISizeCreate(0, size.height - parentMarginY);
    ZFUISize maxCellSize = ZFUISizeZero();
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        zffloat prevLineSpace = (lineIndex > 0 ? parent->childSpaceX() : (zffloat)0);
        zfindex childIndexStart = childIndex;
        lineSizeHint.width = size.width - lineSizeUsed - lineTailMargin;
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureVerticalLine(parent, lineSizeHint, v_ZFUISizeType::e_Fill, childIndexStart, childIndex, maxCellSize);

        zffloat usedSize = (positiveDirectionY ? parent->childMargin().top : parent->childMargin().bottom);
        for(zfindex i = childIndexStart; i < childIndex; ++i) {
            ZFUIView *child = parent->childAt(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParam();
            if(!child->visible() && !layoutParam->reserveSpace()) {
                continue ;
            }
            zffloat prevSpace = (i > childIndexStart ? parent->childSpaceY() : (zffloat)0);
            zffloat childSize = 0;
            if(layoutParam->sizeParam().height == v_ZFUISizeType::e_Fill) {
                childSize = zfmMax<zffloat>(0, size.height - usedSize - prevSpace);
            }
            else {
                childSize = child->layoutMeasuredSize().height;
            }
            child->viewFrame(ZFUIContentScaleTypeApply(
                        parent->childScaleType(),
                        ZFUIRectApplyMargin(ZFUIRectCreate(
                                positiveDirectionX
                                ? lineSizeUsed + prevLineSpace
                                : size.width - lineSizeUsed - prevLineSpace - lineSize.width,
                                positiveDirectionY
                                ? usedSize + prevSpace
                                : size.height - usedSize - prevSpace - childSize,
                                lineSize.width,
                                childSize
                                ), layoutParam->margin()),
                        ZFUISizeCreate(child->layoutMeasuredSize().width, childSize),
                        layoutParam->align()
                        ));
            usedSize += prevSpace + childSize;
        }

        lineSizeUsed += prevLineSpace + lineSize.width;
    }
}

static void _ZFP_ZFUIFlowLayout_layoutGridHorizontal(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        , ZF_IN_OUT ZFUISize &maxCellSize
        ) {
    ZFUISize contentSize = ZFUISizeCreate(
            size.width - ZFUIMarginGetWidth(parent->childMargin()),
            size.height - ZFUIMarginGetHeight(parent->childMargin())
            );
    zfindex childCountPerLine = (maxCellSize.width + parent->childSpaceX() > 0
            ? (zfindex)((contentSize.width + parent->childSpaceX()) / (maxCellSize.width + parent->childSpaceX()))
            : 0
            );
    if(childCountPerLine > 0) {
        maxCellSize.width = (contentSize.width + parent->childSpaceX()) / childCountPerLine - parent->childSpaceX();
        if(parent->childCountPerLine() > 0 && childCountPerLine > parent->childCountPerLine()) {
            childCountPerLine = parent->childCountPerLine();
        }
    }
    else {
        childCountPerLine = 1;
    }

    zffloat childStart, childOffset;
    if(parent->orientation() == v_ZFUIOrientation::e_Right) {
        childStart = size.width - parent->childMargin().right - maxCellSize.width;
        childOffset = 0 - maxCellSize.width - parent->childSpaceX();
    }
    else {
        childStart = parent->childMargin().left;
        childOffset = maxCellSize.width + parent->childSpaceX();
    }
    zffloat lineStart, lineOffset;
    if(parent->orientationSecondary() == v_ZFUIOrientation::e_Bottom) {
        lineStart = size.height - parent->childMargin().bottom - maxCellSize.height;
        lineOffset = 0 - maxCellSize.height - parent->childSpaceY();
    }
    else {
        lineStart = parent->childMargin().top;
        lineOffset = maxCellSize.height + parent->childSpaceY();
    }
    zffloat childStartSaved = childStart;

    zfindex childIndex = 0;
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        childStart = childStartSaved;
        for(zfindex i = 0; i < childCountPerLine && childIndex + i < parent->childCount(); ++i) {
            ZFUIView *child = parent->childAt(childIndex + i);
            child->viewFrame(ZFUIContentScaleTypeApply(
                        parent->childScaleType()
                        , ZFUIRectApplyMargin(ZFUIRectCreate(
                                childStart
                                , lineStart
                                , maxCellSize.width
                                , maxCellSize.height
                                ), child->layoutParam()->margin())
                        , child->layoutMeasuredSize()
                        , child->layoutParam()->align()
                        ));
            childStart += childOffset;
        }
        childIndex += childCountPerLine;
        lineStart += lineOffset;
    }
}
static void _ZFP_ZFUIFlowLayout_layoutGridVertical(
        ZF_IN ZFUIFlowLayout *parent
        , ZF_IN const ZFUISize &size
        , ZF_IN_OUT ZFUISize &maxCellSize
        ) {
    ZFUISize contentSize = ZFUISizeCreate(
            size.width - ZFUIMarginGetWidth(parent->childMargin()),
            size.height - ZFUIMarginGetHeight(parent->childMargin())
            );

    zfindex childCountPerLine = (maxCellSize.height + parent->childSpaceY() > 0
            ? (zfindex)((contentSize.height + parent->childSpaceY()) / (maxCellSize.height + parent->childSpaceY()))
            : 0
            );
    if(childCountPerLine > 0) {
        maxCellSize.height = (contentSize.height + parent->childSpaceY()) / childCountPerLine - parent->childSpaceY();
        if(parent->childCountPerLine() > 0 && childCountPerLine > parent->childCountPerLine()) {
            childCountPerLine = parent->childCountPerLine();
        }
    }
    else {
        childCountPerLine = 1;
    }

    zffloat childStart, childOffset;
    if(parent->orientation() == v_ZFUIOrientation::e_Bottom) {
        childStart = size.height - parent->childMargin().bottom - maxCellSize.height;
        childOffset = 0 - maxCellSize.height - parent->childSpaceY();
    }
    else {
        childStart = parent->childMargin().top;
        childOffset = maxCellSize.height + parent->childSpaceY();
    }
    zffloat lineStart, lineOffset;
    if(parent->orientationSecondary() == v_ZFUIOrientation::e_Right) {
        lineStart = size.width - parent->childMargin().right - maxCellSize.width;
        lineOffset = 0 - maxCellSize.width - parent->childSpaceX();
    }
    else {
        lineStart = parent->childMargin().left;
        lineOffset = maxCellSize.width + parent->childSpaceX();
    }
    zffloat childStartSaved = childStart;

    zfindex childIndex = 0;
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex) {
        childStart = childStartSaved;
        for(zfindex i = 0; i < childCountPerLine && childIndex + i < parent->childCount(); ++i) {
            ZFUIView *child = parent->childAt(childIndex + i);
            child->viewFrame(ZFUIContentScaleTypeApply(
                        parent->childScaleType()
                        , ZFUIRectApplyMargin(ZFUIRectCreate(
                                lineStart
                                , childStart
                                , maxCellSize.width
                                , maxCellSize.height
                                ), child->layoutParam()->margin())
                        , child->layoutMeasuredSize()
                        , child->layoutParam()->align()
                        ));
            childStart += childOffset;
        }
        childIndex += childCountPerLine;
        lineStart += lineOffset;
    }
}

ZF_NAMESPACE_GLOBAL_END

