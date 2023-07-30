#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_zfasync_test : zfextends ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_zfasync_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFThread);
        ZFFramework_test_asyncTestCheck();

        ZFTestCase *testCase = this;

        ZFLISTENER_0(thread1) {
            zfLog() << "thread1: " << ZFThread::currentThread();

            ZFLISTENER_0(thread2) {
                zfLog() << "thread2: " << ZFThread::currentThread();
            } ZFLISTENER_END()
            ZFLISTENER_0(thread2OnStop) {
                zfLog() << "thread2 stop: " << ZFThread::currentThread();
            } ZFLISTENER_END()
            zfasync(thread2, thread2OnStop);
        } ZFLISTENER_END()

        ZFLISTENER_1(thread1OnStop
                , ZFTestCase *, testCase
                ) {
            zfLog() << "thread1 stop: " << ZFThread::currentThread();

            ZFLISTENER_0(thread3) {
                zfLog() << "thread3: " << ZFThread::currentThread();
            } ZFLISTENER_END()
            ZFLISTENER_1(thread3OnStop
                    , ZFTestCase *, testCase
                    ) {
                zfLog() << "thread3 stop: " << ZFThread::currentThread();

                testCase->testCaseStop();
            } ZFLISTENER_END()
            zfasync(thread3, thread3OnStop);
        } ZFLISTENER_END()

        zfasync(thread1, thread1OnStop);
    }
};
ZFOBJECT_REGISTER(ZFCore_zfasync_test)

ZF_NAMESPACE_GLOBAL_END

