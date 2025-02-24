#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFMethod_order_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFCore_ZFMethod_order_test_Object, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        ZFLogTrim("base::objectOnInit()");
    }
    zfoverride
    virtual void objectOnInitFinish(void) {
        zfsuper::objectOnInitFinish();
        ZFLogTrim("base::objectOnInitFinish()");
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethod_order_test_Object)

ZF_GLOBAL_INITIALIZER_INIT(ZFCore_ZFMethod_order_test_ObjectReg) {
    ZFLISTENER(onInit) {
        ZFLogTrim("DynReg::onInit()");
    } ZFLISTENER_END()
    ZFDynamic()
        .classBegin(ZFCore_ZFMethod_order_test_Object::ClassData())
        .onInit(onInit)
        .classEnd();

    ZFLISTENER(objectOnInitImpl) {
        ZFLogTrim("DynReg::objectOnInit(const zfchar *)");
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
        ZFLogTrim("UserReg::objectOnInit(int)");
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

        this->output("plain alloc");
        zfobj<ZFCore_ZFMethod_order_test_Object> o0;

        this->outputSeparator();
        this->output("alloc with string param");
        ZFCore_ZFMethod_order_test_Object::ClassData()->newInstance(zfobj<v_zfstring>("param"));

        this->outputSeparator();
        this->output("alloc with int param");
        ZFCore_ZFMethod_order_test_Object::ClassData()->newInstance(zfobj<v_zfint>(123));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethod_order_test)

ZF_NAMESPACE_GLOBAL_END

