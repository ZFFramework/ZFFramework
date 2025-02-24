#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFStyle_test_Object : zfextend ZFStyle {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFStyle_test_Object, ZFStyle)

    ZFPROPERTY_ASSIGN(zfint, myProperty)
    ZFPROPERTY_ON_ATTACH_INLINE(zfint, myProperty) {
        ZFLog() << propertyValue;
    }
};

zfclass ZFCore_ZFStyle_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFStyle_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->outputSeparator();
        this->output("prepare style");

        const zfstring &styleKey = _ZFP_ZFCore_ZFStyle_test_Object::ClassData()->classNameFull();
        {
            zfobj<_ZFP_ZFCore_ZFStyle_test_Object> styleValue;
            styleValue->myProperty(123);
            ZFStyleSet(styleKey, styleValue);
        }

        this->outputSeparator();
        this->output("attach object to style");
        zfobj<_ZFP_ZFCore_ZFStyle_test_Object> obj;
        obj->styleKey(_ZFP_ZFCore_ZFStyle_test_Object::ClassData()->classNameFull());

        this->outputSeparator();
        this->output("notify change style");
        ZFStyleUpdateBegin();
        ZFStyleSet("ZFCore_ZFStyle_test", zfobj<v_zfstring>());
        ZFStyleSet("ZFCore_ZFStyle_test", zfnull);
        ZFStyleUpdateEnd();

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFStyle_test)

ZF_NAMESPACE_GLOBAL_END

