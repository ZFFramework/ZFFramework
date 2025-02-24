#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFLambda_test : zfextend ZFFramework_test_TestCase {
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
        this->output(zfstr("before invoke: %s", capture));
        refCapture.executeExact<void>();
        this->output(zfstr("after invoke: %s", capture));

        ZFCallback outer;
        {
            zfstring value = "init";
            ZFTestCase *testCase = this;
            ZFLAMBDA_2(valueCapture
                    , zfstring, value
                    , ZFTestCase *, testCase
                    , void
                    ) {
                testCase->output(zfstr("value capture: %s", value));
            } ZFLAMBDA_END()
            outer = valueCapture;
        }
        outer.executeExact<void>();


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

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFLambda_test)

ZF_NAMESPACE_GLOBAL_END

