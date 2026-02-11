#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUITextEdit_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUITextEdit_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUITextEdit);
        ZFFramework_test_asyncTestCheck();

        zfautoT<ZFUIWindow> window;
        zfautoT<ZFUIView> container;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        ZFLISTENER(onClickReturn) {
            ZFUITextEdit *textEdit = zfargs.sender();
            textEdit->focusRequest(zffalse);
        } ZFLISTENER_END()

        zfobj<ZFUITextEdit> view;
        container->child(view)->c_widthFill()->c_margin(40);
        view->bgColor(ZFUIColorCreateRGB(0xFF0000));
        view->observerAdd(ZFUITextEdit::E_TextOnReturnClick(), onClickReturn);
        view->placeholder()->text("first text edit");

        zfobj<ZFUITextEdit> view2;
        container->child(view2)->c_sizeFill()->c_margin(40, 100, 40, 40);
        view2->bgColor(ZFUIColorCreateRGB(0x0000FF));
        view2->observerAdd(ZFUITextEdit::E_TextOnReturnClick(), onClickReturn);
        view2->placeholder()->text("second text edit");

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUITextEdit *view
            ) {
        zfobj<ZFArray> settings;
        ZFUIKit_test_prepareSettingForBoolProperty(settings, view, ZFPropertyAccess(ZFUITextEdit, editEnable));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUITextEdit_test)

ZF_NAMESPACE_GLOBAL_END

