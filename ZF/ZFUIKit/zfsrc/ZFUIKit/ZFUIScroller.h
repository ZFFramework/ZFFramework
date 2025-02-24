/**
 * @file ZFUIScroller.h
 * @brief scroller for #ZFUIScrollView
 */

#ifndef _ZFI_ZFUIScroller_h_
#define _ZFI_ZFUIScroller_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIScroller
/**
 * @brief scroller used to achieve scroll logic,
 *   typically #ZFUIScrollerDefault would suit most cases
 *
 * to use a scroller, you must:
 * -  setup scroll animation callback: #scrollAniStartCallback, #scrollAniStopCallback,
 *   which would notify scroll animation events at proper time (#scrollAniOnUpdate)
 * -  change settings by proper method
 * -  notify drag events by #scrollOnDragBegin, #scrollOnDrag, #scrollOnDragEnd
 *
 * \n
 * to implement a scroller, you must:
 * -  implement all necessary methods
 * -  invoke scroll animation at proper time, by #scrollAniStartCallback and #scrollAniStopCallback
 */
zfinterface ZFLIB_ZFUIKit ZFUIScroller : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFUIScroller, ZFInterface)

public:
    /**
     * @brief used to start scroll animation's timer
     *
     * impl must return a #v_zftimet (stored in #ZFArgs::result) to indicates current timestamp
     */
    ZFListener scrollAniStartCallback;
    /**
     * @brief used to stop scroll animation's timer
     */
    ZFListener scrollAniStopCallback;

public:
    /**
     * @brief called when owner scroll view's size changed,
     *   you should fix content frame to match bounds if necessary
     */
    virtual void scrollOwnerSizeOnUpdate(ZF_IN zffloat ownerSize) zfpurevirtual;
    /**
     * @brief whether use bounce
     */
    virtual void scrollBounceOnUpdate(
            ZF_IN zfbool scrollBounce
            , ZF_IN zfbool scrollBounceAlways
            ) zfpurevirtual;

    /**
     * @brief whether align to page
     */
    virtual void scrollAlignToPageOnUpdate(ZF_IN zfbool scrollAlignToPage) zfpurevirtual;

    /**
     * @brief used to update scroll content,
     *   you should keep or update previous scroll animation if necessary
     */
    virtual void scrollContentOnUpdate(
            ZF_IN zffloat contentOffset
            , ZF_IN zffloat contentSize
            ) zfpurevirtual;

public:
    /**
     * @brief cancel over scroll and ensure content offset in range
     */
    virtual void scrollToFitRange(void) zfpurevirtual;
    /**
     * @brief util method to stop scroll animation
     */
    virtual void scrollStop(void) {
        this->scrollWithoutAnimation(this->scrollContentOffset());
    }
    /**
     * @brief scroll without scroll animation,
     *   you should stop old scroll animation if necessary
     */
    virtual void scrollWithoutAnimation(ZF_IN zffloat contentOffset) zfpurevirtual;
    /**
     * @brief see #ZFUIScrollView::scrollByPoint
     */
    virtual void scrollByPoint(ZF_IN zffloat point) zfpurevirtual;
    /**
     * @brief return end position of #scrollByPoint,
     *   must return current content offset if not scrolling
     */
    virtual zffloat scrollByPointEndPoint(void) zfpurevirtual;
    /**
     * @brief see #ZFUIScrollView::scrollBySpeed
     */
    virtual void scrollBySpeed(ZF_IN zffloat speed) zfpurevirtual;
    /**
     * @brief return current speed of #scrollBySpeed
     */
    virtual zffloat scrollBySpeedCurrentSpeed(void) zfpurevirtual;
    /**
     * @brief return end position of #scrollBySpeed,
     *   must return current content offset if not scrolling
     */
    virtual zffloat scrollBySpeedEndPointPredicted(void) zfpurevirtual;

    /**
     * @brief return end position,
     *   must return current content offset if not scrolling
     */
    virtual zffloat scrollEndPointPredicted(void) zfpurevirtual;

    /**
     * @brief whether this direction's scroller want higher priority,
     *   typically true if currently under progress to scroll to align to page
     */
    virtual zfbool scrollRequireFocus(void) zfpurevirtual;

public:
    /**
     * @brief used to get current owner size
     */
    virtual zffloat scrollOwnerSize(void) zfpurevirtual;
    /**
     * @brief used to get current content offset
     */
    virtual zffloat scrollContentOffset(void) zfpurevirtual;
    /**
     * @brief used to get current content size
     */
    virtual zffloat scrollContentSize(void) zfpurevirtual;

public:
    /**
     * @brief drag begin
     *
     * note drag events is not ensured paired
     */
    virtual void scrollOnDragBegin(
            ZF_IN zffloat mousePos
            , ZF_IN zftimet mouseTime
            ) zfpurevirtual;
    /**
     * @brief drag
     *
     * note drag events is not ensured paired
     */
    virtual void scrollOnDrag(
            ZF_IN zffloat mousePos
            , ZF_IN zftimet mouseTime
            ) zfpurevirtual;
    /**
     * @brief drag end
     *
     * note drag events is not ensured paired\n
     * \n
     * if scroll animation is needed, use #scrollAniStartCallback to start
     */
    virtual void scrollOnDragEnd(
            ZF_IN zftimet mouseTime
            , ZF_IN zfbool needScrollAni
            ) zfpurevirtual;

public:
    /**
     * @brief called to update scroll animation
     */
    virtual void scrollAniOnUpdate(ZF_IN zftimet time) zfpurevirtual;
};

// ============================================================
/**
 * @brief change default scroller
 *
 * use null to use default scroller
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void, ZFUIScrollerClass
        , ZFMP_IN(const ZFClass *, cls)
        )
/**
 * @brief get the class of default scroller
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, const ZFClass *, ZFUIScrollerClass)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScroller_h_

