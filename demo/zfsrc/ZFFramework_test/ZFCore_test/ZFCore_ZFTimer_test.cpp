#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFTimer_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFTimer_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFTimer);
        ZFFramework_test_asyncTestCheck();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFTimer");
        this->testCaseOutput(zfstr("current thread: %s", ZFThread::currentThread()->objectInfo()));

        zfblockedAlloc(ZFTimer, timer);

        ZFTestCase *owner = this;
        ZFLISTENER_1(timerOnActivate
                , ZFTestCase *, owner
                ) {
            ZFTimer *timer = zfargs.sender();
            zfLogTrim("timer event, current thread: %s", ZFThread::currentThread());
            if(timer->timerActivatedCount() >= 3) {
                timer->timerStop();

                ZFLISTENER_1(action
                        , ZFTestCase *, owner
                        ) {
                    owner->testCaseStop();
                } ZFLISTENER_END()
                ZFThread::mainThread()->taskQueueAdd(action);
            }
        } ZFLISTENER_END()
        timer->observerAdd(ZFTimer::EventTimerOnActivate(), timerOnActivate);
        timer->timerInterval((zftimet)1000);
        timer->timerStart();

        ZFTestCase *testCase = this;
        ZFLISTENER_1(timerOnce
                , zfautoT<ZFTestCase *>, testCase
                ) {
            testCase->testCaseOutput(zfstr("timerOnce activated, current thread: %s", ZFThread::currentThread()->objectInfo()));
        } ZFLISTENER_END()
        testCase->testCaseOutput("timerOnce start");
        ZFTimerOnce(500, timerOnce);
        testCase->testCaseOutput("timerOnce stop");
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTimer_test)

ZF_NAMESPACE_GLOBAL_END

