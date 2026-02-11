#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFThread ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFThread)
#define ZFImpl_sys_Android_JNI_NAME_ZFThread ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFThread)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFThread, ZFImpl_sys_Android_JNI_NAME_ZFThread)

// ============================================================
// global type
zfclassNotPOD _ZFP_ZFThreadImpl_sys_Android_ExecuteData {
public:
    ZFListener runnable;
    ZFListener runnableCleanup;

public:
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData(
            ZF_IN ZFListener runnable
            , ZF_IN ZFListener runnableCleanup
            )
    : runnable(runnable)
    , runnableCleanup(runnableCleanup)
    {
    }
};

// ============================================================
// global data
typedef jlong _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType;
typedef jint _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType;
typedef zfstlhashmap<_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_sys_Android_ThreadMapType;
typedef zfstlhashmap<_ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType, _ZFP_ZFThreadImpl_sys_Android_ExecuteData *> _ZFP_ZFThreadImpl_sys_Android_ExecuteDataMapType;

static _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType _ZFP_ZFThreadImpl_sys_Android_getNativeThreadId(void) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFThread(), "native_currentThread",
        JNIGetMethodSig(JNIType::S_long(), JNIParamTypeContainer()
        ).c_str());
    _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType ret = JNIUtilCallStaticLongMethod(jniEnv
        , ZFImpl_sys_Android_jclassZFThread()
        , jmId
        );
    return ret;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_sys_Android_DataHolder, ZFLevelZFFrameworkEssential) {
    mainThread = zfobjAlloc(ZFThreadMainThread);
    threadMap[_ZFP_ZFThreadImpl_sys_Android_getNativeThreadId()] = mainThread;
    executeId = 1;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_sys_Android_DataHolder) {
    zfobjRelease(mainThread);
    mainThread = zfnull;
}
public:
    ZFThread *mainThread;
    _ZFP_ZFThreadImpl_sys_Android_ThreadMapType threadMap;
    _ZFP_ZFThreadImpl_sys_Android_ExecuteDataMapType executeDataMap;
    _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType executeId;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_sys_Android_DataHolder)
#define _ZFP_ZFThreadImpl_sys_Android_mainThreadInstance (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_sys_Android_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->threadMap)
#define _ZFP_ZFThreadImpl_sys_Android_executeDataMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->executeDataMap)
#define _ZFP_ZFThreadImpl_sys_Android_executeId (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->executeId)

