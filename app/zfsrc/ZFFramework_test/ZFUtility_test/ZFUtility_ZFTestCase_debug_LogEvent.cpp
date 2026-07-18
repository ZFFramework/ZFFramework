#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFTestCase_debug_LogEvent) {
    ZFLISTENER(testCaseOnOutput) {
        ZFLogTrim()
            << ZFLogCurTimeString()
            << zfstr(" [%s] ", zfargs.sender()->classData()->classNameFull())
            << zfargs.param0()->to<v_zfstring *>()->zfv;
    } ZFLISTENER_END()
    this->testCaseOnOutputListener = testCaseOnOutput;
    ZFGlobalObserver().observerAdd(ZFTestCase::E_TestCaseOnOutput(), this->testCaseOnOutputListener);

    ZFLISTENER(testCaseOnStart) {
        ZFLogTrim()
            << ZFLogCurTimeString()
            << zfstr(" [%s] ", zfargs.sender()->classData()->classNameFull())
            << "========================== start ===========================";
    } ZFLISTENER_END()
    this->testCaseOnStartListener = testCaseOnStart;
    ZFGlobalObserver().observerAdd(ZFTestCase::E_TestCaseOnStart(), this->testCaseOnStartListener);

    ZFLISTENER(testCaseOnProgress) {
        ZFLogTrim()
            << ZFLogCurTimeString()
            << zfstr(" [%s] ", zfargs.sender()->classData()->classNameFull())
            << "progress updated";
    } ZFLISTENER_END()
    this->testCaseOnProgressListener = testCaseOnProgress;
    ZFGlobalObserver().observerAdd(ZFTestCase::E_TestCaseOnProgress(), this->testCaseOnProgressListener);

    ZFLISTENER(testCaseOnStop) {
        ZFLogTrim()
            << ZFLogCurTimeString()
            << zfstr(" [%s] ", zfargs.sender()->classData()->classNameFull())
            << "-------------------------- stop ----------------------------";
    } ZFLISTENER_END()
    this->testCaseOnStopListener = testCaseOnStop;
    ZFGlobalObserver().observerAdd(ZFTestCase::E_TestCaseOnStop(), this->testCaseOnStopListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFTestCase_debug_LogEvent) {
    ZFGlobalObserver().observerRemove(ZFTestCase::E_TestCaseOnOutput(), this->testCaseOnOutputListener);
    ZFGlobalObserver().observerRemove(ZFTestCase::E_TestCaseOnStart(), this->testCaseOnStartListener);
    ZFGlobalObserver().observerRemove(ZFTestCase::E_TestCaseOnProgress(), this->testCaseOnProgressListener);
    ZFGlobalObserver().observerRemove(ZFTestCase::E_TestCaseOnStop(), this->testCaseOnStopListener);
}
private:
    ZFListener testCaseOnOutputListener;
    ZFListener testCaseOnStartListener;
    ZFListener testCaseOnProgressListener;
    ZFListener testCaseOnStopListener;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFTestCase_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

