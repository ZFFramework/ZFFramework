#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL(ZFLIB_APP, ZFCore_EventFilter_test)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFCore_EventFilter_test)
ZF_NAMESPACE_END(ZFGlobalEvent)

zfclass ZFCore_EventFilter_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_EventFilter_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFLISTENER(normalObserver) {
            zfLogT() << "normal observer";
        } ZFLISTENER_END(normalObserver)
        ZFGlobalObserver().observerAdd(
            ZFGlobalEvent::EventZFCore_EventFilter_test(),
            normalObserver);

        ZFLISTENER(eventFilter) {
            zfLogT() << "event filter";
            zfargs.eventFiltered(zftrue);
        } ZFLISTENER_END(eventFilter)
        ZFGlobalObserver().observerAdd(ZFObserverAddParam()
            .eventId(ZFGlobalEvent::EventZFCore_EventFilter_test())
            .observer(eventFilter)
            .observerLevel(ZFLevelAppHigh));

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

