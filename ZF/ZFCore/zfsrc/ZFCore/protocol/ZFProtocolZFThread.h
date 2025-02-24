/**
 * @file ZFProtocolZFThread.h
 * @brief protocol for ZFThread
 */

#ifndef _ZFI_ZFProtocolZFThread_h_
#define _ZFI_ZFProtocolZFThread_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFThread
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFThread)
public:
    /**
     * @brief see #ZFThread::nativeThreadUnregister
     *
     * you should not retain or release ownerZFThread
     */
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread) zfpurevirtual;
    /**
     * @brief see #ZFThread::nativeThreadUnregister, #nativeThreadRegister
     *
     * you should not retain or release ownerZFThread
     */
    virtual void nativeThreadUnregister(ZF_IN void *token) zfpurevirtual;
    /**
     * @brief get thread from token created by nativeThreadRegister
     */
    virtual ZFThread *threadForToken(ZF_IN void *token) zfpurevirtual;

    /**
     * @brief see #ZFThread::mainThread
     */
    virtual ZFThread *mainThread(void) zfpurevirtual;

    /**
     * @brief see #ZFThread::currentThread
     */
    virtual ZFThread *currentThread(void) zfpurevirtual;

    /**
     * @brief see #ZFThread::sleep
     */
    virtual void *sleepTokenCreate(void);
    /**
     * @brief see #ZFThread::sleep
     */
    virtual void sleepTokenDestroy(ZF_IN void *sleepToken);
    /**
     * @brief see #ZFThread::sleep
     */
    virtual zfbool sleep(
            ZF_IN void *sleepToken
            , ZF_IN zftimet miliSecs
            );
    /**
     * @brief see #ZFThread::sleep
     */
    virtual void sleepCancel(ZF_IN void *sleepToken);

    /**
     * @brief whether #executeInMainThread available
     */
    virtual zfbool executeInMainThreadAvailable(void) {
        return zftrue;
    }
    /**
     * @brief run the runnable in main thread
     *
     * you should queue it to run even if current thread is main thread
     */
    virtual void *executeInMainThread(ZF_IN const ZFListener &runnable) zfpurevirtual;
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInMainThreadCleanup(ZF_IN void *nativeToken) zfpurevirtual;

    /**
     * @brief run the runnable in new thread
     *
     * you must:
     * -# call the runnable in new thread
     *   (can be canceled by #executeInNewThreadCleanup,
     *   so the runnable may not run at this case)
     * -# cleanup native state if necessary
     * -# call the runnableCleanup, even if runnable is canceled
     *
     * NOTE: the created thread may be blocked by semaphore to achieve app level thread pool logic
     */
    virtual void *executeInNewThread(
            ZF_IN const ZFListener &runnable
            , ZF_IN const ZFListener &runnableCleanup
            ) zfpurevirtual;
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInNewThreadCleanup(ZF_IN void *nativeToken) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFThread)

// ============================================================
/** @brief see #ZFThread */
zfclass ZFLIB_ZFCore ZFThreadMainThread : zfextend ZFThread {
    ZFOBJECT_DECLARE(ZFThreadMainThread, ZFThread)

public:
    zfoverride
    virtual void threadStart(
            ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            ) {
        ZFCoreLogTrim("you must not start main thread");
    }
    zfoverride
    virtual zfbool threadStarted(void) {
        return zftrue;
    }
    zfoverride
    virtual zfbool threadRunning(void) {
        return zftrue;
    }
    zfoverride
    virtual void threadStop(void) {
        ZFCoreLogTrim("you must not stop main thread");
    }
    zfoverride
    virtual void threadWait(void) {
        ZFCoreLogTrim("you must not wait main thread");
    }
    zfoverride
    virtual zfbool threadWait(ZF_IN zftimet miliSecs) {
        ZFCoreLogTrim("you must not wait main thread");
        return zffalse;
    }
    zfoverride
    virtual zfbool isMainThread(void) {
        return zftrue;
    }
};

// ============================================================
/** @brief see #ZFMainThreadTaskImplSet */
typedef void *(*ZFMainThreadTaskImplCallbackExecute)(ZF_IN const ZFListener &runnable);
/** @brief see #ZFMainThreadTaskImplSet */
typedef void (*ZFMainThreadTaskImplCallbackCleanup)(ZF_IN void *nativeToken);

/**
 * @brief set custom impl for #ZFThread::taskQueueAdd for main thread
 *
 * main thread is a special case for thread task management,
 * in ZFFramework, there are mainly two way of main thread task management:
 * -  most platforms (Android, iOS, etc) has builtin event loop logic which can not be touched by ZFFramework,
 *   but supplies task post API,
 *   ZFFramework would use the platform specified API to achieve main thread task queue logic
 * -  some platforms (SDL for example) let app to supply event loop,
 *   which requires app to manually implement the main thread task queue logic,
 *   at this case, ZFFramework allow impl to specify custom impl to resolve main thread task,
 *   by using this method, app can embed default thread impl to the customized main event loop
 *
 * whether the custom impl would be used, depends on the ZFThread protocol impl
 */
extern ZFLIB_ZFCore void ZFMainThreadTaskImplSet(
        ZF_IN ZFMainThreadTaskImplCallbackExecute executeImpl
        , ZF_IN_OPT ZFMainThreadTaskImplCallbackCleanup cleanupImpl = zfnull
        );
/** @brief see #ZFMainThreadTaskImplSet */
extern ZFLIB_ZFCore ZFMainThreadTaskImplCallbackExecute ZFMainThreadTaskImplGetExecute(void);
/** @brief see #ZFMainThreadTaskImplSet */
extern ZFLIB_ZFCore ZFMainThreadTaskImplCallbackCleanup ZFMainThreadTaskImplGetCleanup(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFThread_h_

