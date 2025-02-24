/**
 * @file ZFTimer.h
 * @brief timer utility
 */

#ifndef _ZFI_ZFTimer_h_
#define _ZFI_ZFTimer_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTimerPrivate;
/**
 * @brief timer utility
 *
 * usage:
 * @code
 *   ZFTimer *timer = zfAlloc(ZFTimer, ...);
 *   timer->start();
 *   zfRelease(timer); // safe to release after start, timer would be auto released after stop
 * @endcode
 */
zfclass ZFLIB_ZFCore ZFTimer : zfextend ZFTaskId {
    ZFOBJECT_DECLARE(ZFTimer, ZFTaskId)
    ZFALLOC_CACHE_RELEASE({
            cache->stop();
            cache->interval(1000);
        })

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer started
     * @note on what thread this event is fired depends on impl
     */
    ZFEVENT(TimerOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer activated
     * @note on what thread this event is fired depends on impl,
     *   unless #eventOnMainThread was set
     */
    ZFEVENT(TimerOnActivate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer stopped
     * @note on what thread this event is fired depends on impl
     */
    ZFEVENT(TimerOnStop)

protected:
    /**
     * @brief init with timer settings
     *
     * you may change them after timer created,
     * but you must not if timer is started
     */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(zftimet, interval)
            )
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    /** @brief for internal use only */
    ZFMETHOD_DECLARE_0(void *, nativeTimer)

public:
    /**
     * @brief timer's interval when start a timer in mili seconds, default is 1000
     *
     * assert fail if interval is less than 0
     */
    ZFPROPERTY_ASSIGN(zftimet, interval, 1000)
    ZFPROPERTY_ON_VERIFY_DECLARE(zftimet, interval)

    /**
     * @brief whether timer activate on main thread, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, eventOnMainThread, zftrue)

public:
    /**
     * @brief start the timer
     */
    ZFMETHOD_DECLARE_0(void, start)
    /**
     * @brief stop the timer
     */
    ZFMETHOD_DECLARE_0(void, stop)
    /**
     * @brief true if started
     */
    ZFMETHOD_DECLARE_0(zfbool, started)

public:
    /**
     * @brief get timer's current fired count, with 1 as first fired time's value,
     *   keep after timer stop, but reset before timer start
     */
    ZFMETHOD_DECLARE_0(zfindex, activatedCount)

    /** @brief internal timer id for impl only */
    zfidentity timerImplId(void);

public:
    zffinal void _ZFP_ZFTimer_timerOnActivate(ZF_IN zfidentity timerImplId);
protected:
    /** @brief see #E_TimerOnStart */
    virtual inline void timerOnStart(void) {
        this->observerNotify(ZFTimer::E_TimerOnStart());
    }
    /** @brief see #E_TimerOnActivate */
    virtual inline void timerOnActivate(void) {
        this->observerNotify(ZFTimer::E_TimerOnActivate());
    }
    /** @brief see #E_TimerOnStop */
    virtual inline void timerOnStop(void) {
        this->observerNotify(ZFTimer::E_TimerOnStop());
    }

private:
    _ZFP_ZFTimerPrivate *d;
};

// ============================================================
/**
 * @brief util to start timer
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfautoT<ZFTimer>, ZFTimerStart
        , ZFMP_IN(zftimet, interval)
        , ZFMP_IN(const ZFListener &, timerCallback)
        )

/**
 * @brief util to start timer to run only once
 *
 * we would try to run the timerCallback in the same thread that call this method
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfautoT<ZFTimer>, ZFTimerOnce
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(const ZFListener &, timerCallback)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimer_h_

