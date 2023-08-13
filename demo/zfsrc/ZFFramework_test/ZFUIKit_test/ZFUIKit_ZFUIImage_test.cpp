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

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, view);
        container->childAdd(view)->c_sizeFill();

        // encode image to binary and load it again to test
        zfautoObject imageHolder = zfRes("test_normal.png");
        zfblockedAlloc(ZFIOBufferByMemory, io);
        ZFUIImageToOutput(io->output(), imageHolder);
        zfautoObject imageNew = ZFUIImageFromInput(io->input());
        view->image(imageNew);

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIImageView *view
            ) {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImage_test)

ZF_NAMESPACE_GLOBAL_END

