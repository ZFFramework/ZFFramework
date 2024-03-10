#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFUdp_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFNet_ZFUdp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUdp);
        ZFFramework_test_asyncTestCheck();

        zfuint serverPort = 0;
        {
            zfblockedAlloc(ZFUdp, server);
            ZFLISTENER(serverOnDealloc) {
                zfLogTrim() << "server dealloc";
            } ZFLISTENER_END()
            server->observerAdd(ZFObject::EventObjectBeforeDealloc(), serverOnDealloc);

            this->testCaseOutput("server start");
            zfbool serverOpenSuccess = server->open(zfmRand(1025, 65536));
            ZFTestCaseAssert(serverOpenSuccess);
            this->testCaseOutput(zfstr("server start success: %s", server->objectInfo()));
            serverPort = server->port();

            ZFLISTENER_1(serverRecvThread
                    , zfautoT<ZFUdp *>, server
                    ) {
                ZFBuffer buf;
                ZFUdpAddr recvAddr;
                while(server->valid()) {
                    zfindex recvSize = server->recv(recvAddr, buf, 4096);
                    if(recvSize > 0) {
                        zfLog() << "server recv: " << buf.text();
                        server->send(recvAddr, "server reply");
                        break;
                    }
                    ZFThread::sleep(100);
                }
            } ZFLISTENER_END()
            zfasync(serverRecvThread);
        }

        {
            zfblockedAlloc(ZFUdp, client);
            ZFLISTENER(clientOnDealloc) {
                zfLogTrim() << "client dealloc";
            } ZFLISTENER_END()
            client->observerAdd(ZFObject::EventObjectBeforeDealloc(), clientOnDealloc);

            this->testCaseOutput("client start");
            zfbool clientOpenSuccess = client->open();
            ZFTestCaseAssert(clientOpenSuccess);
            this->testCaseOutput(zfstr("client start success: %s", client->objectInfo()));

            ZFTestCase *testCase = this;
            ZFLISTENER_2(clientRecvThread
                    , zfautoT<ZFTestCase *>, testCase
                    , zfautoT<ZFUdp *>, client
                    ) {
                ZFBuffer buf;
                ZFUdpAddr hostAddr;
                while(client->valid()) {
                    zfindex recvSize = client->recv(hostAddr, buf);
                    if(recvSize > 0) {
                        zfLog() << "client recv: " << buf.text();

                        testCase->testCaseStop();
                        break;
                    }
                    ZFThread::sleep(100);
                }
            } ZFLISTENER_END()
            zfasync(clientRecvThread);

            client->send("localhost", serverPort, "client send");
        }
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFUdp_test)

ZF_NAMESPACE_GLOBAL_END

