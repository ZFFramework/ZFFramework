#include "ZFGlobalEvent_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(AppOnCreate)
ZFEVENT_GLOBAL_REGISTER(AppOnDestroy)
ZFEVENT_GLOBAL_REGISTER(AppOnMemoryLow)
ZF_NAMESPACE_END(ZFGlobalEvent)

static void _ZFP_ZFGlobalEvent_common_AppOnCreate(void) {
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppOnCreate());
}
static void _ZFP_ZFGlobalEvent_common_AppOnDestroy(void) {
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_AppOnDestroy());
}

ZF_STATIC_REGISTER_INIT(_ZFP_ZFGlobalEvent_common_register) {
    ZFFrameworkInitFinishCallbacks.add(_ZFP_ZFGlobalEvent_common_AppOnCreate);
    ZFFrameworkCleanupPrepareCallbacks.add(_ZFP_ZFGlobalEvent_common_AppOnDestroy);
}
ZF_STATIC_REGISTER_DESTROY(_ZFP_ZFGlobalEvent_common_register) {
    ZFFrameworkInitFinishCallbacks.removeElement(_ZFP_ZFGlobalEvent_common_AppOnCreate);
    ZFFrameworkCleanupPrepareCallbacks.removeElement(_ZFP_ZFGlobalEvent_common_AppOnDestroy);
}
ZF_STATIC_REGISTER_END(_ZFP_ZFGlobalEvent_common_register)

ZF_GLOBAL_INITIALIZER_INIT(zfAllocWithCache_autoClean) {
    this->cleanListener = ZFCallbackForFunc(zfself::clean);
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_AppOnMemoryLow(), this->cleanListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfAllocWithCache_autoClean) {
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_AppOnMemoryLow(), this->cleanListener);
}
ZFListener cleanListener;
static void clean(ZF_IN const ZFArgs &zfargs) {
    zfAllocCacheRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(zfAllocWithCache_autoClean)

ZF_NAMESPACE_GLOBAL_END

