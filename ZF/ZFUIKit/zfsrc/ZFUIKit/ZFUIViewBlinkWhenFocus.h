/**
 * @file ZFUIViewBlinkWhenFocus.h
 * @brief auto blink focused view
 */

#ifndef _ZFI_ZFUIViewBlinkWhenFocus_h_
#define _ZFI_ZFUIViewBlinkWhenFocus_h_

#include "ZFUIView.h"
#include "ZFUIImageIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief filter to exclude certain view to be auto blinked, empty by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIKit, ZFFilterForZFObject, ZFUIViewBlinkWhenFocusFilter)

// ============================================================
/**
 * @brief whether auto start #ZFUIViewBlinkWhenFocusAutoApplyStart, true by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIKit, zfbool, ZFUIViewBlinkWhenFocusAutoApply)
/**
 * @brief default mask image for #ZFUIViewBlinkWhenFocusAutoApplyStart, an alpha white image by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIKit, zfauto, ZFUIViewBlinkWhenFocusMaskImage)

/**
 * @brief util method to automatically blink focused view
 *
 * by default:
 * -  #ZFUIViewBlinkWhenFocusAutoApplyStart would be called automatically during #ZFFrameworkInit
 *   as level #ZFLevelAppLow if #ZFUIViewBlinkWhenFocusAutoApply
 * -  #ZFUIViewBlinkWhenFocusAutoApplyStop would be called automatically during #ZFFrameworkCleanup
 *   as level #ZFLevelZFFrameworkLow
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void, ZFUIViewBlinkWhenFocusAutoApplyStart
        , ZFMP_IN_OPT(ZFUIImage *, img, zfnull)
        )
/**
 * @brief see #ZFUIViewBlinkWhenFocusAutoApplyStart
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, void, ZFUIViewBlinkWhenFocusAutoApplyStop)
/**
 * @brief see #ZFUIViewBlinkWhenFocusAutoApplyStart
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, zfbool, ZFUIViewBlinkWhenFocusAutoApplyStarted)

/**
 * @brief temporary pause the auto blink
 *
 * can be called more than one time, but must be paired with #ZFUIViewBlinkWhenFocusAutoApplyResume
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, void, ZFUIViewBlinkWhenFocusAutoApplyPause)
/** @brief see #ZFUIViewBlinkWhenFocusAutoApplyPause */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, void, ZFUIViewBlinkWhenFocusAutoApplyResume)
/** @brief see #ZFUIViewBlinkWhenFocusAutoApplyPause */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, zfindex, ZFUIViewBlinkWhenFocusAutoApplyPaused)

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyStart
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkWhenFocusAutoApplyStart)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyStop
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkWhenFocusAutoApplyStop)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyPause
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkWhenFocusAutoApplyPause)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyResume
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkWhenFocusAutoApplyResume)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view is blinked
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkWhenFocusViewBlinkOn)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view is blinked
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkWhenFocusViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
// ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
/**
 * @brief pause #ZFUIViewBlinkWhenFocusAutoApplyPause for a specified time,
 *   and resume it automatically after time out
 *
 * the task would be scheduled and run in future even if time is 0
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void, ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
        , ZFMP_IN_OPT(zftimet, time, 0)
        )
/**
 * @brief cancel #ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
 *
 * typically you should not call this method manually,
 * which would cancel all the task that started by #ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, void, ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewBlinkWhenFocus_h_

