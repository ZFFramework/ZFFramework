#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// state for thread
zfclass _ZFP_I_ZFLuaStateHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFLuaStateHolder, ZFObject)
    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
    ZFOBJECT_SINGLETON_DECLARE(_ZFP_I_ZFLuaStateHolder, instance)

public:
    ZFThread *ownerThread;
    void *L;
    zfbool autoClose;
    ZFCoreArrayPOD<void *> LList;

public:
    static ZFCoreArrayPOD<_ZFP_I_ZFLuaStateHolder *> &attachList(void)
    {
        static ZFCoreArrayPOD<_ZFP_I_ZFLuaStateHolder *> d;
        return d;
    }

public:
    static _ZFP_I_ZFLuaStateHolder *prepareForCurrentThread(void)
    {
        if(!ZFThread::implAvailable())
        {
            return _ZFP_I_ZFLuaStateHolder::instance();
        }
        ZFThread *curThread = ZFThread::currentThread();
        if(curThread == zfnull)
        {
            return _ZFP_I_ZFLuaStateHolder::instance();
        }
        _ZFP_I_ZFLuaStateHolder *holder = curThread->objectTag<_ZFP_I_ZFLuaStateHolder *>(_ZFP_I_ZFLuaStateHolder::ClassData()->className());
        if(holder == zfnull)
        {
            holder = zfAlloc(_ZFP_I_ZFLuaStateHolder);
            curThread->objectTag(_ZFP_I_ZFLuaStateHolder::ClassData()->className(), holder);
            zfRelease(holder);
            _ZFP_I_ZFLuaStateHolder::attachList().add(holder);

            holder->ownerThread = curThread;
            ZFLISTENER_1(onDetach
                    , ZFThread *, curThread
                    ) {
                curThread->objectTag(_ZFP_I_ZFLuaStateHolder::ClassData()->className(), zfnull);
            } ZFLISTENER_END(onDetach)
            curThread->observerAddForOnce(ZFThread::EventThreadOnUnregister(), onDetach, ZFLevelZFFrameworkPostNormal);
        }
        return holder;
    }

public:
    void *LInit(void)
    {
        if(L == zfnull)
        {
            L = ZFPROTOCOL_ACCESS(ZFLua)->luaStateOpen();
            ZFPROTOCOL_ACCESS(ZFLua)->luaStateAttach(L);
            autoClose = zftrue;
            LList.add(L);

            zfblockedAlloc(v_ZFPtr, LHolder);
            LHolder->zfv = L;
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventLuaStateOnAttach(), LHolder);
        }
        return L;
    }
    void LChange(ZF_IN void *L, ZF_IN zfbool autoClose)
    {
        if(this->L == L && this->autoClose == autoClose)
        {
            return;
        }
        _cleanup();
        this->L = L;
        this->autoClose = autoClose;
    }

    void LAttach(ZF_IN void *L)
    {
        ZFPROTOCOL_ACCESS(ZFLua)->luaStateAttach(L);
        LList.add(L);

        zfblockedAlloc(v_ZFPtr, LHolder);
        LHolder->zfv = L;
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventLuaStateOnAttach(), LHolder);
    }
    void LDetach(ZF_IN void *L)
    {
        zfblockedAlloc(v_ZFPtr, LHolder);
        LHolder->zfv = L;
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventLuaStateOnDetach(), LHolder);

        LList.removeElement(L);
        ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(L);
    }

private:
    void _cleanup(void)
    {
        _ZFP_I_ZFLuaStateHolder::attachList().removeElement(this);
        if(L != zfnull)
        {
            LList.removeElement(L);

            zfblockedAlloc(v_ZFPtr, LHolder);
            LHolder->zfv = L;
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventLuaStateOnDetach(), LHolder);

            ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(L);
            if(autoClose)
            {
                ZFPROTOCOL_ACCESS(ZFLua)->luaStateClose(L);
            }
            L = zfnull;
            autoClose = zffalse;
        }
        for(zfindex i = 0; i < LList.count(); ++i)
        {
            zfblockedAlloc(v_ZFPtr, LHolder);
            LHolder->zfv = LList[i];
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventLuaStateOnDetach(), LHolder);

            ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(LList[i]);
        }
        LList.removeAll();
        ownerThread = zfnull;
    }
protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->ownerThread = zfnull;
        this->L = zfnull;
        this->autoClose = zffalse;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }
};
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(_ZFP_I_ZFLuaStateHolder, instance, ZFLevelZFFrameworkNormal)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaStateAutoClean, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaStateAutoClean)
{
    zfCoreMutexLocker();
    ZFCoreArrayPOD<void *> stateList;
    ZFCoreArrayPOD<ZFThread *> threadList;
    ZFLuaStateListForAllThread(stateList, threadList);
    for(zfindex i = threadList.count() - 1; i != zfindexMax(); --i)
    {
        threadList[i]->objectTag(_ZFP_I_ZFLuaStateHolder::ClassData()->className(), zfnull);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaStateAutoClean)

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaState)
{
    zfCoreMutexLocker();
    return _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LInit();
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateChange,
                       ZFMP_IN(void *, L))
{
    if(L != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LChange(L, zffalse);
    }
}

ZFMETHOD_FUNC_DEFINE_0(ZFCoreArrayPOD<void *>, ZFLuaStateList)
{
    zfCoreMutexLocker();
    return _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LList;
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFLuaStateListForAllThread,
                       ZFMP_OUT(ZFCoreArray<void *>, luaStateList),
                       ZFMP_OUT(ZFCoreArray<ZFThread *>, threadList))
{
    zfCoreMutexLocker();
    ZFCoreArrayPOD<_ZFP_I_ZFLuaStateHolder *> &d = _ZFP_I_ZFLuaStateHolder::attachList();

    for(zfindex iHolder = 0; iHolder < d.count(); ++iHolder)
    {
        _ZFP_I_ZFLuaStateHolder *holder = d[iHolder];
        for(zfindex iL = 0; iL < holder->LList.count(); ++iL)
        {
            luaStateList.add(holder->LList[iL]);
            threadList.add(holder->ownerThread);
        }
    }
}

ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaStateOpen)
{
    zfCoreMutexLocker();
    return ZFPROTOCOL_ACCESS(ZFLua)->luaStateOpen();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateClose,
                       ZFMP_IN(void *, L))
{
    zfCoreMutexLocker();
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L);
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateClose(L);
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateAttach,
                       ZFMP_IN(void *, L))
{
    if(L != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LAttach(L);
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateDetach,
                       ZFMP_IN(void *, L))
{
    if(L != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LDetach(L);
    }
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(LuaStateOnAttach)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(LuaStateOnDetach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
// notify update metadata when class data changed
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaStateAutoUpdate, ZFLevelZFFrameworkNormal)
{
    this->classDataOnChangeListener = ZFCallbackForFunc(zfself::classDataOnChange);
    ZFClassDataChangeObserver().observerAdd(ZFGlobalEvent::EventClassDataChange(), this->classDataOnChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaStateAutoUpdate)
{
    ZFClassDataChangeObserver().observerRemove(ZFGlobalEvent::EventClassDataChange(), this->classDataOnChangeListener);
}
public:
    ZFListener classDataOnChangeListener;
    static void classDataOnChange(ZF_IN const ZFArgs &zfargs)
    {
        zfCoreMutexLocker();
        v_ZFClassDataChangeData *changed = zfargs.param0T();

        ZFCoreArrayPOD<void *> stateList;
        ZFCoreArrayPOD<ZFThread *> threadList;
        ZFLuaStateListForAllThread(stateList, threadList);
        ZFPROTOCOL_INTERFACE_CLASS(ZFLua) *impl = ZFPROTOCOL_ACCESS(ZFLua);
        for(zfindex i = 0; i < stateList.count(); ++i)
        {
            impl->classDataChange(stateList[i], changed->zfv);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFLuaStateAutoUpdate)

ZF_NAMESPACE_GLOBAL_END

