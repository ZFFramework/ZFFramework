#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAudio_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFAudio_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFAudio);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        ZFCoreArray<ZFAudio *> audios;
        audios.add(this->audio0());
        audios.add(this->audio1());
        audios.add(this->audio2());

        ZFLISTENER_1(testCaseOnStop
                , ZFCoreArray<ZFAudio *>, audios
                ) {
            for(zfindex i = 0; i < audios.count(); ++i) {
                audios[i]->stop();
            }
        } ZFLISTENER_END()
        this->observerAdd(ZFTestCase::E_TestCaseOnStop(), testCaseOnStop);

        audios[0]->loop(zfindexMax());

        zffloat btnWidth = 100;
        zffloat btnHeight = 48;
        zffloat space = 5;
        for(zfindex iAudio = 0; iAudio < 3; ++iAudio) {
            zffloat btnX = iAudio * (btnWidth + space);
            zffloat btnY = 0;
            ZFAudio *audio = audios[iAudio];

            zfobj<ZFUITextView> stateLabel;
            container->child(stateLabel)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            stateLabel->textAlign(ZFUIAlign::e_Center);
            ZFLISTENER_2(stateOnUpdate
                    , ZFAudio *, audio
                    , ZFUITextView *, stateLabel
                    ) {
                stateLabel->text(audio->stateHint());
            } ZFLISTENER_END()
            audio->observerAdd(ZFAudio::E_AudioOnLoad(), stateOnUpdate);
            audio->observerAdd(ZFAudio::E_AudioOnStart(), stateOnUpdate);
            audio->observerAdd(ZFAudio::E_AudioOnStop(), stateOnUpdate);
            audio->observerAdd(ZFAudio::E_AudioOnResume(), stateOnUpdate);
            audio->observerAdd(ZFAudio::E_AudioOnPause(), stateOnUpdate);
            audio->observerAdd(ZFAudio::E_AudioOnLoop(), stateOnUpdate);
            ZFListener(stateOnUpdate).execute();

            zfobj<ZFUIKit_test_Button> loadBtn;
            container->child(loadBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            loadBtn->label()->text("load");
            ZFLISTENER_1(loadOnClick
                    , ZFAudio *, audio
                    ) {
                audio->load(ZFInputForRes("test_audio.mp3"));
            } ZFLISTENER_END()
            loadBtn->onClick(loadOnClick);

            zfobj<ZFUIKit_test_Button> startBtn;
            container->child(startBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            startBtn->label()->text("start");
            ZFLISTENER_1(startOnClick
                    , ZFAudio *, audio
                    ) {
                audio->start();
            } ZFLISTENER_END()
            startBtn->onClick(startOnClick);

            zfobj<ZFUIKit_test_Button> stopBtn;
            container->child(stopBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            stopBtn->label()->text("stop");
            ZFLISTENER_1(stopOnClick
                    , ZFAudio *, audio
                    ) {
                audio->stop();
            } ZFLISTENER_END()
            stopBtn->onClick(stopOnClick);

            zfobj<ZFUIKit_test_Button> pauseBtn;
            container->child(pauseBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            pauseBtn->label()->text("pause");
            ZFLISTENER_1(pauseOnClick
                    , ZFAudio *, audio
                    ) {
                audio->pause();
            } ZFLISTENER_END()
            pauseBtn->onClick(pauseOnClick);

            zfobj<ZFUIKit_test_Button> resumeBtn;
            container->child(resumeBtn)->c_sizeFill(btnWidth, btnHeight)->c_alignLeftTop(btnX, btnY);
            btnY += btnHeight + space;
            resumeBtn->label()->text("resume");
            ZFLISTENER_1(resumeOnClick
                    , ZFAudio *, audio
                    ) {
                audio->resume();
            } ZFLISTENER_END()
            resumeBtn->onClick(resumeOnClick);
        }
    }

    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFAudio>, audio0, zfobj<ZFAudio>())
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFAudio>, audio1, zfobj<ZFAudio>())
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFAudio>, audio2, zfobj<ZFAudio>())
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAudio_test)

ZF_NAMESPACE_GLOBAL_END

