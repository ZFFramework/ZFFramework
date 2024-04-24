#include "ZFAniBuilder.h"
#include "ZFAnimationGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFAniBuilderPrivate {
public:
    zfuint refCount;
    zftimet aniInterval;
    ZFObjectHolder *aniTargetHolder;
    ZFAnimationGroup *aniGroup;
    ZFAnimationGroup *aniStep;
public:
    _ZFP_ZFAniBuilderPrivate(void)
    : refCount(1)
    , aniInterval(0)
    , aniTargetHolder(zfnull)
    , aniGroup(zfnull)
    , aniStep(zfnull)
    {
        this->aniGroup = zfAlloc(ZFAnimationGroup);
        this->aniGroup->aniParallel(zffalse);
    }
    ~_ZFP_ZFAniBuilderPrivate(void) {
        this->aniStepCommit();
        zfRetainChange(this->aniStep, zfnull);
        zfRetainChange(this->aniGroup, zfnull);
        zfRetainChange(this->aniTargetHolder, zfnull);
    }
public:
    void aniStepPrepare(void) {
        if(this->aniStep == zfnull) {
            this->aniStep = zfAlloc(ZFAnimationGroup);
        }
    }
    void aniStepCommit(ZF_IN_OPT const ZFListener &cb = ZFListener()) {
        if(this->aniStep != zfnull) {
            this->aniStep->observerAddForOnce(ZFAnimation::EventAniOnStopOrInvalid(), cb);
            this->aniGroup->childAniAdd(this->aniStep);
            zfRetainChange(this->aniStep, zfnull);
        }
    }
};

// ============================================================
const ZFAniBuilder &ZFAniBuilder::ani(
        ZF_IN const zfchar *name
        , ZF_IN ZFObject *from
        , ZF_IN ZFObject *to
        , ZF_IN_OPT zftimet aniDuration /* = 0 */
        , ZF_IN_OPT ZFTimeLineCurve *aniCurve /* = zfnull */
        ) const {
    zfautoT<ZFAnimationTimeLine> ani = ZFAni(this->aniTarget(), name, from, to);
    if(ani != zfnull) {
        ani->aniInterval(this->aniInterval());
        d->aniStepPrepare();
        d->aniStep->childAniAdd(ani);
        ani->aniDuration(aniDuration);
        ani->aniCurve(aniCurve);
    }
    return *this;
}
const ZFAniBuilder &ZFAniBuilder::ani(
        ZF_IN const zfchar *name
        , ZF_IN const zfchar *from
        , ZF_IN const zfchar *to
        , ZF_IN_OPT zftimet aniDuration /* = 0 */
        , ZF_IN_OPT ZFTimeLineCurve *aniCurve /* = zfnull */
        ) const {
    zfautoT<ZFAnimationTimeLine> ani = ZFAni(this->aniTarget(), name, from, to);
    if(ani != zfnull) {
        ani->aniInterval(this->aniInterval());
        d->aniStepPrepare();
        d->aniStep->childAniAdd(ani);
        ani->aniDuration(aniDuration);
        ani->aniCurve(aniCurve);
    }
    return *this;
}

const ZFAniBuilder &ZFAniBuilder::customAni(
        ZF_IN const ZFListener &customAniCallback
        , ZF_IN_OPT zftimet aniDuration /* = 0 */
        , ZF_IN_OPT ZFTimeLineCurve *aniCurve /* = zfnull */
        ) const {
    zfautoT<ZFAnimationTimeLine> ani = ZFAni(this->aniTarget(), customAniCallback);
    if(ani != zfnull) {
        ani->aniInterval(this->aniInterval());
        d->aniStepPrepare();
        d->aniStep->childAniAdd(ani);
        ani->aniDuration(aniDuration);
        ani->aniCurve(aniCurve);
    }
    return *this;
}

const ZFAniBuilder &ZFAniBuilder::wait(ZF_IN zftimet waitTime) const {
    d->aniStepCommit();
    zfobj<ZFAnimation> ani;
    ani->aniDuration(waitTime);
    d->aniStepPrepare();
    d->aniStep->childAniAdd(ani);
    d->aniStepCommit();
    return *this;
}
const ZFAniBuilder &ZFAniBuilder::step(ZF_IN_OPT const ZFListener &cb /* = ZFListener() */) const {
    d->aniStepCommit(cb);
    return *this;
}

const ZFAniBuilder &ZFAniBuilder::aniInterval(ZF_IN const zftimet &v) const {
    d->aniInterval = v;
    return *this;
}
zftimet const &ZFAniBuilder::aniInterval(void) const {
    return d->aniInterval;
}

