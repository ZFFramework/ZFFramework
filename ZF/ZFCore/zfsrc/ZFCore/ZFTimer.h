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
zfclass ZFLIB_ZFCore ZFTimer : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFTimer, ZFObject)
    ZFALLOC_CACHE_RELEASE({
            cache->timerStop();
            cache->timerInterval(1000);
            cache->timerDelay(0);
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
     *   unless #timerActivateOnMainThread was set
     */
    ZFEVENT(TimerOnActivate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer stopped, ensured in the same thread that stop the timer
     * @note timer start and stop event is not ensured paired
     */
    ZFEVENT(TimerOnStop)

protected:
    /**
     * @brief init with timer settings
     *
     * you may change them after timer created,
     * but you must not if timer is started
     */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(zftimet, timerInterval)
            , ZFMP_IN_OPT(zftimet, timerDelay, 0)
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
    ZFPROPERTY_ASSIGN(zftimet, timerInterval, 1000)
    ZFPROPERTY_ON_VERIFY_DECLARE(zftimet, timerInterval)

    /**
     * @brief timer's delay when start a timer in mili seconds, default is 0
     *
     * if delay is less than 10, it's treated as 0\n
     * else, first timer event would be fired after (timerDelay + timerInterval)
     */
    ZFPROPERTY_ASSIGN(zftimet, timerDelay, 0)
    ZFPROPERTY_ON_VERIFY_DECLARE(zftimet, timerDelay)

    /**
     * @brief whether timer activate on main thread, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, timerActivateOnMainThread, zftrue)

public:
    /**
     * @brief start the timer
     */
    ZFMETHOD_DECLARE_0(void, timerStart)
    /**
     * @brief stop the timer
     */
    ZFMETHOD_DECLARE_0(void, timerStop)
    /**
     * @brief true if started
     */
    ZFMETHOD_DECLARE_0(zfbool, timerStarted)

public:
    /**
     * @brief get timer's current fired count, with 1 as first fired time's value,
     *   keep after timer stop, but reset before timer start
     */
    ZFMETHOD_DECLARE_0(zfindex, timerActivatedCount)

public:
    zffinal void _ZFP_ZFTimer_timerOnStart(void);
    zffinal void _ZFP_ZFTimer_timerOnActivate(void);
    zffinal void _ZFP_ZFTimer_timerOnStop(void);
protected:
    /** @brief see #EventTimerOnStart */
    virtual inline void timerOnStart(void) {
        this->observerNotify(ZFTimer::EventTimerOnStart());
    }
    /** @brief see #EventTimerOnActivate */
    virtual inline void timerOnActivate(void) {
        this->observerNotify(ZFTimer::EventTimerOnActivate());
    }
    /** @brief see #EventTimerOnStop */
    virtual inline void timerOnStop(void) {
        this->observerNotify(ZFTimer::EventTimerOnStop());
    }

private:
    _ZFP_ZFTimerPrivate *d;
};

// ============================================================
/**
 * @brief util to start timer
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfautoT<ZFTimer>, ZFTimerStart
        , ZFMP_IN(zftimet, timerInterval)
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

