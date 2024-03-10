#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFMethod_order_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFCore_ZFMethod_order_test_Object, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        zfLogTrim("base::objectOnInit()");
    }
    zfoverride
    virtual void objectOnInitFinish(void) {
        zfsuper::objectOnInitFinish();
        zfLogTrim("base::objectOnInitFinish()");
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethod_order_test_Object)

ZF_GLOBAL_INITIALIZER_INIT(ZFCore_ZFMethod_order_test_ObjectReg) {
    ZFLISTENER(onInit) {
        zfLogTrim("DynReg::onInit()");
    } ZFLISTENER_END()
    ZFDynamic()
        .classBegin(ZFCore_ZFMethod_order_test_Object::ClassData())
        .onInit(onInit)
        .classEnd();

    ZFLISTENER(objectOnInitImpl) {
        zfLogTrim("DynReg::objectOnInit(const zfchar *)");
    } ZFLISTENER_END()
    ZFObjectOnInitDynamicRegister(
            ZFCore_ZFMethod_order_test_Object::ClassData()
            , ZFMP()
            .mp(ZFTypeId_zfstring())
            , objectOnInitImpl
            );
}
ZF_GLOBAL_INITIALIZER_END(ZFCore_ZFMethod_order_test_ObjectReg)

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        zfLogTrim("UserReg::objectOnInit(int)");
    }, ZFCore_ZFMethod_order_test_Object
    , ZFMP_IN(zfint, p0)
    )

// ============================================================
zfclass ZFCore_ZFMethod_order_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFMethod_order_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->testCaseOutput("plain alloc");
        zfblockedAlloc(ZFCore_ZFMethod_order_test_Object, o0);

        this->testCaseOutputSeparator();
        this->testCaseOutput("alloc with string param");
        ZFCore_ZFMethod_order_test_Object::ClassData()->newInstance(zflineAlloc(v_zfstring, "param"));

        this->testCaseOutputSeparator();
        this->testCaseOutput("alloc with int param");
        ZFCore_ZFMethod_order_test_Object::ClassData()->newInstance(zflineAlloc(v_zfint, 123));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethod_order_test)

ZF_NAMESPACE_GLOBAL_END

