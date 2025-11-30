#include "ZFLog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLog_ZFDI_errorCallbacks, ZFLevelZFFrameworkEssential) {
    _o = ZFCallbackForFunc(_onOutput);
    _update(_o);
    ZFLISTENER_1(logLevelOnUpdate
            , ZFOutput, _o
            ) {
        _update(_o);
    } ZFLISTENER_END()
    _logLevelOnUpdate = logLevelOnUpdate;
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_LogLevelOnUpdate(), _logLevelOnUpdate);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLog_ZFDI_errorCallbacks) {
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_LogLevelOnUpdate(), _logLevelOnUpdate);
}
private:
    ZFOutput _o;
    ZFListener _logLevelOnUpdate;
private:
    static zfindex _onOutput(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        return ZFOutputDefault().execute(src, count);
    }
    static void _update(ZF_IN const ZFOutput &o) {
        ZFDI_errorCallbacks().removeElement(o);
        if(ZFLogD()) {
            ZFDI_errorCallbacks().add(o);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFLog_ZFDI_errorCallbacks)

ZF_NAMESPACE_GLOBAL_END

