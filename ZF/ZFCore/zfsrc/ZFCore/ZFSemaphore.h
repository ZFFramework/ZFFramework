/**
 * @file ZFSemaphore.h
 * @brief semaphore utility
 */

#ifndef _ZFI_ZFSemaphore_h_
#define _ZFI_ZFSemaphore_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFSemaphorePrivate;
/**
 * @brief semaphore utility
 */
zfclass ZFLIB_ZFCore ZFSemaphore : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFSemaphore, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /** @brief for internal use only */
    ZFMETHOD_DECLARE_0(void *, nativeSemaphore)

public:
    /** @brief lock */
    ZFMETHOD_DECLARE_0(void, semaphoreLock)
    /** @brief unlock */
    ZFMETHOD_DECLARE_0(void, semaphoreUnlock)

public:
    /**
     * @brief notify one waiter
     *
     * which waiter is notified, is decided by system\n
     * if there's no waiter, do nothing (this signal is ignored),
     * and new waiter need to wait new signal
     * @note must wrapped by #semaphoreLock/#semaphoreUnlock
     */
    ZFMETHOD_DECLARE_0(void, semaphoreSignal)

    /**
     * @brief notify all waiters
     * @note must wrapped by #semaphoreLock/#semaphoreUnlock
     */
    ZFMETHOD_DECLARE_0(void, semaphoreBroadcast)

    /**
     * @brief wait until signal
     * @note must wrapped by #semaphoreLock/#semaphoreUnlock
     */
    ZFMETHOD_DECLARE_0(void, semaphoreWait)

    /**
     * @brief wait until signal or timeout
     * @note must wrapped by #semaphoreLock/#semaphoreUnlock
     */
    ZFMETHOD_DECLARE_1(zfbool, semaphoreWait
            , ZFMP_IN(zftimet, miliSecs)
            )

public:
    /** @brief util to #semaphoreLock/#semaphoreSignal/#semaphoreUnlock */
    ZFMETHOD_DECLARE_0(void, lockAndSignal)
    /** @brief util to #semaphoreLock/#semaphoreBroadcast/#semaphoreUnlock */
    ZFMETHOD_DECLARE_0(void, lockAndBroadcast)
    /** @brief util to #semaphoreLock/#semaphoreWait/#semaphoreUnlock */
    ZFMETHOD_DECLARE_0(void, lockAndWait)
    /** @brief util to #semaphoreLock/#semaphoreWait/#semaphoreUnlock */
    ZFMETHOD_DECLARE_1(zfbool, lockAndWait
            , ZFMP_IN(zftimet, miliSecs)
            )

private:
    _ZFP_ZFSemaphorePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSemaphore_h_