const ZFAniBuilder &ZFAniBuilder::aniOnEvent(
        ZF_IN zfidentity aniEvent
        , ZF_IN const ZFListener &cb
        ) const {
    d->aniGroup->observerAdd(aniEvent, cb);
    return *this;
}

const ZFAniBuilder &ZFAniBuilder::aniStart(void) const {
    d->aniStepCommit();
    d->aniGroup->aniStart();
    return *this;
}
const ZFAniBuilder &ZFAniBuilder::aniStart(ZF_IN const ZFListener &onStopOrOnInvalidCallback) const {
    this->aniOnStopOrInvalid(onStopOrOnInvalidCallback);
    this->aniStart();
    return *this;
}

ZFObject *ZFAniBuilder::aniTarget(void) const {
    return d->aniTargetHolder ? d->aniTargetHolder->objectHolded().toObject() : zfnull;
}
const ZFAniBuilder &ZFAniBuilder::aniTarget(ZF_IN ZFObject *aniTarget) const {
    zfRetainChange(d->aniTargetHolder, aniTarget ? aniTarget->objectHolder() : zfnull);
    return *this;
}
zfautoT<ZFAnimation> ZFAniBuilder::toAnimation(void) const {
    return d->aniGroup;
}

ZFAniBuilder::ZFAniBuilder(ZF_IN_OPT ZFObject *aniTarget /* = zfnull */) {
    d = zfnew(_ZFP_ZFAniBuilderPrivate);
    if(aniTarget != zfnull) {
        d->aniTargetHolder = zfRetain(aniTarget->objectHolder());
    }
}
ZFAniBuilder::ZFAniBuilder(ZF_IN const ZFAniBuilder &ref) {
    d = ref.d;
    ++(d->refCount);
}
ZFAniBuilder::~ZFAniBuilder(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}
ZFAniBuilder &ZFAniBuilder::operator = (ZF_IN const ZFAniBuilder &ref) {
    _ZFP_ZFAniBuilderPrivate *dTmp = d;
    ++(ref.d->refCount);
    d = ref.d;
    --(dTmp->refCount);
    if(dTmp->refCount == 0) {
        zfdelete(dTmp);
    }
    return *this;
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFAniBuilder, ZFAniBuilder)

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject->to<v_ZFAniBuilder *>()->zfv.aniTarget(aniTarget);
    }, v_ZFAniBuilder
    , ZFMP_IN(ZFObject *, aniTarget)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFAniBuilder, const ZFAniBuilder &, ani
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(const zfchar *, from)
        , ZFMP_IN(const zfchar *, to)
        , ZFMP_IN_OPT(zftimet, aniDuration, 0)
        , ZFMP_IN_OPT(ZFTimeLineCurve *, aniCurve, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFAniBuilder, const ZFAniBuilder &, ani
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(ZFObject *, from)
        , ZFMP_IN(ZFObject *, to)
        , ZFMP_IN_OPT(zftimet, aniDuration, 0)
        , ZFMP_IN_OPT(ZFTimeLineCurve *, aniCurve, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFAniBuilder, const ZFAniBuilder &, customAni
        , ZFMP_IN(const ZFListener &, customAniCallback)
        , ZFMP_IN_OPT(zftimet, aniDuration, 0)
        , ZFMP_IN_OPT(ZFTimeLineCurve *, aniCurve, zfnull)
        )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, wait
        , ZFMP_IN(zftimet, waitTime)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, step
        , ZFMP_IN_OPT(const ZFListener &, cb, ZFListener())
        )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, aniInterval
        , ZFMP_IN(const zftimet &, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFAniBuilder, zftimet const &, aniInterval)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, aniOnInvalid
        , ZFMP_IN(const ZFListener &, cb)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, aniOnStart
        , ZFMP_IN(const ZFListener &, cb)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, aniOnStop
        , ZFMP_IN(const ZFListener &, cb)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, aniOnStopOrInvalid
        , ZFMP_IN(const ZFListener &, cb)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAniBuilder, const ZFAniBuilder &, aniOnEvent
        , ZFMP_IN(zfidentity, aniEvent)
        , ZFMP_IN(const ZFListener &, cb)
        )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFAniBuilder, const ZFAniBuilder &, aniStart)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, aniStart
        , ZFMP_IN(const ZFListener &, onStopOrOnInvalidCallback)
        )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFAniBuilder, ZFObject *, aniTarget)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAniBuilder, const ZFAniBuilder &, aniTarget
        , ZFMP_IN(ZFObject *, aniTarget)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFAniBuilder, zfautoT<ZFAnimation>, toAnimation)

void ZFAniBuilder::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "ZFAniBuilder:";
    if(d->aniGroup != zfnull) {
        d->aniGroup->objectInfoT(ret);
    }
    else {
        ret += ZFTOKEN_zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

