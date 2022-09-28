#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFAnimation_debug_LogEvent)
{
    this->taskOwner = zfAlloc(ZFObject);

    ZFLISTENER(aniOnAlloc) {
        if(listenerData.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData()))
        {
            zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "alloc";
        }
    } ZFLISTENER_END(aniOnAlloc)
    ZFGlobalObserver().observerAdd(ZFObject::EventObjectAfterAlloc(), aniOnAlloc);

    ZFLISTENER(aniOnDealloc) {
        if(listenerData.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData()))
        {
            zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "dealloc";
        }
    } ZFLISTENER_END(aniOnDealloc)
    ZFGlobalObserver().observerAdd(ZFObject::EventObjectBeforeDealloc(), aniOnDealloc);

    ZFLISTENER(aniOnInvalid) {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "invalid";
    } ZFLISTENER_END(aniOnInvalid)
    ZFGlobalObserver().observerAdd(ZFAnimation::EventAniOnInvalid(), aniOnInvalid);

    ZFLISTENER(aniOnStart) {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "start";
    } ZFLISTENER_END(aniOnStart)
    ZFGlobalObserver().observerAdd(ZFAnimation::EventAniOnStart(), aniOnStart);

    ZFLISTENER(aniOnStop) {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "stop";
    } ZFLISTENER_END(aniOnStop)
    ZFGlobalObserver().observerAdd(ZFAnimation::EventAniOnStop(), aniOnStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFAnimation_debug_LogEvent)
{
    ZFGlobalObserver().observerRemoveByOwner(this->taskOwner);
    zfRelease(this->taskOwner);
}
private:
    ZFObject *taskOwner;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFAnimation_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

