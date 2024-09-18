#include "ZFTestCase.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTestCase)

ZFEVENT_REGISTER(ZFTestCase, TestCaseOnOutput)
ZFEVENT_REGISTER(ZFTestCase, TestCaseOnStart)
ZFEVENT_REGISTER(ZFTestCase, TestCaseOnProgress)
ZFEVENT_REGISTER(ZFTestCase, TestCaseOnStop)

void ZFTestCase::objectOnDeallocPrepare(void) {
    ZFCoreAssert(!this->testCaseIsRunning());
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_0(ZFTestCase, zfbool, testCaseIsRunning) {
    return this->_testCaseIsRunning;
}
void ZFTestCase::testCaseOutput(ZF_IN const zfchar *info) {
    this->testCaseOnOutput(info);
}
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTestCase, void, testCaseOutput
        , ZFMP_IN(const zfchar *, info)
        )
ZFMETHOD_DEFINE_0(ZFTestCase, void, testCaseStart) {
    if(this->_testCaseIsRunning) {
        return;
    }
    this->_testCaseIsRunning = zftrue;
    zfRetain(this);
    this->testCaseOnStart();
}

ZFMETHOD_DEFINE_1(ZFTestCase, void, testCaseProgress
        , ZFMP_IN_OPT(ZFObject *, progress, zfnull)
        ) {
    if(!this->_testCaseIsRunning) {
        return;
    }
    this->testCaseOnProgress(progress);

    zfRelease(progress);
}

ZFMETHOD_DEFINE_1(ZFTestCase, void, testCaseStop
        , ZFMP_IN_OPT(ZFResultTypeEnum, testCaseResult, ZFResultType::e_Success)
        ) {
    if(!this->_testCaseIsRunning) {
        return;
    }
    this->_testCaseIsRunning = zffalse;
    this->testCaseOnStop(testCaseResult);
}

ZF_NAMESPACE_GLOBAL_END

