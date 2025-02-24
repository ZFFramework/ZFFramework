#include "ZFGlobalTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFGlobalTimer : zfextend ZFTimer {
    ZFOBJECT_DECLARE(_ZFP_I_ZFGlobalTimer, ZFTimer)

public:
    zfbool hasTimerObserver(void) {
        return ZFBitTest(_ZFP_stateFlag, stateFlag_observerHasAdd);
    }

private:
    enum {
        stateFlag_observerHasAdd = 1 << 0,
        stateFlag_pendingStop = 1 << 1,
    };
    zfuint _ZFP_stateFlag;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _ZFP_stateFlag = 0;
        this->observerHolder().observerHasAddStateAttach(zfself::E_TimerOnActivate(), &_ZFP_stateFlag, stateFlag_observerHasAdd);
    }

    zfoverride
    virtual inline void timerOnActivate(void) {
        zfsuper::timerOnActivate();
        if(!ZFBitTest(_ZFP_stateFlag, stateFlag_observerHasAdd)) {
            if(ZFBitTest(_ZFP_stateFlag, stateFlag_pendingStop)) {
                ZFBitUnset(_ZFP_stateFlag, stateFlag_pendingStop);
                this->stop();
            }
            else {
                ZFBitSet(_ZFP_stateFlag, stateFlag_pendingStop);
            }
        }
        else {
            ZFBitUnset(_ZFP_stateFlag, stateFlag_pendingStop);
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFGlobalTimer)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFGlobalTimerDataHolder, ZFLevelZFFrameworkHigh) {
    this->globalTimer = zfnull;
    this->globalTimerIntervalDefault = 33; // 1000/30, 30fps by default
    this->globalTimerInterval = this->globalTimerIntervalDefault;
    this->globalTimerManualStep = zffalse;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFGlobalTimerDataHolder) {
    if(this->globalTimer != zfnull) {
        this->globalTimer->stop();
        zfRetainChange(this->globalTimer, zfnull);
    }
}
zftimet globalTimerIntervalDefault;
zftimet globalTimerInterval;
_ZFP_I_ZFGlobalTimer *globalTimer;
zfbool globalTimerManualStep;
void checkCleanup(void) {
    if(!this->globalTimer->hasTimerObserver()) {
        this->globalTimer->stop();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFGlobalTimerDataHolder)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFGlobalTimerAutoDetach, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFGlobalTimerAutoDetach) {
    ZFGlobalTimerDetachAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFGlobalTimerAutoDetach)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(void, ZFGlobalTimerAttach
        , ZFMP_IN(const ZFListener &, timerCallback)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        ) {
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkHigh) != ZFFrameworkStateAvailable || !timerCallback) {
        return;
    }
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull) {
        d->globalTimer = zfAlloc(_ZFP_I_ZFGlobalTimer);
        d->globalTimer->interval(ZFGlobalTimerInterval());
    }
    if(!d->globalTimer->started() && !d->globalTimerManualStep) {
        d->globalTimer->interval(ZFGlobalTimerInterval());
        d->globalTimer->start();
    }
    d->globalTimer->observerAdd(ZFTimer::E_TimerOnActivate(), timerCallback, observerLevel);
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFGlobalTimerAttachOnce
        , ZFMP_IN(const ZFListener &, timerCallback)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        ) {
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkHigh) != ZFFrameworkStateAvailable || !timerCallback) {
        return;
    }
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull) {
        d->globalTimer = zfAlloc(_ZFP_I_ZFGlobalTimer);
        d->globalTimer->interval(ZFGlobalTimerInterval());
    }
    if(!d->globalTimer->started() && !d->globalTimerManualStep) {
        d->globalTimer->interval(ZFGlobalTimerInterval());
        d->globalTimer->start();
    }
    d->globalTimer->observerAddForOnce(ZFTimer::E_TimerOnActivate(), timerCallback, observerLevel);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerDetach
        , ZFMP_IN(const ZFListener &, timerCallback)
        ) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull) {
        return;
    }
    d->globalTimer->observerRemove(ZFTimer::E_TimerOnActivate(), timerCallback);
    d->checkCleanup();
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerDetachAll) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull) {
        return;
    }
    d->globalTimer->observerRemoveAll(ZFTimer::E_TimerOnActivate());
    d->checkCleanup();
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(zftimet const &, ZFGlobalTimerIntervalDefault) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerIntervalDefault;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerIntervalDefault
        , ZFMP_IN(zftimet const &, intervalDefault)
        ) {
    ZFCoreAssert(intervalDefault > 0);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerIntervalDefault = intervalDefault;
}
ZFMETHOD_FUNC_DEFINE_0(zftimet const &, ZFGlobalTimerInterval) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerInterval;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerInterval
        , ZFMP_IN(zftimet const &, interval)
        ) {
    ZFCoreAssert(interval > 0);
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimerInterval != interval) {
        d->globalTimerInterval = interval;
        if(d->globalTimer != zfnull && d->globalTimer->started()) {
            d->globalTimer->stop();
            d->globalTimer->interval(d->globalTimerInterval);
            d->globalTimer->start();
        }
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerManualStep) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    d->globalTimerManualStep = zftrue;
    if(d->globalTimer != zfnull) {
        d->globalTimer->stop();
        d->globalTimer->observerNotify(ZFTimer::E_TimerOnActivate());
    }
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerManualStepCancel) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(!d->globalTimerManualStep) {
        return;
    }
    d->globalTimerManualStep = zffalse;
    if(d->globalTimer != zfnull) {
        if(d->globalTimer->hasTimerObserver()) {
            d->globalTimer->interval(ZFGlobalTimerInterval());
            d->globalTimer->start();
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

