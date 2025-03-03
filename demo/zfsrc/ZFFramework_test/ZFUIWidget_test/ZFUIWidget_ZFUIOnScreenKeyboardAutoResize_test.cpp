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

        container->bgColor(ZFUIColorGreen());
        for(zfindex i = 0; i < 3; ++i) {
            zfobj<ZFUITextEdit> view;
            container->child(view)->c_widthFill()->c_margin(10);
            view->bgColor(ZFUIColorRandom());
        }
        container->childAt(0)->layoutParam()->align(v_ZFUIAlign::e_Top);
        container->childAt(1)->layoutParam()->align(v_ZFUIAlign::e_Center);
        container->childAt(2)->layoutParam()->align(v_ZFUIAlign::e_Bottom);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIOnScreenKeyboardAutoResize_test)

ZF_NAMESPACE_GLOBAL_END

