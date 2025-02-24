#include "ZFTestCase.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTestCase)

ZFEVENT_REGISTER(ZFTestCase, TestCaseOnOutput)
ZFEVENT_REGISTER(ZFTestCase, TestCaseOnStart)
ZFEVENT_REGISTER(ZFTestCase, TestCaseOnProgress)
ZFEVENT_REGISTER(ZFTestCase, TestCaseOnStop)

void ZFTestCase::objectOnDeallocPrepare(void) {
    ZFCoreAssert(!this->started());
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_0(ZFTestCase, zfbool, started) {
    return this->_started;
}
void ZFTestCase::output(ZF_IN const zfchar *info) {
    this->testCaseOnOutput(info);
}
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTestCase, void, output
        , ZFMP_IN(const zfchar *, info)
        )
ZFMETHOD_DEFINE_0(ZFTestCase, void, start) {
    if(this->_started) {
        return;
    }
    this->_started = zftrue;
    zfRetain(this);
    this->testCaseOnStart();
}

ZFMETHOD_DEFINE_1(ZFTestCase, void, notifyProgress
        , ZFMP_IN_OPT(ZFObject *, progress, zfnull)
        ) {
    if(!this->_started) {
        return;
    }
    this->testCaseOnProgress(progress);

    zfRelease(progress);
}

ZFMETHOD_DEFINE_1(ZFTestCase, void, stop
        , ZFMP_IN(ZFResultType, testCaseResult)
        ) {
    if(!this->_started) {
        return;
    }
    this->_started = zffalse;
    this->testCaseOnStop(testCaseResult);
}

ZF_NAMESPACE_GLOBAL_END

