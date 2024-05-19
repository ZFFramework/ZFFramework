#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"

#if ZF_ENV_sys_Qt

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <QMutex>
#else
#include <QRecursiveMutex>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_Qt {
public:
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    typedef QMutex Mutex;
#else
    typedef QRecursiveMutex Mutex;
#endif
public:
    static void *implInit(void) {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
        return zfnew(QMutex, QMutex::Recursive);
#else
        return zfnew(QRecursiveMutex);
#endif
    }
    static void implDealloc(ZF_IN void *implObject) {
        Mutex *mutex = (Mutex *)implObject;
        zfdelete(mutex);
    }
    static void implLock(ZF_IN void *implObject) {
        Mutex *mutex = (Mutex *)implObject;
        mutex->lock();
    }
    static void implUnlock(ZF_IN void *implObject) {
        Mutex *mutex = (Mutex *)implObject;
        mutex->unlock();
    }
    static zfbool implTryLock(ZF_IN void *implObject) {
        Mutex *mutex = (Mutex *)implObject;
        return mutex->tryLock();
    }
};
ZFOBJECT_MUTEX_IMPL_DEFINE(ZFObjectMutexImpl_sys_Qt, ZFProtocolLevel::e_SystemHigh, {
        ZFObjectMutexImplSet(
                _ZFP_ZFObjectMutexImpl_sys_Qt::implInit,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implDealloc,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implLock,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implUnlock,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implTryLock
            );
    })

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt

