#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFTestCase_debug_LogEvent)
{
    ZFLISTENER(testCaseOnOutput) {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << listenerData.param0<v_zfstring *>()->zfv;
    } ZFLISTENER_END(testCaseOnOutput)
    this->testCaseOnOutputListener = testCaseOnOutput;
    ZFGlobalObserver().observerAdd(ZFTestCase::EventTestCaseOnOutput(), this->testCaseOnOutputListener);

    ZFLISTENER(testCaseOnStart) {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << "========================== start ===========================";
    } ZFLISTENER_END(testCaseOnStart)
    this->testCaseOnStartListener = testCaseOnStart;
    ZFGlobalObserver().observerAdd(ZFTestCase::EventTestCaseOnStart(), this->testCaseOnStartListener);

    ZFLISTENER(testCaseOnProgress) {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << "progress updated";
    } ZFLISTENER_END(testCaseOnProgress)
    this->testCaseOnProgressListener = testCaseOnProgress;
    ZFGlobalObserver().observerAdd(ZFTestCase::EventTestCaseOnProgress(), this->testCaseOnProgressListener);

    ZFLISTENER(testCaseOnStop) {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << "-------------------------- stop ----------------------------";
    } ZFLISTENER_END(testCaseOnStop)
    this->testCaseOnStopListener = testCaseOnStop;
    ZFGlobalObserver().observerAdd(ZFTestCase::EventTestCaseOnStop(), this->testCaseOnStopListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFTestCase_debug_LogEvent)
{
    ZFGlobalObserver().observerRemove(ZFTestCase::EventTestCaseOnOutput(), this->testCaseOnOutputListener);
    ZFGlobalObserver().observerRemove(ZFTestCase::EventTestCaseOnStart(), this->testCaseOnStartListener);
    ZFGlobalObserver().observerRemove(ZFTestCase::EventTestCaseOnProgress(), this->testCaseOnProgressListener);
    ZFGlobalObserver().observerRemove(ZFTestCase::EventTestCaseOnStop(), this->testCaseOnStopListener);
}
private:
    ZFListener testCaseOnOutputListener;
    ZFListener testCaseOnStartListener;
    ZFListener testCaseOnProgressListener;
    ZFListener testCaseOnStopListener;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFTestCase_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

