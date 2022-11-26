#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_scale_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_scale_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUITextView, view);
        container->childAdd(view)->c_alignCenter();
        view->text("test text");

        this->prepareSettingButton(window);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window)
    {
        zfblockedAlloc(ZFArray, settings);

        { // UIScale
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            ZFLISTENER(buttonTextGetter) {
                v_zfstring *text = zfargs.param0T();
                text->zfv = zfstringWithFormat(
                    "scale: %f",
                    ZFUISysWindow::mainWindow()->rootView()->UIScale());
            } ZFLISTENER_END(buttonTextGetter)
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER(buttonClickListener) {
                ZFUIRootView *rootView = ZFUISysWindow::mainWindow()->rootView();
                if(rootView->UIScale() == 1)
                {
                    rootView->UIScale(2);
                }
                else
                {
                    rootView->UIScale(1);
                }
            } ZFLISTENER_END(buttonClickListener)
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_scale_test)

ZF_NAMESPACE_GLOBAL_END

