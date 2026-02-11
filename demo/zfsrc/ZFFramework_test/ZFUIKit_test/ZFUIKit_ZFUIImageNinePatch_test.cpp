#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIImageNinePatch_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageNinePatch_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIImage);
        ZFFramework_test_protocolCheck(ZFUIImageView);
        ZFFramework_test_asyncTestCheck();

        zfautoT<ZFUIWindow> window;
        zfautoT<ZFUIView> container;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIImageView> imageView;
        container->child(imageView)->c_alignCenter();
        imageView->image(zfres("test_normal.png").to<ZFCopyable *>()->copy());
        imageView->image()->imageNinePatch(ZFUIMarginCreate(
            imageView->image()->imageSize().width / 2,
            imageView->image()->imageSize().height / 2));

        this->prepareSettingButton(window, imageView);
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIImageView *imageView
            ) {
        zfobj<ZFArray> settings;

        { // wrap or fill
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , ZFUIImageView *, imageView
                    ) {
                v_zfstring *text = zfargs.param0();
                zfbool fill = (imageView->layoutParam()->sizeParam().width == v_ZFUISizeType::e_Fill);
                text->zfv = fill ? "fill" : "wrap";
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIImageView *, imageView
                    ) {
                zfbool fill = (imageView->layoutParam()->sizeParam().width == v_ZFUISizeType::e_Fill);
                imageView->layoutParam()->sizeParam(fill ? ZFUISizeParamWrapWrap() : ZFUISizeParamFillFill());
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImageNinePatch_test)

ZF_NAMESPACE_GLOBAL_END

