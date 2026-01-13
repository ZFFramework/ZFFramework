/**
 * @file ZFThreadPool.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThreadPool_h_
#define _ZFI_ZFThreadPool_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFThreadPoolPrivate;
/**
 * @brief thread pool util, internal impl of #zfasync,
 *   recommended to use #zfasync for short
 */
zfclass ZFLIB_ZFCore ZFThreadPool : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFThreadPool, ZFObject)
    ZFOBJECT_SINGLETON_DECLARE_DETAIL(private, public, ZFThreadPool, instance)

    /** @brief max thread */
    ZFPROPERTY_ASSIGN(zfuint, maxThread, 5)

    /** @brief see #zfasync */
    ZFMETHOD_DECLARE_2(zfautoT<ZFTaskId>, start
            , ZFMP_IN(const ZFListener &, callback)
            , ZFMP_IN(const ZFListener &, finishCallback)
            )

    /**
     * @brief cancel all task, called automatically before dealloc
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

protected:
    /** @brief auto update #ZFThread::threadName */
    virtual zfstring threadPoolName(void) {
        return zfstr("%s:%s", this->classData()->className(), (const void *)this);
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFThreadPoolPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThreadPool_h_

