#include "ZFAniForTimeline.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAniForTimelinePrivate
zfclassNotPOD _ZFP_ZFAniForTimelinePrivate {
public:
    zfbool isGlobalTimer;

    ZFListener globalTimerTask;
    zfuint globalTimerFrameCount;
    zfuint globalTimerFrameIndex;

    ZFTimer *builtinTimer;
    zftimet builtinTimerStartTime;
public:
    _ZFP_ZFAniForTimelinePrivate(void)
    : isGlobalTimer(zffalse)
    , globalTimerTask()
    , globalTimerFrameCount(0)
    , globalTimerFrameIndex(0)
    , builtinTimer(zfnull)
    , builtinTimerStartTime(0)
    {
    }
    ~_ZFP_ZFAniForTimelinePrivate(void) {
        zfRetainChange(this->builtinTimer, zfnull);
    }

public:
    static void doStart(ZF_IN ZFAniForTimeline *owner) {
        if(owner->aniInterval() == 0) {
            owner->d->isGlobalTimer = zftrue;
            owner->d->globalTimerFrameCount = (zfuint)zfmRound(owner->aniDurationFixed() / ZFGlobalTimerIntervalDefault());
            owner->d->globalTimerFrameIndex = 0;

            ZFLISTENER_1(globalTimerOnActivate
                    , ZFAniForTimeline *, owner
                    ) {
                _ZFP_ZFAniForTimelinePrivate::globalTimerOnActivate(zfargs, owner);
            } ZFLISTENER_END()
            owner->d->globalTimerTask = globalTimerOnActivate;
            ZFGlobalTimerAttach(owner->d->globalTimerTask);
        }
        else {
            owner->d->isGlobalTimer = zffalse;
            if(owner->d->builtinTimer == zfnull) {
                owner->d->builtinTimer = zfAlloc(ZFTimer);

                ZFLISTENER_1(builtinTimerOnActivate
                        , ZFAniForTimeline *, owner
                        ) {
                    _ZFP_ZFAniForTimelinePrivate::builtinTimerOnActivate(zfargs, owner);
                } ZFLISTENER_END()
                owner->d->builtinTimer->observerAdd(ZFTimer::EventTimerOnActivate(), builtinTimerOnActivate);
            }
            owner->d->builtinTimer->timerInterval(owner->aniInterval() > 0 ? owner->aniInterval() : ZFGlobalTimerIntervalDefault());
            owner->d->builtinTimerStartTime = ZFTime::timestamp();
            owner->d->builtinTimer->timerStart();
        }
        _update(owner, 0);
    }
    static void doStop(ZF_IN ZFAniForTimeline *owner) {
        if(owner->d->isGlobalTimer) {
            ZFGlobalTimerDetach(owner->d->globalTimerTask);
            owner->d->globalTimerTask = zfnull;
        }
        else {
            owner->d->builtinTimer->timerStop();
        }
    }

private:
    static void globalTimerOnActivate(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFAniForTimeline *owner
            ) {
        ++(owner->d->globalTimerFrameIndex);
        zffloat progress = 1;
        if(owner->d->globalTimerFrameIndex < owner->d->globalTimerFrameCount) {
            progress = (zffloat)owner->d->globalTimerFrameIndex / owner->d->globalTimerFrameCount;
        }
        _update(owner, progress);
        if(owner->d->globalTimerFrameIndex >= owner->d->globalTimerFrameCount) {
            doStop(owner);
            owner->aniImplNotifyStop();
        }
    }
    static void builtinTimerOnActivate(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFAniForTimeline *owner
            ) {
        zftimet curTime = ZFTime::timestamp();
        zffloat progress = ((zffloat)(curTime - owner->d->builtinTimerStartTime)) / owner->aniDurationFixed();
        _update(owner, progress);
        if(curTime - owner->d->builtinTimerStartTime >= owner->aniDurationFixed()) {
            doStop(owner);
            owner->aniImplNotifyStop();
        }
    }
    static void _update(
            ZF_IN ZFAniForTimeline *owner
            , ZF_IN zffloat progress
            ) {
        if(progress < 0) {progress = 0;}
        else if(progress > 1) {progress = 1;}
        if(owner->aniCurve() != zfnull) {
            progress = owner->aniCurve()->progressUpdate(progress);
        }
        owner->aniTimelineOnUpdate(progress);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniForTimeline)

ZFEVENT_REGISTER(ZFAniForTimeline, AniTimelineOnUpdate)

// ============================================================
// object
void ZFAniForTimeline::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAniForTimelinePrivate);
}
void ZFAniForTimeline::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfidentity ZFAniForTimeline::objectHash(void) {
    return zfidentityHash(zfsuper::objectHash()
        , this->aniInterval()
        , (this->aniCurve() ? this->aniCurve()->objectHash() : (zfidentity)0)
        );
}
ZFCompareResult ZFAniForTimeline::objectCompare(ZF_IN ZFObject *anotherObj) {
    if(anotherObj == this) {return ZFCompareEqual;}
    if(anotherObj != zfnull && anotherObj->classData()->classIsTypeOf(zfself::ClassData())
            && ZFClassUtil::allPropertyIsEqual(this, anotherObj)
            ) {
        return ZFCompareEqual;
    }
    return ZFCompareUncomparable;
}

// ============================================================
// start stop
void ZFAniForTimeline::aniImplDelay(void) {
    d->isGlobalTimer = (this->aniInterval() == 0);
    if(!d->isGlobalTimer) {
        zfsuper::aniImplDelay();
        return;
    }
    d->globalTimerFrameCount = (zfuint)zfmRound(this->aniDurationFixed() / ZFGlobalTimerIntervalDefault());
    if(d->globalTimerFrameCount == 0) {
        d->globalTimerTask = zfnull;
        this->aniImplDelayNotifyFinish();
        return;
    }
    d->globalTimerFrameIndex = 0;
    ZFAniForTimeline *owner = this;
    ZFLISTENER_1(globalTimerOnActivate
            , ZFAniForTimeline *, owner
            ) {
        ++(owner->d->globalTimerFrameIndex);
        if(owner->d->globalTimerFrameIndex >= owner->d->globalTimerFrameCount) {
            owner->aniImplDelayNotifyFinish();
        }
    } ZFLISTENER_END()
    d->globalTimerTask = globalTimerOnActivate;
    ZFGlobalTimerAttach(owner->d->globalTimerTask);
}
void ZFAniForTimeline::aniImplDelayCancel(void) {
    if(!d->isGlobalTimer) {
        zfsuper::aniImplDelayCancel();
        return;
    }
    if(d->globalTimerTask) {
        ZFGlobalTimerDetach(d->globalTimerTask);
        d->globalTimerTask = zfnull;
    }
}

void ZFAniForTimeline::aniImplStart(void) {
    zfsuper::aniImplStart();
    _ZFP_ZFAniForTimelinePrivate::doStart(this);
}
void ZFAniForTimeline::aniImplStop(void) {
    _ZFP_ZFAniForTimelinePrivate::doStop(this);
    zfsuper::aniImplStop();
}

void ZFAniForTimeline::aniTimelineOnUpdate(ZF_IN zffloat progress) {
    this->observerNotify(
        ZFAniForTimeline::EventAniTimelineOnUpdate(),
        zfobj<v_zffloat>(progress));
}

ZF_NAMESPACE_GLOBAL_END

