#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFImplementDynamicRegister_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFImplementDynamicRegister_test, ZFFramework_test_TestCase)

    zfclass ExistClass : zfextend ZFObject {
        ZFOBJECT_DECLARE(ExistClass, ZFObject, ZFCore_ZFImplementDynamicRegister_test)
        ZFMETHOD_INLINE_0(void, funcInExistClass) {
            ZFLogTrim("funcInExistClass: %s, cast to AttachClass: %s", this, zfcast(AttachClass *, this));
        }
    };
    zfclass AttachClass : zfextend ZFObject {
        ZFOBJECT_DECLARE(AttachClass, ZFObject, ZFCore_ZFImplementDynamicRegister_test)
        ZFMETHOD_INLINE_0(void, funcInAttachClass) {
            ZFLogTrim("funcInAttachClass: %s, cast to ExistClass: %s", this, zfcast(ExistClass *, this));
        }
    };

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFImplementDynamicRegister(ExistClass::ClassData(), AttachClass::ClassData());
        {
            zfobj<ExistClass> obj;
            AttachClass *p = obj;
            ExistClass *p2 = zfcast(ExistClass *, p);
            ZFTestCaseAssert(p != zfnull);
            ZFTestCaseAssert(p2 != zfnull);
            ZFTestCaseAssert(ExistClass::ClassData()->classIsTypeOf(AttachClass::ClassData()));

            zfobj<AttachClass> obj2;
            ZFTestCaseAssert(zfcast(ExistClass *, obj2) == zfnull);

            obj->funcInExistClass();
            p->funcInAttachClass();

            obj->invoke("funcInExistClass");
            obj->invoke("funcInAttachClass");

            p->invoke("funcInExistClass");
            p->invoke("funcInAttachClass");
        }
        ZFImplementDynamicUnregister(ExistClass::ClassData(), AttachClass::ClassData());
        ZFTestCaseAssert(!ExistClass::ClassData()->classIsTypeOf(AttachClass::ClassData()));
        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFImplementDynamicRegister_test)

ZF_NAMESPACE_GLOBAL_END

