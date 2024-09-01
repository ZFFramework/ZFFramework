#include "ZFAniForTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAniForTimerPrivate
zfclassNotPOD _ZFP_ZFAniForTimerPrivate {
public:
    zfbool useGlobalTimer;

    ZFListener globalTimerTask;
    zfuint globalTimerFrameCount;
    zfuint globalTimerFrameIndex;

    ZFTimer *builtinTimer;
    zftimet builtinTimerStartTime;
public:
    _ZFP_ZFAniForTimerPrivate(void)
    : useGlobalTimer(zffalse)
    , globalTimerTask()
    , globalTimerFrameCount(0)
    , globalTimerFrameIndex(0)
    , builtinTimer(zfnull)
    , builtinTimerStartTime(0)
    {
    }
    ~_ZFP_ZFAniForTimerPrivate(void) {
        zfRetainChange(this->builtinTimer, zfnull);
    }

public:
    static void doStart(ZF_IN ZFAniForTimer *owner) {
        if(owner->aniInterval() == 0) {
            owner->d->useGlobalTimer = zftrue;
            owner->d->globalTimerFrameCount = (zfuint)zfmRound(owner->aniDurationFixed() / ZFGlobalTimerIntervalDefault());
            owner->d->globalTimerFrameIndex = 0;

            ZFLISTENER_1(globalTimerOnActivate
                    , ZFAniForTimer *, owner
                    ) {
                _ZFP_ZFAniForTimerPrivate::globalTimerOnActivate(owner);
            } ZFLISTENER_END()
            owner->d->globalTimerTask = globalTimerOnActivate;
            ZFGlobalTimerAttach(owner->d->globalTimerTask);
        }
        else {
            owner->d->useGlobalTimer = zffalse;
            if(owner->d->builtinTimer == zfnull) {
                owner->d->builtinTimer = zfAlloc(ZFTimer);

                ZFLISTENER_1(builtinTimerOnActivate
                        , ZFAniForTimer *, owner
                        ) {
                    _ZFP_ZFAniForTimerPrivate::builtinTimerOnActivate(owner);
                } ZFLISTENER_END()
                owner->d->builtinTimer->observerAdd(ZFTimer::EventTimerOnActivate(), builtinTimerOnActivate);
            }
            owner->d->builtinTimer->timerInterval(owner->aniInterval() > 0 ? owner->aniInterval() : ZFGlobalTimerIntervalDefault());
            owner->d->builtinTimerStartTime = ZFTime::timestamp();
            owner->d->builtinTimer->timerStart();
        }
        _update(owner, 0);
    }
    static void doStop(ZF_IN ZFAniForTimer *owner) {
        if(owner->d->useGlobalTimer) {
            ZFGlobalTimerDetach(owner->d->globalTimerTask);
            owner->d->globalTimerTask = zfnull;
        }
        else {
            owner->d->builtinTimer->timerStop();
        }
    }

private:
    static void globalTimerOnActivate(ZF_IN ZFAniForTimer *owner) {
        ++(owner->d->globalTimerFrameIndex);
        zffloat progress = 1;
        if(owner->d->globalTimerFrameIndex < owner->d->globalTimerFrameCount) {
            progress = (zffloat)owner->d->globalTimerFrameIndex / owner->d->globalTimerFrameCount;
        }
        _update(owner, progress);
        if(owner->d->globalTimerFrameIndex >= owner->d->globalTimerFrameCount) {
            owner->aniImplNotifyStop();
        }
    }
    static void builtinTimerOnActivate(ZF_IN ZFAniForTimer *owner) {
        zftimet curTime = ZFTime::timestamp();
        zffloat progress = ((zffloat)(curTime - owner->d->builtinTimerStartTime)) / owner->aniDurationFixed();
        _update(owner, progress);
        if(curTime - owner->d->builtinTimerStartTime >= owner->aniDurationFixed()) {
            owner->aniImplNotifyStop();
        }
    }
    static void _update(
            ZF_IN ZFAniForTimer *owner
            , ZF_IN zffloat progress
            ) {
        if(progress < 0) {progress = 0;}
        else if(progress > 1) {progress = 1;}
        if(owner->aniCurve() != zfnull) {
            progress = owner->aniCurve()->progressUpdate(progress);
        }
        owner->aniTimerOnUpdate(progress);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniForTimer)

ZFEVENT_REGISTER(ZFAniForTimer, AniTimerOnUpdate)

// ============================================================
// object
void ZFAniForTimer::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAniForTimerPrivate);
}
void ZFAniForTimer::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
// start stop
void ZFAniForTimer::aniImplDelay(void) {
    d->useGlobalTimer = (this->aniInterval() == 0);
    if(!d->useGlobalTimer) {
        zfsuper::aniImplDelay();
        return;
    }
    d->globalTimerFrameCount = (zfuint)zfmRound(this->aniDelay() / ZFGlobalTimerIntervalDefault());
    if(d->globalTimerFrameCount == 0) {
        d->globalTimerTask = zfnull;
        this->aniImplDelayNotifyFinish();
        return;
    }
    d->globalTimerFrameIndex = 0;
    ZFAniForTimer *owner = this;
    ZFLISTENER_1(globalTimerOnActivate
            , ZFAniForTimer *, owner
            ) {
        ++(owner->d->globalTimerFrameIndex);
        if(owner->d->globalTimerFrameIndex >= owner->d->globalTimerFrameCount) {
            ZFGlobalTimerDetach(owner->d->globalTimerTask);
            owner->d->globalTimerTask = zfnull;
            owner->aniImplDelayNotifyFinish();
        }
    } ZFLISTENER_END()
    d->globalTimerTask = globalTimerOnActivate;
    ZFGlobalTimerAttach(owner->d->globalTimerTask);
}
void ZFAniForTimer::aniImplDelayCancel(void) {
    if(!d->useGlobalTimer) {
        zfsuper::aniImplDelayCancel();
        return;
    }
    if(d->globalTimerTask) {
        ZFGlobalTimerDetach(d->globalTimerTask);
        d->globalTimerTask = zfnull;
    }
}

void ZFAniForTimer::aniImplStart(void) {
    zfsuper::aniImplStart();
    _ZFP_ZFAniForTimerPrivate::doStart(this);
}
void ZFAniForTimer::aniImplStop(void) {
    _ZFP_ZFAniForTimerPrivate::doStop(this);
    zfsuper::aniImplStop();
}

void ZFAniForTimer::aniTimerOnUpdate(ZF_IN zffloat progress) {
    this->observerNotify(
        ZFAniForTimer::EventAniTimerOnUpdate(),
        zfobj<v_zffloat>(progress));
}

ZF_NAMESPACE_GLOBAL_END

