#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFTestCase_debug_LogEvent)
{
    ZFLISTENER(testCaseOnOutput) {
        zfLogTrim()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", zfargs.sender()->classData()->classNameFull())
            << zfargs.param0()->to<v_zfstring *>()->zfv;
    } ZFLISTENER_END()
    this->testCaseOnOutputListener = testCaseOnOutput;
    ZFGlobalObserver().observerAdd(ZFTestCase::EventTestCaseOnOutput(), this->testCaseOnOutputListener);

    ZFLISTENER(testCaseOnStart) {
        zfLogTrim()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", zfargs.sender()->classData()->classNameFull())
            << "========================== start ===========================";
    } ZFLISTENER_END()
    this->testCaseOnStartListener = testCaseOnStart;
    ZFGlobalObserver().observerAdd(ZFTestCase::EventTestCaseOnStart(), this->testCaseOnStartListener);

    ZFLISTENER(testCaseOnProgress) {
        zfLogTrim()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", zfargs.sender()->classData()->classNameFull())
            << "progress updated";
    } ZFLISTENER_END()
    this->testCaseOnProgressListener = testCaseOnProgress;
    ZFGlobalObserver().observerAdd(ZFTestCase::EventTestCaseOnProgress(), this->testCaseOnProgressListener);

    ZFLISTENER(testCaseOnStop) {
        zfLogTrim()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", zfargs.sender()->classData()->classNameFull())
            << "-------------------------- stop ----------------------------";
    } ZFLISTENER_END()
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

