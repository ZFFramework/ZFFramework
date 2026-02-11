#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFDynamic_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFDynamic_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->outputSeparator();
        this->output("ZFDynamic");

        ZFLISTENER(methodCallback) {
            zfobj<v_zfstring> ret;
            ret->zfv += zfargs.param0()->to<v_zfstring *>()->zfv;
            ret->zfv += "(modified)";
            zfobjAutoRelease(zfobjRetain(ret)); // take care of this
            zfargs.result(ret);
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

        obj->invoke("testProp", zfobj<v_zfstring>("testValue"));
        ZFLog() << obj->invoke("testProp");
        ZFLog() << obj->invoke("testMethod", zfobj<v_zfstring>("testParam"));

        const ZFMethod *method = ZFMethodForName("ZFDynamicTestNS", "testMethod");
        ZFLog() << method;
        ZFLog() << method->methodInvoke(zfnull, zfobj<v_zfstring>("testParam"));

        d.removeAll();
        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFDynamic_test)

ZF_NAMESPACE_GLOBAL_END

