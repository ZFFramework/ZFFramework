#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL(ZFCore_EventFilter_test)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFCore_EventFilter_test)
ZF_NAMESPACE_END(ZFGlobalEvent)

zfclass ZFCore_EventFilter_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_EventFilter_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        ZFLISTENER(normalObserver) {
            zfLog() << "normal observer";
        } ZFLISTENER_END()
        ZFGlobalObserver().observerAdd(
            ZFGlobalEvent::EventZFCore_EventFilter_test(),
            normalObserver);

        ZFLISTENER(eventFilter) {
            zfLog() << "event filter";
            zfargs.eventFiltered(zftrue);
        } ZFLISTENER_END()
        ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventZFCore_EventFilter_test(),
                eventFilter,
                ZFLevelAppHigh
            );

        this->testCaseOutputSeparator();
        this->testCaseOutput("notify with event filter, only filter would be called");
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFCore_EventFilter_test());

        this->testCaseOutputSeparator();
        this->testCaseOutput("notify without event filter");
        ZFGlobalObserver().observerRemove(
            ZFGlobalEvent::EventZFCore_EventFilter_test(),
            eventFilter);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFCore_EventFilter_test());

        ZFGlobalObserver().observerRemoveAll(ZFGlobalEvent::EventZFCore_EventFilter_test());
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_EventFilter_test)

ZF_NAMESPACE_GLOBAL_END

