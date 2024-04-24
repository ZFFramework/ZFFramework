#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFProperty_test_TestBase : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFProperty_test_TestBase, ZFObject)

public:
    // retain
    ZFPROPERTY_RETAIN(ZFObject *, propertyRetain)
    ZFPROPERTY_RETAIN_READONLY(ZFObject *, propertyRetainReadonly, ZFPropertyNoInitValue)

    // assign
    ZFPROPERTY_ASSIGN(zfstring, propertyAssign)
    ZFPROPERTY_ASSIGN_READONLY(zfstring, propertyAssignReadonly, ZFPropertyNoInitValue)

    // weak
    ZFPROPERTY_ASSIGN(ZFObject *, propertyWeak)

    // custom callback
    ZFPROPERTY_ON_INIT_INLINE(ZFObject *, propertyRetain) {
        zfLog();
    }
    ZFPROPERTY_ON_VERIFY_INLINE(ZFObject *, propertyRetain) {
        zfLog();
    }
    ZFPROPERTY_ON_ATTACH_INLINE(ZFObject *, propertyRetain) {
        zfLog();
    }
    ZFPROPERTY_ON_DETACH_INLINE(ZFObject *, propertyRetain) {
        zfLog();
    }

    ZFPROPERTY_ON_INIT_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_VERIFY_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_DETACH_DECLARE(zfstring, propertyAssign)
};
ZFPROPERTY_ON_INIT_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    zfLog();
}
ZFPROPERTY_ON_VERIFY_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    zfLog();
}
ZFPROPERTY_ON_ATTACH_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    zfLog();
}
ZFPROPERTY_ON_DETACH_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    zfLog();
}

zfclass _ZFP_ZFCore_ZFProperty_test_TestChild: zfextend _ZFP_ZFCore_ZFProperty_test_TestBase {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFProperty_test_TestChild, _ZFP_ZFCore_ZFProperty_test_TestBase)

public:
    ZFPROPERTY_ASSIGN(zfstring, propertyInChild)
};

// ============================================================
zfclass ZFCore_ZFProperty_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFProperty_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFProperty");
        {
            zfobj<_ZFP_ZFCore_ZFProperty_test_TestChild> p;

            // property list
            const ZFClass *clsTmp = p->classData();
            ZFOutput log = zfLog();
            ZFOutputFormat::getFormat<ZFLogFormat *>(log)->autoEndl(zffalse);
            log << "list: ";
            for(zfindex i = 0; i < clsTmp->propertyCount(); ++i) {
                if(i > 0) {
                    log << ", ";
                }
                log << clsTmp->propertyAt(i)->propertyName();
            }
            log = zfnull;

            // reflect
            p->propertyAssign("oldValue");
            zfLog() << "before: " << p->propertyAssign();
            p->classData()->propertySetterForName("propertyAssign")->execute<void, zfstring const &>(p, "newValue");
            zfLog() << "after: " << p->propertyAssign();
            zfLog() << "access by reflect: " << p->classData()->propertyGetterForName("propertyAssign")->execute<zfstring const &>(p);

            // retain
            zfLogTrim();
            zfLogTrim() << "retain";

            p->propertyRetain(zfobj<ZFObject>());
            zfLog() << p->propertyRetain();

#if 0 // this should not compile
            p->propertyRetainReadonly(zfobj<ZFObject>());
#endif
            zfLog() << p->propertyRetainReadonly();

            // assign
            zfLogTrim();
            zfLogTrim() << "assign";

            p->propertyAssign(zfstring());
            zfLog() << p->propertyAssign();

#if 0 // this should not compile
            p->propertyAssignReadonly(zfstring());
#endif
            zfLog() << p->propertyAssignReadonly();

            // weak
            {
                zfobj<ZFObject> value;
                p->propertyWeak(value);
                zfLog() << p->propertyWeak();
            }
            zfLog() << p->propertyWeak();

            // copy
            this->testCaseOutputSeparator();
            this->testCaseOutput("copy propertis");
            zfobj<_ZFP_ZFCore_ZFProperty_test_TestBase> pBase;
            zfobj<_ZFP_ZFCore_ZFProperty_test_TestChild> pChild;
            pBase->propertyAssign("string set in another");
            ZFPropertyCopyAll(pChild, pBase);
            this->testCaseOutputSeparator();
            zfLogTrim() << "after copy: " << pChild->propertyAssign();

            this->testCaseOutputSeparator();
            this->testCaseOutput("copy by ZFPropertyCopyAll");
            pChild->propertyAssign("");
            ZFPropertyCopyAll(pChild, pBase);
            zfLogTrim() << "after copy: " << pChild->propertyAssign();
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFProperty_test)

ZF_NAMESPACE_GLOBAL_END

