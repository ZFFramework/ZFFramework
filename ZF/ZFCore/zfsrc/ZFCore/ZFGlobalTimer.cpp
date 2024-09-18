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
        this->observerHolder().observerHasAddStateAttach(zfself::EventTimerOnActivate(), &_ZFP_stateFlag, stateFlag_observerHasAdd);
    }

    zfoverride
    virtual inline void timerOnActivate(void) {
        zfsuper::timerOnActivate();
        if(!ZFBitTest(_ZFP_stateFlag, stateFlag_observerHasAdd)) {
            if(ZFBitTest(_ZFP_stateFlag, stateFlag_pendingStop)) {
                ZFBitUnset(_ZFP_stateFlag, stateFlag_pendingStop);
                this->timerStop();
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
        this->globalTimer->timerStop();
        zfRetainChange(this->globalTimer, zfnull);
    }
}
zftimet globalTimerIntervalDefault;
zftimet globalTimerInterval;
_ZFP_I_ZFGlobalTimer *globalTimer;
zfbool globalTimerManualStep;
void checkCleanup(void) {
    if(!this->globalTimer->hasTimerObserver()) {
        this->globalTimer->timerStop();
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
        d->globalTimer->timerInterval(ZFGlobalTimerInterval());
    }
    if(!d->globalTimer->timerStarted() && !d->globalTimerManualStep) {
        d->globalTimer->timerInterval(ZFGlobalTimerInterval());
        d->globalTimer->timerStart();
    }
    d->globalTimer->observerAdd(ZFTimer::EventTimerOnActivate(), timerCallback, observerLevel);
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
        d->globalTimer->timerInterval(ZFGlobalTimerInterval());
    }
    if(!d->globalTimer->timerStarted() && !d->globalTimerManualStep) {
        d->globalTimer->timerInterval(ZFGlobalTimerInterval());
        d->globalTimer->timerStart();
    }
    d->globalTimer->observerAddForOnce(ZFTimer::EventTimerOnActivate(), timerCallback, observerLevel);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerDetach
        , ZFMP_IN(const ZFListener &, timerCallback)
        ) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull) {
        return;
    }
    d->globalTimer->observerRemove(ZFTimer::EventTimerOnActivate(), timerCallback);
    d->checkCleanup();
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerDetachAll) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull) {
        return;
    }
    d->globalTimer->observerRemoveAll(ZFTimer::EventTimerOnActivate());
    d->checkCleanup();
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(zftimet const &, ZFGlobalTimerIntervalDefault) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerIntervalDefault;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerIntervalDefault
        , ZFMP_IN(zftimet const &, timerIntervalDefault)
        ) {
    ZFCoreAssert(timerIntervalDefault > 0);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerIntervalDefault = timerIntervalDefault;
}
ZFMETHOD_FUNC_DEFINE_0(zftimet const &, ZFGlobalTimerInterval) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerInterval;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerInterval
        , ZFMP_IN(zftimet const &, timerInterval)
        ) {
    ZFCoreAssert(timerInterval > 0);
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimerInterval != timerInterval) {
        d->globalTimerInterval = timerInterval;
        if(d->globalTimer != zfnull && d->globalTimer->timerStarted()) {
            d->globalTimer->timerStop();
            d->globalTimer->timerInterval(d->globalTimerInterval);
            d->globalTimer->timerStart();
        }
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerManualStep) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    d->globalTimerManualStep = zftrue;
    if(d->globalTimer != zfnull) {
        d->globalTimer->timerStop();
        d->globalTimer->observerNotify(ZFTimer::EventTimerOnActivate());
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
            d->globalTimer->timerInterval(ZFGlobalTimerInterval());
            d->globalTimer->timerStart();
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

