#include "ZFUIListCellView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIListCellView)

void ZFUIListCellView::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();

    zffloat itemMargin = ZFUIGlobalStyle::DefaultStyle()->itemMargin();

    this->internalBgViewAdd(this->cellIconContainer());
    this->cellIconContainer()->layoutParam()->align(ZFUIAlign::e_Center);
    this->cellIconContainer()->layoutParam()->margin(ZFUIMarginCreate(itemMargin, itemMargin, 0, itemMargin));

    this->internalBgViewAdd(this->cellCenterContainer());
    this->cellCenterContainer()->layoutParam()->align(ZFUIAlign::e_Center);
    this->cellCenterContainer()->layoutParam()->margin(ZFUIMarginCreate(itemMargin));

    this->internalBgViewAdd(this->cellAccessoryContainer());
    this->cellAccessoryContainer()->layoutParam()->align(ZFUIAlign::e_Center);
    this->cellAccessoryContainer()->layoutParam()->margin(ZFUIMarginCreate(0, itemMargin, itemMargin, itemMargin));
}

static void _ZFP_ZFUIListCellView_measureContent(
        ZF_IN ZFUIListCellView *view
        , ZF_IN const ZFUISize &sizeHint
        , ZF_OUT ZFUISize &cellIconSize
        , ZF_OUT ZFUISize &cellCenterSize
        , ZF_OUT ZFUISize &cellAccessorySize
        ) {
    view->cellIconContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellIconSize = view->cellIconContainer()->layoutMeasuredSize();
    if(cellIconSize.width > 0) {
        cellIconSize = ZFUISizeApplyMarginReversely(cellIconSize, view->cellIconContainer()->layoutParam()->margin());
    }

    view->cellAccessoryContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellAccessorySize = view->cellAccessoryContainer()->layoutMeasuredSize();
    if(cellAccessorySize.width > 0) {
        cellAccessorySize = ZFUISizeApplyMarginReversely(cellAccessorySize, view->cellAccessoryContainer()->layoutParam()->margin());
    }

    ZFUISize cellCenterSizeHint = sizeHint;
    if(cellCenterSizeHint.width >= 0) {
        cellCenterSizeHint.width -= cellIconSize.width + cellAccessorySize.width;
        cellCenterSizeHint.width -= ZFUIMarginGetWidth(view->cellCenterContainer()->layoutParam()->margin());
        if(cellCenterSizeHint.width < 0) {
            cellCenterSizeHint.width = 0;
        }
    }
    view->cellCenterContainer()->layoutMeasure(cellCenterSizeHint, ZFUISizeParamFillWrap());
    cellCenterSize = view->cellCenterContainer()->layoutMeasuredSize();
    cellCenterSize.width += ZFUIMarginGetWidth(view->cellCenterContainer()->layoutParam()->margin());
}
void ZFUIListCellView::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUIListCellView_measureContent(this, sizeHint, cellIconSize, cellCenterSize, cellAccessorySize);
    ret.width = cellIconSize.width + cellCenterSize.width + cellAccessorySize.width;
    ret.height = zfmMax(zfmMax(cellIconSize.height, cellCenterSize.height), cellAccessorySize.height);
}
void ZFUIListCellView::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);

    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUIListCellView_measureContent(this, ZFUIRectGetSize(bounds), cellIconSize, cellCenterSize, cellAccessorySize);

    this->cellIconContainer()->viewFrame(ZFUIAlignApply(
        ZFUIAlign::e_Left | ZFUIAlignGetY(this->cellIconContainer()->layoutParam()->align()),
        bounds,
        this->cellIconContainer()->layoutMeasuredSize(),
        this->cellIconContainer()->layoutParam()->margin()));

    ZFUIMargin cellCenterMargin = this->cellCenterContainer()->layoutParam()->margin();
    cellCenterMargin.left += cellIconSize.width;
    cellCenterMargin.right += cellAccessorySize.width;
    this->cellCenterContainer()->viewFrame(ZFUIAlignApply(
        ZFUIAlign::e_Left | ZFUIAlignGetY(this->cellCenterContainer()->layoutParam()->align()),
        bounds,
        this->cellCenterContainer()->layoutMeasuredSize(),
        cellCenterMargin));

    this->cellAccessoryContainer()->viewFrame(ZFUIAlignApply(
        ZFUIAlign::e_Right | ZFUIAlignGetY(this->cellAccessoryContainer()->layoutParam()->align()),
        bounds,
        this->cellAccessoryContainer()->layoutMeasuredSize(),
        this->cellAccessoryContainer()->layoutParam()->margin()));
}
zfbool ZFUIListCellView::internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
    if(internalView == this->cellIconContainer()
            || internalView == this->cellCenterContainer()
            || internalView == this->cellAccessoryContainer()
            ) {
        return zffalse;
    }
    else {
        return zfsuper::internalViewShouldLayout(internalView);
    }
}

ZF_NAMESPACE_GLOBAL_END