// ============================================================
static void _ZFP_ZFThreadImpl_sys_Android_updateExecuteId(void) {
    ++_ZFP_ZFThreadImpl_sys_Android_executeId;
    if(_ZFP_ZFThreadImpl_sys_Android_executeId > 30000) {
        _ZFP_ZFThreadImpl_sys_Android_executeId = 1;
    }
}

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_sys_Android, ZFThread, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Thread")
public:
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *token = zfnew(_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType);
        *token = _ZFP_ZFThreadImpl_sys_Android_getNativeThreadId();
        ZFCoreAssertWithMessage(_ZFP_ZFThreadImpl_sys_Android_threadMap.find(*token) == _ZFP_ZFThreadImpl_sys_Android_threadMap.end(),
            "thread already registered: %s", ownerZFThread);
        _ZFP_ZFThreadImpl_sys_Android_threadMap[*token] = ownerZFThread;
        return (void *)token;
    }
    virtual void nativeThreadUnregister(ZF_IN void *token) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *threadId = (_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *)token;
        _ZFP_ZFThreadImpl_sys_Android_threadMap.erase(*threadId);
        zfdelete((_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *)token);
    }
    virtual ZFThread *threadForToken(ZF_IN void *token) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_Android_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_sys_Android_threadMap.find(
            *(_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *)token);
        if(it != _ZFP_ZFThreadImpl_sys_Android_threadMap.end()) {
            return it->second;
        }
        return zfnull;
    }

    virtual ZFThread *mainThread(void) {
        ZFCoreMutexLocker();
        return _ZFP_ZFThreadImpl_sys_Android_mainThreadInstance;
    }
    virtual ZFThread *currentThread(void) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_Android_ThreadMapType::const_iterator it =
            _ZFP_ZFThreadImpl_sys_Android_threadMap.find(_ZFP_ZFThreadImpl_sys_Android_getNativeThreadId());
        if(it == _ZFP_ZFThreadImpl_sys_Android_threadMap.end()) {
            return zfnull;
        }
        return it->second;
    }

    virtual void *executeInMainThread(ZF_IN const ZFListener &runnable) {
        ZFCoreMutexLock();
        _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = zfnew(_ZFP_ZFThreadImpl_sys_Android_ExecuteData,
            runnable,
            zfnull);
        _ZFP_ZFThreadImpl_sys_Android_updateExecuteId();
        _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType curId = _ZFP_ZFThreadImpl_sys_Android_executeId;
        _ZFP_ZFThreadImpl_sys_Android_executeDataMap[curId] = d;
        ZFCoreMutexUnlock();

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFThread(), "native_executeInMainThread",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_int())
            ).c_str());

        jobject nativeToken = JNIUtilCallStaticObjectMethod(jniEnv
            , ZFImpl_sys_Android_jclassZFThread()
            , jmId
            , curId
            );
        return JNIUtilNewGlobalRef(jniEnv, nativeToken);
    }
    virtual void executeInMainThreadCleanup(ZF_IN void *nativeToken) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFThread(), "native_executeInMainThreadCleanup",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        jobject nativeTokenTmp = (jobject)nativeToken;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFThread(), jmId, nativeTokenTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTokenTmp);
    }

    virtual void *executeInNewThread(
            ZF_IN const ZFListener &runnable
            , ZF_IN const ZFListener &runnableCleanup
            ) {
        ZFCoreMutexLock();
        _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = zfnew(_ZFP_ZFThreadImpl_sys_Android_ExecuteData,
            runnable,
            runnableCleanup);
        _ZFP_ZFThreadImpl_sys_Android_updateExecuteId();
        _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType curId = _ZFP_ZFThreadImpl_sys_Android_executeId;
        _ZFP_ZFThreadImpl_sys_Android_executeDataMap[curId] = d;
        ZFCoreMutexUnlock();

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFThread(), "native_executeInNewThread",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_int())
            ).c_str());

        jobject nativeToken = JNIUtilCallStaticObjectMethod(jniEnv
            , ZFImpl_sys_Android_jclassZFThread()
            , jmId
            , curId
            );
        return JNIUtilNewGlobalRef(jniEnv, nativeToken);
    }
    virtual void executeInNewThreadCleanup(ZF_IN void *nativeToken) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFThread(), "native_executeInNewThreadCleanup",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        jobject nativeTokenTmp = (jobject)nativeToken;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFThread(), jmId, nativeTokenTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTokenTmp);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
static _ZFP_ZFThreadImpl_sys_Android_ExecuteData *_ZFP_ZFThreadImpl_sys_Android_getExecuteData(_ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType executeDataId) {
    ZFCoreMutexLocker();
    _ZFP_ZFThreadImpl_sys_Android_ExecuteDataMapType::iterator it = _ZFP_ZFThreadImpl_sys_Android_executeDataMap.find(executeDataId);
    if(it == _ZFP_ZFThreadImpl_sys_Android_executeDataMap.end()) {
        ZFCoreCriticalShouldNotGoHere();
        return zfnull;
    }
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = it->second;
    _ZFP_ZFThreadImpl_sys_Android_executeDataMap.erase(it);
    return d;
}
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFThread
        , void, native_1doExecuteInMainThread
        , jint executeDataId
        , jobject nativeThread
        ) {
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = _ZFP_ZFThreadImpl_sys_Android_getExecuteData(executeDataId);
    d->runnable.execute();
    zfdelete(d);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFThread
        , void, native_1doExecuteInNewThread
        , jint executeDataId
        , _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType nativeThread
        ) {
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = _ZFP_ZFThreadImpl_sys_Android_getExecuteData(executeDataId);
    d->runnable.execute();
    d->runnableCleanup.execute();
    zfdelete(d);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

