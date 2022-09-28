/**
 * @file ZFThread.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_h_
#define _ZFI_ZFThread_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFThreadPrivate;
/**
 * @brief thread utility
 *
 * you can use thread by one of these method:
 * -  inherit from ZFThread and override ZFThread::threadOnRun
 * -  declare a callback and set as thread's run loop by #ZFThread::threadRunnable
 *
 * for app level code, it's recommended to use #zfasync since it may contain thread pool logic
 */
zfclass ZF_ENV_EXPORT ZFThread : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFThread, ZFObject)

public:
    // ============================================================
    // observer events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called in the same thread of the thread task
     */
    ZFOBSERVER_EVENT(ThreadOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called in the same thread of the thread task
     */
    ZFOBSERVER_EVENT(ThreadOnStop)

    // ============================================================
    // thread control
public:
    /**
     * @brief register a thread which is not started by ZFThread, assert fail if already registered
     *
     * return a token for #nativeThreadUnregister to unregister\n
     * \n
     * you must call nativeThreadUnregister before the thread ends\n
     * \n
     * you must take good care when calling this method\n
     * main thread has no need to register
     */
    ZFMETHOD_DECLARE_STATIC_0(void *, nativeThreadRegister)
    /**
     * @brief see #nativeThreadRegister
     *
     * it's safe to unregister in different thread using token
     */
    ZFMETHOD_DECLARE_STATIC_1(void, nativeThreadUnregister,
                              ZFMP_IN(void *, token))

public:
    /**
     * @brief return main thread
     */
    ZFMETHOD_DECLARE_STATIC_0(ZFThread *, mainThread)
    /**
     * @brief return current thread,
     *   or zfnull if thread isn't started or registered by ZFThread
     *   or not registered by #ZFThread::nativeThreadRegister
     */
    ZFMETHOD_DECLARE_STATIC_0(ZFThread *, currentThread)

    /**
     * @brief make current thread sleep for miliSecs,
     *   note this method may be not accurate
     */
    ZFMETHOD_DECLARE_STATIC_1(void, sleep,
                              ZFMP_IN(zftimet, miliSecs))

    /**
     * @brief util to run callback in specified thread,
     *   or run in #mainThread if the thread is null
     *   or #taskQueueAvailable is false
     *
     * return the actual thread that added the task,
     * you may use #taskQueueRemove to remove the task
     */
    ZFMETHOD_DECLARE_STATIC_3(ZFThread *, executeInThread,
                              ZFMP_IN(ZFThread *, thread),
                              ZFMP_IN(const ZFListener &, callback),
                              ZFMP_IN_OPT(ZFObject *, userData, zfnull))

    /**
     * @brief util to run callback in current thread's #taskQueueAdd,
     *   or run in #mainThread if current thread's #taskQueueAvailable is false
     *
     * return the actual thread that added the task,
     * you may use #taskQueueRemove to remove the task
     */
    ZFMETHOD_DECLARE_STATIC_2(ZFThread *, post,
                              ZFMP_IN(const ZFListener &, callback),
                              ZFMP_IN_OPT(ZFObject *, userData, zfnull))

    // ============================================================
    // thread instance method
protected:
    /**
     * @brief init from listener
     */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(const ZFListener &, runnable))
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief the main callback to run
     *
     * default is invalid callback, called after the builtin #threadOnRun\n
     * you must make sure thread isn't running when setting new runnable\n
     * this value take effects only if you start thread by #threadStart
     * @see threadStart, threadOnRun, threadWait
     */
    ZFPROPERTY_ASSIGN(ZFListener, threadRunnable)

    /**
     * @brief start thread, do nothing if already started
     */
    ZFMETHOD_DECLARE_1(void, threadStart,
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
    /**
     * @brief return true if start is called and hasn't been stopped or end
     */
    ZFMETHOD_DECLARE_0(zfbool, threadStarted)
    /**
     * @brief return true only if thread is running
     */
    ZFMETHOD_DECLARE_0(zfbool, threadRunning)
    /**
     * @brief request stop the thread, may not be stopped immediately
     *
     * if thread is started but not running, this function will cancel the thread's run\n
     * otherwise, whether the thread is continue running or stopped, is up to the runnable
     * @see threadStopRequested
     */
    ZFMETHOD_DECLARE_0(void, threadStop)
    /**
     * @brief return true if stop is called after start
     */
    ZFMETHOD_DECLARE_0(zfbool, threadStopRequested)

    /**
     * @brief wait until thread finished running
     *
     * return immediately if thread isn't running
     *
     * note, do not wait on thread that has called #taskQueueInit,
     * since it would never end until #taskQueueCleanup
     */
    ZFMETHOD_DECLARE_0(void, threadWait)

    /**
     * @brief wait until thread finished running, or timeout
     *
     * return immediately if thread isn't running
     * @return true if wait success or thread isn't running\n
     *         false if wait timeout or error
     */
    ZFMETHOD_DECLARE_1(zfbool, threadWait,
                       ZFMP_IN(zftimet, miliSecs))

    /**
     * @brief return true if current thread is main thread
     *
     * subclass should not override this method
     */
    ZFMETHOD_DECLARE_0(zfbool, isMainThread)
    /**
     * @brief true if the thread is registered by #nativeThreadRegister
     */
    ZFMETHOD_DECLARE_0(zfbool, isNativeThread)

    /**
     * @brief add object to auto release pool attached to this thread
     *
     * usually this method is called by #zfautoRelease
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolAdd(ZF_IN ZFObject *obj);
    /**
     * @brief manually drain auto release pool
     *
     * must ensure that all objects in pool are safe to release\n
     * called automatically after each time that runnable ends
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolDrain();

public:
    /**
     * @brief prepare a task queue that can be run in this thread
     *
     * normally, thread would be terminated if #threadRunnable finished\n
     * you may use this method to create a task queue loop,
     * and use #taskQueueAdd to run multiple task in this thread in order\n
     * once #taskQueueInit called,
     * the thread would never end even if all task finished,
     * unless #taskQueueCleanup called, this is useful to achieve custom message loop\n
     * \n
     * #mainThread is a special case since it already has builtin message loop,
     * and the #mainThread is always considered #taskQueueInit has been called, the difference:
     * -  #taskQueueInit : do nothing
     * -  #taskQueueCleanup : do nothing
     * -  #taskQueueAdd / #taskQueueRemove : similar behavior, but add/remove task to main thread loop queue
     *
     * once #taskQueueInit called, you may post runnable to be run in the specified thread by #taskQueueAdd
     */
    ZFMETHOD_DECLARE_0(void, taskQueueInit)
    /** @brief see #taskQueueInit */
    ZFMETHOD_DECLARE_0(void, taskQueueCleanup)
    /** @brief see #taskQueueInit */
    ZFMETHOD_DECLARE_0(zfbool, taskQueueAvailable)
    /** @brief see #taskQueueInit */
    ZFMETHOD_DECLARE_0(zfindex, taskQueueCount)
    /** @brief see #taskQueueInit */
    ZFMETHOD_DECLARE_2(void, taskQueueAdd,
                       ZFMP_IN(const ZFListener &, task),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
    /** @brief see #taskQueueRemove */
    ZFMETHOD_DECLARE_2(void, taskQueueRemove,
                       ZFMP_IN(const ZFListener &, task),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))

protected:
    /**
     * @brief internal runnable, do nothing by default, you may override it
     *
     * note: called before #threadRunnable
     */
    ZFMETHOD_DECLARE_PROTECTED_2(void, threadOnRun,
                                 ZFMP_IN(const ZFListenerData &, listenerData),
                                 ZFMP_IN(ZFObject *, userData))

protected:
    /** @brief see #EventThreadOnStart */
    virtual inline void threadOnStart(void)
    {
        this->observerNotify(ZFThread::EventThreadOnStart());
    }
    /** @brief see #EventThreadOnStop */
    virtual inline void threadOnStop(void)
    {
        this->observerNotify(ZFThread::EventThreadOnStop());
    }

private:
    _ZFP_ZFThreadPrivate *d;
    static void _ZFP_ZFThread_threadCallback(ZF_IN const ZFListenerData &, ZF_IN ZFObject *);
    static void _ZFP_ZFThread_mainThreadCallback(ZF_IN const ZFListenerData &, ZF_IN ZFObject *);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_h_

