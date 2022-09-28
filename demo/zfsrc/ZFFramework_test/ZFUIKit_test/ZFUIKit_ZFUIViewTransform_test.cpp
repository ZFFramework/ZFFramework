#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIViewTransform_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIViewTransform_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, view);
        container->childAdd(view, ZFUISizeParamFillFill(), ZFUIAlign::e_TopInner, ZFUIMarginMake(0, 100, 0, 0));
        view->layoutParam()->sizeHint(ZFUISizeMake(100));
        view->image(zfRes("test_normal.png"));

        zffloat margin = 120;

        // viewTranslateX
        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateXDec);
        window->childAdd(viewTranslateXDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 200));
        viewTranslateXDec->buttonLabelText("viewTranslateX -");
        ZFLISTENER(viewTranslateXOnDec) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateX(view->viewTranslateX() - 10);
        } ZFLISTENER_END(viewTranslateXOnDec)
        viewTranslateXDec->onClick(viewTranslateXOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateXInc);
        window->childAdd(viewTranslateXInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 200));
        viewTranslateXInc->buttonLabelText("viewTranslateX +");
        ZFLISTENER(viewTranslateXOnInc) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateX(view->viewTranslateX() + 10);
        } ZFLISTENER_END(viewTranslateXOnInc)
        viewTranslateXInc->onClick(viewTranslateXOnInc, view->objectHolder());

        // viewTranslateY
        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateYDec);
        window->childAdd(viewTranslateYDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 150));
        viewTranslateYDec->buttonLabelText("viewTranslateY -");
        ZFLISTENER(viewTranslateYOnDec) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateY(view->viewTranslateY() - 10);
        } ZFLISTENER_END(viewTranslateYOnDec)
        viewTranslateYDec->onClick(viewTranslateYOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateYInc);
        window->childAdd(viewTranslateYInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 150));
        viewTranslateYInc->buttonLabelText("viewTranslateY +");
        ZFLISTENER(viewTranslateYOnInc) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateY(view->viewTranslateY() + 10);
        } ZFLISTENER_END(viewTranslateYOnInc)
        viewTranslateYInc->onClick(viewTranslateYOnInc, view->objectHolder());

        // viewScaleX
        zfblockedAlloc(ZFUIKit_test_Button, viewScaleXDec);
        window->childAdd(viewScaleXDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 100));
        viewScaleXDec->buttonLabelText("viewScaleX -");
        ZFLISTENER(viewScaleXOnDec) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleX(view->viewScaleX() - 0.1f);
        } ZFLISTENER_END(viewScaleXOnDec)
        viewScaleXDec->onClick(viewScaleXOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewScaleXInc);
        window->childAdd(viewScaleXInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 100));
        viewScaleXInc->buttonLabelText("viewScaleX +");
        ZFLISTENER(viewScaleXOnInc) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleX(view->viewScaleX() + 0.1f);
        } ZFLISTENER_END(viewScaleXOnInc)
        viewScaleXInc->onClick(viewScaleXOnInc, view->objectHolder());

        // viewScaleY
        zfblockedAlloc(ZFUIKit_test_Button, viewScaleYDec);
        window->childAdd(viewScaleYDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 50));
        viewScaleYDec->buttonLabelText("viewScaleY -");
        ZFLISTENER(viewScaleYOnDec) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleY(view->viewScaleY() - 0.1f);
        } ZFLISTENER_END(viewScaleYOnDec)
        viewScaleYDec->onClick(viewScaleYOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewScaleYInc);
        window->childAdd(viewScaleYInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 50));
        viewScaleYInc->buttonLabelText("viewScaleY +");
        ZFLISTENER(viewScaleYOnInc) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleY(view->viewScaleY() + 0.1f);
        } ZFLISTENER_END(viewScaleYOnInc)
        viewScaleYInc->onClick(viewScaleYOnInc, view->objectHolder());

        // viewRotate
        zfblockedAlloc(ZFUIKit_test_Button, viewRotateDec);
        window->childAdd(viewRotateDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 0));
        viewRotateDec->buttonLabelText("viewRotate -");
        ZFLISTENER(viewRotateOnDec) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewRotate(view->viewRotate() - 10);
        } ZFLISTENER_END(viewRotateOnDec)
        viewRotateDec->onClick(viewRotateOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewRotateInc);
        window->childAdd(viewRotateInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 0));
        viewRotateInc->buttonLabelText("viewRotate +");
        ZFLISTENER(viewRotateOnInc) {
            ZFUIImageView *view = userData->objectHolded();
            view->viewRotate(view->viewRotate() + 10);
        } ZFLISTENER_END(viewRotateOnInc)
        viewRotateInc->onClick(viewRotateOnInc, view->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewTransform_test)

ZF_NAMESPACE_GLOBAL_END

