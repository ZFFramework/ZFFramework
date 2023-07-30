#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFHttpRequest_test : zfextends ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFNet_ZFHttpRequest_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFHttpRequest);
        ZFFramework_test_asyncTestCheck();

        const zfchar *url = ZFHttpRequest::httpsAvailable()
            ? "https://bing.com"
            : "http://bing.com"
            ;
        ZFHttpMethodEnum httpMethod = ZFHttpMethod::e_GET;

        this->testCaseOutput("header test");
        zfblockedAlloc(ZFHttpRequest, headerTest, url, httpMethod);
        headerTest->header("k1", "v1_1");
        headerTest->header("k1", "v1_2");
        headerTest->header("k2", "v2_1");
        headerTest->header("k2", "v2_2");
        headerTest->headerRemove("k2");
        this->testCaseOutput("%s", headerTest->contentInfo().cString());
        this->testCaseOutputSeparator();

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
        } ZFLISTENER_END()
        zflineAlloc(ZFHttpRequest, url, httpMethod)
            ->request(onRecv);

        zflineAlloc(ZFHttpRequest, url, httpMethod)
            ->request(onRecv)
            ->requestCancel();
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFHttpRequest_test)

ZF_NAMESPACE_GLOBAL_END

