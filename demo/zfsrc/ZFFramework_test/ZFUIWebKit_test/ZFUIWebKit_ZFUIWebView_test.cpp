#include "ZFUIWebKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWebKit_ZFUIWebView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWebKit_ZFUIWebView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIWebView, testView);
        container->childAdd(testView)->c_sizeFill()->c_margin(40);
        testView->viewBackgroundColor(ZFUIColorRed());

        this->prepareSettingButton(window, testView);

        ZFLISTENER(loadStateOnChange) {
            zfLogTrimT() << "webLoadingOnChange" << listenerData.sender()->to<ZFUIWebView *>()->webLoading();
        } ZFLISTENER_END(loadStateOnChange)
        testView->observerAdd(ZFUIWebView::EventWebLoadStateOnChange(), loadStateOnChange);

        testView->webLoadUrl("http://www.baidu.com");
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIWebView *testView)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWebKit_ZFUIWebView_test)

ZF_NAMESPACE_GLOBAL_END

