#include "ZFProtocolZFThread.h"
#include "../ZFSemaphore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFThread)

void *ZFPROTOCOL_INTERFACE_CLASS(ZFThread)::sleepTokenCreate(void) {
    return zfobjAlloc(ZFSemaphore);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFThread)::sleepTokenDestroy(ZF_IN void *sleepToken) {
    zfobjRelease((ZFSemaphore *)sleepToken);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFThread)::sleep(
            ZF_IN void *sleepToken
            , ZF_IN zftimet miliSecs
            ) {
    return ((ZFSemaphore *)sleepToken)->lockAndWait(miliSecs);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFThread)::sleepCancel(ZF_IN void *sleepToken) {
    ((ZFSemaphore *)sleepToken)->lockAndBroadcast();
}

// ============================================================
static ZFMainThreadTaskImplCallbackExecute _ZFP_ZFMainThreadTaskImplExecute = zfnull;
static ZFMainThreadTaskImplCallbackCleanup _ZFP_ZFMainThreadTaskImplCleanup = zfnull;

void ZFMainThreadTaskImplSet(
        ZF_IN ZFMainThreadTaskImplCallbackExecute executeImpl
        , ZF_IN ZFMainThreadTaskImplCallbackCleanup cleanupImpl
        ) {
    _ZFP_ZFMainThreadTaskImplExecute = executeImpl;
    _ZFP_ZFMainThreadTaskImplCleanup = cleanupImpl;
}
ZFMainThreadTaskImplCallbackExecute ZFMainThreadTaskImplGetExecute(void) {
    return _ZFP_ZFMainThreadTaskImplExecute;
}
ZFMainThreadTaskImplCallbackCleanup ZFMainThreadTaskImplGetCleanup(void) {
    return _ZFP_ZFMainThreadTaskImplCleanup;
}

ZF_NAMESPACE_GLOBAL_END

