#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGCImmediately,
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L ? L : ZFLuaState());
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCHolder, ZFLevelZFFrameworkHigh)
{
    this->luaStateOnDetachListener = ZFCallbackForFunc(luaStateOnDetach);
    ZFGlobalObserver().observerAdd(
        ZFGlobalEvent::EventLuaStateOnDetach(),
        this->luaStateOnDetachListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCHolder)
{
    for(zfstlmap<void *, zfbool>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ZFLuaGCImmediately(it->first);
    }
    ZFGlobalObserver().observerRemove(
        ZFGlobalEvent::EventLuaStateOnDetach(),
        this->luaStateOnDetachListener);
    if(this->gcTask != zfnull)
    {
        this->gcTask->timerStop();
    }
}
zfautoObjectT<ZFTimer *> gcTask;
zfstlmap<void *, zfbool> m;
ZFListener luaStateOnDetachListener;
static void luaStateOnDetach(ZF_IN const ZFArgs &zfargs)
{
    zfCoreMutexLocker();
    zfstlmap<void *, zfbool> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    m.erase(zfargs.param0()->to<v_ZFPtr *>()->zfv);
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCHolder)

static void _ZFP_ZFLuaGCResolve(ZF_IN const ZFArgs &zfargs)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
    zfstlmap<void *, zfbool> &m = d->m;
    zfCoreMutexLock();
    d->gcTask = zfnull;
    while(!m.empty())
    {
        zfstlmap<void *, zfbool>::iterator it = m.begin();
        void *L = it->first;
        m.erase(it);

        zfCoreMutexUnlock();
        ZFLuaGCImmediately(L);
        zfCoreMutexLock();
    }
    zfCoreMutexUnlock();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGC,
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFThread))
    {
        ZFLuaGCImmediately(L);
        return ;
    }

    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaGCHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder);
    zfstlmap<void *, zfbool> &m = d->m;
    if(m.find(L) == m.end())
    {
        m[L] = zftrue;
        if(d->gcTask == zfnull)
        {
            d->gcTask = ZFTimerOnce(1000, ZFCallbackForFunc(_ZFP_ZFLuaGCResolve));
        }
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCAutoApply, ZFLevelZFFrameworkNormal)
{
    this->classChangeListener = ZFCallbackForFunc(zfself::classChange);
    ZFClassDataChangeObserver().observerAdd(
        ZFGlobalEvent::EventClassDataChange(),
        this->classChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCAutoApply)
{
    ZFClassDataChangeObserver().observerRemove(
        ZFGlobalEvent::EventClassDataChange(),
        this->classChangeListener);
}
ZFListener classChangeListener;
static void classChange(ZF_IN const ZFArgs &zfargs)
{
    const ZFClassDataChangeData &data = zfargs.param0()->to<v_ZFClassDataChangeData *>()->zfv;
    if(data.changedClass != zfnull && data.changeType == ZFClassDataChangeTypeDetach)
    {
        ZFCoreArrayPOD<void *> L;
        ZFLuaStateListT(L);
        for(zfindex i = 0; i < L.count(); ++i)
        {
            ZFLuaGC(L[i]);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCAutoApply)

ZF_NAMESPACE_GLOBAL_END

