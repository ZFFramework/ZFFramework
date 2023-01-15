#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAudio_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFAudio_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFAudio);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        // global audio does not belongs to any
        zflineAlloc(ZFAudio, ZFInputForRes("test_audio.mp3"))->start();

        ZFCoreArrayPOD<ZFAudio *> audios;
        audios.add(this->audio0());
        audios.add(this->audio1());
        audios.add(this->audio2());

        ZFLISTENER_1(testCaseOnStop
                , ZFCoreArrayPOD<ZFAudio *>, audios
                ) {
            for(zfindex i = 0; i < audios.count(); ++i)
            {
                audios[i]->stop();
            }
        } ZFLISTENER_END(testCaseOnStop)
        this->observerAdd(ZFTestCase::EventTestCaseOnStop(), testCaseOnStop);

        audios[0]->loop(zfindexMax());

        zffloat btnWidth = 100;
        zffloat btnHeight = 48;
        zffloat space = 5;
        for(zfindex iAudio = 0; iAudio < 3; ++iAudio)
        {
            zffloat btnX = iAudio * (btnWidth + space);
            zffloat btnY = 0;
            ZFAudio *audio = audios[iAudio];

            zfblockedAlloc(ZFUITextView, stateLabel);
            container->childAdd(stateLabel)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            stateLabel->textAlign(ZFUIAlign::e_Center);
            ZFLISTENER_2(stateOnUpdate
                    , ZFAudio *, audio
                    , ZFUITextView *, stateLabel
                    ) {
                stateLabel->text(audio->stateHint());
            } ZFLISTENER_END(stateOnUpdate)
            audio->observerAdd(ZFAudio::EventAudioOnLoad(), stateOnUpdate);
            audio->observerAdd(ZFAudio::EventAudioOnStart(), stateOnUpdate);
            audio->observerAdd(ZFAudio::EventAudioOnStop(), stateOnUpdate);
            audio->observerAdd(ZFAudio::EventAudioOnResume(), stateOnUpdate);
            audio->observerAdd(ZFAudio::EventAudioOnPause(), stateOnUpdate);
            audio->observerAdd(ZFAudio::EventAudioOnLoop(), stateOnUpdate);
            ZFListener(stateOnUpdate).execute();

            zfblockedAlloc(ZFUIKit_test_Button, loadBtn);
            container->childAdd(loadBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            loadBtn->label()->text("load");
            ZFLISTENER_1(loadOnClick
                    , ZFAudio *, audio
                    ) {
                audio->load(ZFInputForRes("test_audio.mp3"));
            } ZFLISTENER_END(loadOnClick)
            loadBtn->onClick(loadOnClick);

            zfblockedAlloc(ZFUIKit_test_Button, startBtn);
            container->childAdd(startBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            startBtn->label()->text("start");
            ZFLISTENER_1(startOnClick
                    , ZFAudio *, audio
                    ) {
                audio->start();
            } ZFLISTENER_END(startOnClick)
            startBtn->onClick(startOnClick);

            zfblockedAlloc(ZFUIKit_test_Button, stopBtn);
            container->childAdd(stopBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            stopBtn->label()->text("stop");
            ZFLISTENER_1(stopOnClick
                    , ZFAudio *, audio
                    ) {
                audio->stop();
            } ZFLISTENER_END(stopOnClick)
            stopBtn->onClick(stopOnClick);

            zfblockedAlloc(ZFUIKit_test_Button, pauseBtn);
            container->childAdd(pauseBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            pauseBtn->label()->text("pause");
            ZFLISTENER_1(pauseOnClick
                    , ZFAudio *, audio
                    ) {
                audio->pause();
            } ZFLISTENER_END(pauseOnClick)
            pauseBtn->onClick(pauseOnClick);

            zfblockedAlloc(ZFUIKit_test_Button, resumeBtn);
            container->childAdd(resumeBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            resumeBtn->label()->text("resume");
            ZFLISTENER_1(resumeOnClick
                    , ZFAudio *, audio
                    ) {
                audio->resume();
            } ZFLISTENER_END(resumeOnClick)
            resumeBtn->onClick(resumeOnClick);
        }
    }

    ZFPROPERTY_RETAIN_READONLY(ZFAudio *, audio0, zflineAlloc(ZFAudio))
    ZFPROPERTY_RETAIN_READONLY(ZFAudio *, audio1, zflineAlloc(ZFAudio))
    ZFPROPERTY_RETAIN_READONLY(ZFAudio *, audio2, zflineAlloc(ZFAudio))
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAudio_test)

ZF_NAMESPACE_GLOBAL_END

