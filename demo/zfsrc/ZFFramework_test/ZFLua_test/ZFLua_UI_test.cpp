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

        zfautoObject result = ZFLuaExecute(
                "local window = ZFUIWindow()\n"
                "window:windowShow()\n"
                "window:viewBackgroundColor(ZFUIColorRandom())\n"
                "local button = ZFUIButtonBasic()\n"
                "window:childAdd(button):alignRightTop()\n"
                "button:viewBackgroundColor(ZFUIColorRandom())\n"
                "button:buttonLabelText('close')\n"
                "button:observerAdd(ZFUIButton.EventButtonOnClick(), function (listenerData, userData)\n"
                "        userData:objectHolded():windowHide()\n"
                "    end, window:objectHolder())\n"
                "return window\n"
            );
        zfCoreAssert(result != zfnull);

        ZFLISTENER(windowOnHide) {
            ZFLISTENER(testCaseStopDelay) {
                ZFLuaGC();
                ZFTestCase *testCase = userData->objectHolded();
                testCase->testCaseStop();
            } ZFLISTENER_END(testCaseStopDelay)
            ZFThread::post(testCaseStopDelay, userData);
        } ZFLISTENER_END(windowOnHide)
        result->observerAdd(ZFUIWindow::EventWindowOnHide(), windowOnHide, this->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFLua_UI_test)

ZF_NAMESPACE_GLOBAL_END

