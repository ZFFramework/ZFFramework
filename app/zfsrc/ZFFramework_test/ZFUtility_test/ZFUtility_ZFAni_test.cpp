#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFUtility_ZFAni_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFUtility_ZFAni_test_Object, ZFObject)

    ZFPROPERTY_ASSIGN(zfint, testProp)
    ZFPROPERTY_ON_ATTACH_INLINE(zfint, testProp) {
        ZFLogTrim() << "[ZFAni_test] testProp: " << this->testProp();
    }

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        ZFLogTrim() << "[ZFAni_test] objectOnInit " << this->objectHash();
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        ZFLogTrim() << "[ZFAni_test] objectOnDealloc " << this->objectHash();
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnRetain(void) {
        zfsuper::objectOnRetain();
        ZFLogTrim() << "[ZFAni_test] objectOnRetain " << this->objectHash() << this->objectRetainCount();
    }
    zfoverride
    virtual void objectOnRelease(void) {
        ZFLogTrim() << "[ZFAni_test] objectOnRelease " << this->objectHash() << this->objectRetainCount();
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

        zfobj<_ZFP_ZFUtility_ZFAni_test_Object> target;
        zfauto aniHolder = ZFAni("testProp"
                , zfobj<v_zfstring>("-100")
                , zfobj<v_zfstring>("200")
                )
            ->c_target(target)
            ;
        ZFAnimation *ani = aniHolder;
        ani->duration(2000);

        ZFTestCase *owner = this;

        ZFLISTENER_1(aniOnStop
                , ZFTestCase *, owner
                ) {
            owner->stop();
        } ZFLISTENER_END()
        ani->observerAdd(ZFAnimation::E_AniOnStop(), aniOnStop);

        ZFLISTENER(aniOnDealloc
                ) {
            ZFLogTrim() << "[ZFAni_test] aniOnDealloc " << zfargs.sender()->objectHash();
        } ZFLISTENER_END()
        ani->observerAdd(ZFObject::E_ObjectBeforeDealloc(), aniOnDealloc);

        ani->start();
    }
};
ZFOBJECT_REGISTER(ZFUtility_ZFAni_test)

ZF_NAMESPACE_GLOBAL_END

