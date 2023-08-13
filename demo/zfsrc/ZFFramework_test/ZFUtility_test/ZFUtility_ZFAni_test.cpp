#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFUtility_ZFAni_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFUtility_ZFAni_test_Object, ZFObject)

    ZFPROPERTY_ASSIGN(zfint, testProp)
    ZFPROPERTY_ON_ATTACH_INLINE(zfint, testProp) {
        zfLogTrim() << "[ZFAni_test] testProp: " << this->testProp();
    }

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        zfLogTrim() << "[ZFAni_test] objectOnInit " << this->objectHash();
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        zfLogTrim() << "[ZFAni_test] objectOnDealloc " << this->objectHash();
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnRetain(void) {
        zfsuper::objectOnRetain();
        zfLogTrim() << "[ZFAni_test] objectOnRetain " << this->objectHash() << this->objectRetainCount();
    }
    zfoverride
    virtual void objectOnRelease(void) {
        zfLogTrim() << "[ZFAni_test] objectOnRelease " << this->objectHash() << this->objectRetainCount();
        zfsuper::objectOnRelease();
    }
};

zfclass ZFUtility_ZFAni_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUtility_ZFAni_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFTimer);
        ZFFramework_test_asyncTestCheck();

        zfblockedAlloc(_ZFP_ZFUtility_ZFAni_test_Object, target);
        zfautoObject aniHolder = ZFAni(target, "testProp", "-100", "200");
        ZFAnimation *ani = aniHolder;
        ani->aniDuration(2000);

        ZFTestCase *owner = this;

        ZFLISTENER_1(aniOnStop
                , ZFTestCase *, owner
                ) {
            owner->testCaseStop();
        } ZFLISTENER_END()
        ani->observerAdd(ZFAnimation::EventAniOnStop(), aniOnStop);

        ZFLISTENER(aniOnDealloc
                ) {
            zfLogTrim() << "[ZFAni_test] aniOnDealloc " << zfargs.sender()->objectHash();
        } ZFLISTENER_END()
        ani->observerAdd(ZFObject::EventObjectBeforeDealloc(), aniOnDealloc);

        ani->aniStart();
    }
};
ZFOBJECT_REGISTER(ZFUtility_ZFAni_test)

ZF_NAMESPACE_GLOBAL_END

