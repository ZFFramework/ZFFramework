#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFProperty_test_TestBase : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFProperty_test_TestBase, ZFObject)

public:
    // retain
    ZFPROPERTY_RETAIN(zfanyT<ZFObject>, propertyRetain)
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFObject>, propertyRetainReadonly, ZFPropertyNoInitValue)

    // assign
    ZFPROPERTY_ASSIGN(zfstring, propertyAssign)
    ZFPROPERTY_ASSIGN_READONLY(zfstring, propertyAssignReadonly, ZFPropertyNoInitValue)

    // weak
    ZFPROPERTY_ASSIGN(zfanyT<ZFObject>, propertyWeak)

    // custom callback
    ZFPROPERTY_ON_INIT_INLINE(zfanyT<ZFObject>, propertyRetain) {
        ZFLog();
    }
    ZFPROPERTY_ON_VERIFY_INLINE(zfanyT<ZFObject>, propertyRetain) {
        ZFLog();
    }
    ZFPROPERTY_ON_ATTACH_INLINE(zfanyT<ZFObject>, propertyRetain) {
        ZFLog();
    }
    ZFPROPERTY_ON_DETACH_INLINE(zfanyT<ZFObject>, propertyRetain) {
        ZFLog();
    }

    ZFPROPERTY_ON_INIT_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_VERIFY_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_DETACH_DECLARE(zfstring, propertyAssign)
};
ZFPROPERTY_ON_INIT_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    ZFLog();
}
ZFPROPERTY_ON_VERIFY_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    ZFLog();
}
ZFPROPERTY_ON_ATTACH_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    ZFLog();
}
ZFPROPERTY_ON_DETACH_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign) {
    ZFLog();
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

        this->outputSeparator();
        this->output("ZFProperty");
        {
            zfobj<_ZFP_ZFCore_ZFProperty_test_TestChild> p;

            // property list
            const ZFClass *clsTmp = p->classData();
            ZFOutput log = ZFLog();
            zfanyT<ZFLogFormat> fmt = ZFOutputFormat::getFormat(log);
            fmt->autoEndl(zffalse);
            log << "list: ";
            {
                zfbool first = zftrue;
                for(zfiter it = clsTmp->propertyIter(); it; ++it) {
                    if(first) {
                        first = zffalse;
                    }
                    else {
                        log << ", ";
                    }
                    log << clsTmp->propertyIterValue(it)->propertyName();
                }
            }
            log = zfnull;

            // reflect
            p->propertyAssign("oldValue");
            ZFLog() << "before: " << p->propertyAssign();
            p->classData()->propertySetterForName("propertyAssign")->executeExact<void, zfstring const &>(p, "newValue");
            ZFLog() << "after: " << p->propertyAssign();
            ZFLog() << "access by reflect: " << p->classData()->propertyGetterForName("propertyAssign")->executeExact<zfstring const &>(p);

            // retain
            ZFLogTrim();
            ZFLogTrim() << "retain";

            p->propertyRetain(zfobj<ZFObject>());
            ZFLog() << p->propertyRetain();

#if 0 // this should not compile
            p->propertyRetainReadonly(zfobj<ZFObject>());
#endif
            ZFLog() << p->propertyRetainReadonly();

            // assign
            ZFLogTrim();
            ZFLogTrim() << "assign";

            p->propertyAssign(zfstring());
            ZFLog() << p->propertyAssign();

#if 0 // this should not compile
            p->propertyAssignReadonly(zfstring());
#endif
            ZFLog() << p->propertyAssignReadonly();

            // weak
            {
                zfobj<ZFObject> value;
                p->propertyWeak(value);
                ZFLog() << p->propertyWeak();
            }
            ZFLog() << p->propertyWeak();

            // copy
            this->outputSeparator();
            this->output("copy propertis");
            zfobj<_ZFP_ZFCore_ZFProperty_test_TestBase> pBase;
            zfobj<_ZFP_ZFCore_ZFProperty_test_TestChild> pChild;
            pBase->propertyAssign("string set in another");
            ZFPropertyCopyAll(pChild, pBase);
            this->outputSeparator();
            ZFLogTrim() << "after copy: " << pChild->propertyAssign();

            this->outputSeparator();
            this->output("copy by ZFPropertyCopyAll");
            pChild->propertyAssign("");
            ZFPropertyCopyAll(pChild, pBase);
            ZFLogTrim() << "after copy: " << pChild->propertyAssign();
        }

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFProperty_test)

ZF_NAMESPACE_GLOBAL_END

