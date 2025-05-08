#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

#include "ZFCore/ZFSTLWrapper/zfstlset.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGCImmediately
        , ZFMP_IN_OPT(void *, L, zfnull)
        ) {
    if(L == zfnull) {
        L = ZFLuaStateCheck();
        if(L == zfnull) {
            return;
        }
    }
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCHolder, ZFLevelZFFrameworkHigh) {
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(luaStateOnDetach));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCHolder) {
    for(zfstlset<void *>::iterator it = m.begin(); it != m.end(); ++it) {
        ZFLuaGCImmediately(*it);
    }
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(luaStateOnDetach));
    if(this->gcTask != zfnull) {
        this->gcTask->stop();
    }
}
zfautoT<ZFTimer> gcTask;
zfstlset<void *> m;
static void luaStateOnDetach(ZF_IN const ZFArgs &zfargs) {
    ZFCoreMutexLocker();
    zfstlset<void *> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    m.erase(const_cast<void *>(zfargs.param0()->to<v_zfptr *>()->zfv));
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCHolder)

static void _ZFP_ZFLuaGCResolve(ZF_IN const ZFArgs &zfargs) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
    zfstlset<void *> &m = d->m;
    ZFCoreMutexLock();
    d->gcTask = zfnull;
    while(!m.empty()) {
        zfstlset<void *>::iterator it = m.begin();
        void *L = *it;
        m.erase(it);

        ZFCoreMutexUnlock();
        ZFLuaGCImmediately(L);
        ZFCoreMutexLock();
    }
    ZFCoreMutexUnlock();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGC
        , ZFMP_IN_OPT(void *, L, zfnull)
        ) {
    if(L == zfnull) {
        L = ZFLuaStateCheck();
        if(L == zfnull) {
            return;
        }
    }
    if(!ZFThread::implMainThreadTaskAvailable()) {
        ZFLuaGCImmediately(L);
        return;
    }

    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
    zfstlset<void *> &m = d->m;
    if(m.find(L) == m.end()) {
        m.insert(L);
        if(d->gcTask == zfnull) {
            d->gcTask = ZFTimerOnce(1000, ZFCallbackForFunc(_ZFP_ZFLuaGCResolve));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

