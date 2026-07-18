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

        this->outputSeparator();
        this->output("ZFTimer");
        this->output(zfstr("current thread: %s", ZFThread::currentThread()->objectInfo()));

        zfobj<ZFTimer> timer;

        ZFTestCase *owner = this;
        ZFLISTENER_1(timerOnActivate
                , ZFTestCase *, owner
                ) {
            ZFTimer *timer = zfargs.sender();
            ZFLogTrim("timer event, current thread: %s", ZFThread::currentThread());
            if(timer->activatedCount() >= 3) {
                timer->stop();

                ZFLISTENER_1(action
                        , ZFTestCase *, owner
                        ) {
                    owner->stop();
                } ZFLISTENER_END()
                zfpost(action);
            }
        } ZFLISTENER_END()
        timer->observerAdd(ZFTimer::E_TimerOnActivate(), timerOnActivate);
        timer->interval((zftimet)1000);
        timer->start();

        ZFTestCase *testCase = this;
        ZFLISTENER_1(timerOnce
                , zfautoT<ZFTestCase>, testCase
                ) {
            testCase->output(zfstr("timerOnce activated, current thread: %s", ZFThread::currentThread()->objectInfo()));
        } ZFLISTENER_END()
        testCase->output("timerOnce start");
        ZFTimerOnce(500, timerOnce);
        testCase->output("timerOnce stop");
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTimer_test)

ZF_NAMESPACE_GLOBAL_END

