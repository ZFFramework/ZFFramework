#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFUdp_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFNet_ZFUdp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
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
            ZFTestCaseAssert(server->open() && server->port() != 0);
            this->testCaseOutput("server start success on port: %u", server->port());
            serverPort = server->port();

            ZFLISTENER_1(serverRecvThread
                    , zfautoObjectT<ZFUdp *>, server
                    ) {
                while(server->port() != 0)
                {
                    zfautoObjectT<ZFUdpPacket *> recv = server->recv();
                    if(recv != zfnull)
                    {
                        zfLog() << "server recv:" << recv;
                        zfblockedAlloc(ZFUdpPacket, send);
                        send->host = recv->host;
                        send->port = recv->port;
                        send->data.bufferAppend("server reply");
                        server->send(send);

                        server->close();
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
            ZFTestCaseAssert(client->open() && client->port() != 0);
            this->testCaseOutput("client start success on port: %u", client->port());

            ZFTestCase *testCase = this;
            ZFLISTENER_2(clientRecvThread
                    , zfautoObjectT<ZFTestCase *>, testCase
                    , zfautoObjectT<ZFUdp *>, client
                    ) {
                while(client->port() != 0)
                {
                    zfautoObjectT<ZFUdpPacket *> recv = client->recv();
                    if(recv != zfnull)
                    {
                        zfLog() << "client recv:" << recv;

                        client->close();
                        testCase->testCaseStop();
                        break;
                    }
                    ZFThread::sleep(100);
                }
            } ZFLISTENER_END()
            zfasync(clientRecvThread);

            zfblockedAlloc(ZFUdpPacket, send);
            send->host = "localhost";
            send->port = serverPort;
            send->data.bufferAppend("client send");
            client->send(send);
        }
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFUdp_test)

ZF_NAMESPACE_GLOBAL_END

