#include "ZFLua_test.h"
#include "ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_UI_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_UI_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFLua);
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        zfautoObject result = ZFLuaExecute(
                "local window = ZFUIWindow()\n"
                "window:windowShow()\n"
                "window:viewBackgroundColor(ZFUIColorRandom())\n"
                "local button = ZFUIButtonBasic()\n"
                "window:childAdd(button):alignRightTop()\n"
                "button:viewBackgroundColor(ZFUIColorRandom())\n"
                "button:label():text('close')\n"
                "button:observerAdd(ZFUIButton.EventButtonOnClick(), function(zfargs)\n"
                "    window:windowHide()\n"
                "end)\n"
                "return window\n"
            );
        zfCoreAssert(result != zfnull);

        ZFTestCase *testCase = this;
        ZFLISTENER_1(windowOnHide
                , ZFTestCase *, testCase
                ) {
            ZFLISTENER_1(testCaseStopDelay
                    , ZFTestCase *, testCase
                    ) {
                ZFLuaGC();
                testCase->testCaseStop();
            } ZFLISTENER_END()
            ZFThread::post(testCaseStopDelay);
        } ZFLISTENER_END()
        result->observerAdd(ZFUIWindow::EventWindowOnHide(), windowOnHide);
    }
};
ZFOBJECT_REGISTER(ZFLua_UI_test)

ZF_NAMESPACE_GLOBAL_END

