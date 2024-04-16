#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFDynamic_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFDynamic_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFDynamic");

        ZFLISTENER(methodCallback) {
            ZFMethodInvokeData *d = zfargs.param0()->toAny();
            zfblockedAlloc(v_zfstring, ret);
            ret->zfv += d->param0->to<v_zfstring *>()->zfv;
            ret->zfv += "(modified)";
            zfautoRelease(zfRetain(ret)); // take care of this
            d->ret = ret;
        } ZFLISTENER_END()
        ZFDynamic d = ZFDynamic()
            .classBegin("ZFDynamicTest", "ZFObject")
                .property(ZFTypeId_zfstring(), "testProp")
                .method(ZFTypeId_zfstring(), "testMethod", ZFMP()
                    .mp(ZFTypeId_zfstring())
                    , methodCallback)
            .classEnd()
            .NSBegin("ZFDynamicTestNS")
                .method(ZFTypeId_zfstring(), "testMethod", ZFMP()
                    .mp(ZFTypeId_zfstring())
                    , methodCallback)
            .NSEnd()
            ;
        const ZFClass *cls = ZFClass::classForName("ZFDynamicTest");
        zfauto obj = cls->newInstance();

        obj->invoke("testProp", zflineAlloc(v_zfstring, "testValue"));
        zfLog() << obj->invoke("testProp");
        zfLog() << obj->invoke("testMethod", zflineAlloc(v_zfstring, "testParam"));

        const ZFMethod *method = ZFMethodForName("ZFDynamicTestNS", "testMethod");
        zfLog() << method;
        zfLog() << method->methodInvoke(zfnull, zflineAlloc(v_zfstring, "testParam"));

        d.removeAll();
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFDynamic_test)

ZF_NAMESPACE_GLOBAL_END

