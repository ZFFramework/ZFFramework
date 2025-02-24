#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFHttpRequest_test : zfextend ZFFramework_test_TestCase {
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
        ZFHttpMethod httpMethod = ZFHttpMethod::e_GET;

        this->output("header test");
        zfobj<ZFHttpRequest> headerTest(url, httpMethod);
        headerTest->header("k1", "v1_1");
        headerTest->header("k1", "v1_2");
        headerTest->header("k2", "v2_1");
        headerTest->header("k2", "v2_2");
        headerTest->headerRemove("k2");
        this->output(zfstr("%s", headerTest->contentInfo()));
        this->outputSeparator();

        ZFTestCase *testCase = this;
        ZFLISTENER_1(onRecv
                , ZFTestCase *, testCase
                ) {
            ZFHttpRequest *request = zfargs.sender();
            ZFHttpResponse *response = zfargs.param0();

            testCase->output(zfstr("send: %s", request->objectInfo()));
            testCase->output(zfstr("%s", request->contentInfo()));
            testCase->output(zfstr("recv: %s", response->objectInfo()));
            testCase->output(zfstr("%s", response->contentInfo()));
            testCase->stop();
        } ZFLISTENER_END()
        zfobj<ZFHttpRequest>(url, httpMethod)
            ->c_request(onRecv);

        zfobj<ZFHttpRequest>(url, httpMethod)
            ->c_request(onRecv)
            ->c_requestCancel();
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFHttpRequest_test)

ZF_NAMESPACE_GLOBAL_END

