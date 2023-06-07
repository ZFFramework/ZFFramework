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
        ZFFramework_test_protocolCheck(ZFUIViewTransform);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, view);
        container->childAdd(view)->c_sizeFill(100, 100)->c_alignTop()->c_margin(0, 100, 0, 0);
        view->image(zfRes("test_normal.png"));

        zffloat margin = 120;

        // viewTranslateX
        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateXDec);
        window->childAdd(viewTranslateXDec)->c_alignBottom()->c_margin(0, 0, margin, 200);
        viewTranslateXDec->label()->text("viewTranslateX -");
        ZFLISTENER_1(viewTranslateXOnDec
                , ZFUIImageView *, view
                ) {
            view->viewTranslateX(view->viewTranslateX() - 10);
        } ZFLISTENER_END()
        viewTranslateXDec->onClick(viewTranslateXOnDec);

        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateXInc);
        window->childAdd(viewTranslateXInc)->c_alignBottom()->c_margin(margin, 0, 0, 200);
        viewTranslateXInc->label()->text("viewTranslateX +");
        ZFLISTENER_1(viewTranslateXOnInc
                , ZFUIImageView *, view
                ) {
            view->viewTranslateX(view->viewTranslateX() + 10);
        } ZFLISTENER_END()
        viewTranslateXInc->onClick(viewTranslateXOnInc);

        // viewTranslateY
        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateYDec);
        window->childAdd(viewTranslateYDec)->c_alignBottom()->c_margin(0, 0, margin, 150);
        viewTranslateYDec->label()->text("viewTranslateY -");
        ZFLISTENER_1(viewTranslateYOnDec
                , ZFUIImageView *, view
                ) {
            view->viewTranslateY(view->viewTranslateY() - 10);
        } ZFLISTENER_END()
        viewTranslateYDec->onClick(viewTranslateYOnDec);

        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateYInc);
        window->childAdd(viewTranslateYInc)->c_alignBottom()->c_margin(margin, 0, 0, 150);
        viewTranslateYInc->label()->text("viewTranslateY +");
        ZFLISTENER_1(viewTranslateYOnInc
                , ZFUIImageView *, view
                ) {
            view->viewTranslateY(view->viewTranslateY() + 10);
        } ZFLISTENER_END()
        viewTranslateYInc->onClick(viewTranslateYOnInc);

        // viewScaleX
        zfblockedAlloc(ZFUIKit_test_Button, viewScaleXDec);
        window->childAdd(viewScaleXDec)->c_alignBottom()->c_margin(0, 0, margin, 100);
        viewScaleXDec->label()->text("viewScaleX -");
        ZFLISTENER_1(viewScaleXOnDec
                , ZFUIImageView *, view
                ) {
            view->viewScaleX(view->viewScaleX() - 0.1f);
        } ZFLISTENER_END()
        viewScaleXDec->onClick(viewScaleXOnDec);

        zfblockedAlloc(ZFUIKit_test_Button, viewScaleXInc);
        window->childAdd(viewScaleXInc)->c_alignBottom()->c_margin(margin, 0, 0, 100);
        viewScaleXInc->label()->text("viewScaleX +");
        ZFLISTENER_1(viewScaleXOnInc
                , ZFUIImageView *, view
                ) {
            view->viewScaleX(view->viewScaleX() + 0.1f);
        } ZFLISTENER_END()
        viewScaleXInc->onClick(viewScaleXOnInc);

        // viewScaleY
        zfblockedAlloc(ZFUIKit_test_Button, viewScaleYDec);
        window->childAdd(viewScaleYDec)->c_alignBottom()->c_margin(0, 0, margin, 50);
        viewScaleYDec->label()->text("viewScaleY -");
        ZFLISTENER_1(viewScaleYOnDec
                , ZFUIImageView *, view
                ) {
            view->viewScaleY(view->viewScaleY() - 0.1f);
        } ZFLISTENER_END()
        viewScaleYDec->onClick(viewScaleYOnDec);

        zfblockedAlloc(ZFUIKit_test_Button, viewScaleYInc);
        window->childAdd(viewScaleYInc)->c_alignBottom()->c_margin(margin, 0, 0, 50);
        viewScaleYInc->label()->text("viewScaleY +");
        ZFLISTENER_1(viewScaleYOnInc
                , ZFUIImageView *, view
                ) {
            view->viewScaleY(view->viewScaleY() + 0.1f);
        } ZFLISTENER_END()
        viewScaleYInc->onClick(viewScaleYOnInc);

        // viewRotateZ
        zfblockedAlloc(ZFUIKit_test_Button, viewRotateZDec);
        window->childAdd(viewRotateZDec)->c_alignBottom()->c_margin(0, 0, margin, 0);
        viewRotateZDec->label()->text("viewRotateZ -");
        ZFLISTENER_1(viewRotateZOnDec
                , ZFUIImageView *, view
                ) {
            view->viewRotateZ(view->viewRotateZ() - 10);
        } ZFLISTENER_END()
        viewRotateZDec->onClick(viewRotateZOnDec);

        zfblockedAlloc(ZFUIKit_test_Button, viewRotateZInc);
        window->childAdd(viewRotateZInc)->c_alignBottom()->c_margin(margin, 0, 0, 0);
        viewRotateZInc->label()->text("viewRotateZ +");
        ZFLISTENER_1(viewRotateZOnInc
                , ZFUIImageView *, view
                ) {
            view->viewRotateZ(view->viewRotateZ() + 10);
        } ZFLISTENER_END()
        viewRotateZInc->onClick(viewRotateZOnInc);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewTransform_test)

ZF_NAMESPACE_GLOBAL_END

