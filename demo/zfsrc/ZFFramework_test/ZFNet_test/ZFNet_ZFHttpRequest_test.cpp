#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFHttpRequest_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFNet_ZFHttpRequest_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFHttpRequest);
        ZFFramework_test_asyncTestCheck();

        ZFTestCase *testCase = this;
        ZFLISTENER_1(onRecv
                , ZFTestCase *, testCase
                ) {
            ZFHttpRequest *request = zfargs.senderT();
            ZFHttpResponse *response = zfargs.param0T();

            testCase->testCaseOutput("send: %s", request->objectInfo().cString());
            testCase->testCaseOutput("%s", request->contentInfo().cString());
            testCase->testCaseOutput("recv: %s", response->objectInfo().cString());
            testCase->testCaseOutput("%s", response->contentInfo().cString());
            testCase->testCaseStop();
        } ZFLISTENER_END(onRecv)
        zflineAlloc(ZFHttpRequest, "https://bing.com", "GET")
            ->request(onRecv);

        zflineAlloc(ZFHttpRequest, "https://bing.com", "GET")
            ->request(onRecv)
            ->requestCancel();
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFHttpRequest_test)

ZF_NAMESPACE_GLOBAL_END

