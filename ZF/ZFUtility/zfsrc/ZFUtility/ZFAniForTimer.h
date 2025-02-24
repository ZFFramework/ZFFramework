/**
 * @file ZFAniForTimer.h
 * @brief abstract animation based on timer
 */

#ifndef _ZFI_ZFAniForTimer_h_
#define _ZFI_ZFAniForTimer_h_

#include "ZFAnimation.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFAniForTimerPrivate;
/**
 * @brief animation based on timer
 *
 * this is a dummy animation holder which do nothing by default,
 * you should either:
 * -  supply subclass and implement actual animation by overriding
 *   #aniTimerOnUpdate
 * -  attach observer to #E_AniTimerOnUpdate
 *
 * serializable data:
 * @code
 *   <ZFAniForTimer />
 * @endcode
 */
zfclass ZFLIB_ZFUtility ZFAniForTimer : zfextend ZFAnimation {
    ZFOBJECT_DECLARE(ZFAniForTimer, ZFAnimation)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a float #v_zffloat containing current timer progress
     */
    ZFEVENT(AniTimerOnUpdate)

    // ============================================================
    // object
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    // ============================================================
    // property
public:
    /**
     * @brief curve for timer, null to use linear curve, null by default
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFCurve>, curve)
    /**
     * @brief interval to update timer, in miliseconds, 0 by default
     *
     * -  when set to 0, we would use special logic to achieve global timer control:
     *   -# calculate frame count by #durationFixed/#ZFGlobalTimerIntervalDefault
     *   -# step each frame by #ZFGlobalTimerInterval, until reach the frame count
     *
     *   for example, if you increace #ZFGlobalTimerInterval,
     *   the animation would looks slower\n
     *   this is useful to achieve accurate timer control
     * -  when set to -1, it's always treated as #ZFGlobalTimerIntervalDefault when used
     * -  for other positive value, we use it as specified timer interval
     */
    ZFPROPERTY_ASSIGN(zftimet, aniInterval, 0)

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);

protected:
    /**
     * @brief called to do the actual update
     *
     * note, progress is based on #curve,
     * which typically has value in range [0, 1] as base value,
     * but may exceeds the range for bounce curve
     */
    virtual void aniTimerOnUpdate(ZF_IN zffloat progress);

private:
    _ZFP_ZFAniForTimerPrivate *d;
    friend zfclassFwd _ZFP_ZFAniForTimerPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAniForTimer_h_

