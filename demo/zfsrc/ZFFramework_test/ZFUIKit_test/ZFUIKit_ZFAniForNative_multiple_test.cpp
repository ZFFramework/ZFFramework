#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAniForNative_multiple_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFAniForNative_multiple_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFAniForNative);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIView> view;
        container->child(view)->c_alignCenter();
        view->viewSizeFixed(80, 60);
        view->bgColor(ZFUIColorRandom());

        ZFLISTENER_1(startOnClick
                , ZFUIView *, view
                ) {
            zfobj<ZFAniGroup> aniGroup;
            aniGroup->target(view);

            zfobj<ZFAniForNative> ani0;
            aniGroup->child(ani0);
            ani0->translateXTo(view->width());
            ani0->duration(3000);

            zfobj<ZFAniForNative> ani1;
            aniGroup->child(ani1);
            ani1->rotateZTo(180);
            ani1->duration(3000);

            aniGroup->start();
        } ZFLISTENER_END()
        zfobj<ZFUIKit_test_Button> startButton;
        startButton->label()->text("start");
        startButton->observerAdd(ZFUIButton::E_ButtonOnClick(), startOnClick);
        container->child(startButton)->c_alignRightTop();
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAniForNative_multiple_test)

ZF_NAMESPACE_GLOBAL_END

