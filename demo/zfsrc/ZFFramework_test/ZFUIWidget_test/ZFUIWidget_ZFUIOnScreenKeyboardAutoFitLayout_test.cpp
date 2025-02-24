#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIOnScreenKeyboardAutoFitLayout> layout;
        container->child(layout)->c_sizeFill();
        this->prepareSettingButton(window, layout);

        layout->bgColor(ZFUIColorGreen());
        for(zfindex i = 0; i < 3; ++i) {
            zfobj<ZFUITextEdit> view;
            layout->child(view)->c_widthFill()->c_margin(10);
            view->bgColor(ZFUIColorRandom());
        }
        layout->childAt(0)->layoutParam()->align(ZFUIAlign::e_Top);
        layout->childAt(1)->layoutParam()->align(ZFUIAlign::e_Center);
        layout->childAt(2)->layoutParam()->align(ZFUIAlign::e_Bottom);
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIOnScreenKeyboardAutoFitLayout *layout
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitEnable));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitFocusedViewToVisible));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitScrollEnable));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test)

ZF_NAMESPACE_GLOBAL_END

