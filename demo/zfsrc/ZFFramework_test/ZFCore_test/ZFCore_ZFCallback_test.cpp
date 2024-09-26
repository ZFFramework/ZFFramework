#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFCallback_test_Class : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFCallback_test_Class, ZFObject)

    ZFMETHOD_INLINE_0(void, classMember) {
        ZFLog();
    }

    ZFMETHOD_INLINE_STATIC_0(void, classStaticMember) {
        ZFLog();
    }
};

static void _ZFP_ZFCore_ZFCallback_test_StaticFunction(void) {
    ZFLog();
}

zfclass ZFCore_ZFCallback_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFCallback_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->output(zfstr("try execute %s", this->callbackClassMember.objectInfo()));
        this->callbackClassMember.executeExact<void>();

        this->outputSeparator();
        this->output(zfstr("try execute %s", this->callbackClassStaticMember.objectInfo()));
        this->callbackClassStaticMember.executeExact<void>();

        this->outputSeparator();
        this->output(zfstr("try execute %s", this->callbackFunction.objectInfo()));
        this->callbackFunction.executeExact<void>();

        ZFLAMBDA(localCallback
                , void
                , ZFMP_IN(const zfstring &, param)
                ) {
            ZFLog() << param;
        } ZFLAMBDA_END()
        this->outputSeparator();
        this->output(zfstr("try execute %s", localCallback.objectInfo()));
        localCallback.executeExact<void, const zfstring &>("param");

        this->stop();
    }

private:
    _ZFP_ZFCore_ZFCallback_test_Class *ownerClass;
    ZFCallback callbackClassMember;
    ZFCallback callbackClassStaticMember;
    ZFCallback callbackFunction;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->ownerClass = zfAlloc(_ZFP_ZFCore_ZFCallback_test_Class);

        this->callbackClassMember = ZFCallbackForMemberMethod(
            this->ownerClass,
            this->ownerClass->classData()->methodForName("classMember"));

        this->callbackClassStaticMember = ZFCallbackForMethod(
            _ZFP_ZFCore_ZFCallback_test_Class::ClassData()->methodForName("classStaticMember"));

        this->callbackFunction = ZFCallbackForFunc(_ZFP_ZFCore_ZFCallback_test_StaticFunction);
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        zfRelease(this->ownerClass);
        this->ownerClass = zfnull;
        zfsuper::objectOnDealloc();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFCallback_test)

ZF_NAMESPACE_GLOBAL_END

