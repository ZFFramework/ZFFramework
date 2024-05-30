#include "zfsynchronize.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE
#define _ZFP_zfsynchronizeLog(callerInfo, fmt, ...) \
    printf("%s", zfstr("%s %s\n", callerInfo, zfstr(fmt, ##__VA_ARGS__)).cString())

_ZFP_zfsynchronizeContainerWithLog::_ZFP_zfsynchronizeContainerWithLog(
        ZF_IN zfany const &obj
        , ZF_IN const ZFCallerInfo &callerInfo
        )
: m_obj(obj)
, callerInfo(callerInfo)
{
    if(obj == zfnull) {
        zfCoreCriticalErrorPrepare();
        _ZFP_zfsynchronizeLog(callerInfo,
            "try synchronize a null object");
        zfCoreCriticalError();
        return;
    }
    _ZFP_zfsynchronizeLog(callerInfo,
        "lock object %s",
        m_obj->objectInfoOfInstance());
    m_obj->_ZFP_ZFObjectLock();
    _ZFP_zfsynchronizeLog(callerInfo,
        "lock object %s success",
        m_obj->objectInfoOfInstance());
}
_ZFP_zfsynchronizeContainerWithLog::~_ZFP_zfsynchronizeContainerWithLog(void) {
    _ZFP_zfsynchronizeLog(callerInfo,
        "unlock object %s",
        m_obj->objectInfoOfInstance());
    m_obj->_ZFP_ZFObjectUnlock();
    _ZFP_zfsynchronizeLog(callerInfo,
        "unlock object %s success",
        m_obj->objectInfoOfInstance());
}

// ============================================================
void _ZFP_zfsynchronizeLockWithLog(
        ZF_IN zfany const &obj
        , ZF_IN const ZFCallerInfo &callerInfo
        ) {
    if(obj == zfnull) {
        zfCoreCriticalErrorPrepare();
        _ZFP_zfsynchronizeLog(callerInfo,
            "try lock a null object");
        zfCoreCriticalError();
        return;
    }

    _ZFP_zfsynchronizeLog(callerInfo,
        "lock object %s",
        obj->objectInfoOfInstance());
    obj->_ZFP_ZFObjectLock();
    _ZFP_zfsynchronizeLog(callerInfo,
        "lock object %s success",
        callerInfo,
        obj->objectInfoOfInstance());
}
void _ZFP_zfsynchronizeUnlockWithLog(
        ZF_IN zfany const &obj
        , ZF_IN const ZFCallerInfo &callerInfo
        ) {
    if(obj == zfnull) {
        zfCoreCriticalErrorPrepare();
        _ZFP_zfsynchronizeLog(callerInfo,
            "try unlock a null object");
        zfCoreCriticalError();
        return;
    }

    _ZFP_zfsynchronizeLog(callerInfo,
        "unlock object %s",
        obj->objectInfoOfInstance());
    obj->_ZFP_ZFObjectUnlock();
    _ZFP_zfsynchronizeLog(callerInfo,
        "unlock object %s success",
        obj->objectInfoOfInstance());
}
#endif // #if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfsynchronizeLock
        , ZFMP_IN(zfany const &, obj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfsynchronizeUnlock
        , ZFMP_IN(zfany const &, obj)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

