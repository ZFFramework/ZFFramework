#include "ZFAnimationTimeLine.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAnimationTimeLinePrivate
zfclassNotPOD _ZFP_ZFAnimationTimeLinePrivate {
public:
    zfbool isGlobalTimer;

    ZFListener globalTimerTask;
    zfuint globalTimerFrameCount;
    zfuint globalTimerFrameIndex;

    ZFTimer *builtinTimer;
    zftimet builtinTimerStartTime;
public:
    _ZFP_ZFAnimationTimeLinePrivate(void)
    : isGlobalTimer(zffalse)
    , globalTimerTask()
    , globalTimerFrameCount(0)
    , globalTimerFrameIndex(0)
    , builtinTimer(zfnull)
    , builtinTimerStartTime(0)
    {
    }
    ~_ZFP_ZFAnimationTimeLinePrivate(void) {
        zfRetainChange(this->builtinTimer, zfnull);
    }

public:
    static void doStart(ZF_IN ZFAnimationTimeLine *owner) {
        if(owner->aniInterval() == 0) {
            owner->d->isGlobalTimer = zftrue;
            owner->d->globalTimerFrameCount = (zfuint)zfmRound(owner->aniDurationFixed() / ZFGlobalTimerIntervalDefault());
            owner->d->globalTimerFrameIndex = 0;

            ZFLISTENER_1(globalTimerOnActivate
                    , ZFAnimationTimeLine *, owner
                    ) {
                _ZFP_ZFAnimationTimeLinePrivate::globalTimerOnActivate(zfargs, owner);
            } ZFLISTENER_END()
            owner->d->globalTimerTask = globalTimerOnActivate;
            ZFGlobalTimerAttach(owner->d->globalTimerTask);
        }
        else {
            owner->d->isGlobalTimer = zffalse;
            if(owner->d->builtinTimer == zfnull) {
                owner->d->builtinTimer = zfAlloc(ZFTimer);

                ZFLISTENER_1(builtinTimerOnActivate
                        , ZFAnimationTimeLine *, owner
                        ) {
                    _ZFP_ZFAnimationTimeLinePrivate::builtinTimerOnActivate(zfargs, owner);
                } ZFLISTENER_END()
                owner->d->builtinTimer->observerAdd(ZFTimer::EventTimerOnActivate(), builtinTimerOnActivate);
            }
            owner->d->builtinTimer->timerInterval(owner->aniInterval() > 0 ? owner->aniInterval() : ZFGlobalTimerIntervalDefault());
            owner->d->builtinTimerStartTime = ZFTime::timestamp();
            owner->d->builtinTimer->timerStart();
        }
        _update(owner, 0);
    }
    static void doStop(ZF_IN ZFAnimationTimeLine *owner) {
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
            , ZF_IN ZFAnimationTimeLine *owner
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
            , ZF_IN ZFAnimationTimeLine *owner
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
            ZF_IN ZFAnimationTimeLine *owner
            , ZF_IN zffloat progress
            ) {
        if(progress < 0) {progress = 0;}
        else if(progress > 1) {progress = 1;}
        if(owner->aniCurve() != zfnull) {
            progress = owner->aniCurve()->progressUpdate(progress);
        }
        owner->aniTimeLineOnUpdate(progress);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationTimeLine)

ZFEVENT_REGISTER(ZFAnimationTimeLine, AniTimeLineOnUpdate)

// ============================================================
// object
void ZFAnimationTimeLine::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAnimationTimeLinePrivate);
}
void ZFAnimationTimeLine::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfidentity ZFAnimationTimeLine::objectHash(void) {
    return zfidentityHash(zfsuper::objectHash()
        , this->aniInterval()
        , (this->aniCurve() ? this->aniCurve()->objectHash() : (zfidentity)0)
        );
}
ZFCompareResult ZFAnimationTimeLine::objectCompare(ZF_IN ZFObject *anotherObj) {
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
void ZFAnimationTimeLine::aniImplStart(void) {
    zfsuper::aniImplStart();
    _ZFP_ZFAnimationTimeLinePrivate::doStart(this);
}
void ZFAnimationTimeLine::aniImplStop(void) {
    _ZFP_ZFAnimationTimeLinePrivate::doStop(this);
    zfsuper::aniImplStop();
}

void ZFAnimationTimeLine::aniTimeLineOnUpdate(ZF_IN zffloat progress) {
    this->observerNotify(
        ZFAnimationTimeLine::EventAniTimeLineOnUpdate(),
        zfobj<v_zffloat>(progress));
}

ZF_NAMESPACE_GLOBAL_END

