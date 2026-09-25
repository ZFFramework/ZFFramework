#include "ZFAniForTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAniForTimerPrivate
zfclassNotPOD _ZFP_ZFAniForTimerPrivate {
public:
    zfbool useGlobalTimer;

    ZFListener globalTimerTask;
    zftimet globalTimerOffsetTime;

    ZFTimer *builtinTimer;
    zftimet builtinTimerStartTime;
public:
    _ZFP_ZFAniForTimerPrivate(void)
    : useGlobalTimer(zffalse)
    , globalTimerTask()
    , globalTimerOffsetTime(0)
    , builtinTimer(zfnull)
    , builtinTimerStartTime(0)
    {
    }
    ~_ZFP_ZFAniForTimerPrivate(void) {
        zfobjRetainChange(this->builtinTimer, zfnull);
    }

public:
    static void doStart(ZF_IN ZFAniForTimer *owner) {
        if(owner->aniInterval() == 0) {
            owner->d->useGlobalTimer = zftrue;
            owner->d->globalTimerOffsetTime = 0;

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
                owner->d->builtinTimer = zfobjAlloc(ZFTimer);

                ZFLISTENER_1(builtinTimerOnActivate
                        , ZFAniForTimer *, owner
                        ) {
                    _ZFP_ZFAniForTimerPrivate::builtinTimerOnActivate(owner);
                } ZFLISTENER_END()
                owner->d->builtinTimer->observerAdd(ZFTimer::E_TimerOnActivate(), builtinTimerOnActivate);
            }
            owner->d->builtinTimer->interval(owner->aniInterval() > 0 ? owner->aniInterval() : ZFGlobalTimerIntervalDefault());
            owner->d->builtinTimerStartTime = ZFTime::timestamp();
            owner->d->builtinTimer->start();
        }
        _update(owner, 0);
    }
    static void doStop(ZF_IN ZFAniForTimer *owner) {
        if(owner->d->useGlobalTimer) {
            ZFGlobalTimerDetach(owner->d->globalTimerTask);
            owner->d->globalTimerTask = zfnull;
        }
        else {
            owner->d->builtinTimer->stop();
        }
    }

private:
    static void globalTimerOnActivate(ZF_IN ZFAniForTimer *owner) {
        zftimet durationFixed = owner->durationFixed();
        owner->d->globalTimerOffsetTime += ZFGlobalTimerIntervalDefault();
        zffloat progress = (owner->d->globalTimerOffsetTime > durationFixed - ZFGlobalTimerIntervalDefault() / 2)
            ? 1.0f
            : ((zffloat)owner->d->globalTimerOffsetTime / durationFixed)
            ;
        _update(owner, progress);
        if(progress >= 1.0f) {
            owner->aniImplNotifyStop();
        }
    }
    static void builtinTimerOnActivate(ZF_IN ZFAniForTimer *owner) {
        zftimet curTime = ZFTime::timestamp();
        zftimet durationFixed = owner->durationFixed();
        zffloat progress = (curTime - owner->d->builtinTimerStartTime
                >= durationFixed - (owner->aniInterval() > 0 ? owner->aniInterval() : ZFGlobalTimerIntervalDefault()))
            ? 1.0f
            : ((zffloat)(curTime - owner->d->builtinTimerStartTime)) / durationFixed
            ;
        _update(owner, progress);
        if(progress >= 1.0f) {
            owner->aniImplNotifyStop();
        }
    }
    static void _update(
            ZF_IN ZFAniForTimer *owner
            , ZF_IN zffloat progress
            ) {
        if(progress < 0) {progress = 0;}
        else if(progress > 1) {progress = 1;}
        if(owner->curve() != zfnull) {
            progress = owner->curve()->curveUpdate(progress);
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
void ZFAniForTimer::aniImplStart(void) {
    zfsuper::aniImplStart();
    _ZFP_ZFAniForTimerPrivate::doStart(this);
}
void ZFAniForTimer::aniImplStop(void) {
    _ZFP_ZFAniForTimerPrivate::doStop(this);
    zfsuper::aniImplStop();
}

void ZFAniForTimer::aniTimerOnUpdate(ZF_IN zffloat progress) {
    this->observerNotify(ZFAniForTimer::E_AniTimerOnUpdate(), ZFArgs().param0(zfobj<v_zffloat>(progress)));
}

ZF_NAMESPACE_GLOBAL_END

