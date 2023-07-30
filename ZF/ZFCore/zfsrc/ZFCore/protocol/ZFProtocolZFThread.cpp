#include "ZFProtocolZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFThread)

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

