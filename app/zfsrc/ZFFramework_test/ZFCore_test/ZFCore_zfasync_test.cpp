#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_zfasync_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_zfasync_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFThread);
        ZFFramework_test_asyncTestCheck();

        ZFTestCase *testCase = this;

        ZFLISTENER_0(thread1) {
            ZFLog() << "thread1: " << ZFThread::currentThread();

            ZFLISTENER_0(thread2) {
                ZFLog() << "thread2: " << ZFThread::currentThread();
            } ZFLISTENER_END()
            ZFLISTENER_0(thread2OnStop) {
                ZFLog() << "thread2 stop: " << ZFThread::currentThread();
            } ZFLISTENER_END()
            zfasync(thread2, thread2OnStop);
        } ZFLISTENER_END()

        ZFLISTENER_1(thread1OnStop
                , ZFTestCase *, testCase
                ) {
            ZFLog() << "thread1 stop: " << ZFThread::currentThread();

            ZFLISTENER_0(thread3) {
                ZFLog() << "thread3: " << ZFThread::currentThread();
            } ZFLISTENER_END()
            ZFLISTENER_1(thread3OnStop
                    , ZFTestCase *, testCase
                    ) {
                ZFLog() << "thread3 stop: " << ZFThread::currentThread();

                testCase->stop();
            } ZFLISTENER_END()
            zfasync(thread3, thread3OnStop);
        } ZFLISTENER_END()

        zfasync(thread1, thread1OnStop);
    }
};
ZFOBJECT_REGISTER(ZFCore_zfasync_test)

ZF_NAMESPACE_GLOBAL_END

