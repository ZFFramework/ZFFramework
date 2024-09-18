/**
 * @file ZFCoreMutex.h
 * @brief core mutex
 */

#ifndef _ZFI_ZFCoreMutex_h_
#define _ZFI_ZFCoreMutex_h_

#include "ZFCoreTypeDef_ClassType.h"
#include "ZFCoreTypeDef_CoreType.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief mutex impl */
typedef void (*ZFCoreMutexImplCallbackLock)(ZF_IN void *token);
/** @brief mutex impl */
typedef void (*ZFCoreMutexImplCallbackUnlock)(ZF_IN void *token);

extern ZFLIB_ZFCore void *_ZFP_ZFCoreMutexImplObject;
extern ZFLIB_ZFCore ZFCoreMutexImplCallbackLock _ZFP_ZFCoreMutexImplLock;
extern ZFLIB_ZFCore ZFCoreMutexImplCallbackUnlock _ZFP_ZFCoreMutexImplUnlock;

/**
 * @brief #ZFCoreMutexLock's implementation, change with caution
 */
inline void ZFCoreMutexImplSet(
        ZF_IN void *implObject
        , ZF_IN ZFCoreMutexImplCallbackLock implLock
        , ZF_IN ZFCoreMutexImplCallbackUnlock implUnlock
        ) {
    _ZFP_ZFCoreMutexImplObject = implObject;
    _ZFP_ZFCoreMutexImplLock = implLock;
    _ZFP_ZFCoreMutexImplUnlock = implUnlock;
}

/** @brief see #ZFCoreMutexImplSet */
inline void *ZFCoreMutexImplGetObject(void) {return _ZFP_ZFCoreMutexImplObject;}
/** @brief see #ZFCoreMutexImplSet */
inline ZFCoreMutexImplCallbackLock ZFCoreMutexImplGetLock(void) {return _ZFP_ZFCoreMutexImplLock;}
/** @brief see #ZFCoreMutexImplSet */
inline ZFCoreMutexImplCallbackUnlock ZFCoreMutexImplGetUnlock(void) {return _ZFP_ZFCoreMutexImplUnlock;}

/** @brief see #ZFCoreMutexImplSet */
inline zfbool ZFCoreMutexImplAvailable(void) {return (_ZFP_ZFCoreMutexImplObject != zfnull);}

// ============================================================
/**
 * @brief internal use only
 *
 * you may change it at run time by changing #ZFCoreMutexImplSet
 */
#define ZFCoreMutexLock() \
    do { \
        if(_ZFP_ZFCoreMutexImplObject) { \
            _ZFP_ZFCoreMutexImplLock(_ZFP_ZFCoreMutexImplObject); \
        } \
    } while(zffalse)
/** @brief see #ZFCoreMutexLock */
#define ZFCoreMutexUnlock() \
    do { \
        if(_ZFP_ZFCoreMutexImplObject) { \
            _ZFP_ZFCoreMutexImplUnlock(_ZFP_ZFCoreMutexImplObject); \
        } \
    } while(zffalse)

/**
 * @brief see #ZFCoreMutexLocker
 */
zfclassLikePOD ZFLIB_ZFCore ZFCoreMutexLockerHolder {
public:
    /** @cond ZFPrivateDoc */
    ZFCoreMutexLockerHolder(void) {
        ZFCoreMutexLock();
    }
    ~ZFCoreMutexLockerHolder(void) {
        ZFCoreMutexUnlock();
    }
    /** @endcond */
};
/**
 * @brief util method to lock current block
 *
 * usage:
 * @code
 *   {
 *       ZFCoreMutexLocker(); // lock
 *       if(...) return; // safe to return
 *   } // unlock after block
 * @endcode
 *
 * you may also use the helper #ZFCoreMutexLockerHolder
 * @code
 *   Type var = (ZFCoreMutexLockerHolder(), yourFuncSynced());
 * @endcode
 */
#define ZFCoreMutexLocker() ZFCoreMutexLockerHolder _ZFP_ZFCoreMutexLocker_hold

#if 0
    #undef ZFCoreMutexLock
    #define ZFCoreMutexLock()
    #undef ZFCoreMutexUnlock
    #define ZFCoreMutexUnlock()
    #undef ZFCoreMutexLocker
    #define ZFCoreMutexLocker()
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreMutex_h_

