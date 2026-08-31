/**
 * @file ZFUIViewPositionMap.h
 * @brief map view's position to screen coordinate
 */

#ifndef _ZFI_ZFUIViewPositionMap_h_
#define _ZFI_ZFUIViewPositionMap_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief map view's position to screen coordinate
 *
 * the localPos is relative to view's left top origin,
 * without applying view's scale/translate/rotate
 *
 * @note (ZFTAG_LIMITATION) result value would be invalid until whole layout step finished,
 *   due to impl's limitation,
 *   we are unable to be notified which time the layout step would finish,
 *   so the best solution to check valid position is using delay,
 *   use #zfpost or #ZFTimerOnce is recommended
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIViewPositionMap
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIPoint &, localPos, ZFUIPointZero())
        )

/**
 * @brief util to #ZFUIViewPositionMap to calculate view's position on screen
 *
 * note the view must not have scale or rotation, or the result may be invalid
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, ZFUIRect, ZFUIViewPositionOnScreen
        , ZFMP_IN(ZFUIView *, view)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewPositionMap_h_

