/**
 * @file ZFAutoReleasePool.h
 * @brief automatically release all object in pool upon pool's drain or delete
 */

#ifndef _ZFI_ZFAutoReleasePool_h_
#define _ZFI_ZFAutoReleasePool_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief auto release pool similar to Object-C's NSAutoreleasePool
 *
 * automatically release all object in pool upon pool's drain or delete
 * @see zfobjRelease
 */
zfclass ZFLIB_ZFCore ZFAutoReleasePool : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFAutoReleasePool, ZFObject)

    /**
     * @brief global memory pool,
     *   you should drain it manually and
     *   you take full responsibility to keep it safe to drain
     *
     * note, this singleton instance is ensured with level #ZFLevelZFFrameworkHigh,
     * and ensured to be drained during #ZFLevelZFFrameworkNormal
     */
    ZFOBJECT_SINGLETON_DECLARE(ZFAutoReleasePool, instance)

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief add an object which would be auto released upon pool's drain or delete
     */
    void poolAdd(ZF_IN ZFObject *obj) {
        if(obj != zfnull) {
            ZFCoreMutexLocker();
            ZFCoreAssertWithMessage(obj != this, "add autorelease pool to itself isn't allowed");
            d.add(obj);
        }
    }
    /**
     * @brief manually drain a pool, all object in the pool would be released immediately
     * @note you must make sure all object in pool are safe to release
     *   when draining the pool
     */
    void poolDrain() {
        if(!d.isEmpty()) {
            ZFCoreMutexLock();
            ZFCoreArray<ZFObject *> tmp;
            tmp.swap(d);
            ZFCoreMutexUnlock();

            for(zfindex i = 0; i < tmp.count(); ++i) {
                zfobjRelease(tmp[i]);
            }
        }
    }
    /**
     * @brief manually drain a pool, all object in the pool would be released immediately
     * @note you must make sure all object in pool are safe to release
     *   when draining the pool
     */
    void poolDrain(ZF_IN zfindex count) {
        if(d.count() > count) {
            ZFCoreMutexLock();
            ZFCoreArray<ZFObject *> tmp;
            tmp.addFrom(d.arrayBuf(), d.count() - count);
            d.remove(0, d.count() - count);
            ZFCoreMutexUnlock();

            for(zfindex i = 0; i < tmp.count(); ++i) {
                zfobjRelease(tmp[i]);
            }
        }
    }
    /**
     * @brief current object count
     */
    zfindex poolCount(void) {
        return d.count();
    }

private:
    ZFCoreArray<ZFObject *> d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAutoReleasePool_h_

