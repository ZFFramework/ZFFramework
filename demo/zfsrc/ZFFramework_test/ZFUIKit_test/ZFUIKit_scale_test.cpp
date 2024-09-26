#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_scale_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_scale_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUITextView> view;
        container->child(view)->c_alignCenter();
        view->text("test text");

        this->prepareSettingButton(window);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window) {
        zfobj<ZFArray> settings;

        { // UIScale
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);
            ZFLISTENER(buttonTextGetter) {
                v_zfstring *text = zfargs.param0();
                text->zfv = zfstr(
                    "scale: %s",
                    ZFUISysWindow::mainWindow()->rootView()->UIScale());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER(buttonClickListener) {
                ZFUIRootView *rootView = ZFUISysWindow::mainWindow()->rootView();
                if(rootView->UIScale() == 1) {
                    rootView->UIScale(2);
                }
                else {
                    rootView->UIScale(1);
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_scale_test)

ZF_NAMESPACE_GLOBAL_END

