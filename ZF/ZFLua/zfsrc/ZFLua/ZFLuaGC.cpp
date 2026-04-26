#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

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
typedef zfstlhashmap<void *, zfautoT<ZFTaskId> > _ZFP_ZFLuaGCMap;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCHolder, ZFLevelZFFrameworkHigh) {
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(luaStateOnDetach));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCHolder) {
    ZFCoreMutexLocker();
    for(_ZFP_ZFLuaGCMap::iterator it = m.begin(); it != m.end(); ++it) {
        it->second->stop();
        _ZFP_ZFLuaGC_log("%s run on destroy", L);
        ZFLuaGCImmediately(it->first);
    }
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(luaStateOnDetach));
}
_ZFP_ZFLuaGCMap m;
static void luaStateOnDetach(ZF_IN const ZFArgs &zfargs) {
    ZFCoreMutexLocker();
    _ZFP_ZFLuaGCMap &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    m.erase(const_cast<void *>(zfargs.param0()->to<v_zfptr *>()->zfv));
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
    _ZFP_ZFLuaGCMap &m = d->m;
    if(m.find(L) == m.end()) {
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
            d->m.erase(L);
        } ZFLISTENER_END()
        m[L] = zfpostDelayed(1000, gcAction, gcThread);
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

