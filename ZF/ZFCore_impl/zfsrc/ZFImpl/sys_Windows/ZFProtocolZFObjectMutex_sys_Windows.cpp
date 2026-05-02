#include "ZFImpl_sys_Windows_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

#if ZF_ENV_sys_Windows
#include <Windows.h>

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_Windows {
public:
    static void *implInit(void) {
        CRITICAL_SECTION *mutex = (CRITICAL_SECTION *)zfmalloc(sizeof(CRITICAL_SECTION));
        InitializeCriticalSection(mutex);
        return mutex;
    }
    static void implDealloc(ZF_IN void *implObject) {
        zffree(implObject);
    }
    static void implLock(ZF_IN void *implObject) {
        CRITICAL_SECTION *mutex = (CRITICAL_SECTION *)implObject;
        EnterCriticalSection(mutex);
    }
    static void implUnlock(ZF_IN void *implObject) {
        CRITICAL_SECTION *mutex = (CRITICAL_SECTION *)implObject;
        LeaveCriticalSection(mutex);
    }
    static zfbool implTryLock(ZF_IN void *implObject) {
        CRITICAL_SECTION *mutex = (CRITICAL_SECTION *)implObject;
        return (zfbool)TryEnterCriticalSection(mutex);
    }
};
ZFOBJECT_MUTEX_IMPL_DEFINE(ZFObjectMutexImpl_sys_Windows, v_ZFProtocolLevel::e_SystemLow, {
        ZFObjectMutexImplSet(
                _ZFP_ZFObjectMutexImpl_sys_Windows::implInit,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implDealloc,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implLock,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implUnlock,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implTryLock
            );
    })

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Windows

