#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFInputForHttp_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFNet_ZFInputForHttp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFInputReadToOutput(ZFOutputDefault(), ZFInputForHttp("https://github.com"));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFInputForHttp_test)

ZF_NAMESPACE_GLOBAL_END
