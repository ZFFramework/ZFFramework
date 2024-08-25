#include "ZFUITextEditWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextEditWidget
ZFOBJECT_REGISTER(ZFUITextEditWidget)
ZFSTYLE_DEFAULT_DEFINE(ZFUITextEditWidget)

ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEditWidget, zfanyT<ZFUIImageView>, textEditBackgroundView) {
    zfobj<ZFUIImageView> textEditBackgroundView;
    propertyValue = textEditBackgroundView;
    textEditBackgroundView->image(zfRes("ZFUIWidget/ZFUITextEditWidget_background.xml"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEditWidget, zfanyT<ZFUIButtonBasic>, textEditClearButton) {
    zfobj<ZFUIButtonBasic> textEditClearButton;
    propertyValue = textEditClearButton;
    textEditClearButton->icon()->image(zfRes("ZFUIWidget/ZFUITextEditWidget_clearButton.xml"));
    textEditClearButton->viewSizeMin(ZFUISizeZero());
    textEditClearButton->viewVisible(zffalse);
}

static void _ZFP_ZFUITextEditWidget_updateClearButton(ZF_IN ZFUITextEditWidget *textEditWidget) {
    if(textEditWidget->textEditClearButtonAutoEnable()) {
        textEditWidget->textEditClearButton()->viewVisible(
            !textEditWidget->text().isEmpty());
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEditWidget, zfbool, textEditClearButtonAutoEnable) {
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

void ZFUITextEditWidget::objectOnInit(void) {
    zfsuper::objectOnInit();

    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView();
    this->internalImplViewAdd(textEditBackgroundView);
    textEditBackgroundView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    textEditBackgroundView->serializableRefLayoutParam()->sizeParam(ZFUISizeParamFillFill());

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton();
    this->internalBgViewAdd(textEditClearButton);
    textEditClearButton->layoutParam()->layoutAlign(ZFUIAlign::e_Right);
    textEditClearButton->serializableRefLayoutParam()->layoutAlign(ZFUIAlign::e_Right);

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

    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView();
    if(textEditBackgroundView->viewVisible()) {
        ZFUIImage *image = textEditBackgroundView->image();
        if(image != zfnull) {
            ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, image->imageNinePatch());
        }
    }

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton();
    if(textEditClearButton->viewVisible()) {
        textEditClearButton->layoutMeasure(ZFUISizeInvalid(), ZFUISizeParamWrapWrap());
        zffloat size = ZFUIMarginGetWidth(textEditClearButton->layoutParam()->layoutMargin()) + textEditClearButton->layoutMeasuredSize().width;
        if(ZFBitTest(textEditClearButton->layoutParam()->layoutAlign(), ZFUIAlign::e_Left)) {
            nativeImplViewMargin.left += size;
        }
        else {
            nativeImplViewMargin.right += size;
        }
    }
}
zfbool ZFUITextEditWidget::internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
    if(internalView == this->textEditClearButton()) {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}
void ZFUITextEditWidget::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton();
    if(!textEditClearButton->viewVisible()) {
        return;
    }

    ZFUIMargin margin = ZFUIMarginZero();
    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView();
    if(textEditBackgroundView->viewVisible()) {
        ZFUIImage *image = textEditBackgroundView->image();
        if(image != zfnull) {
            margin = image->imageNinePatch();
        }
    }
    textEditClearButton->viewFrame(ZFUIAlignApply(
        textEditClearButton->layoutParam()->layoutAlign(),
        bounds,
        textEditClearButton->layoutMeasuredSize(),
        margin));
}
void ZFUITextEditWidget::textOnUpdate(ZF_IN const zfstring &oldText) {
    zfsuper::textOnUpdate(oldText);
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

ZF_NAMESPACE_GLOBAL_END

