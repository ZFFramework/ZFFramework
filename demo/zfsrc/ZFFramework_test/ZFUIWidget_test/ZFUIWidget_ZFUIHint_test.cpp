#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIHint_test : zfextends ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIHint_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUISysWindow);
        ZFFramework_test_asyncTestCheck();

        zfautoObject hint = ZFUIHintShow("this is a normal hint");
        ZFTestCase *testCase = this;
#if 0
        ZFLISTENER_1(hintOnHide
                , ZFTestCase *, testCase
                ) {
            zfLog();
            testCase->testCaseStop();
        } ZFLISTENER_END()
#else
        ZFLISTENER_1(hintOnHide
                , ZFTestCase *, testCase
                ) {
            for(zfindex i = 0; i < 3; ++i) {
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
            } ZFLISTENER_END()
            last.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), lastHintOnHide);
        } ZFLISTENER_END()
#endif
        hint.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), hintOnHide);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIHint_test)

ZF_NAMESPACE_GLOBAL_END

