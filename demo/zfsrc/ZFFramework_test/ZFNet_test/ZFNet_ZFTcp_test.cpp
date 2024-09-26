#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNet_ZFTcp_test : zfextend ZFFramework_test_TestCase
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
            zfobj<ZFTcp> server;
            ZFLISTENER(serverOnDealloc) {
                ZFLogTrim() << "server dealloc";
            } ZFLISTENER_END()
            server->observerAdd(ZFObject::EventObjectBeforeDealloc(), serverOnDealloc);

            this->output("server start");
            zfbool serverOpenSuccess = server->open(zfnull, zfmRand(1025, 65536));
            ZFTestCaseAssert(serverOpenSuccess);
            this->output(zfstr("server start success: %s", server));
            serverPort = server->port();

            ZFLISTENER_1(serverRecvThread
                    , zfautoT<ZFTcp>, server
                    ) {
                ZFBuffer buf;
                while(server->valid())
                {
                    zfautoT<ZFTcp> conn = server->accept();
                    if(conn != zfnull)
                    {
                        buf.length(0);
                        conn->recv(buf, 4096);
                        ZFLog() << "server recv: " << buf.text();

                        conn->send("server reply");
                        break;
                    }
                    ZFThread::sleep(100);
                }
            } ZFLISTENER_END()
            zfasync(serverRecvThread);
        }

        {
            zfobj<ZFTcp> client;
            ZFLISTENER(clientOnDealloc) {
                ZFLogTrim() << "client dealloc";
            } ZFLISTENER_END()
            client->observerAdd(ZFObject::EventObjectBeforeDealloc(), clientOnDealloc);

            this->output("client start");
            zfbool clientOpenSuccess = client->open("localhost", serverPort);
            ZFTestCaseAssert(clientOpenSuccess);
            this->output(zfstr("client start success: %s", client));

            ZFTestCase *testCase = this;
            ZFLISTENER_2(clientRecvThread
                    , zfautoT<ZFTestCase>, testCase
                    , zfautoT<ZFTcp>, client
                    ) {
                ZFBuffer buf;
                while(client->valid())
                {
                    buf.length(0);
                    client->recv(buf);
                    ZFLog() << "client recv: " << buf.text();

                    testCase->stop();
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

