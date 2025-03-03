#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFFramework_test_global.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_0(void, TestCaseRunner) {
    zfobj<ZFObject> owner;

    ZFLISTENER(testOnFail) {
        ZFResultType resultType = zfargs.param0().to<v_ZFResultType *>()->zfv();
        ZFCoreAssertWithMessageTrim(resultType != v_ZFResultType::e_Fail, "[TestCaseRunner] test failed: %s", zfargs.sender());
    } ZFLISTENER_END()
    ZFObserverGroup(owner, ZFGlobalObserver()).observerAdd(ZFTestCase::E_TestCaseOnStop(), testOnFail);

    ZFLISTENER_1(testOnFinish
            , zfauto, owner
            ) {
        ZFLogTrim("[TestCaseRunner] stop");
        ZFObserverGroupRemove(owner);
        ZFApp::appExit();
    } ZFLISTENER_END()
    ZFObserverGroup(owner, ZFGlobalObserver()).observerAdd(ZFGlobalEvent::E_TestCaseRunAllOnStop(), testOnFinish);

    ZFFramework_test_asyncTestAvailable(zffalse);
    ZFLogTrim("[TestCaseRunner] start");
    ZFTestCaseRunAllStart();
}

ZFMAIN_PARAM_DISPATCH(TestCaseRunner) {
    if(ZFApp::appParams().isEmpty() || (zftrue
                && ZFRegExpFind(ZFApp::appParams()[0], "-t") == ZFIndexRangeZero()
                && ZFRegExpFind(ZFApp::appParams()[0], "--test") == ZFIndexRangeZero()
                )) {
        return;
    }
    zfargs.eventFiltered(zftrue);

    TestCaseRunner();
}

ZF_NAMESPACE_GLOBAL_END

