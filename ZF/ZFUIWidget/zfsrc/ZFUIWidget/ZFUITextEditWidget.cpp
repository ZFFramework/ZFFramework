#include "ZFUITextEditWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextEditWidget
ZFOBJECT_REGISTER(ZFUITextEditWidget)
ZFSTYLE_DEFAULT_DEFINE(ZFUITextEditWidget)

ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEditWidget, zfanyT<ZFUIImageView>, backgroundView) {
    zfobj<ZFUIImageView> backgroundView;
    propertyValue = backgroundView;
    backgroundView->image(zfres("ZFUIWidget/ZFUITextEditWidget_background.xml"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEditWidget, zfanyT<ZFUIButtonBasic>, clearButton) {
    zfobj<ZFUIButtonBasic> clearButton;
    propertyValue = clearButton;
    clearButton->icon()->image(zfres("ZFUIWidget/ZFUITextEditWidget_clearButton.xml"));
    clearButton->viewSizeMin(ZFUISizeZero());
    clearButton->visible(zffalse);
}

static void _ZFP_ZFUITextEditWidget_updateClearButton(ZF_IN ZFUITextEditWidget *textEditWidget) {
    if(textEditWidget->clearButtonAutoEnable()) {
        textEditWidget->clearButton()->visible(
            !textEditWidget->text().isEmpty());
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEditWidget, zfbool, clearButtonAutoEnable) {
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

void ZFUITextEditWidget::objectOnInit(void) {
    zfsuper::objectOnInit();

    ZFUIImageView *backgroundView = this->backgroundView();
    this->internalImplViewAdd(backgroundView);
    backgroundView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    backgroundView->serializableRefLayoutParam()->sizeParam(ZFUISizeParamFillFill());

    ZFUIButtonBasic *clearButton = this->clearButton();
    this->internalBgViewAdd(clearButton);
    clearButton->layoutParam()->align(ZFUIAlign::e_Right);
    clearButton->serializableRefLayoutParam()->align(ZFUIAlign::e_Right);

    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}
void ZFUITextEditWidget::objectOnDealloc(void) {
    zfsuper::objectOnDealloc();
}

void ZFUITextEditWidget::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds) {
    zfsuper::layoutOnLayoutPrepare(bounds);
    this->nativeImplViewMarginUpdate();
}
void ZFUITextEditWidget::nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin) {
    zfsuper::nativeImplViewMarginImplUpdate(nativeImplViewMargin);

    ZFUIImageView *backgroundView = this->backgroundView();
    if(backgroundView->visible()) {
        ZFUIImage *image = backgroundView->imageState();
        if(image != zfnull) {
            ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, image->imageNinePatch());
        }
    }

    ZFUIButtonBasic *clearButton = this->clearButton();
    if(clearButton->visible()) {
        clearButton->layoutMeasure(ZFUISizeInvalid(), ZFUISizeParamWrapWrap());
        zffloat size = ZFUIMarginGetWidth(clearButton->layoutParam()->margin()) + clearButton->layoutMeasuredSize().width;
        if(ZFBitTest(clearButton->layoutParam()->align(), ZFUIAlign::e_Left)) {
            nativeImplViewMargin.left += size;
        }
        else {
            nativeImplViewMargin.right += size;
        }
    }
}
zfbool ZFUITextEditWidget::internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
    if(internalView == this->clearButton()) {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}
void ZFUITextEditWidget::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);

    ZFUIButtonBasic *clearButton = this->clearButton();
    if(!clearButton->visible()) {
        return;
    }

    ZFUIMargin margin = ZFUIMarginZero();
    ZFUIImageView *backgroundView = this->backgroundView();
    if(backgroundView->visible()) {
        ZFUIImage *image = backgroundView->imageState();
        if(image != zfnull) {
            margin = image->imageNinePatch();
        }
    }
    clearButton->viewFrame(ZFUIAlignApply(
        clearButton->layoutParam()->align(),
        bounds,
        clearButton->layoutMeasuredSize(),
        margin));
}
void ZFUITextEditWidget::textOnUpdate(ZF_IN const zfstring &oldText) {
    zfsuper::textOnUpdate(oldText);
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

ZF_NAMESPACE_GLOBAL_END

