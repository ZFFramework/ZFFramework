#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAniForNative_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFAniForNative_test, ZFFramework_test_TestCase)

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

        this->ani()->target(view);

        this->prepareSettingButton(window, this->ani());

        ZFAnimation *ani = this->ani();
        ZFLISTENER_1(startOnClick
                , ZFAnimation *, ani
                ) {
            ani->start();
        } ZFLISTENER_END()
        zfobj<ZFUIKit_test_Button> startButton;
        startButton->label()->text("start");
        startButton->observerAdd(ZFUIButton::E_ButtonOnClick(), startOnClick);
        container->child(startButton)->c_alignRightTop();
    }

    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFAniForNative>, ani, zfobj<ZFAniForNative>())

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFAniForNative *ani
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingForResetProperty(settings, ani, ZFCoreArrayCreate(const ZFProperty *
                , ZFPropertyAccess(ZFAniForNative, curve)
                , ZFPropertyAccess(ZFAniForNative, alphaFrom)
                , ZFPropertyAccess(ZFAniForNative, alphaTo)
                , ZFPropertyAccess(ZFAniForNative, scaleXFrom)
                , ZFPropertyAccess(ZFAniForNative, scaleXTo)
                , ZFPropertyAccess(ZFAniForNative, scaleYFrom)
                , ZFPropertyAccess(ZFAniForNative, scaleYTo)
                , ZFPropertyAccess(ZFAniForNative, scaleZFrom)
                , ZFPropertyAccess(ZFAniForNative, scaleZTo)
                , ZFPropertyAccess(ZFAniForNative, translateXFrom)
                , ZFPropertyAccess(ZFAniForNative, translateXTo)
                , ZFPropertyAccess(ZFAniForNative, translateYFrom)
                , ZFPropertyAccess(ZFAniForNative, translateYTo)
                , ZFPropertyAccess(ZFAniForNative, translateZFrom)
                , ZFPropertyAccess(ZFAniForNative, translateZTo)
                , ZFPropertyAccess(ZFAniForNative, rotateXFrom)
                , ZFPropertyAccess(ZFAniForNative, rotateXTo)
                , ZFPropertyAccess(ZFAniForNative, rotateYFrom)
                , ZFPropertyAccess(ZFAniForNative, rotateYTo)
                , ZFPropertyAccess(ZFAniForNative, rotateZFrom)
                , ZFPropertyAccess(ZFAniForNative, rotateZTo)
            ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zftimet, ZFPropertyAccess(ZFAniForNative, duration),
            ZFCoreArrayCreate(zftimet
                , (zftimet)0
                , (zftimet)3000
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, ZFAniForNativeCurve, ZFPropertyAccess(ZFAniForNative, curve),
            ZFCoreArrayCreate(ZFAniForNativeCurve
                , ZFAniForNativeCurve::e_Linear
                , ZFAniForNativeCurve::e_EaseIn
                , ZFAniForNativeCurve::e_EaseOut
                , ZFAniForNativeCurve::e_EaseInOut
                ));

        { // aniAlpha
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, alphaFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, alphaTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniAlpha

        { // scale
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, scaleXFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, scaleXTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, scaleYFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, scaleYTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, scaleZFrom),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, scaleZTo),
                ZFCoreArrayCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // scale

        { // translate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, translateXFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, translateXTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, translateYFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, translateYTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, translateZFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, translateZTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 100
                    ));
        } // translate

        { // rotate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, rotateXFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, rotateXTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, rotateYFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, rotateYTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, rotateZFrom),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAniForNative, rotateZTo),
                ZFCoreArrayCreate(zffloat
                    , 0
                    , 90
                    ));
        } // rotate

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAniForNative_test)

ZF_NAMESPACE_GLOBAL_END

