/**
 * @file ZFGlobalTimer.h
 * @brief timer utility
 */

#ifndef _ZFI_ZFGlobalTimer_h_
#define _ZFI_ZFGlobalTimer_h_

#include "ZFTimer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief global timer util
 *
 * you can attach any callbacks to same global timer instance,
 * the global timer would be running if more than one timer callback attached,
 * and would be stopped when all of them detached\n
 * you can change timer interval by #ZFGlobalTimerInterval even when it's running\n
 * \n
 * a typical advanced usage for global timer:
 * -# change time based duration to frame based timer by (duration / ZFGlobalTimerIntervalDefault())
 * -# step each frame by attaching your update logic to global timer
 * -# change ZFGlobalTimerInterval to a greater value,
 *   and your logic would be "slowed down"
 *
 * you may also use #ZFGlobalTimerManualStep to manually control the timer steps\n
 * note global timer is ensured run in main thread
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFGlobalTimerAttach
        , ZFMP_IN(const ZFListener &, timerCallback)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFGlobalTimerAttachOnce
        , ZFMP_IN(const ZFListener &, timerCallback)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFGlobalTimerDetach
        , ZFMP_IN(const ZFListener &, timerCallback)
        )
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, void, ZFGlobalTimerDetachAll)

// ============================================================
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zftimet const &, ZFGlobalTimerIntervalDefault)
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFGlobalTimerIntervalDefault
        , ZFMP_IN(zftimet const &, intervalDefault)
        )
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zftimet const &, ZFGlobalTimerInterval)
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFGlobalTimerInterval
        , ZFMP_IN(zftimet const &, interval)
        )

// ============================================================
/**
 * @brief manual control global timer step
 *
 * once called, all task attached to #ZFGlobalTimerAttach
 * would not be notified unless called this method again,
 * you may cancel the manual step and restore to original timer logic
 * by #ZFGlobalTimerManualStep
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, void, ZFGlobalTimerManualStep)
/** @brief see #ZFGlobalTimerManualStep */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, void, ZFGlobalTimerManualStepCancel)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFGlobalTimer_h_

