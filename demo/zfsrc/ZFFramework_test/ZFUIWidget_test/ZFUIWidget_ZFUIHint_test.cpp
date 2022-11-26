#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIHint_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIHint_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUISysWindow);

        zfautoObject hint = ZFUIHintShow("this is a normal hint");
        ZFTestCase *testCase = this;
#if 0
        ZFLISTENER_1(hintOnHide
                , ZFTestCase *, testCase
                ) {
            zfLogT();
            testCase->testCaseStop();
        } ZFLISTENER_END(hintOnHide)
#else
        ZFLISTENER_1(hintOnHide
                , ZFTestCase *, testCase
                ) {
            for(zfindex i = 0; i < 3; ++i)
            {
                ZFUIHintShow(zfstringWithFormat("this is a stacked hint %zi", i));
            }
            zfautoObject last = ZFUIHintShow("this is a very"
                " long long long long long long long long long long"
                " long long long long long long long long long long"
                " long long long long long long long long long hint");
            ZFLISTENER_1(lastHintOnHide
                , ZFTestCase *, testCase
                ) {
                testCase->testCaseStop();
            } ZFLISTENER_END(lastHintOnHide)
            last.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), lastHintOnHide);
        } ZFLISTENER_END(hintOnHide)
#endif
        hint.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), hintOnHide);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIHint_test)

ZF_NAMESPACE_GLOBAL_END

