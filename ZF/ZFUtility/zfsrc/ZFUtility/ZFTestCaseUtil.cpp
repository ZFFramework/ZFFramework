#include "ZFTestCaseUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(TestCaseRunAllOnStart)
ZFEVENT_GLOBAL_REGISTER(TestCaseRunAllOnStop)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFTestCase>, ZFTestCaseRun
        , ZFMP_IN(const ZFClass *, cls)
        ) {
    if(cls == zfnull || !cls->classIsTypeOf(ZFTestCase::ClassData())) {
        return zfnull;
    }
    zfauto testCaseTmp = cls->newInstance();
    if(testCaseTmp == zfnull || !testCaseTmp->classData()->classIsTypeOf(ZFTestCase::ClassData())) {
        return zfnull;
    }
    ZFTestCase *testCase = testCaseTmp;
    testCase->start();
    return testCase->started() ? testCase : zfnull;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFTestCase>, ZFTestCaseRun
        , ZFMP_IN(const zfstring &, classNameFull)
        ) {
    return ZFTestCaseRun(ZFClass::classForName(classNameFull));
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTestCaseRunAllHolder, ZFLevelZFFrameworkEssential) {
    this->running = zffalse;
    this->testCaseRunning = zfnull;
    ZFLISTENER(testCaseOnFinish
            ) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->testCaseRunNext();
    } ZFLISTENER_END()
    this->testCaseFinishListener = testCaseOnFinish;
}
public:
    void testCaseList(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret) {
        this->testCaseListPrepare(ret);
    }
    void start(ZF_IN_OPT const ZFCoreArray<const ZFClass *> *toStart = zfnull) {
        if(this->running) {
            return;
        }
        this->running = zftrue;

        this->testCaseListPrepare(this->testCases, toStart);

        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_TestCaseRunAllOnStart());

        this->testCaseRunNext();
    }
    void stop(void) {
        if(!this->running) {
            return;
        }
        ZFTestCase *toStop = this->testCaseRunning;
        this->testCases.removeAll();
        this->testCaseRunning = zfnull;
        this->running = zffalse;

        if(toStop != zfnull) {
            toStop->observerRemove(ZFTestCase::E_TestCaseOnStop(), this->testCaseFinishListener);
            toStop->stop(v_ZFResultType::e_Cancel);
        }

        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_TestCaseRunAllOnStop());
    }

    void testCaseRunNext(void) {
        if(!this->running) {
            return;
        }

        if(this->testCaseRunning != zfnull) {
            this->testCaseRunning->observerRemove(ZFTestCase::E_TestCaseOnStop(), this->testCaseFinishListener);
        }

        if(this->testCases.isEmpty()) {
            this->stop();
            return;
        }

        const ZFClass *cls = this->testCases.removeLastAndGet();
        this->testCaseRunning = ZFTestCaseRun(cls);
        if(this->testCaseRunning == zfnull) {
            this->testCaseRunNext();
            return;
        }
        this->testCaseRunning->observerAdd(ZFTestCase::E_TestCaseOnStop(), this->testCaseFinishListener);
    }
private:
    zfbool running;
    ZFCoreArray<const ZFClass *> testCases;
    zfautoT<ZFTestCase> testCaseRunning;
    ZFListener testCaseFinishListener;
private:
    static void testCaseListPrepare(
            ZF_OUT ZFCoreArray<const ZFClass *> &ret
            , ZF_IN_OPT const ZFCoreArray<const ZFClass *> *toStart = zfnull
            ) {
        ZFCoreArray<const ZFClass *> allClass;
        if(toStart == zfnull || toStart->isEmpty()) {
            allClass = ZFClassGetAll();
        }
        else {
            allClass = *toStart;
        }

        ret.removeAll();
        const ZFClass *desired = ZFTestCase::ClassData();
        for(zfindex i = 0; i < allClass.count(); ++i) {
            const ZFClass *cls = allClass[i];
            if(cls->classIsTypeOf(desired) && !cls->classIsAbstract()) {
                ret.add(cls);
            }
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFTestCaseRunAllHolder)

ZFMETHOD_FUNC_DEFINE_1(void, ZFTestCaseRunAllStart
        , ZFMP_IN_OPT(const ZFCoreArray<const ZFClass *> *, toStart, zfnull)
        ) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->start(toStart);
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFTestCaseRunAllStop) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->stop();
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFTestCaseGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFClass *> &, ret)
        ) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->testCaseList(ret);
}

ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<const ZFClass *>, ZFTestCaseGetAll) {
    ZFCoreArray<const ZFClass *> ret;
    ZFTestCaseGetAllT(ret);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

