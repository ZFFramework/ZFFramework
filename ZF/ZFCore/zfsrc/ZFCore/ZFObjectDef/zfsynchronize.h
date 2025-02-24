/**
 * @file zfsynchronize.h
 * @brief type define for synchronize logic in ZFFramework
 */

#ifndef _ZFI_zfsynchronize_h_
#define _ZFI_zfsynchronize_h_

#include "ZFObjectCore.h"
#include "zfany.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// internal test
// #define _ZFP_ZFSYNCHRONIZE_LOG_ENABLE 1

// ============================================================
/**
 * @brief similar to  synchronized in Java
 *
 * usage:
 * @code
 *   ZFObject *obj = ...;
 *   ...
 *   // not mutable
 *   {
 *       zfsynchronize(obj);
 *       // mutable
 *   }
 *   // not mutable
 * @endcode
 */
#if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronize(obj) \
        _ZFP_zfsynchronizeContainerWithLog ZFUniqueName(zfsynchronize_holder)(obj \
            , ZFCallerInfoCreate())
#else
    #define zfsynchronize(obj) \
        _ZFP_zfsynchronizeContainer ZFUniqueName(zfsynchronize_holder)(obj)
#endif

/**
 * @brief similar to #zfsynchronize, while you take the responsibility to close it by #zfsynchronizeUnlock
 *
 * usage:
 * @code
 *   zfsynchronizeLock(obj);
 *   // mutable operation
 *   zfsynchronizeUnlock(obj);
 * @endcode
 * it's not necessary to put them in the same code block,
 * but you must ensure the lock and unlock are paired
 * @see zfsynchronize
 */
#if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronizeLock(obj) _ZFP_zfsynchronizeLockWithLog(obj \
        , ZFCallerInfoCreate())
#else
    #define zfsynchronizeLock(obj) _ZFP_zfsynchronizeLock(obj)
#endif

/**
 * @brief see #zfsynchronizeLock
 */
#if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronizeUnlock(obj) _ZFP_zfsynchronizeUnlockWithLog(obj \
        , ZFCallerInfoCreate())
#else
    #define zfsynchronizeUnlock(obj) _ZFP_zfsynchronizeUnlock(obj)
#endif

// ============================================================
// no log version
zffinal zfclassNotPOD ZFLIB_ZFCore _ZFP_zfsynchronizeContainer {
public:
    _ZFP_zfsynchronizeContainer(ZF_IN zfany const &obj)
    : m_obj(obj)
    {
        m_obj->_ZFP_ZFObjectLock();
    }
    ~_ZFP_zfsynchronizeContainer(void) {
        m_obj->_ZFP_ZFObjectUnlock();
    }
private:
    ZFObject *m_obj;
};

inline void _ZFP_zfsynchronizeLock(ZF_IN zfany const &obj) {
    obj->_ZFP_ZFObjectLock();
}
inline void _ZFP_zfsynchronizeUnlock(ZF_IN zfany const &obj) {
    obj->_ZFP_ZFObjectUnlock();
}

// ============================================================
// log version
#if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE
zffinal zfclassNotPOD ZFLIB_ZFCore _ZFP_zfsynchronizeContainerWithLog {
public:
    _ZFP_zfsynchronizeContainerWithLog(
            ZF_IN zfany const &obj
            , ZF_IN const ZFCallerInfo &callerInfo
            );
    ~_ZFP_zfsynchronizeContainerWithLog(void);
private:
    ZFObject *m_obj;
    ZFCallerInfo callerInfo;
};

extern ZFLIB_ZFCore void _ZFP_zfsynchronizeLockWithLog(
        ZF_IN zfany const &obj
        , ZF_IN const ZFCallerInfo &callerInfo
        );
extern ZFLIB_ZFCore void _ZFP_zfsynchronizeUnlockWithLog(
        ZF_IN zfany const &obj
        , ZF_IN const ZFCallerInfo &callerInfo
        );
#endif // #if _ZFP_ZFSYNCHRONIZE_LOG_ENABLE

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfsynchronize_h_

