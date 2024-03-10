#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGCImmediately
        , ZFMP_IN_OPT(void *, L, zfnull)
        ) {
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L ? L : ZFLuaState());
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCHolder, ZFLevelZFFrameworkHigh) {
    this->luaStateOnDetachListener = ZFCallbackForFunc(luaStateOnDetach);
    ZFGlobalObserver().observerAdd(
        ZFGlobalEvent::EventLuaStateOnDetach(),
        this->luaStateOnDetachListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCHolder) {
    for(zfstlmap<void *, zfbool>::iterator it = m.begin(); it != m.end(); ++it) {
        ZFLuaGCImmediately(it->first);
    }
    ZFGlobalObserver().observerRemove(
        ZFGlobalEvent::EventLuaStateOnDetach(),
        this->luaStateOnDetachListener);
    if(this->gcTask != zfnull) {
        this->gcTask->timerStop();
    }
}
zfautoT<ZFTimer *> gcTask;
zfstlmap<void *, zfbool> m;
ZFListener luaStateOnDetachListener;
static void luaStateOnDetach(ZF_IN const ZFArgs &zfargs) {
    zfCoreMutexLocker();
    zfstlmap<void *, zfbool> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    m.erase(zfargs.param0()->to<v_ZFPtr *>()->zfv);
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCHolder)

static void _ZFP_ZFLuaGCResolve(ZF_IN const ZFArgs &zfargs) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
    zfstlmap<void *, zfbool> &m = d->m;
    zfCoreMutexLock();
    d->gcTask = zfnull;
    while(!m.empty()) {
        zfstlmap<void *, zfbool>::iterator it = m.begin();
        void *L = it->first;
        m.erase(it);

        zfCoreMutexUnlock();
        ZFLuaGCImmediately(L);
        zfCoreMutexLock();
    }
    zfCoreMutexUnlock();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGC
        , ZFMP_IN_OPT(void *, L, zfnull)
        ) {
    if(L == zfnull) {
        L = ZFLuaState();
    }
    if(!ZFThread::implMainThreadTaskAvailable()) {
        ZFLuaGCImmediately(L);
        return;
    }

    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
    zfstlmap<void *, zfbool> &m = d->m;
    if(m.find(L) == m.end()) {
        m[L] = zftrue;
        if(d->gcTask == zfnull) {
            d->gcTask = ZFTimerOnce(1000, ZFCallbackForFunc(_ZFP_ZFLuaGCResolve));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

