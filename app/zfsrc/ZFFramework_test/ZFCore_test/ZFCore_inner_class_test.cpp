#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfinterface _ZFP_ZFCore_inner_class_test_Interface : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(_ZFP_ZFCore_inner_class_test_Interface, ZFInterface)
};
zfclass _ZFP_ZFCore_inner_class_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_inner_class_test_Object, ZFObject)

    zfclass InnerObject : zfextend ZFObject, zfimplement _ZFP_ZFCore_inner_class_test_Interface {
        ZFOBJECT_DECLARE(InnerObject, ZFObject, _ZFP_ZFCore_inner_class_test_Object)
        ZFIMPLEMENT_DECLARE(_ZFP_ZFCore_inner_class_test_Interface)
    };
    zfinterface InnerInterface : zfextend ZFInterface {
        ZFINTERFACE_DECLARE(InnerInterface, ZFInterface, _ZFP_ZFCore_inner_class_test_Object)

        zfclass InnerObjectInInterface : zfextend ZFObject {
            ZFOBJECT_DECLARE(InnerObjectInInterface, ZFObject, InnerInterface)
        };

        zfinterface InnerInterfaceInInterface : zfextend ZFInterface {
            ZFINTERFACE_DECLARE(InnerInterfaceInInterface, ZFInterface, InnerInterface)
        };
    };
};

// ============================================================
zfclass ZFCore_inner_class_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_inner_class_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        ZFTestCaseAssert(zfstringIsEqual(
                    "_ZFP_ZFCore_inner_class_test_Object.InnerObject",
                    _ZFP_ZFCore_inner_class_test_Object::InnerObject::ClassData()->classNameFull()));
        ZFTestCaseAssert(
                    ZFClass::classForName("_ZFP_ZFCore_inner_class_test_Object.InnerObject")
                    == _ZFP_ZFCore_inner_class_test_Object::InnerObject::ClassData());

        ZFTestCaseAssert(zfstringIsEqual(
                    "_ZFP_ZFCore_inner_class_test_Object.InnerInterface",
                    _ZFP_ZFCore_inner_class_test_Object::InnerInterface::ClassData()->classNameFull()));
        ZFTestCaseAssert(
                    ZFClass::classForName("_ZFP_ZFCore_inner_class_test_Object.InnerInterface")
                    == _ZFP_ZFCore_inner_class_test_Object::InnerInterface::ClassData());

        ZFTestCaseAssert(zfstringIsEqual(
                    "_ZFP_ZFCore_inner_class_test_Object.InnerInterface.InnerObjectInInterface",
                    _ZFP_ZFCore_inner_class_test_Object::InnerInterface::InnerObjectInInterface::ClassData()->classNameFull()));
        ZFTestCaseAssert(
                    ZFClass::classForName("_ZFP_ZFCore_inner_class_test_Object.InnerInterface.InnerObjectInInterface")
                    == _ZFP_ZFCore_inner_class_test_Object::InnerInterface::InnerObjectInInterface::ClassData());

        ZFTestCaseAssert(zfstringIsEqual(
                    "_ZFP_ZFCore_inner_class_test_Object.InnerInterface.InnerInterfaceInInterface",
                    _ZFP_ZFCore_inner_class_test_Object::InnerInterface::InnerInterfaceInInterface::ClassData()->classNameFull()));
        ZFTestCaseAssert(
                    ZFClass::classForName("_ZFP_ZFCore_inner_class_test_Object.InnerInterface.InnerInterfaceInInterface")
                    == _ZFP_ZFCore_inner_class_test_Object::InnerInterface::InnerInterfaceInInterface::ClassData());

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_inner_class_test)

ZF_NAMESPACE_GLOBAL_END

