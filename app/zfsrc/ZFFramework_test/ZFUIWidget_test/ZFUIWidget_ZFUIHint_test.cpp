#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIHint_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIHint_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIRootWindow);
        ZFFramework_test_asyncTestCheck();

        zfauto hint = ZFUIHintShow("this is a normal hint");
        ZFTestCase *testCase = this;
#if 0
        ZFLISTENER_1(hintOnHide
                , ZFTestCase *, testCase
                ) {
            ZFLog();
            testCase->stop();
        } ZFLISTENER_END()
#else
        ZFLISTENER_1(hintOnHide
                , ZFTestCase *, testCase
                ) {
            for(zfindex i = 0; i < 3; ++i) {
                ZFUIHintShow(zfstr("this is a stacked hint %s", i));
            }
            zfauto last = ZFUIHintShow("this is a very"
                " long long long long long long long long long long"
                " long long long long long long long long long long"
                " long long long long long long long long long hint");
            ZFLISTENER_1(lastHintOnHide
                , ZFTestCase *, testCase
                ) {
                testCase->stop();
            } ZFLISTENER_END()
            last->observerAdd(ZFUIHint::E_HintOnHide(), lastHintOnHide);
        } ZFLISTENER_END()
#endif
        hint->observerAdd(ZFUIHint::E_HintOnHide(), hintOnHide);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIHint_test)

ZF_NAMESPACE_GLOBAL_END

