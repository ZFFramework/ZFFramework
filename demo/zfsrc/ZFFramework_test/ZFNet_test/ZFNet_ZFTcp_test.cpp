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
            void *serverSocket = server->open(zfnull, zfmRand(1024, 65536));
            ZFTestCaseAssert(serverSocket != zfnull);
            this->testCaseOutput("server start success on port: %u", server->port());
            serverPort = server->port();

            ZFLISTENER_2(serverRecvThread
                    , zfautoObjectT<ZFTcp *>, server
                    , void *, serverSocket
                    ) {
                while(server->port() != 0)
                {
                    void *recvSocket = server->accept();
                    if(recvSocket != zfnull)
                    {
                        ZFBuffer buf;
                        server->recv(recvSocket, buf, 4096);
                        zfLog() << "server recv:" << buf.text();

                        server->send(recvSocket, "server reply");

                        server->close(recvSocket);
                        server->close(serverSocket);
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
            void *clientSocket = client->open("localhost", serverPort);
            ZFTestCaseAssert(clientSocket != zfnull);
            this->testCaseOutput("client start success on port: %u", client->port());

            ZFTestCase *testCase = this;
            ZFLISTENER_3(clientRecvThread
                    , zfautoObjectT<ZFTestCase *>, testCase
                    , zfautoObjectT<ZFTcp *>, client
                    , void *, clientSocket
                    ) {
                while(client->port() != 0)
                {
                    zfstring buf;
                    client->recv(clientSocket, ZFOutputForString(buf), zfindexMax());
                    zfLog() << "client recv:" << buf;

                    client->close(clientSocket);
                    testCase->testCaseStop();
                }
            } ZFLISTENER_END()
            zfasync(clientRecvThread);

            client->send(clientSocket, "client send");
        }
    }
};
ZFOBJECT_REGISTER(ZFNet_ZFTcp_test)

ZF_NAMESPACE_GLOBAL_END

