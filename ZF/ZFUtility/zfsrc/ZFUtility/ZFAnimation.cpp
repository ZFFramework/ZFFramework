#include "ZFAnimation.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_DEFINE(zftimet, ZFAnimationDurationDefault, 250)

// ============================================================
zfclass _ZFP_I_ZFAnimationAniList : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAnimationAniList, ZFObject)
public:
    ZFCoreArray<ZFAnimation *> aniList;
};

// ============================================================
// _ZFP_ZFAnimationPrivate
zfclassNotPOD _ZFP_ZFAnimationPrivate {
public:
    ZFObjectHolder *aniTargetHolder;
    zfbool aniRunning;
    zfbool aniImplStartFlag;
    zfbool aniStoppedByUser;
    zfautoT<ZFTimer> aniDummyTimer;
    zfidentity aniId;
    zfindex aniLoopCur;

public:
    _ZFP_ZFAnimationPrivate(void)
    : aniTargetHolder(zfnull)
    , aniRunning(zffalse)
    , aniImplStartFlag(zffalse)
    , aniStoppedByUser(zffalse)
    , aniDummyTimer()
    , aniId(zfidentityInvalid())
    , aniLoopCur(0)
    {
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimation)

ZFEVENT_REGISTER(ZFAnimation, AniOnDelayBegin)
ZFEVENT_REGISTER(ZFAnimation, AniOnDelayEnd)
ZFEVENT_REGISTER(ZFAnimation, AniOnStart)
ZFEVENT_REGISTER(ZFAnimation, AniOnLoop)
ZFEVENT_REGISTER(ZFAnimation, AniOnStop)

ZFMETHOD_DEFINE_0(ZFAnimation, zftimet, aniDurationFixed) {
    return (this->aniDuration() > 0 ? this->aniDuration() : ZFAnimationDurationDefault());
}

ZFMETHOD_DEFINE_1(ZFAnimation, void, aniTarget
        , ZFMP_IN(ZFObject *, aniTarget)
        ) {
    zfCoreAssertWithMessage(!d->aniRunning, "change animation's target while animation is running");
    ZFObjectHolder *aniTargetHolderTmp = d->aniTargetHolder;
    zfblockedRelease(aniTargetHolderTmp);
    d->aniTargetHolder = aniTarget ? zfRetain(aniTarget->objectHolder()) : zfnull;
    this->aniImplTargetOnUpdate(aniTargetHolderTmp ? aniTargetHolderTmp->objectHolded().toObject() : zfnull);
    zfRetainChange(d->aniTargetHolder, aniTarget ? aniTarget->objectHolder() : zfnull);
}
ZFMETHOD_DEFINE_0(ZFAnimation, zfany, aniTarget) {
    return d->aniTargetHolder ? d->aniTargetHolder->objectHolded().toObject() : zfnull;
}

ZFMETHOD_DEFINE_0(ZFAnimation, void, aniStart) {
    this->aniStop();
    this->_ZFP_ZFAnimation_aniReadyStart();

    d->aniStoppedByUser = zffalse;
    d->aniLoopCur = 0;
    if(!this->aniValid()) {
        this->aniOnStart();
        this->aniOnStop(ZFResultType::e_Fail);
        return;
    }

    zfRetain(this);
    zfRetain(this->aniTarget());

    ++(d->aniId);
    d->aniRunning = zftrue;
    d->aniImplStartFlag = zftrue;
    this->aniImplStart();
    this->aniOnStart();
}
ZFMETHOD_DEFINE_0(ZFAnimation, zfbool, aniRunning) {
    return d->aniRunning;
}
ZFMETHOD_DEFINE_0(ZFAnimation, void, aniStop) {
    if(!(d->aniRunning)) {
        return;
    }
    d->aniStoppedByUser = zftrue;
    ++(d->aniId);
    this->aniImplNotifyStop(ZFResultType::e_Cancel);
}
ZFMETHOD_DEFINE_0(ZFAnimation, zfbool, aniStoppedByUser) {
    return d->aniStoppedByUser;
}

ZFMETHOD_DEFINE_0(ZFAnimation, zfidentity, aniId) {
    return d->aniId;
}

ZFMETHOD_DEFINE_0(ZFAnimation, zfbool, aniValid) {
    return this->aniImplCheckValid();
}

ZFMETHOD_DEFINE_0(ZFAnimation, zfindex, aniLoopCur) {
    return d->aniLoopCur;
}

ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnDelayBegin
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::EventAniOnDelayBegin(), cb);
}
ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnDelayEnd
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::EventAniOnDelayEnd(), cb);
}
ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnStart
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::EventAniOnStart(), cb);
}
ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnLoop
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::EventAniOnLoop(), cb);
}
ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnStop
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::EventAniOnStop(), cb);
}

