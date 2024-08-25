#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFStyle_test_Object : zfextend ZFStyleableObject {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFStyle_test_Object, ZFStyleableObject)

    ZFPROPERTY_ASSIGN(zfint, myProperty)
    ZFPROPERTY_ON_ATTACH_INLINE(zfint, myProperty) {
        zfLog() << propertyValue;
    }
};

zfclass ZFCore_ZFStyle_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFStyle_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("prepare style");

        const zfstring &styleKey = _ZFP_ZFCore_ZFStyle_test_Object::ClassData()->classNameFull();
        {
            zfobj<_ZFP_ZFCore_ZFStyle_test_Object> styleValue;
            styleValue->myProperty(123);
            ZFStyleSet(styleKey, styleValue);
        }

        this->testCaseOutputSeparator();
        this->testCaseOutput("attach object to style");
        zfobj<_ZFP_ZFCore_ZFStyle_test_Object> obj;
        obj->styleKey(_ZFP_ZFCore_ZFStyle_test_Object::ClassData()->classNameFull());

        this->testCaseOutputSeparator();
        this->testCaseOutput("notify change style");
        ZFStyleUpdateBegin();
        ZFStyleSet("ZFCore_ZFStyle_test", zfobj<v_zfstring>());
        ZFStyleSet("ZFCore_ZFStyle_test", zfnull);
        ZFStyleUpdateEnd();

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFStyle_test)

ZF_NAMESPACE_GLOBAL_END

