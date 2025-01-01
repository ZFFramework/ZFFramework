#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFAnimation_debug_LogEvent) {
    ZFLISTENER(aniOnAlloc) {
        if(zfargs.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData())) {
            ZFLogTrim() << ZFLogCurTimeString() << " " << zfargs.sender() << " alloc";
        }
    } ZFLISTENER_END()

    ZFLISTENER(aniOnDealloc) {
        if(zfargs.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData())) {
            ZFLogTrim() << ZFLogCurTimeString() << " " << zfargs.sender() << " dealloc";
        }
    } ZFLISTENER_END()

    ZFLISTENER(aniOnStart) {
        ZFLogTrim() << ZFLogCurTimeString() << " " << zfargs.sender() << " start";
    } ZFLISTENER_END()

    ZFLISTENER(aniOnLoop) {
        ZFLogTrim() << ZFLogCurTimeString() << " " << zfargs.sender() << " loop";
    } ZFLISTENER_END()

    ZFLISTENER(aniOnStop) {
        ZFLogTrim() << ZFLogCurTimeString() << " " << zfargs.sender() << " stop";
    } ZFLISTENER_END()

    ZFObserverGroup(this->taskOwner, ZFGlobalObserver())
        .observerAdd(ZFObject::E_ObjectAfterAlloc(), aniOnAlloc)
        .observerAdd(ZFObject::E_ObjectBeforeDealloc(), aniOnDealloc)
        .observerAdd(ZFAnimation::E_AniOnStart(), aniOnStart)
        .observerAdd(ZFAnimation::E_AniOnLoop(), aniOnLoop)
        .observerAdd(ZFAnimation::E_AniOnStop(), aniOnStop)
        ;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFAnimation_debug_LogEvent) {
    ZFObserverGroupRemove(this->taskOwner);
}
private:
    zfobj<ZFObject> taskOwner;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFAnimation_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

