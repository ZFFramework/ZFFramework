#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

// #define _ZFP_ZFLuaGC_DEBUG 1

#if _ZFP_ZFLuaGC_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFLuaGC_log(fmt, ...) \
        zfimplLog("%s [ZFLuaGC] %s", zfimplTime(), zfstr(fmt, ##__VA_ARGS__).cString())
#else
    #define _ZFP_ZFLuaGC_log(fmt, ...)
#endif

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
    _ZFP_ZFLuaGC_log("%s run begin", L);
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L);
    _ZFP_ZFLuaGC_log("%s run end", L);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCHolder, ZFLevelZFFrameworkHigh) {
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(luaStateOnDetach));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCHolder) {
    ZFCoreMutexLocker();
    for(zfiter it = m.iter(); it; ++it) {
        m.iterValue(it)->stop();
        _ZFP_ZFLuaGC_log("%s run on destroy", L);
        ZFLuaGCImmediately(m.iterKey(it));
    }
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(luaStateOnDetach));
}
ZFCoreMap<void *, zfautoT<ZFTaskId> > m;
static void luaStateOnDetach(ZF_IN const ZFArgs &zfargs) {
    ZFCoreMutexLocker();
    ZFCoreMap<void *, zfautoT<ZFTaskId> > &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    m.remove(const_cast<void *>(zfargs.param0()->to<v_zfptr *>()->zfv));
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCHolder)

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGC
        , ZFMP_IN_OPT(void *, L, zfnull)
        ) {
    if(L == zfnull) {
        L = ZFLuaStateCheck();
        if(L == zfnull) {
            return;
        }
    }

    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
    ZFCoreMap<void *, zfautoT<ZFTaskId> > &m = d->m;
    if(!m.isContain(L)) {
        ZFThread *gcThread = ZFThread::currentThread();
        if(gcThread == zfnull) {
            _ZFP_ZFLuaGC_log("%s run with no task available", L);
            return;
        }
        ZFLISTENER_1(gcAction
                , void *, L
                ) {
            if(ZFLuaStateCheck() != zfnull) {
                _ZFP_ZFLuaGC_log("%s run after delayed", L);
                ZFLuaGCImmediately(L);
            }
            else {
                _ZFP_ZFLuaGC_log("%s run after delayed, but no lua state", L);
            }
            ZFCoreMutexLocker();
            ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
            d->m.remove(L);
        } ZFLISTENER_END()
        m.access(L) = zfpostDelayed(1000, gcAction, gcThread);
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, ZFLuaGCForAllThread) {
    ZFCoreArray<void *> luaStateList;
    ZFCoreArray<ZFThread *> threadList;
    ZFLuaStateListForAllThread(luaStateList, threadList);
    ZFLISTENER_0(action
            ) {
        void *L = ZFLuaStateCheck();
        if(L) {
            _ZFP_ZFLuaGC_log("%s run on thread: %s", L, ZFThread::currentThread());
            ZFLuaGCImmediately(L);
        }
        else {
            _ZFP_ZFLuaGC_log("%s run on thread: %s, but no lua state", L, ZFThread::currentThread());
        }
    } ZFLISTENER_END()
    for(zfindex i = 0; i < threadList.count(); ++i) {
        zfpost(action, threadList[i]);
    }
}

ZF_NAMESPACE_GLOBAL_END

