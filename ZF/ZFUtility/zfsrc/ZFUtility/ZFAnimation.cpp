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
    zfweak target;
    zfbool started;
    zfbool aniImplStartFlag;
    zfbool aniImplStartRunning;
    zfbool stoppedByUser;
    ZFListener onStop;
    zfautoT<ZFTimer> aniDummyTimer;
    zfidentity aniId;
    zfindex loopCur;

public:
    _ZFP_ZFAnimationPrivate(void)
    : target(zfnull)
    , started(zffalse)
    , aniImplStartFlag(zffalse)
    , aniImplStartRunning(zffalse)
    , stoppedByUser(zffalse)
    , onStop()
    , aniDummyTimer()
    , aniId(zfidentityInvalid())
    , loopCur(0)
    {
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimation)

ZFEVENT_REGISTER(ZFAnimation, AniOnStart)
ZFEVENT_REGISTER(ZFAnimation, AniOnLoop)
ZFEVENT_REGISTER(ZFAnimation, AniOnStop)

ZFMETHOD_DEFINE_0(ZFAnimation, zftimet, durationFixed) {
    return (this->duration() > 0 ? this->duration() : ZFAnimationDurationDefault());
}

ZFMETHOD_DEFINE_1(ZFAnimation, void, target
        , ZFMP_IN(ZFObject *, target)
        ) {
    ZFCoreAssertWithMessage(!d->started, "change animation's target while animation is running");
    if(d->target != target) {
        this->aniImplTargetOnUpdate(target);
        d->target = target;
    }
}
ZFMETHOD_DEFINE_0(ZFAnimation, zfany, target) {
    return d->target;
}

ZFMETHOD_DEFINE_1(ZFAnimation, void, start
        , ZFMP_IN_OPT(const ZFListener &, onStop, zfnull)
        ) {
    this->stop();
    d->onStop = onStop;
    this->_ZFP_ZFAnimation_aniReadyStart();

    d->stoppedByUser = zffalse;
    d->loopCur = 0;
    if(!this->valid()) {
        this->aniOnStart();
        this->aniOnStop(ZFResultType::e_Fail);
        return;
    }

    zfRetain(this);
    zfRetain(this->target());

    ++(d->aniId);
    d->started = zftrue;
    d->aniImplStartFlag = zftrue;
    d->aniImplStartRunning = zftrue;
    this->aniImplStart();
    d->aniImplStartRunning = zffalse;
    this->aniOnStart();
}
ZFMETHOD_DEFINE_0(ZFAnimation, zfbool, started) {
    return d->started;
}
ZFMETHOD_DEFINE_0(ZFAnimation, void, stop) {
    if(!(d->started)) {
        return;
    }
    d->stoppedByUser = zftrue;
    ++(d->aniId);
    this->aniImplNotifyStop(ZFResultType::e_Cancel);
}
ZFMETHOD_DEFINE_0(ZFAnimation, zfbool, stoppedByUser) {
    return d->stoppedByUser;
}

ZFMETHOD_DEFINE_0(ZFAnimation, zfidentity, aniId) {
    return d->aniId;
}

ZFMETHOD_DEFINE_0(ZFAnimation, zfbool, valid) {
    return this->aniImplCheckValid();
}

ZFMETHOD_DEFINE_0(ZFAnimation, zfindex, loopCur) {
    return d->loopCur;
}

ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnStart
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::E_AniOnStart(), cb);
}
ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnLoop
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::E_AniOnLoop(), cb);
}
ZFMETHOD_DEFINE_1(ZFAnimation, void, aniOnStop
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    this->observerAdd(zfself::E_AniOnStop(), cb);
}

void ZFAnimation::_ZFP_ZFAnimation_aniReadyStart(void) {
    if(this->target() != zfnull) {
        _ZFP_I_ZFAnimationAniList *aniList = this->target()->objectTag(_ZFP_I_ZFAnimationAniList::ClassData()->classNameFull());
        if(aniList == zfnull) {
            aniList = zfAlloc(_ZFP_I_ZFAnimationAniList);
            this->target()->objectTag(_ZFP_I_ZFAnimationAniList::ClassData()->classNameFull(), aniList);
            zfRelease(aniList);
        }
        if(this->autoStopPrev()) {
            while(!aniList->aniList.isEmpty()) {
                aniList->aniList.getFirst()->stop();
            }
        }
        aniList->aniList.add(this);
    }
}
void ZFAnimation::_ZFP_ZFAnimation_aniReadyStop(void) {
    if(this->target() != zfnull) {
        _ZFP_I_ZFAnimationAniList *aniList = this->target()->objectTag(_ZFP_I_ZFAnimationAniList::ClassData()->classNameFull());
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
            this->durationFixed(),
            dummyOnFinish);
    }
}
void ZFAnimation::aniImplStop(void) {
    if(this->classData() == ZFAnimation::ClassData()) {
        if(d->aniDummyTimer != zfnull) {
            d->aniDummyTimer->stop();
            d->aniDummyTimer = zfnull;
        }
    }
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFAnimation, void, aniImplNotifyStop
        , ZFMP_IN_OPT(ZFResultType, resultType, ZFResultType::e_Success)
        )
void ZFAnimation::aniImplNotifyStop(ZF_IN_OPT ZFResultType resultType /* = ZFResultType::e_Success */) {
    if(!d->started || !d->aniImplStartFlag) {
        return;
    }
    this->_ZFP_ZFAnimation_aniReadyStop();
    ZFObject *targetToRelease = this->target();

    d->aniImplStartFlag = zffalse;
    this->aniImplStop();

    if(!d->stoppedByUser && !d->aniImplStartRunning) {
        ++(d->loopCur);
        if(this->loop() == zfindexMax() || d->loopCur <= this->loop()) {
            this->aniOnLoop();
            d->aniImplStartFlag = zftrue;
            d->aniImplStartRunning = zftrue;
            this->aniImplStart();
            d->aniImplStartRunning = zffalse;
            return;
        }
    }

    d->started = zffalse;
    ZFListener onStopSaved = d->onStop;
    d->onStop = zfnull;
    this->aniOnStop(resultType);
    if(onStopSaved) {
        onStopSaved.execute(ZFArgs()
                .sender(this)
                .eventId(zfself::E_AniOnStop())
                .param0(zfobj<v_ZFResultType>(resultType))
                );
    }

    zfRelease(targetToRelease);
    zfRelease(this);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAnimation
        , ZFMP_IN(ZFObject *, target)
        ) {
    this->objectOnInit();
    this->target(target);
}

void ZFAnimation::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAnimationPrivate);
}
void ZFAnimation::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAnimation::objectOnDeallocPrepare(void) {
    this->stop();
    zfsuper::objectOnDeallocPrepare();
}

// ============================================================
ZFOBJECT_REGISTER(ZFAniTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFAniTask
        , ZFMP_IN(ZFAnimation *, impl)
        ) {
    this->objectOnInit();
    this->impl(impl);
}

void ZFAniTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->impl()) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implOnStop
                , zfweakT<zfself>, owner
                ) {
            owner->notifySuccess();
        } ZFLISTENER_END()
        this->impl()->start(implOnStop);
    }
    else {
        this->notifySuccess();
    }
}
void ZFAniTask::taskOnStop(ZF_IN ZFResultType resultType) {
    if(this->impl()) {
        this->impl()->stop();
    }
    zfsuper::taskOnStop(resultType);
}
void ZFAniTask::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    if(this->impl()) {
        return this->impl()->objectInfoT(ret);
    }
    else {
        return zfsuper::objectInfoImpl(ret);
    }
}

ZF_NAMESPACE_GLOBAL_END

