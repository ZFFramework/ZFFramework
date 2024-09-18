#include "ZFUIWebKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWebKit_ZFUIWebView_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWebKit_ZFUIWebView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIWebView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIWebView> testView;
        container->childAdd(testView)->c_sizeFill()->c_margin(40);
        testView->viewBackgroundColor(ZFUIColorRed());

        this->prepareSettingButton(window, testView);

        ZFLISTENER(loadStateOnUpdate) {
            ZFLogTrim() << "webLoadingOnUpdate " << zfargs.sender()->to<ZFUIWebView *>()->webLoading();
        } ZFLISTENER_END()
        testView->observerAdd(ZFUIWebView::EventWebLoadStateOnUpdate(), loadStateOnUpdate);

        testView->webLoadUrl("http://www.baidu.com");
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIWebView *testView
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWebKit_ZFUIWebView_test)

ZF_NAMESPACE_GLOBAL_END

