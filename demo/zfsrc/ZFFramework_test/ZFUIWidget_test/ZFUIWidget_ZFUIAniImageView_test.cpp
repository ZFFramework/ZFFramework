#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIAniImageView_test : zfextends ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIAniImageView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIAniImageView, animatedImageView);
        container->childAdd(animatedImageView)->c_alignCenter();
        ZFLISTENER_1(testCaseOnStop
                , ZFUIAniImageView *, animatedImageView
                ) {
            animatedImageView->aniStop();
        } ZFLISTENER_END()
        this->observerAdd(ZFTestCase::EventTestCaseOnStop(), testCaseOnStop);

        zfautoObjectT<ZFUIImage *> frameSrc = zfRes("test_normal.png");
        animatedImageView->aniLoad(frameSrc, ZFUISizeMake(
                frameSrc->imageSizeFixed().width / 2,
                frameSrc->imageSizeFixed().height / 2
            ),
            4,
            ZFCoreArrayPODCreate(zftimet, 100, 500, 1000));

        this->prepareActionLog(animatedImageView);
        this->prepareActionButton(container, animatedImageView);
        this->prepareSettingButton(window, animatedImageView);
    }

private:
    void prepareActionLog(ZF_IN ZFUIAniImageView *animatedImageView) {
        ZFTestCase *testCase = this;

        ZFLISTENER_1(onFrame
                , ZFTestCase *, testCase
                ) {
            testCase->testCaseOutput(
                "onFrame: %zi",
                zfargs.sender()->to<ZFUIAniImageView *>()->aniFrame());
        } ZFLISTENER_END()
        animatedImageView->observerAdd(ZFUIAniImageView::EventAniOnFrame(), onFrame);

        ZFLISTENER_1(onLoop
                , ZFTestCase *, testCase
                ) {
            testCase->testCaseOutput(
                "onLoop: %zi",
                zfargs.sender()->to<ZFUIAniImageView *>()->aniFrame());
        } ZFLISTENER_END()
        animatedImageView->observerAdd(ZFUIAniImageView::EventAniOnLoop(), onLoop);

        ZFLISTENER_1(onStop
                , ZFTestCase *, testCase
                ) {
            testCase->testCaseOutput(
                "onStop: %zi",
                zfargs.sender()->to<ZFUIAniImageView *>()->aniFrame());
        } ZFLISTENER_END()
        animatedImageView->observerAdd(ZFUIAniImageView::EventAniOnStop(), onStop);
    }
    void prepareActionButton(
            ZF_IN ZFUIView *container
            , ZF_IN ZFUIAniImageView *animatedImageView
            ) {
        ZFLISTENER_1(manualOnClick
                , ZFUIAniImageView *, animatedImageView
                ) {
            animatedImageView->aniFrameNext();
        } ZFLISTENER_END()
        zfblockedAlloc(ZFUIKit_test_Button, manualButton);
        container->childAdd(manualButton)->c_alignLeftBottom();
        manualButton->label()->text("manual");
        manualButton->observerAdd(ZFUIButton::EventButtonOnClick(), manualOnClick);

        ZFLISTENER_1(startOnClick
                , ZFUIAniImageView *, animatedImageView
                ) {
            animatedImageView->aniStart();
        } ZFLISTENER_END()
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        container->childAdd(startButton)->c_alignBottom();
        startButton->label()->text("start");
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick);

        ZFLISTENER_1(stopOnClick
                , ZFUIAniImageView *, animatedImageView
                ) {
            animatedImageView->aniStop();
        } ZFLISTENER_END()
        zfblockedAlloc(ZFUIKit_test_Button, stopButton);
        container->childAdd(stopButton)->c_alignRightBottom();
        stopButton->label()->text("stop");
        stopButton->observerAdd(ZFUIButton::EventButtonOnClick(), stopOnClick);
    }
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIAniImageView *animatedImageView
            ) {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIAniImageView_test)

ZF_NAMESPACE_GLOBAL_END

