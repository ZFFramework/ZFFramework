#include "ZFLua_test.h"
#include "ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_UI_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFLua_UI_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFLua);
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        zfauto result = ZFLuaExecute(
                "local window = ZFUIWindow()\n"
                "window:show()\n"
                "window:bgColor(ZFUIColorRandom())\n"
                "local button = ZFUIButtonBasic()\n"
                "window:child(button):alignRightTop()\n"
                "button:bgColor(ZFUIColorRandom())\n"
                "button:label():text('close')\n"
                "button:observerAdd(ZFUIButton.E_ButtonOnClick(), function(zfargs)\n"
                "    window:hide()\n"
                "end)\n"
                "return window\n"
            );
        ZFCoreAssert(result != zfnull);

        ZFTestCase *testCase = this;
        ZFLISTENER_1(windowOnHide
                , ZFTestCase *, testCase
                ) {
            ZFLISTENER_1(stopDelay
                    , ZFTestCase *, testCase
                    ) {
                ZFLuaGC();
                testCase->stop();
            } ZFLISTENER_END()
            zfpost(stopDelay);
        } ZFLISTENER_END()
        result->observerAdd(ZFUIWindow::E_WindowOnHide(), windowOnHide);
    }
};
ZFOBJECT_REGISTER(ZFLua_UI_test)

ZF_NAMESPACE_GLOBAL_END

