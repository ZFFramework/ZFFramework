#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFThread_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFThread_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFThread);
        ZFFramework_test_asyncTestCheck();

        ZFLogTrim("============================================================");
        ZFLogTrim("ZFThread");
        zfautoT<ZFThread> thread;

#if 1
        ZFLogTrim("execute main and thread without lock (async):");
        ZFLISTENER(asyncFunc) {
            ZFLogTrim("async thread begin");
            for(zfindex i = 0; i < 5; ++i) {
                for(zfindex j = 0; j < 10; ++j) {
                    ZFLogTrim("  async thread: %s %s", i, j);
                    ZFThread::sleep((zftimet)20);
                }
                ZFThread::sleep((zftimet)300);
            }
            ZFLogTrim("async thread end");
        } ZFLISTENER_END()
        thread = zfobj<ZFThread>(asyncFunc);
        thread->threadStart();
        for(zfindex i = 0; i < 5; ++i) {
            for(zfindex j = 0; j < 10; ++j) {
                ZFLogTrim("  main: %s %s", i, j);
                ZFThread::sleep((zftimet)20);
            }
            ZFThread::sleep((zftimet)190);
        }
        ZFLogTrim("main thread wait async thread begin");
        thread->threadWait();
        ZFLogTrim("main thread wait async thread complete");
#endif

#if 1
        ZFLogTrim("============================================================");
        ZFLogTrim("execute main and thread with lock (sync):");
        ZFLISTENER(syncFunc) {
            ZFLogTrim("sync thread begin");
            for(zfindex i = 0; i < 5; ++i) {
                ZFCoreMutexLock();
                for(zfindex j = 0; j < 10; ++j) {
                    ZFLogTrim("  sync thread: %s %s", i, j);
                    ZFThread::sleep((zftimet)20);
                }
                ZFCoreMutexUnlock();
                ZFThread::sleep((zftimet)200);
            }
            ZFLogTrim("sync thread end");
        } ZFLISTENER_END()
        thread = zfobj<ZFThread>(syncFunc);
        thread->threadStart();
        for(zfindex i = 0; i < 5; ++i) {
            ZFCoreMutexLock();
            for(zfindex j = 0; j < 10; ++j) {
                ZFLogTrim("  main:   %s %s", i, j);
                ZFThread::sleep((zftimet)20);
            }
            ZFCoreMutexUnlock();
            ZFThread::sleep((zftimet)190);
        }
        ZFLogTrim("main thread wait sync thread begin");
        thread->threadWait();
        ZFLogTrim("main thread wait sync thread complete");
#endif

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFThread_test)

ZF_NAMESPACE_GLOBAL_END

