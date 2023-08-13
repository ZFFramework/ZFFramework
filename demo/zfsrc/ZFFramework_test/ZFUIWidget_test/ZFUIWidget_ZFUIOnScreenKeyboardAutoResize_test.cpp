#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIOnScreenKeyboardAutoResize_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIOnScreenKeyboardAutoResize_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        ZFUIOnScreenKeyboardAutoResizeStart(window);

        container->viewBackgroundColor(ZFUIColorGreen());
        for(zfindex i = 0; i < 3; ++i) {
            zfblockedAlloc(ZFUITextEdit, view);
            container->childAdd(view)->c_widthFill()->c_margin(10);
            view->viewBackgroundColor(ZFUIColorRandom());
        }
        container->childAt(0)->layoutParam()->layoutAlign(ZFUIAlign::e_Top);
        container->childAt(1)->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        container->childAt(2)->layoutParam()->layoutAlign(ZFUIAlign::e_Bottom);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIOnScreenKeyboardAutoResize_test)

ZF_NAMESPACE_GLOBAL_END

