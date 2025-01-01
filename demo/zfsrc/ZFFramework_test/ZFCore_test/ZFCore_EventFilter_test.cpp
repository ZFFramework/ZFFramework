#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL(ZFLIB_APP, ZFCore_EventFilter_test)
ZFEVENT_GLOBAL_REGISTER(ZFCore_EventFilter_test)
ZF_NAMESPACE_END(ZFGlobalEvent)

zfclass ZFCore_EventFilter_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_EventFilter_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        ZFLISTENER(normalObserver) {
            ZFLog() << "normal observer";
        } ZFLISTENER_END()
        ZFGlobalObserver().observerAdd(
            ZFGlobalEvent::E_ZFCore_EventFilter_test(),
            normalObserver);

        ZFLISTENER(eventFilter) {
            ZFLog() << "event filter";
            zfargs.eventFiltered(zftrue);
        } ZFLISTENER_END()
        ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::E_ZFCore_EventFilter_test(),
                eventFilter,
                ZFLevelAppHigh
            );

        this->outputSeparator();
        this->output("notify with event filter, only filter would be called");
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFCore_EventFilter_test());

        this->outputSeparator();
        this->output("notify without event filter");
        ZFGlobalObserver().observerRemove(
            ZFGlobalEvent::E_ZFCore_EventFilter_test(),
            eventFilter);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFCore_EventFilter_test());

        ZFGlobalObserver().observerRemoveAll(ZFGlobalEvent::E_ZFCore_EventFilter_test());
        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_EventFilter_test)

ZF_NAMESPACE_GLOBAL_END

