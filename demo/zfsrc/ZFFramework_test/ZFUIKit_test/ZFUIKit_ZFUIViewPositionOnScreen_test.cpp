#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIViewPositionOnScreen_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIViewPositionOnScreen_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIViewPositionOnScreen);
        ZFFramework_test_asyncTestCheck();

        zfautoT<ZFUIWindow> window;
        zfautoT<ZFUIView> container;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIKit_test_Button> button;
        container->child(button)->c_alignCenter();
        button->label()->text("click me");

        ZFLISTENER(buttonOnClick) {
            ZFLogTrim() << "window size: " << ZFUIRectGetSize(ZFUIViewUtil::viewRoot(zfargs.sender())->viewFrame());
            ZFLogTrim() << "clicked view's position: " << ZFUIViewPositionOnScreen(zfargs.sender());
        } ZFLISTENER_END()
        button->observerAdd(ZFUIButton::E_ButtonOnClick(), buttonOnClick);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewPositionOnScreen_test)

ZF_NAMESPACE_GLOBAL_END

