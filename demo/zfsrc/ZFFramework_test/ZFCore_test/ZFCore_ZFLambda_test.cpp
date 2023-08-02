#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFLambda_test : zfextends ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFLambda_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        zfstring capture = "init";
        ZFLAMBDA_1(refCapture
                , zfstring &, capture
                , void
                ) {
            capture = "changed";
        } ZFLAMBDA_END()
        this->testCaseOutput(zfstr("before invoke: %s", capture));
        refCapture.executeExact<void>();
        this->testCaseOutput(zfstr("after invoke: %s", capture));

        ZFCallback outter;
        {
            zfstring value = "init";
            ZFTestCase *testCase = this;
            ZFLAMBDA_2(valueCapture
                    , zfstring, value
                    , ZFTestCase *, testCase
                    , void
                    ) {
                testCase->testCaseOutput(zfstr("value capture: %s", value));
            } ZFLAMBDA_END()
            outter = valueCapture;
        }
        outter.executeExact<void>();


        ZFLAMBDA_0(lambdaWithZeroParam
                , void
                ) {
        } ZFLAMBDA_END()
        lambdaWithZeroParam.executeExact<void>();

        ZFLISTENER_1(lambdaListener
                , zfstring &, capture
                ) {
            ZFUNUSED(capture);
        } ZFLISTENER_END()
        lambdaListener.executeExact<void>();
        ZFLISTENER_0(lambdaListenerWithZeroParam
                ) {
        } ZFLISTENER_END()
        lambdaListenerWithZeroParam.executeExact<void>();

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFLambda_test)

ZF_NAMESPACE_GLOBAL_END

