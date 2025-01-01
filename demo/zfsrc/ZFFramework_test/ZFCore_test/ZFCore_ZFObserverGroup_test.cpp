#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFObserverGroup_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFObserverGroup_test, ZFFramework_test_TestCase)

    ZFEVENT(ZFObserverGroupTest0)
    ZFEVENT(ZFObserverGroupTest1)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        zfobj<v_zfstring> testObserver0("testObserver0");
        ZFObserver testObserver1;
        ZFObject *testObserver2 = zfAlloc(v_zfstring, "testObserver2");
        zfobj<v_zfstring> testObserver3("testObserver3");
        ZFObject *owner = zfAlloc(ZFObject);

        ZFLISTENER(observer
                ) {
            ZFLogTrim() << zfargs.sender() << " " << ZFEventNameForId(zfargs.eventId());
        } ZFLISTENER_END()
        ZFObserverGroup(this, testObserver0)
            .observerAdd(zfself::E_ZFObserverGroupTest0(), observer)
            .observerAdd(zfself::E_ZFObserverGroupTest1(), observer)
            ;
        ZFObserverGroup(this, testObserver1)
            .observerAdd(zfself::E_ZFObserverGroupTest0(), observer)
            .observerAdd(zfself::E_ZFObserverGroupTest1(), observer)
            ;
        ZFObserverGroup(this, testObserver2)
            .observerAdd(zfself::E_ZFObserverGroupTest0(), observer)
            .observerAdd(zfself::E_ZFObserverGroupTest1(), observer)
            ;
        ZFObserverGroup(owner, testObserver3)
            .observerAdd(zfself::E_ZFObserverGroupTest0(), observer)
            .observerAdd(zfself::E_ZFObserverGroupTest1(), observer)
            ;

        this->output("before remove, all observer should be called");
        testObserver0->observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver0->observerNotify(zfself::E_ZFObserverGroupTest1());
        testObserver1.observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver1.observerNotify(zfself::E_ZFObserverGroupTest1());
        testObserver2->observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver2->observerNotify(zfself::E_ZFObserverGroupTest1());
        testObserver3->observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver3->observerNotify(zfself::E_ZFObserverGroupTest1());

        this->outputSeparator();
        zfRelease(owner);
        this->output("owner released, all related observer should be removed");
        testObserver3->observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver3->observerNotify(zfself::E_ZFObserverGroupTest1());

        this->outputSeparator();
        zfRelease(testObserver2);
        this->output("target released, all related observer should be removed");

        this->outputSeparator();
        ZFObserverGroupRemove(this);
        this->output("after remove, all observer should be removed");
        testObserver0->observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver0->observerNotify(zfself::E_ZFObserverGroupTest1());
        testObserver1.observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver1.observerNotify(zfself::E_ZFObserverGroupTest1());
        testObserver3->observerNotify(zfself::E_ZFObserverGroupTest0());
        testObserver3->observerNotify(zfself::E_ZFObserverGroupTest1());

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFObserverGroup_test)
ZFEVENT_REGISTER(ZFCore_ZFObserverGroup_test, ZFObserverGroupTest0)
ZFEVENT_REGISTER(ZFCore_ZFObserverGroup_test, ZFObserverGroupTest1)

ZF_NAMESPACE_GLOBAL_END

