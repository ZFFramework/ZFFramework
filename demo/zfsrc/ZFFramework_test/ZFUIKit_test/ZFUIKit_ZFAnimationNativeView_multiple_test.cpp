#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAnimationNativeView_multiple_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFAnimationNativeView_multiple_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFAnimationNativeView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIView, view);
        container->childAdd(view)->c_alignCenter();
        view->viewSizePrefer(ZFUISizeMake(80, 60));
        view->viewBackgroundColor(ZFUIColorRandom());

        ZFLISTENER_1(startOnClick
                , ZFUIView *, view
                ) {
            zfblockedAlloc(ZFAnimationGroup, aniGroup);
            aniGroup->aniTarget(view);

            zfblockedAlloc(ZFAnimationNativeView, ani0);
            aniGroup->childAniAdd(ani0);
            ani0->aniTranslateXTo(1);
            ani0->aniDuration(3000);

            zfblockedAlloc(ZFAnimationNativeView, ani1);
            aniGroup->childAniAdd(ani1);
            ani1->aniRotateZTo(180);
            ani1->aniDuration(3000);

            aniGroup->aniStart();
        } ZFLISTENER_END(startOnClick)
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        startButton->label()->text("start");
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick);
        container->childAdd(startButton)->c_alignRightTop();
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAnimationNativeView_multiple_test)

ZF_NAMESPACE_GLOBAL_END

