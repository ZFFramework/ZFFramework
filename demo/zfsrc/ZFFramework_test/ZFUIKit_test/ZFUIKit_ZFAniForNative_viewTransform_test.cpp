#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAniForNative_viewTransform_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFAniForNative_viewTransform_test, ZFFramework_test_TestCase)

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

        zfobj<ZFUIImageView> view;
        container->childAdd(view)->c_alignCenter();
        view->viewSizePrefer(ZFUISizeCreate(80, 60));
        view->viewBackgroundColor(ZFUIColorRandom());
        view->image(zfres("test_normal.png"));
        view->viewScaleX(1.5f);
        view->viewScaleY(0.5f);
        view->viewRotateZ(40);

        this->ani()->aniTarget(view);

        this->prepareSettingButton(window, this->ani());

        ZFAnimation *ani = this->ani();
        ZFLISTENER_1(startOnClick
                , ZFAnimation *, ani
                ) {
            ani->aniStart();
        } ZFLISTENER_END()
        zfobj<ZFUIKit_test_Button> startButton;
        startButton->label()->text("start");
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick);
        container->childAdd(startButton)->c_alignRightTop();
    }

    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFAniForNative>, ani, zfobj<ZFAniForNative>())

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFAniForNative *ani
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingForResetProperty(settings, ani, ZFCoreArrayCreate(const ZFProperty *
                , ZFPropertyAccess(ZFAniForNative, aniCurve)
                , ZFPropertyAccess(ZFAniForNative, aniAlphaFrom)
                , ZFPropertyAccess(ZFAniForNative, aniAlphaTo)
                , ZFPropertyAccess(ZFAniForNative, aniScaleXFrom)
                , ZFPropertyAccess(ZFAniForNative, aniScaleXTo)
                , ZFPropertyAccess(ZFAniForNative, aniScaleYFrom)
                , ZFPropertyAccess(ZFAniForNative, aniScaleYTo)
                , ZFPropertyAccess(ZFAniForNative, aniScaleZFrom)
                , ZFPropertyAccess(ZFAniForNative, aniScaleZTo)
                , ZFPropertyAccess(ZFAniForNative, aniTranslateXFrom)
                , ZFPropertyAccess(ZFAniForNative, aniTranslateXTo)
                , ZFPropertyAccess(ZFAniForNative, aniTranslateYFrom)
                , ZFPropertyAccess(ZFAniForNative, aniTranslateYTo)
                , ZFPropertyAccess(ZFAniForNative, aniTranslateZFrom)
                , ZFPropertyAccess(ZFAniForNative, aniTranslateZTo)
                , ZFPropertyAccess(ZFAniForNative, aniRotateXFrom)
                , ZFPropertyAccess(ZFAniForNative, aniRotateXTo)
                , ZFPropertyAccess(ZFAniForNative, aniRotateYFrom)
                , ZFPropertyAccess(ZFAniForNative, aniRotateYTo)
                , ZFPropertyAccess(ZFAniForNative, aniRotateZFrom)
                , ZFPropertyAccess(ZFAniForNative, aniRotateZTo)
            ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zftimet, ZFPropertyAccess(ZFAniForNative, aniDuration),
            ZFCoreArrayCreate(zftimet
                , (zftimet)0
                , (zftimet)3000
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, ZFAniForNativeCurveEnum, ZFPropertyAccess(ZFAniForNative, aniCurve),
            ZFCoreArrayCreate(ZFAniForNativeCurveEnum
                , ZFAniForNativeCurve::e_Linear
                , ZFAniForNativeCurve::e_EaseIn
                , ZFAniForNativeCurve::e_EaseOut
                , ZFAniForNativeCurve::e_EaseInOut
                ));

        { // aniAlpha
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniAlphaFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniAlphaTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniAlpha

        { // aniScale
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniScaleXFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniScaleXTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniScaleYFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniScaleYTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniScaleZFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniScaleZTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniScale

        { // aniTranslate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniTranslateXFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniTranslateXTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniTranslateYFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniTranslateYTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniTranslateZFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniTranslateZTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
        } // aniTranslate

        { // aniRotate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniRotateXFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniRotateXTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniRotateYFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniRotateYTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniRotateZFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, aniRotateZTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
        } // aniRotate

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAniForNative_viewTransform_test)

ZF_NAMESPACE_GLOBAL_END

