#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAnimationNativeView_viewTransform_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFAnimationNativeView_viewTransform_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFAnimationNativeView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, view);
        container->childAdd(view)->c_alignCenter();
        view->viewSizePrefer(ZFUISizeMake(80, 60));
        view->viewBackgroundColor(ZFUIColorRandom());
        view->image(zfRes("test_normal.png"));
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
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        startButton->label()->text("start");
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick);
        container->childAdd(startButton)->c_alignRightTop();
    }

    ZFPROPERTY_RETAIN_READONLY(ZFAnimationNativeView *, ani, zflineAlloc(ZFAnimationNativeView))

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFAnimationNativeView *ani
            ) {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingForResetProperty(settings, ani, ZFCoreArrayCreate(const ZFProperty *
                , ZFPropertyAccess(ZFAnimationNativeView, aniCurve)
                , ZFPropertyAccess(ZFAnimationNativeView, aniAlphaFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniAlphaTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateZTo)
            ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zftimet, ZFPropertyAccess(ZFAnimationNativeView, aniDuration),
            ZFCoreArrayCreate(zftimet
                , (zftimet)0
                , (zftimet)3000
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, ZFAnimationNativeViewCurveEnum, ZFPropertyAccess(ZFAnimationNativeView, aniCurve),
            ZFCoreArrayCreate(ZFAnimationNativeViewCurveEnum
                , ZFAnimationNativeViewCurve::e_Linear
                , ZFAnimationNativeViewCurve::e_EaseIn
                , ZFAnimationNativeViewCurve::e_EaseOut
                , ZFAnimationNativeViewCurve::e_EaseInOut
                ));

        { // aniAlpha
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniAlphaFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniAlphaTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniAlpha

        { // aniScale
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleXFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleXTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleYFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleYTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleZFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleZTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniScale

        { // aniTranslate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXFrom),
                ZFCoreArrayCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXTo),
                ZFCoreArrayCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYFrom),
                ZFCoreArrayCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYTo),
                ZFCoreArrayCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZFrom),
                ZFCoreArrayCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZTo),
                ZFCoreArrayCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
        } // aniTranslate

        { // aniTranslatePixel
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
        } // aniTranslatePixel

        { // aniRotate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateXFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateXTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateYFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateYTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateZFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateZTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
        } // aniRotate

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAnimationNativeView_viewTransform_test)

ZF_NAMESPACE_GLOBAL_END

