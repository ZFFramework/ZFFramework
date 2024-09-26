#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFInputForHttp_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFNet_ZFInputForHttp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFHttpRequest);

        ZFInputRead(ZFOutputDefault(), ZFInputForHttp("https://bing.com"));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFInputForHttp_test)

ZF_NAMESPACE_GLOBAL_END