void ZFAnimation::_ZFP_ZFAnimation_aniReadyStart(void) {
    if(this->aniTarget() != zfnull) {
        _ZFP_I_ZFAnimationAniList *aniList = this->aniTarget()->objectTag(_ZFP_I_ZFAnimationAniList::ClassData()->classNameFull());
        if(aniList == zfnull) {
            aniList = zfAlloc(_ZFP_I_ZFAnimationAniList);
            this->aniTarget()->objectTag(_ZFP_I_ZFAnimationAniList::ClassData()->classNameFull(), aniList);
            zfRelease(aniList);
        }
        if(this->aniAutoStopPrev()) {
            while(!aniList->aniList.isEmpty()) {
                aniList->aniList.getFirst()->aniStop();
            }
        }
        aniList->aniList.add(this);
    }
}
void ZFAnimation::_ZFP_ZFAnimation_aniReadyStop(void) {
    if(this->aniTarget() != zfnull) {
        _ZFP_I_ZFAnimationAniList *aniList = this->aniTarget()->objectTag(_ZFP_I_ZFAnimationAniList::ClassData()->classNameFull());
        if(aniList != zfnull) {
            aniList->aniList.removeElement(this);
        }
    }
}
void ZFAnimation::_ZFP_ZFAnimation_aniDummyNotifyStop(void) {
    d->aniDummyTimer = zfnull;
    this->aniImplNotifyStop();
}

void ZFAnimation::aniImplStart(void) {
    // start a dummy animation if not implemented
    if(this->classData() == ZFAnimation::ClassData()) {
        ZFAnimation *owner = this;
        ZFLISTENER_1(dummyOnFinish
                , ZFAnimation *, owner
                ) {
            owner->_ZFP_ZFAnimation_aniDummyNotifyStop();
        } ZFLISTENER_END()
        d->aniDummyTimer = ZFTimerOnce(
            this->aniDurationFixed(),
            dummyOnFinish);
    }
}
void ZFAnimation::aniImplStop(void) {
    if(this->classData() == ZFAnimation::ClassData()) {
        if(d->aniDummyTimer != zfnull) {
            d->aniDummyTimer->timerStop();
            d->aniDummyTimer = zfnull;
        }
    }
}

void ZFAnimation::aniImplNotifyStop(ZF_IN_OPT ZFResultTypeEnum resultType /* = ZFResultType::e_Success */) {
    if(!d->aniRunning || !d->aniImplStartFlag) {
        return;
    }
    this->_ZFP_ZFAnimation_aniReadyStop();
    ZFObject *aniTargetToRelease = this->aniTarget();

    d->aniImplStartFlag = zffalse;
    this->aniImplStop();

    if(!d->aniStoppedByUser) {
        ++(d->aniLoopCur);
        if(this->aniLoop() == zfindexMax() || d->aniLoopCur <= this->aniLoop()) {
            this->aniOnLoop();
            d->aniImplStartFlag = zftrue;
            this->aniImplStart();
            return;
        }
    }

    d->aniRunning = zffalse;
    this->aniOnStop(resultType);

    zfRelease(aniTargetToRelease);
    zfRelease(this);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAnimation
        , ZFMP_IN(ZFObject *, aniTarget)
        ) {
    this->objectOnInit();
    this->aniTarget(aniTarget);
}

void ZFAnimation::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAnimationPrivate);
}
void ZFAnimation::objectOnDealloc(void) {
    zfRetainChange(d->aniTargetHolder, zfnull);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAnimation::objectOnDeallocPrepare(void) {
    this->aniStop();
    zfsuper::objectOnDeallocPrepare();
}

ZF_NAMESPACE_GLOBAL_END

