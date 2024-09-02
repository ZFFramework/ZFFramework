#include "ZFNet_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFNet_ZFHttpRequest_debug_LogEvent) {
    ZFLISTENER(onSend) {
        ZFHttpRequest *send = zfargs.sender();
        zfLogTrim("[ZFHttpRequest] send: %s", send);
    } ZFLISTENER_END()

    ZFLISTENER(onRecv) {
        ZFHttpResponse *recv = zfargs.param0();
        ZFResultTypeEnum resultType = zfargs.param1().to<ZFResultType *>()->zfv();
        zfLogTrim("[ZFHttpRequest] recv %s: %s", resultType, recv);
    } ZFLISTENER_END()

    ZFObserverGroup(this->taskOwner, ZFGlobalObserver())
        .observerAdd(ZFHttpRequest::EventOnRequest(), onSend)
        .observerAdd(ZFHttpRequest::EventOnResponse(), onRecv)
        ;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFNet_ZFHttpRequest_debug_LogEvent) {
    ZFObserverGroupRemove(this->taskOwner);
}
private:
    zfobj<ZFObject> taskOwner;
ZF_GLOBAL_INITIALIZER_END(ZFNet_ZFHttpRequest_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

