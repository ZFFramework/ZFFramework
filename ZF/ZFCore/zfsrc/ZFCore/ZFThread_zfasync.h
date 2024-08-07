/**
 * @file ZFThread_zfasync.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_zfasync_h_
#define _ZFI_ZFThread_zfasync_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to run code asynchronizely
 *
 * callback would run in new thread,
 * finishCallback would run in #ZFThread::mainThread\n
 * \n
 * for the callback:
 * -  #ZFArgs::param0 is a #ZFObject holds running task id,
 *   it's set to null if canceled by #zfasyncCancel,
 *   you may check it during thread running
 * -  #ZFArgs::result can be set to store the callback's result,
 *   which would passed to finishCallback as #ZFArgs::param0
 *
 * for the finishCallback:
 * -  #ZFArgs::result is the result object passed from callback
 *
 * when #zfasyncCancel called after this method,
 * the callback may or may not be canceled,
 * but finishCallback would be canceled
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfauto, zfasync
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )

/**
 * @brief try to cancel the task or finishCallback started by #zfasync
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, zfasyncCancel
        , ZFMP_IN(const zfauto &, taskId)
        )

// ============================================================
zfclassFwd _ZFP_ZFThreadPoolPrivate;
/**
 * @brief thread pool util, internal impl of #zfasync
 */
zfclass ZFLIB_ZFCore ZFThreadPool : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFThreadPool, ZFObject)
    ZFOBJECT_SINGLETON_DECLARE_DETAIL(private, public, ZFThreadPool, instance)

    /** @brief max thread */
    ZFPROPERTY_ASSIGN(zfuint, maxThread, 8)

    /** @brief see #zfasync */
    ZFMETHOD_DECLARE_2(zfauto, start
            , ZFMP_IN(const ZFListener &, callback)
            , ZFMP_IN(const ZFListener &, finishCallback)
            )
    /** @brief see #zfasyncCancel */
    ZFMETHOD_DECLARE_1(void, stop
            , ZFMP_IN(const zfauto &, taskId)
            )

    /**
     * @brief cancel all task, called automatically before dealloc
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFThreadPoolPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfasync_h_

