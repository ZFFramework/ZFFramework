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
        container->childAdd(view)->c_alignCenter();
        view->viewSizePrefer(ZFUISizeMake(80, 60));
        view->viewBackgroundColor(ZFUIColorRandom());

        ZFLISTENER_1(startOnClick
                , ZFUIView *, view
                ) {
            zfobj<ZFAniGroup> aniGroup;
            aniGroup->aniTarget(view);

            zfobj<ZFAniForNative> ani0;
            aniGroup->child(ani0);
            ani0->aniTranslateXTo(1);
            ani0->aniDuration(3000);

            zfobj<ZFAniForNative> ani1;
            aniGroup->child(ani1);
            ani1->aniRotateZTo(180);
            ani1->aniDuration(3000);

            aniGroup->aniStart();
        } ZFLISTENER_END()
        zfobj<ZFUIKit_test_Button> startButton;
        startButton->label()->text("start");
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick);
        container->childAdd(startButton)->c_alignRightTop();
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAniForNative_multiple_test)

ZF_NAMESPACE_GLOBAL_END

