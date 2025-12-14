#include "ZFUICellView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUICellView)

void ZFUICellView::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();

    zffloat itemMargin = ZFUIGlobalStyle::DefaultStyle()->itemMargin();

    this->internalBgViewAdd(this->cellIconContainer());
    this->cellIconContainer()->layoutParam()->align(v_ZFUIAlign::e_Center);
    this->cellIconContainer()->layoutParam()->margin(ZFUIMarginCreate(itemMargin, itemMargin, 0, itemMargin));

    this->internalBgViewAdd(this->cellCenterContainer());
    this->cellCenterContainer()->layoutParam()->align(v_ZFUIAlign::e_Center);
    this->cellCenterContainer()->layoutParam()->margin(ZFUIMarginCreate(itemMargin));

    this->internalBgViewAdd(this->cellAccessoryContainer());
    this->cellAccessoryContainer()->layoutParam()->align(v_ZFUIAlign::e_Center);
    this->cellAccessoryContainer()->layoutParam()->margin(ZFUIMarginCreate(0, itemMargin, itemMargin, itemMargin));
}

static void _ZFP_ZFUICellView_measureContent(
        ZF_IN ZFUICellView *view
        , ZF_IN const ZFUISize &sizeHint
        , ZF_OUT ZFUISize &cellIconSize
        , ZF_OUT ZFUISize &cellCenterSize
        , ZF_OUT ZFUISize &cellAccessorySize
        ) {
    view->cellIconContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellIconSize = view->cellIconContainer()->layoutMeasuredSize();
    if(cellIconSize.width > 0) {
        ZFUISizeApplyMarginReverselyT(cellIconSize, cellIconSize, view->cellIconContainer()->layoutParam()->margin());
    }

    view->cellAccessoryContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellAccessorySize = view->cellAccessoryContainer()->layoutMeasuredSize();
    if(cellAccessorySize.width > 0) {
        ZFUISizeApplyMarginReverselyT(cellAccessorySize, cellAccessorySize, view->cellAccessoryContainer()->layoutParam()->margin());
    }

    ZFUISize cellCenterSizeHint = sizeHint;
    if(cellCenterSizeHint.width >= 0) {
        cellCenterSizeHint.width -= cellIconSize.width + cellAccessorySize.width;
        if(cellCenterSizeHint.width < 0) {
            cellCenterSizeHint.width = 0;
        }
    }
    view->cellCenterContainer()->layoutMeasure(cellCenterSizeHint, ZFUISizeParamFillWrap());
    cellCenterSize = view->cellCenterContainer()->layoutMeasuredSize();
    cellCenterSize.width += ZFUIMarginGetWidth(view->cellCenterContainer()->layoutParam()->margin());
}
void ZFUICellView::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUICellView_measureContent(this, sizeHint, cellIconSize, cellCenterSize, cellAccessorySize);
    ret.width = cellIconSize.width + cellCenterSize.width + cellAccessorySize.width;
    ret.height = zfmMax(zfmMax(cellIconSize.height, cellCenterSize.height), cellAccessorySize.height);
}
void ZFUICellView::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);

    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUICellView_measureContent(this, ZFUIRectGetSize(bounds), cellIconSize, cellCenterSize, cellAccessorySize);

    this->cellIconContainer()->viewFrame(ZFUIAlignApply(
                v_ZFUIAlign::e_Left | ZFUIAlignGetY(this->cellIconContainer()->layoutParam()->align()),
                ZFUIRectApplyMargin(bounds, this->cellIconContainer()->layoutParam()->margin()),
                this->cellIconContainer()->layoutMeasuredSize()
                ));

    ZFUIMargin cellCenterMargin = this->cellCenterContainer()->layoutParam()->margin();
    cellCenterMargin.left += cellIconSize.width;
    cellCenterMargin.right += cellAccessorySize.width;
    this->cellCenterContainer()->viewFrame(ZFUIAlignApply(
                v_ZFUIAlign::e_Left | ZFUIAlignGetY(this->cellCenterContainer()->layoutParam()->align()),
                ZFUIRectApplyMargin(bounds, cellCenterMargin),
                this->cellCenterContainer()->layoutMeasuredSize()
                ));

    this->cellAccessoryContainer()->viewFrame(ZFUIAlignApply(
                v_ZFUIAlign::e_Right | ZFUIAlignGetY(this->cellAccessoryContainer()->layoutParam()->align()),
                ZFUIRectApplyMargin(bounds, this->cellAccessoryContainer()->layoutParam()->margin()),
                this->cellAccessoryContainer()->layoutMeasuredSize()
                ));
}
zfbool ZFUICellView::internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
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

