/**
 * @file ZFProtocolZFTimer.h
 * @brief protocol for ZFTimer
 */

#ifndef _ZFI_ZFProtocolZFTimer_h_
#define _ZFI_ZFProtocolZFTimer_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFTimer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFTimer
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFTimer)
public:
    /**
     * @brief create and return a native timer
     */
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) zfpurevirtual;
    /**
     * @brief destroy native timer
     */
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer) zfpurevirtual;
    /**
     * @brief start timer
     */
    virtual void start(
            ZF_IN ZFTimer *timer
            , ZF_IN zfidentity timerImplId
            ) zfpurevirtual;
    /**
     * @brief stop the timer
     */
    virtual void stop(ZF_IN ZFTimer *timer) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
    /**
     * @brief must be called to notify timer event
     */
    zffinal void notifyTimerActivate(
            ZF_IN ZFTimer *timer
            , ZF_IN zfidentity timerImplId
            ) {
        timer->_ZFP_ZFTimer_timerOnActivate(timerImplId);
    }
ZFPROTOCOL_INTERFACE_END(ZFTimer)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFTimer_h_

