#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFTcp_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFNet_ZFTcp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFTcp);
        ZFFramework_test_asyncTestCheck();

        zfuint serverPort = 0;
        {
            zfblockedAlloc(ZFTcp, server);
            ZFLISTENER(serverOnDealloc) {
                zfLogTrim() << "server dealloc";
            } ZFLISTENER_END()
            server->observerAdd(ZFObject::EventObjectBeforeDealloc(), serverOnDealloc);

            this->testCaseOutput("server start");
            zfbool serverOpenSuccess = server->open(zfnull, zfmRand(1025, 65536));
            ZFTestCaseAssert(serverOpenSuccess);
            this->testCaseOutput("server start success: %s", server->objectInfo().cString());
            serverPort = server->port();

            ZFLISTENER_1(serverRecvThread
                    , zfautoObjectT<ZFTcp *>, server
                    ) {
                ZFBuffer buf;
                while(server->valid())
                {
                    zfautoObjectT<ZFTcp *> conn = server->accept();
                    if(conn != zfnull)
                    {
                        buf.bufferSize(0);
                        conn->recv(buf, 4096);
                        zfLog() << "server recv:" << buf.text();

                        conn->send("server reply");
                        break;
                    }
                    ZFThread::sleep(100);
                }
            } ZFLISTENER_END()
            zfasync(serverRecvThread);
        }

        {
            zfblockedAlloc(ZFTcp, client);
            ZFLISTENER(clientOnDealloc) {
                zfLogTrim() << "client dealloc";
            } ZFLISTENER_END()
            client->observerAdd(ZFObject::EventObjectBeforeDealloc(), clientOnDealloc);

            this->testCaseOutput("client start");
            zfbool clientOpenSuccess = client->open("localhost", serverPort);
            ZFTestCaseAssert(clientOpenSuccess);
            this->testCaseOutput("client start success: %s", client->objectInfo().cString());

            ZFTestCase *testCase = this;
            ZFLISTENER_2(clientRecvThread
                    , zfautoObjectT<ZFTestCase *>, testCase
                    , zfautoObjectT<ZFTcp *>, client
                    ) {
                ZFBuffer buf;
                while(client->valid())
                {
                    buf.bufferSize(0);
                    client->recv(buf);
                    zfLog() << "client recv:" << buf.text();

                    testCase->testCaseStop();
                    break;
                }
            } ZFLISTENER_END()
            zfasync(clientRecvThread);

            client->send("client send");
        }
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFTcp_test)

ZF_NAMESPACE_GLOBAL_END
