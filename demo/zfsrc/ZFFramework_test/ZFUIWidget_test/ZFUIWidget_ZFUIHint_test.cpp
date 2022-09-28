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
#if 0
        ZFLISTENER(hintOnHide) {
            zfLogT();
            userData->objectHolded<ZFTestCase *>()->testCaseStop();
        } ZFLISTENER_END(hintOnHide)
#else
        ZFLISTENER(hintOnHide) {
            for(zfindex i = 0; i < 3; ++i)
            {
                ZFUIHintShow(zfstringWithFormat("this is a stacked hint %zi", i));
            }
            zfautoObject last = ZFUIHintShow("this is a very"
                " long long long long long long long long long long"
                " long long long long long long long long long long"
                " long long long long long long long long long hint");
            ZFLISTENER(lastHintOnHide) {
                userData->objectHolded<ZFTestCase *>()->testCaseStop();
            } ZFLISTENER_END(lastHintOnHide)
            last.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), lastHintOnHide, userData);
        } ZFLISTENER_END(hintOnHide)
#endif
        hint.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), hintOnHide, this->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIHint_test)

ZF_NAMESPACE_GLOBAL_END

