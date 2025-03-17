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
zfclass ZFLIB_ZFUtility ZFAnimation : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFAnimation, ZFStyle)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation started
     */
    ZFEVENT(AniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation looped,
     * current loop count can be accessed by #loopCur
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
     * @brief animation's duration in miliseconds, 0 to use #ZFAnimationDurationDefault, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, duration)
    /**
     * @brief util method to #duration
     */
    ZFMETHOD_DECLARE_0(zftimet, durationFixed)

    /**
     * @brief loop count, 0 by default
     *
     * 0 mean no loop, use #zfindexMax for infinite loop
     */
    ZFPROPERTY_ASSIGN(zfindex, loop)

    /**
     * @brief curve for animation, null to use linear curve, null by default
     *
     * how the curve affects animation, depends on implementation
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFCurve>, curve)

public:
    /**
     * @brief animation's target, assign only,
     *   but would be retain automatically while animation is running
     */
    ZFMETHOD_DECLARE_1(void, target
            , ZFMP_IN(ZFObject *, target)
            )
    /**
     * @brief animation's target
     */
    ZFMETHOD_DECLARE_0(zfany, target)

    // ============================================================
    // start stop
public:
    /**
     * @brief start the animation
     * @note an animation would be retained automatically when it's running,
     *   and released after stopped
     */
    ZFMETHOD_DECLARE_1(void, start
            , ZFMP_IN_OPT(const ZFListener &, onStop, zfnull)
            )
    /**
     * @brief true if the animation is running
     */
    ZFMETHOD_DECLARE_0(zfbool, started)
    /**
     * @brief stop the animation or do nothing if not started
     */
    ZFMETHOD_DECLARE_0(void, stop)
    /**
     * @brief true if the animation is stopped by calling #stop
     *
     * this state would be kept until next #start called
     */
    ZFMETHOD_DECLARE_0(zfbool, stoppedByUser)

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
    ZFMETHOD_DECLARE_0(zfbool, valid)

    /**
     * @brief current loop count, 0 for first time
     *
     * reset when #start, see #loop for more info
     */
    ZFMETHOD_DECLARE_0(zfindex, loopCur)

public:
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
    /** @brief called when #target changed */
    virtual void aniImplTargetOnUpdate(ZF_IN ZFObject *targetOld) {}
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
    zffinal void _ZFP_ZFAnimation_aniDummyNotifyStop(void);
protected:
    /**
     * @brief for subclass to achieve actual animation
     */
    virtual void aniImplStart(void);
    /**
     * @brief for subclass to stop actual animation
     */
    virtual void aniImplStop(void);
    /** @brief see #E_AniOnStart */
    virtual inline void aniOnStart(void) {
        this->observerNotify(ZFAnimation::E_AniOnStart());
    }
    /** @brief see #E_AniOnLoop */
    virtual inline void aniOnLoop(void) {
        this->observerNotify(ZFAnimation::E_AniOnLoop());
    }
    /** @brief see #E_AniOnStop */
    virtual inline void aniOnStop(ZF_IN ZFResultType resultType) {
        this->observerNotify(ZFAnimation::E_AniOnStop(), zfobj<v_ZFResultType>(resultType));
    }
public:
    /**
     * @brief subclass must notify after the animation stop
     */
    zffinal void aniImplNotifyStop(ZF_IN_OPT ZFResultType resultType = v_ZFResultType::e_Success);

protected:
    /** @brief init with #target */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(ZFObject *, target)
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
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
        zfsuper::objectInfoImplAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

private:
    _ZFP_ZFAnimationPrivate *d;
};

// ============================================================
/**
 * @brief animation task
 */
zfclass ZFLIB_ZFUtility ZFAniTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFAniTask, ZFTask)

public:
    /** @brief the impl */
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, impl)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(ZFAnimation *, impl)
            )

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimation_h_

