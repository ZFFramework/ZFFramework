#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// state for thread
zfclass _ZFP_I_ZFLuaStateHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFLuaStateHolder, ZFObject)
    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
    ZFOBJECT_SINGLETON_DECLARE(_ZFP_I_ZFLuaStateHolder, instance)

public:
    ZFThread *ownerThread;
    void *L;
    zfbool autoClose;
    ZFCoreArray<void *> LList;

public:
    static ZFCoreArray<_ZFP_I_ZFLuaStateHolder *> &attachList(void) {
        static ZFCoreArray<_ZFP_I_ZFLuaStateHolder *> d;
        return d;
    }

public:
    static _ZFP_I_ZFLuaStateHolder *prepareForCurrentThread(void) {
        if(!ZFThread::implAvailable()) {
            return _ZFP_I_ZFLuaStateHolder::instance();
        }
        ZFThread *curThread = ZFThread::currentThread();
        if(curThread == zfnull) {
            return _ZFP_I_ZFLuaStateHolder::instance();
        }
        _ZFP_I_ZFLuaStateHolder *holder = curThread->objectTag(_ZFP_I_ZFLuaStateHolder::ClassData()->className());
        if(holder == zfnull) {
            holder = zfAlloc(_ZFP_I_ZFLuaStateHolder);
            curThread->objectTag(_ZFP_I_ZFLuaStateHolder::ClassData()->className(), holder);
            zfRelease(holder);
            _ZFP_I_ZFLuaStateHolder::attachList().add(holder);

            holder->ownerThread = curThread;
            ZFLISTENER_1(onDetach
                    , ZFThread *, curThread
                    ) {
                curThread->objectTag(_ZFP_I_ZFLuaStateHolder::ClassData()->className(), zfnull);
            } ZFLISTENER_END()
            curThread->observerAddForOnce(ZFThread::E_ThreadOnUnregister(), onDetach, ZFLevelZFFrameworkPostNormal);
        }
        return holder;
    }

public:
    void *LInit(void) {
        if(L == zfnull) {
            L = ZFPROTOCOL_ACCESS(ZFLua)->luaStateOpen();
            ZFPROTOCOL_ACCESS(ZFLua)->luaStateAttach(L);
            autoClose = zftrue;
            LList.add(L);

            zfobj<v_zfptr> LHolder;
            LHolder->zfv = L;
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_LuaStateOnAttach(), LHolder);
        }
        return L;
    }
    void LChange(
            ZF_IN void *L
            , ZF_IN zfbool autoClose
            ) {
        if(this->L == L && this->autoClose == autoClose) {
            return;
        }
        _cleanup();
        this->L = L;
        this->autoClose = autoClose;
    }

    void LAttach(ZF_IN void *L) {
        ZFPROTOCOL_ACCESS(ZFLua)->luaStateAttach(L);
        LList.add(L);

        zfobj<v_zfptr> LHolder;
        LHolder->zfv = L;
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_LuaStateOnAttach(), LHolder);
    }
    void LDetach(ZF_IN void *L) {
        zfobj<v_zfptr> LHolder;
        LHolder->zfv = L;
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_LuaStateOnDetach(), LHolder);

        LList.removeElement(L);
        ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(L);
    }

