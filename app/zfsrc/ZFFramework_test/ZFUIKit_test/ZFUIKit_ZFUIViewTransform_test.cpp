#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIViewTransform_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIViewTransform_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIViewTransform);
        ZFFramework_test_asyncTestCheck();

        zfautoT<ZFUIWindow> window;
        zfautoT<ZFUIView> container;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIImageView> view;
        container->child(view)->c_sizeFill(100, 100)->c_alignTop()->c_margin(0, 100, 0, 0);
        view->image(zfres("test_normal.png"));

        zffloat margin = 120;

        // translateX
        zfobj<ZFUIKit_test_Button> translateXDec;
        window->child(translateXDec)->c_alignBottom()->c_margin(0, 0, margin, 200);
        translateXDec->label()->text("translateX -");
        ZFLISTENER_1(translateXOnDec
                , ZFUIImageView *, view
                ) {
            view->translateX(view->translateX() - 10);
        } ZFLISTENER_END()
        translateXDec->onClick(translateXOnDec);

        zfobj<ZFUIKit_test_Button> translateXInc;
        window->child(translateXInc)->c_alignBottom()->c_margin(margin, 0, 0, 200);
        translateXInc->label()->text("translateX +");
        ZFLISTENER_1(translateXOnInc
                , ZFUIImageView *, view
                ) {
            view->translateX(view->translateX() + 10);
        } ZFLISTENER_END()
        translateXInc->onClick(translateXOnInc);

        // translateY
        zfobj<ZFUIKit_test_Button> translateYDec;
        window->child(translateYDec)->c_alignBottom()->c_margin(0, 0, margin, 150);
        translateYDec->label()->text("translateY -");
        ZFLISTENER_1(translateYOnDec
                , ZFUIImageView *, view
                ) {
            view->translateY(view->translateY() - 10);
        } ZFLISTENER_END()
        translateYDec->onClick(translateYOnDec);

        zfobj<ZFUIKit_test_Button> translateYInc;
        window->child(translateYInc)->c_alignBottom()->c_margin(margin, 0, 0, 150);
        translateYInc->label()->text("translateY +");
        ZFLISTENER_1(translateYOnInc
                , ZFUIImageView *, view
                ) {
            view->translateY(view->translateY() + 10);
        } ZFLISTENER_END()
        translateYInc->onClick(translateYOnInc);

        // scaleX
        zfobj<ZFUIKit_test_Button> scaleXDec;
        window->child(scaleXDec)->c_alignBottom()->c_margin(0, 0, margin, 100);
        scaleXDec->label()->text("scaleX -");
        ZFLISTENER_1(scaleXOnDec
                , ZFUIImageView *, view
                ) {
            view->scaleX(view->scaleX() - 0.1f);
        } ZFLISTENER_END()
        scaleXDec->onClick(scaleXOnDec);

        zfobj<ZFUIKit_test_Button> scaleXInc;
        window->child(scaleXInc)->c_alignBottom()->c_margin(margin, 0, 0, 100);
        scaleXInc->label()->text("scaleX +");
        ZFLISTENER_1(scaleXOnInc
                , ZFUIImageView *, view
                ) {
            view->scaleX(view->scaleX() + 0.1f);
        } ZFLISTENER_END()
        scaleXInc->onClick(scaleXOnInc);

        // scaleY
        zfobj<ZFUIKit_test_Button> scaleYDec;
        window->child(scaleYDec)->c_alignBottom()->c_margin(0, 0, margin, 50);
        scaleYDec->label()->text("scaleY -");
        ZFLISTENER_1(scaleYOnDec
                , ZFUIImageView *, view
                ) {
            view->scaleY(view->scaleY() - 0.1f);
        } ZFLISTENER_END()
        scaleYDec->onClick(scaleYOnDec);

        zfobj<ZFUIKit_test_Button> scaleYInc;
        window->child(scaleYInc)->c_alignBottom()->c_margin(margin, 0, 0, 50);
        scaleYInc->label()->text("scaleY +");
        ZFLISTENER_1(scaleYOnInc
                , ZFUIImageView *, view
                ) {
            view->scaleY(view->scaleY() + 0.1f);
        } ZFLISTENER_END()
        scaleYInc->onClick(scaleYOnInc);

        // rotateZ
        zfobj<ZFUIKit_test_Button> rotateZDec;
        window->child(rotateZDec)->c_alignBottom()->c_margin(0, 0, margin, 0);
        rotateZDec->label()->text("rotateZ -");
        ZFLISTENER_1(rotateZOnDec
                , ZFUIImageView *, view
                ) {
            view->rotateZ(view->rotateZ() - 10);
        } ZFLISTENER_END()
        rotateZDec->onClick(rotateZOnDec);

        zfobj<ZFUIKit_test_Button> rotateZInc;
        window->child(rotateZInc)->c_alignBottom()->c_margin(margin, 0, 0, 0);
        rotateZInc->label()->text("rotateZ +");
        ZFLISTENER_1(rotateZOnInc
                , ZFUIImageView *, view
                ) {
            view->rotateZ(view->rotateZ() + 10);
        } ZFLISTENER_END()
        rotateZInc->onClick(rotateZOnInc);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewTransform_test)

ZF_NAMESPACE_GLOBAL_END

