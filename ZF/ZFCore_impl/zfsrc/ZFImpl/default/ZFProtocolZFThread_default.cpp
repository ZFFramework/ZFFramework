#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#else // #if ZF_ENV_sys_Windows
    #include <pthread.h>
    #include <unistd.h>
#endif // #if ZF_ENV_sys_Windows #else

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global type
zfclassNotPOD _ZFP_ZFThreadImpl_default_ExecuteData {
public:
    ZFListener runnable;
    ZFListener runnableCleanup;

public:
    _ZFP_ZFThreadImpl_default_ExecuteData(
            ZF_IN ZFListener runnable
            , ZF_IN ZFListener runnableCleanup
            )
    : runnable(runnable)
    , runnableCleanup(runnableCleanup)
    {
    }
};

// ============================================================
// native wrapper
#if ZF_ENV_sys_Windows
typedef DWORD _ZFP_ZFThreadImpl_default_NativeThreadIdType;
static _ZFP_ZFThreadImpl_default_NativeThreadIdType _ZFP_ZFThreadImpl_default_getNativeThreadId(void) {
    return GetCurrentThreadId();
}
static void _ZFP_ZFThreadImpl_default_threadCallback(_ZFP_ZFThreadImpl_default_ExecuteData *data);
static DWORD WINAPI _ZFP_ZFThreadImpl_default_nativeCallback(LPVOID param) {
    _ZFP_ZFThreadImpl_default_threadCallback((_ZFP_ZFThreadImpl_default_ExecuteData *)param);
    return 0;
}
static void _ZFP_ZFThreadImpl_default_startNativeThread(_ZFP_ZFThreadImpl_default_ExecuteData *data) {
    CreateThread(NULL, 0, _ZFP_ZFThreadImpl_default_nativeCallback, data, 0, NULL);
}
#else // #if ZF_ENV_sys_Windows
typedef pthread_t _ZFP_ZFThreadImpl_default_NativeThreadIdType;
static _ZFP_ZFThreadImpl_default_NativeThreadIdType _ZFP_ZFThreadImpl_default_getNativeThreadId(void) {
    return pthread_self();
}
static void _ZFP_ZFThreadImpl_default_threadCallback(_ZFP_ZFThreadImpl_default_ExecuteData *data);
static void *_ZFP_ZFThreadImpl_default_nativeCallback(void *param) {
    _ZFP_ZFThreadImpl_default_threadCallback((_ZFP_ZFThreadImpl_default_ExecuteData *)param);
    return zfnull;
}
static void _ZFP_ZFThreadImpl_default_startNativeThread(_ZFP_ZFThreadImpl_default_ExecuteData *data) {
    pthread_t tid = 0;
    pthread_create(&tid, NULL, _ZFP_ZFThreadImpl_default_nativeCallback, data);
}
#endif // #if ZF_ENV_sys_Windows #else

// ============================================================
// global data
typedef zfstlmap<_ZFP_ZFThreadImpl_default_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_default_ThreadMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_default_DataHolder, ZFLevelZFFrameworkEssential) {
    mainThread = zfAlloc(ZFThreadMainThread);
    threadMap[_ZFP_ZFThreadImpl_default_getNativeThreadId()] = mainThread;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_default_DataHolder) {
    zfRelease(mainThread);
}
public:
    ZFThread *mainThread;
    _ZFP_ZFThreadImpl_default_ThreadMapType threadMap;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_default_DataHolder)
#define _ZFP_ZFThreadImpl_default_mainThread (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_default_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_default_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_default_DataHolder)->threadMap)

void _ZFP_ZFThreadImpl_default_threadCallback(_ZFP_ZFThreadImpl_default_ExecuteData *data) {
    data->runnable.execute();
    data->runnableCleanup.execute();
    zfdelete(data);
}

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_default, ZFThread, v_ZFProtocolLevel::e_Default)
public:
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread) {
        _ZFP_ZFThreadImpl_default_NativeThreadIdType *token = zfnew(_ZFP_ZFThreadImpl_default_NativeThreadIdType);
        *token = _ZFP_ZFThreadImpl_default_getNativeThreadId();
        ZFCoreAssertWithMessage(_ZFP_ZFThreadImpl_default_threadMap.find(*token) == _ZFP_ZFThreadImpl_default_threadMap.end(),
            "thread already registered: %s", ownerZFThread);
        _ZFP_ZFThreadImpl_default_threadMap[*token] = ownerZFThread;
        return (void *)token;
    }
    virtual void nativeThreadUnregister(ZF_IN void *token) {
        _ZFP_ZFThreadImpl_default_threadMap.erase(_ZFP_ZFThreadImpl_default_getNativeThreadId());
        zfdelete((_ZFP_ZFThreadImpl_default_NativeThreadIdType *)token);
    }
    virtual ZFThread *threadForToken(ZF_IN void *token) {
        _ZFP_ZFThreadImpl_default_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_default_threadMap.find(
            *(_ZFP_ZFThreadImpl_default_NativeThreadIdType *)token);
        if(it != _ZFP_ZFThreadImpl_default_threadMap.end()) {
            return it->second;
        }
        return zfnull;
    }
    virtual ZFThread *mainThread(void) {
        return _ZFP_ZFThreadImpl_default_mainThread;
    }
    virtual ZFThread *currentThread(void) {
        _ZFP_ZFThreadImpl_default_NativeThreadIdType nativeCurrentThread = _ZFP_ZFThreadImpl_default_getNativeThreadId();

        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_default_ThreadMapType::const_iterator it = _ZFP_ZFThreadImpl_default_threadMap.find(nativeCurrentThread);
        if(it == _ZFP_ZFThreadImpl_default_threadMap.end()) {
            return zfnull;
        }
        return it->second;
    }

    virtual zfbool executeInMainThreadAvailable(void) {
        return ZFMainThreadTaskImplGetExecute() != zfnull;
    }
    virtual void *executeInMainThread(ZF_IN const ZFListener &runnable) {
        if(ZFMainThreadTaskImplGetExecute() == zfnull) {
            ZFCoreCriticalMessageTrim("[ZFThread] executeInMainThread not available");
            return zfnull;
        }
        return ZFMainThreadTaskImplGetExecute()(runnable);
    }
    virtual void executeInMainThreadCleanup(ZF_IN void *nativeToken) {
        if(ZFMainThreadTaskImplGetCleanup() != zfnull) {
            ZFMainThreadTaskImplGetCleanup()(nativeToken);
        }
    }

    virtual void *executeInNewThread(
            ZF_IN const ZFListener &runnable
            , ZF_IN const ZFListener &runnableCleanup
            ) {
        _ZFP_ZFThreadImpl_default_ExecuteData *data = zfnew(_ZFP_ZFThreadImpl_default_ExecuteData
                , runnable
                , runnableCleanup
            );
        _ZFP_ZFThreadImpl_default_startNativeThread(data);
        return zfnull;
    }
    virtual void executeInNewThreadCleanup(ZF_IN void *nativeToken) {
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_default)

ZF_NAMESPACE_GLOBAL_END

