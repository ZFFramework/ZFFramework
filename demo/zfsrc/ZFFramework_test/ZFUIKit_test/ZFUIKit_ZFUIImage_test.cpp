#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIImage_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImage_test, ZFFramework_test_TestCase)

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

        zfobj<ZFUIImageView> view;
        container->child(view)->c_sizeFill();

        // encode image to binary and load it again to test
        zfauto imageHolder = zfres("test_normal.png");
        zfobj<ZFIOBuffer> io;
        ZFUIImageToOutput(io->output(), imageHolder);
        io->input().ioSeek(0);
        zfauto imageNew = ZFUIImageFromInput(io->input());
        view->image(imageNew);

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIImageView *view
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImage_test)

ZF_NAMESPACE_GLOBAL_END