private:
    void _cleanup(void) {
        _ZFP_I_ZFLuaStateHolder::attachList().removeElement(this);
        if(L != zfnull) {
            LList.removeElement(L);

            zfobj<v_zfptr> LHolder;
            LHolder->zfv = L;
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_LuaStateOnDetach(), LHolder);

            ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(L);
            if(autoClose) {
                ZFPROTOCOL_ACCESS(ZFLua)->luaStateClose(L);
            }
            L = zfnull;
            autoClose = zffalse;
        }
        for(zfindex i = 0; i < LList.count(); ++i) {
            zfobj<v_zfptr> LHolder;
            LHolder->zfv = LList[i];
            ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_LuaStateOnDetach(), LHolder);

            ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(LList[i]);
        }
        LList.removeAll();
        ownerThread = zfnull;
    }
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->ownerThread = zfnull;
        this->L = zfnull;
        this->autoClose = zffalse;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }
};
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(_ZFP_I_ZFLuaStateHolder, instance, ZFLevelZFFrameworkNormal)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaStateAutoClean, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaStateAutoClean) {
    ZFCoreMutexLocker();
    ZFCoreArray<void *> stateList;
    ZFCoreArray<ZFThread *> threadList;
    ZFLuaStateListForAllThread(stateList, threadList);
    for(zfindex i = threadList.count() - 1; i != zfindexMax(); --i) {
        threadList[i]->objectTag(_ZFP_I_ZFLuaStateHolder::ClassData()->className(), zfnull);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaStateAutoClean)

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaState) {
    ZFCoreMutexLocker();
    return _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LInit();
}
ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaStateCheck) {
    ZFCoreMutexLocker();
    return _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->L;
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateChange
        , ZFMP_IN(void *, L)
        ) {
    if(L != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LChange(L, zffalse);
    }
}

ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<void *>, ZFLuaStateList) {
    ZFCoreMutexLocker();
    return _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LList;
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFLuaStateListForAllThread
        , ZFMP_OUT(ZFCoreArray<void *> &, luaStateList)
        , ZFMP_OUT(ZFCoreArray<ZFThread *> &, threadList)
        ) {
    ZFCoreMutexLocker();
    ZFCoreArray<_ZFP_I_ZFLuaStateHolder *> &d = _ZFP_I_ZFLuaStateHolder::attachList();

    for(zfindex iHolder = 0; iHolder < d.count(); ++iHolder) {
        _ZFP_I_ZFLuaStateHolder *holder = d[iHolder];
        for(zfindex iL = 0; iL < holder->LList.count(); ++iL) {
            luaStateList.add(holder->LList[iL]);
            threadList.add(holder->ownerThread);
        }
    }
}

ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaStateOpen) {
    ZFCoreMutexLocker();
    return ZFPROTOCOL_ACCESS(ZFLua)->luaStateOpen();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateClose
        , ZFMP_IN(void *, L)
        ) {
    ZFCoreMutexLocker();
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L);
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateClose(L);
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateAttach
        , ZFMP_IN(void *, L)
        ) {
    if(L != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LAttach(L);
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateDetach
        , ZFMP_IN(void *, L)
        ) {
    if(L != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_I_ZFLuaStateHolder::prepareForCurrentThread()->LDetach(L);
    }
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(LuaStateOnAttach)
ZFEVENT_GLOBAL_REGISTER(LuaStateOnDetach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
// notify update metadata when class data changed
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaStateAutoUpdate, ZFLevelZFFrameworkNormal) {
    this->classDataOnUpdateListener = ZFCallbackForFunc(zfself::classDataOnUpdate);
    ZFClassDataUpdateObserver().observerAdd(ZFGlobalEvent::E_ClassDataUpdate(), this->classDataOnUpdateListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaStateAutoUpdate) {
    ZFClassDataUpdateObserver().observerRemove(ZFGlobalEvent::E_ClassDataUpdate(), this->classDataOnUpdateListener);
}
public:
    ZFListener classDataOnUpdateListener;
    static void classDataOnUpdate(ZF_IN const ZFArgs &zfargs) {
        ZFCoreMutexLocker();
        v_ZFClassDataUpdateData *changed = zfargs.param0();

        ZFCoreArray<void *> stateList;
        ZFCoreArray<ZFThread *> threadList;
        ZFLuaStateListForAllThread(stateList, threadList);
        ZFPROTOCOL_INTERFACE_CLASS(ZFLua) *impl = ZFPROTOCOL_ACCESS(ZFLua);
        for(zfindex i = 0; i < stateList.count(); ++i) {
            impl->classDataUpdate(stateList[i], changed->zfv);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFLuaStateAutoUpdate)

ZF_NAMESPACE_GLOBAL_END

