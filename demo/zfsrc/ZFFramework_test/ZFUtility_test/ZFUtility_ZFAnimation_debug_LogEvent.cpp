#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFAnimation_debug_LogEvent) {
    this->taskOwner = zfAlloc(ZFObject);

    ZFLISTENER(aniOnAlloc) {
        if(zfargs.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData())) {
            zfLogTrim() << zfLogCurTimeString() << " " << zfargs.sender() << " alloc";
        }
    } ZFLISTENER_END()
    ZFGlobalObserver().observerAdd(ZFObject::EventObjectAfterAlloc(), aniOnAlloc);

    ZFLISTENER(aniOnDealloc) {
        if(zfargs.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData())) {
            zfLogTrim() << zfLogCurTimeString() << " " << zfargs.sender() << " dealloc";
        }
    } ZFLISTENER_END()
    ZFGlobalObserver().observerAdd(ZFObject::EventObjectBeforeDealloc(), aniOnDealloc);

    ZFLISTENER(aniOnInvalid) {
        zfLogTrim() << zfLogCurTimeString() << " " << zfargs.sender() << " invalid";
    } ZFLISTENER_END()
    ZFGlobalObserver().observerAdd(ZFAnimation::EventAniOnInvalid(), aniOnInvalid);

    ZFLISTENER(aniOnStart) {
        zfLogTrim() << zfLogCurTimeString() << " " << zfargs.sender() << " start";
    } ZFLISTENER_END()
    ZFGlobalObserver().observerAdd(ZFAnimation::EventAniOnStart(), aniOnStart);

    ZFLISTENER(aniOnStop) {
        zfLogTrim() << zfLogCurTimeString() << " " << zfargs.sender() << " stop";
    } ZFLISTENER_END()
    ZFGlobalObserver().observerAdd(ZFAnimation::EventAniOnStop(), aniOnStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFAnimation_debug_LogEvent) {
    ZFGlobalObserver().observerRemoveByOwner(this->taskOwner);
    zfRelease(this->taskOwner);
}
private:
    ZFObject *taskOwner;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFAnimation_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

