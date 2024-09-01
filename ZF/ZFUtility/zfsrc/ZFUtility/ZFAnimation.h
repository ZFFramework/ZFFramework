/**
 * @file ZFAnimation.h
 * @brief base class of all animation
 */

#ifndef _ZFI_ZFAnimation_h_
#define _ZFI_ZFAnimation_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global default animation duration, 250 by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUtility, zftimet, ZFAnimationDurationDefault)

// ============================================================
zfclassFwd _ZFP_ZFAnimationPrivate;
/**
 * @brief base class of all animation
 */
zfclass ZFLIB_ZFUtility ZFAnimation : zfextend ZFStyleableObject {
    ZFOBJECT_DECLARE(ZFAnimation, ZFStyleableObject)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation delay begin
     */
    ZFEVENT(AniOnDelayBegin)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation delay finished,
     * param0 is a #ZFResultType to show the delay result
     */
    ZFEVENT(AniOnDelayEnd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation started
     * @note for delayed animation,
     *   this event would be fired before delay,
     *   use #EventAniOnDelayEnd if necessary
     */
    ZFEVENT(AniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation looped,
     * current loop count can be accessed by #aniLoopCur
     */
    ZFEVENT(AniOnLoop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation stopped or invalid,
     * param0 is a #ZFResultType to show the result
     */
    ZFEVENT(AniOnStop)

    // ============================================================
    // property
public:
    /**
     * @brief animation's delay in miliseconds, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, aniDelay)
    /**
     * @brief animation's duration in miliseconds, 0 to use #ZFAnimationDurationDefault, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, aniDuration)
    /**
     * @brief util method to #aniDuration
     */
    ZFMETHOD_DECLARE_0(zftimet, aniDurationFixed)
    /**
     * @brief whether automatically stop previous animation attached to #aniTarget,
     *   false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, aniAutoStopPrev)

    /**
     * @brief loop count, 0 by default
     *
     * 0 mean no loop, use #zfindexMax for infinite loop
     */
    ZFPROPERTY_ASSIGN(zfindex, aniLoop)

public:
    /**
     * @brief animation's target, assign only,
     *   but would be retain automatically while animation is running
     *
     * this is not necessary to be set during animation's running
     * (although most animation subclass need a target)
     */
    ZFMETHOD_DECLARE_1(void, aniTarget
            , ZFMP_IN(ZFObject *, aniTarget)
            )
    /**
     * @brief animation's target
     */
    ZFMETHOD_DECLARE_0(zfany, aniTarget)

    // ============================================================
    // start stop
public:
    /**
     * @brief start the animation
     * @note an animation would be retained automatically when it's running,
     *   and released after stopped
     */
    ZFMETHOD_DECLARE_0(void, aniStart)
    /**
     * @brief true if the animation is running (or delaying)
     */
    ZFMETHOD_DECLARE_0(zfbool, aniRunning)
    /**
     * @brief true if the animation is delaying
     */
    ZFMETHOD_DECLARE_0(zfbool, aniDelaying)
    /**
     * @brief stop the animation or do nothing if not started
     */
    ZFMETHOD_DECLARE_0(void, aniStop)
    /**
     * @brief true if the animation is stopped by calling #aniStop
     *
     * this state would be kept until next #aniStart called
     */
    ZFMETHOD_DECLARE_0(zfbool, aniStoppedByUser)

public:
    /**
     * @brief get current animation's id
     *
     * animation id would be updated automatically
     * each time animation is started and stopped
     */
    ZFMETHOD_DECLARE_0(zfidentity, aniId)

    /**
     * @brief check whether animation is valid, see #aniImplCheckValid
     */
    ZFMETHOD_DECLARE_0(zfbool, aniValid)

    /**
     * @brief current loop count, 0 for first time
     *
     * reset when #aniStart, see #aniLoop for more info
     */
    ZFMETHOD_DECLARE_0(zfindex, aniLoopCur)

public:
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, aniOnDelayBegin
            , ZFMP_IN(const ZFListener &, cb)
            )
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, aniOnDelayEnd
            , ZFMP_IN(const ZFListener &, cb)
            )
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, aniOnStart
            , ZFMP_IN(const ZFListener &, cb)
            )
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, aniOnLoop
            , ZFMP_IN(const ZFListener &, cb)
            )
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, aniOnStop
            , ZFMP_IN(const ZFListener &, cb)
            )

protected:
    /** @brief called when #aniTarget changed */
    virtual void aniImplTargetOnUpdate(ZF_IN ZFObject *aniTargetOld) {}
    /**
     * @brief called to check whether the animation is currently valid,
     *   an invalid animation is ensured can't be started
     *
     * by default, this method checks nothing,
     * for common case, impl may check whether the target is valid
     */
    virtual zfbool aniImplCheckValid(void) {
        return zftrue;
    }

public:
    zffinal void _ZFP_ZFAnimation_aniImplDelayNotifyFinish(void);
    zffinal void _ZFP_ZFAnimation_aniReadyStart(void);
    zffinal void _ZFP_ZFAnimation_aniReadyStop(void);
    zffinal void _ZFP_ZFAnimation_aniDummyNotifyStop(void);
protected:
    /**
     * @brief for subclass to achieve delay logic
     *
     * by default, this method would implement delay by #ZFTimerOnce,
     * you may override and supply your own implementation without call super,
     * if you do, you must override or call all of these methods:
     * -  aniImplDelay: to achieve delay
     * -  aniImplDelayCancel: to achieve delay cancel
     * -  aniImplDelayNotifyFinish: to notify delay finished
     */
    virtual void aniImplDelay(void);
    /**
     * @brief see #aniImplDelay
     */
    virtual void aniImplDelayCancel(void);
    /**
     * @brief see #aniImplDelay
     */
    zffinal void aniImplDelayNotifyFinish(void);
    /**
     * @brief for subclass to achieve actual animation
     */
    virtual void aniImplStart(void);
    /**
     * @brief for subclass to stop actual animation
     */
    virtual void aniImplStop(void);
    /** @brief see #EventAniOnDelayBegin */
    virtual inline void aniOnDelayBegin(void) {
        this->observerNotify(ZFAnimation::EventAniOnDelayBegin());
    }
    /** @brief see #EventAniOnDelayEnd */
    virtual inline void aniOnDelayEnd(ZF_IN ZFResultTypeEnum resultType) {
        this->observerNotify(ZFAnimation::EventAniOnDelayEnd(), zfobj<ZFResultType>(resultType));
    }
    /** @brief see #EventAniOnStart */
    virtual inline void aniOnStart(void) {
        this->observerNotify(ZFAnimation::EventAniOnStart());
    }
    /** @brief see #EventAniOnLoop */
    virtual inline void aniOnLoop(void) {
        this->observerNotify(ZFAnimation::EventAniOnLoop());
    }
    /** @brief see #EventAniOnStop */
    virtual inline void aniOnStop(ZF_IN ZFResultTypeEnum resultType) {
        this->observerNotify(ZFAnimation::EventAniOnStop(), zfobj<ZFResultType>(resultType));
    }
    /**
     * @brief subclass must notify after the animation stop
     */
    zffinal void aniImplNotifyStop(ZF_IN_OPT ZFResultTypeEnum resultType = ZFResultType::e_Success);

protected:
    /** @brief init with #aniTarget */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(ZFObject *, aniTarget)
            )

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

private:
    _ZFP_ZFAnimationPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimation_h_

