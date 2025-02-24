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
            zfobj<ZFUdp> server;
            ZFLISTENER(serverOnDealloc) {
                ZFLogTrim() << "server dealloc";
            } ZFLISTENER_END()
            server->observerAdd(ZFObject::E_ObjectBeforeDealloc(), serverOnDealloc);

            this->output("server start");
            zfbool serverOpenSuccess = server->open(zfmRand(1025, 65536));
            ZFTestCaseAssert(serverOpenSuccess);
            this->output(zfstr("server start success: %s", server->objectInfo()));
            serverPort = server->port();

            ZFLISTENER_1(serverRecvThread
                    , zfautoT<ZFUdp>, server
                    ) {
                zfstring buf;
                ZFUdpAddr recvAddr;
                while(server->valid()) {
                    buf.removeAll();
                    zfindex recvSize = server->recv(recvAddr, buf, 4096);
                    if(recvSize > 0) {
                        ZFLog() << "server recv: " << buf;
                        server->send(recvAddr, "server reply");
                        break;
                    }
                    ZFThread::sleep(100);
                }
            } ZFLISTENER_END()
            zfasync(serverRecvThread);
        }

        {
            zfobj<ZFUdp> client;
            ZFLISTENER(clientOnDealloc) {
                ZFLogTrim() << "client dealloc";
            } ZFLISTENER_END()
            client->observerAdd(ZFObject::E_ObjectBeforeDealloc(), clientOnDealloc);

            this->output("client start");
            zfbool clientOpenSuccess = client->open();
            ZFTestCaseAssert(clientOpenSuccess);
            this->output(zfstr("client start success: %s", client->objectInfo()));

            ZFTestCase *testCase = this;
            ZFLISTENER_2(clientRecvThread
                    , zfautoT<ZFTestCase>, testCase
                    , zfautoT<ZFUdp>, client
                    ) {
                zfstring buf;
                ZFUdpAddr hostAddr;
                while(client->valid()) {
                    buf.removeAll();
                    zfindex recvSize = client->recv(hostAddr, buf);
                    if(recvSize > 0) {
                        ZFLog() << "client recv: " << buf;

                        testCase->stop();
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

