#include "ZFAniGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFAniGroupChildData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFAniGroupChildData, ZFObject)
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, child)
    ZFPROPERTY_ASSIGN(zfweak, childTarget)
    ZFPROPERTY_ASSIGN(zftimet, childDuration)
    ZFPROPERTY_ASSIGN(zfautoT<ZFCurve>, childCurve)

public:
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ZFObjectInfoT(ret, this->child());
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFAniGroupChildData)

// ============================================================
// _ZFP_ZFAniGroupPrivate
zfclass _ZFP_ZFAniGroupPrivate : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFAniGroupPrivate, ZFObject)

public:
    ZFAniGroup *pimplOwner;
    zfobj<ZFArray> childDatas; // _ZFP_ZFAniGroupChildData
private:
    /*
     * for parallel group, stores all running child
     * for serial group, stores running child and all child to run
     */
    zfobj<ZFArray> childBuf; // _ZFP_ZFAniGroupChildData
    ZFListener cachedOnStartListener;
    ZFListener cachedOnStopListener;
    zfobj<ZFArray> childDelayTimers; // ZFTimer

public:
    void doStart(void) {
        if(this->childDatas->isEmpty()) {
            this->pimplOwner->aniImplNotifyStop();
            return;
        }
        if(!this->cachedOnStartListener) {
            if(this->pimplOwner->_ZFP_ZFAniGroup_queueType()) {
                this->cachedOnStartListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAniGroupPrivate, onChildStart_queue));
                this->cachedOnStopListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAniGroupPrivate, onChildStop_queue));
            }
            else {
                this->cachedOnStartListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAniGroupPrivate, onChildStart_group));
                this->cachedOnStopListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAniGroupPrivate, onChildStop_group));
            }
        }

        if(this->pimplOwner->_ZFP_ZFAniGroup_queueType()) {
            this->doStartQueue();
        }
        else {
            this->doStartGroup();
        }
    }
    void doStop(void) {
        if(!this->childDelayTimers->isEmpty()) {
            for(zfindex i = 0; i < this->childDelayTimers->count(); ++i) {
                ZFTimer *timer = this->childDelayTimers->get(i);
                if(timer != zfnull) {
                    timer->stop();
                }
            }
            this->childDelayTimers->removeAll();
        }
        if(!this->childBuf->isEmpty()) {
            zfobj<ZFArray> childToStop;
            childToStop->addFrom(this->childBuf);
            this->childBuf->removeAll();
            if(this->pimplOwner->_ZFP_ZFAniGroup_queueType()) {
                for(zfindex i = 0; i < childToStop->count(); ++i) {
                    this->childCleanup(childToStop->get(i));
                }
                _ZFP_ZFAniGroupChildData *childData = childToStop->getFirst();
                childData->child()->stop();
            }
            else {
                for(zfindex i = 0; i < childToStop->count(); ++i) {
                    _ZFP_ZFAniGroupChildData *childData = childToStop->get(i);
                    this->childCleanup(childData);
                    childData->child()->stop();
                }
            }
        }
    }

private:
    void doStartGroup(void) {
        zfidentity aniId = this->pimplOwner->aniId();
        zfobj<ZFArray> tmpArray(this->childDatas);
        this->childBuf->addFrom(this->childDatas);
        for(zfindex i = 0; aniId == this->pimplOwner->aniId() && i < tmpArray->count(); ++i) {
            _ZFP_ZFAniGroupChildData *childData = tmpArray->get(i);
            this->childSetup(childData);
            childData->child()->start();
        }
    }
    void doStartQueue(void) {
        this->childBuf->addFrom(this->childDatas);
        _ZFP_ZFAniGroupChildData *childData = this->childBuf->getFirst();
        this->childSetup(childData);
        childData->child()->start();
    }

private:
    void childSetup(ZF_IN _ZFP_ZFAniGroupChildData *childData) {
        childData->child()->observerAdd(ZFAnimation::E_AniOnStart(), this->cachedOnStartListener);
        childData->child()->observerAdd(ZFAnimation::E_AniOnStop(), this->cachedOnStopListener);
    }
    void childCleanup(ZF_IN _ZFP_ZFAniGroupChildData *childData) {
        childData->child()->observerRemove(ZFAnimation::E_AniOnStart(), this->cachedOnStartListener);
        childData->child()->observerRemove(ZFAnimation::E_AniOnStop(), this->cachedOnStopListener);
    }
private:
    _ZFP_ZFAniGroupChildData *checkChild(
            ZF_IN ZFObject *childAni
            , ZF_IN zfbool autoRemove = zffalse
            ) {
        for(zfindex i = 0; i < this->childBuf->count(); ++i) {
            _ZFP_ZFAniGroupChildData *tmp = this->childBuf->get(i);
            if(tmp->child() == childAni) {
                if(autoRemove) { // retained in childDatas, safe to be removed directly
                    this->childBuf->remove(i);
                }
                return tmp;
            }
        }
        return zfnull;
    }
private:
    ZFMETHOD_INLINE_1(void, onChildStart_group
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        _ZFP_ZFAniGroupChildData *childData = this->checkChild(zfargs.sender());
        if(childData == zfnull) {
            return;
        }
        this->pimplOwner->childAniOnStart(childData->child());
    }
    ZFMETHOD_INLINE_1(void, onChildStop_group
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        _ZFP_ZFAniGroupChildData *childData = this->checkChild(zfargs.sender(), ZF_HINT("autoRemove")zftrue);
        if(childData == zfnull) {
            return;
        }
        this->childCleanup(childData);
        this->pimplOwner->childAniOnStop(childData->child());

        if(this->childBuf->isEmpty()) {
            this->pimplOwner->aniImplNotifyStop();
        }
    }
    ZFMETHOD_INLINE_1(void, onChildStart_queue
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        _ZFP_ZFAniGroupChildData *childData = this->childBuf->getFirst();
        if(childData == zfnull || childData->child() != zfargs.sender()) {
            return;
        }
        this->pimplOwner->childAniOnStart(childData->child());
    }
    ZFMETHOD_INLINE_1(void, onChildStop_queue
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        zfautoT<_ZFP_ZFAniGroupChildData> childData = this->childBuf->getFirst();
        if(childData == zfnull || childData->child() != zfargs.sender()) {
            return;
        }
        this->childBuf->removeFirst();
        this->childCleanup(childData);
        this->pimplOwner->childAniOnStop(childData->child());

        if(this->childBuf->isEmpty()) {
            this->pimplOwner->aniImplNotifyStop();
        }
        else {
            childData = this->childBuf->getFirst();
            this->childSetup(childData);
            childData->child()->start();
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniGroup)

ZFEVENT_REGISTER(ZFAniGroup, ChildAniOnStart)
ZFEVENT_REGISTER(ZFAniGroup, ChildAniOnStop)

// ============================================================
zftimet ZFAniGroup::durationFixed(void) {
    zftimet ret = 0;
    if(this->_ZFP_ZFAniGroup_queueType()) {
        for(zfindex i = 0; i < this->childCount(); ++i) {
            ZFAnimation *ani = this->childAt(i);
            if(this->autoUpdateDuration() && this->duration() != 0) {
                ani->duration(this->duration());
            }
            zftimet childDuration = this->childDurationAt(i);
            ret += (childDuration == 0 ? ani->durationFixed() : childDuration);
        }
    }
    else {
        for(zfindex i = 0; i < this->childCount(); ++i) {
            ZFAnimation *ani = this->childAt(i);
            if(this->autoUpdateDuration() && this->duration() != 0) {
                ani->duration(this->duration());
            }
            zftimet childDuration = this->childDurationAt(i);
            ret = zfmMax<zftimet>(ret, childDuration == 0 ? ani->durationFixed() : childDuration);
        }
    }
    return (ret != 0)
        ? ret
        : zfsuper::durationFixed()
        ;
}

// ============================================================
// serialize
zfbool ZFAniGroup::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->childRemoveAll();

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &categoryData = serializableData.childAt(i);
        if(categoryData.resolved()) {continue;}
        zfstring category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfstringIsEqual(category, ZFSerializableKeyword_ZFAniGroup_child)) {
            zftimet childDuration = 0;
            ZFSerializableUtilSerializeAttrFromData(categoryData, outErrorHint, outErrorPos,
                    check, ZFSerializableKeyword_ZFAniGroup_childDuration, zftimet, childDuration, {
                        return zffalse;
                    });

            zfautoT<ZFCurve> childCurve;
            ZFSerializableUtilSerializeCategoryFromData(categoryData, outErrorHint, outErrorPos,
                    check, ZFSerializableKeyword_ZFAniGroup_childCurve, ZFObject, childCurve, {
                        return zffalse;
                    });

            zfauto element;
            if(!ZFObjectFromDataT(element, categoryData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            if(element == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                    "null child");
                return zffalse;
            }
            if(!element->classData()->classIsTypeOf(ZFAnimation::ClassData())) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                    "object %s not type of %s", element->objectInfoOfInstance(), ZFAnimation::ClassData());
                return zffalse;
            }
            this->child(element);
            if(childDuration != 0) {
                this->childDurationAt(this->childCount() - 1, childDuration);
            }
            if(childCurve) {
                this->childCurveAt(this->childCount() - 1, childCurve);
            }

            categoryData.resolveMark();
        }
    }
    return zftrue;
}
zfbool ZFAniGroup::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    if(ref == zfnull) {
        for(zfindex i = 0; i < this->childCount(); ++i) {
            ZFSerializableData elementData;
            ZFAnimation *child = this->childAt(i);

            if(this->autoUpdateDuration() && this->duration() != 0) {
                ZFPropertyValueReset(ZFPropertyAccess(ZFAnimation, duration), child);
            }
            if(this->autoUpdateCurve() && this->curve()) {
                ZFPropertyValueReset(ZFPropertyAccess(ZFAnimation, curve), child);
            }
            if(!ZFObjectToDataT(elementData, child, outErrorHint)) {
                return zffalse;
            }

            ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                    ZFSerializableKeyword_ZFAniGroup_childDuration, zftimet, this->childDurationAt(i), (zftimet)0, {
                        return zffalse;
                    });
            ZFSerializableUtilSerializeCategoryToDataNoRef(serializableData, outErrorHint,
                    ZFSerializableKeyword_ZFAniGroup_childCurve, ZFObject, this->childCurveAt(i), zfnull, {
                        return zffalse;
                    });
            elementData.category(ZFSerializableKeyword_ZFAniGroup_child);
            serializableData.child(elementData);
        }
    }
    else {
        zfbool mismatch = zffalse;
        if(this->childCount() != ref->childCount()) {
            mismatch = zftrue;
        }
        else {
            for(zfindex i = 0; i < this->childCount(); ++i) {
                if(ZFObjectCompareValue(this->childAt(i), ref->childAt(i)) != ZFCompareEqual
                        || this->childDurationAt(i) != ref->childDurationAt(i)
                        || ZFObjectCompareValue(this->childCurveAt(i), ref->childCurveAt(i)) != ZFCompareEqual
                        ) {
                    mismatch = zftrue;
                    break;
                }
            }
        }
        if(mismatch) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "animation group contents mismatch, this: %s, ref: %s",
                d->childDatas->objectInfoOfContent(), ref->d->childDatas->objectInfoOfContent());
            return zffalse;
        }
    }

    return zftrue;
}

// ============================================================
// object
void ZFAniGroup::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZFAniGroupPrivate);
    d->pimplOwner = this;
}
void ZFAniGroup::objectOnDealloc(void) {
    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAniGroup::objectOnDeallocPrepare(void) {
    this->stop();
    this->childRemoveAll();
    zfsuper::objectOnDeallocPrepare();
}
ZFCompareResult ZFAniGroup::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zffalse
            || this->childCount() != another->childCount()
            || this->autoUpdateTarget() != another->autoUpdateTarget()
            || this->autoUpdateDuration() != another->autoUpdateDuration()
            || this->autoUpdateCurve() != another->autoUpdateCurve()
            ) {
        return ZFCompareUncomparable;
    }
    for(zfindex i = 0; i < this->childCount(); ++i) {
        if(ZFObjectCompareValue(this->childAt(i), another->childAt(i)) != ZFCompareEqual) {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareEqual;
}

// ============================================================
// child animation control
ZFMETHOD_DEFINE_2(ZFAniGroup, void, child
        , ZFMP_IN(ZFAnimation *, ani)
        , ZFMP_IN_OPT(zfindex, index, zfindexMax())
        ) {
    if(ani == zfnull) {
        return;
    }
    ZFCoreAssertWithMessage(!this->started(), "you must not modify child animation while group is running");
    zfobj<_ZFP_ZFAniGroupChildData> childData;
    childData->child(ani);
    d->childDatas->add(childData, index);
}
ZFMETHOD_DEFINE_0(ZFAniGroup, zfindex, childCount) {
    return d->childDatas->count();
}
ZFMETHOD_DEFINE_1(ZFAniGroup, zfanyT<ZFAnimation>, childAt
        , ZFMP_IN(zfindex, index)
        ) {
    _ZFP_ZFAniGroupChildData *childData = d->childDatas->get(index);
    return childData->child();
}
ZFMETHOD_DEFINE_1(ZFAniGroup, void, childRemoveAt
        , ZFMP_IN(zfindex, index)
        ) {
    ZFCoreAssertWithMessage(!this->started(), "you must not modify child animation while group is running");
    d->childDatas->remove(index);
}
ZFMETHOD_DEFINE_0(ZFAniGroup, void, childRemoveAll) {
    ZFCoreAssertWithMessage(!this->started(), "you must not modify child animation while group is running");
    d->childDatas->removeAll();
}

ZFMETHOD_DEFINE_1(ZFAniGroup, zfany, childTargetAt
        , ZFMP_IN(zfindex , index)
        ) {
    _ZFP_ZFAniGroupChildData *childData = d->childDatas->get(index);
    return childData->childTarget();
}
ZFMETHOD_DEFINE_2(ZFAniGroup, void, childTargetAt
        , ZFMP_IN(zfindex , index)
        , ZFMP_IN(zfany, target)
        ) {
    _ZFP_ZFAniGroupChildData *childData = d->childDatas->get(index);
    childData->childTarget(target);
}

ZFMETHOD_DEFINE_1(ZFAniGroup, zftimet, childDurationAt
        , ZFMP_IN(zfindex , index)
        ) {
    _ZFP_ZFAniGroupChildData *childData = d->childDatas->get(index);
    return childData->childDuration();
}
ZFMETHOD_DEFINE_2(ZFAniGroup, void, childDurationAt
        , ZFMP_IN(zfindex , index)
        , ZFMP_IN(zftimet, duration)
        ) {
    _ZFP_ZFAniGroupChildData *childData = d->childDatas->get(index);
    childData->childDuration(duration);
}

ZFMETHOD_DEFINE_1(ZFAniGroup, zfautoT<ZFCurve>, childCurveAt
        , ZFMP_IN(zfindex , index)
        ) {
    _ZFP_ZFAniGroupChildData *childData = d->childDatas->get(index);
    return childData->childCurve();
}
ZFMETHOD_DEFINE_2(ZFAniGroup, void, childCurveAt
        , ZFMP_IN(zfindex , index)
        , ZFMP_IN(ZFCurve *, curve)
        ) {
    _ZFP_ZFAniGroupChildData *childData = d->childDatas->get(index);
    childData->childCurve(curve);
}

// ============================================================
// start stop
zfbool ZFAniGroup::aniImplCheckValid(void) {
    if(!zfsuper::aniImplCheckValid()) {
        return zffalse;
    }
    for(zfindex i = 0; i < this->childCount(); ++i) {
        ZFAnimation *childAni = this->childAt(i);
        if(this->autoUpdateTarget()) {
            ZFObject *childTarget = this->childTargetAt(i);
            if(childTarget == zfnull) {
                if(this->target()) {
                    childAni->target(this->target());
                }
            }
            else {
                childAni->target(childTarget);
            }
        }
        if(this->autoUpdateDuration()) {
            zftimet childDuration = this->childDurationAt(i);
            if(childDuration == 0) {
                if(this->duration() != 0) {
                    childAni->duration(this->duration());
                }
            }
            else {
                childAni->duration(childDuration);
            }
        }
        if(this->autoUpdateCurve()) {
            zfautoT<ZFCurve> childCurve = this->childCurveAt(i);
            if(!childCurve) {
                if(this->curve()) {
                    childAni->curve(this->curve());
                }
            }
            else {
                childAni->curve(childCurve);
            }
        }
        if(!childAni->valid()) {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFAniGroup::aniImplStart(void) {
    zfsuper::aniImplStart();
    d->doStart();
}
void ZFAniGroup::aniImplStop(void) {
    d->doStop();
    zfsuper::aniImplStop();
}

void ZFAniGroup::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    d->childDatas->objectInfoT(ret);
}

// ============================================================
// util for chained call
ZFMETHOD_DEFINE_1(ZFAniGroup, void, childTarget
        , ZFMP_IN(ZFObject *, target)
        ) {
    if(this->childCount() > 0) {
        this->childTargetAt(this->childCount() - 1, target);
    }
}
ZFMETHOD_DEFINE_1(ZFAniGroup, void, childDuration
        , ZFMP_IN(zftimet, duration)
        ) {
    if(this->childCount() > 0) {
        this->childDurationAt(this->childCount() - 1, duration);
    }
}
ZFMETHOD_DEFINE_1(ZFAniGroup, void, childCurve
        , ZFMP_IN(ZFCurve *, curve)
        ) {
    if(this->childCount() > 0) {
        this->childCurveAt(this->childCount() - 1, curve);
    }
}

ZFMETHOD_DEFINE_1(ZFAniGroup, void, childOnStart
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    if(this->childCount() > 0) {
        ZFAnimation *child = this->childAt(this->childCount() - 1);
        child->aniOnStart(cb);
    }
}
ZFMETHOD_DEFINE_1(ZFAniGroup, void, childOnLoop
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    if(this->childCount() > 0) {
        ZFAnimation *child = this->childAt(this->childCount() - 1);
        child->aniOnLoop(cb);
    }
}
ZFMETHOD_DEFINE_1(ZFAniGroup, void, childOnStop
        , ZFMP_IN(const ZFListener &, cb)
        ) {
    if(this->childCount() > 0) {
        ZFAnimation *child = this->childAt(this->childCount() - 1);
        child->aniOnStop(cb);
    }
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFAniGroup, void, delay
        , ZFMP_IN_OPT(zftimet, duration, 0)
        ) {
    this->child(zfobj<ZFAnimation>());
    this->childDuration(duration);
}
zfclass _ZFP_I_ZFAniGroupStep : zfextend ZFAnimation {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFAniGroupStep, ZFAnimation)
protected:
    _ZFP_I_ZFAniGroupStep(void) : runImpl(), cancelImpl(), needCancel(zffalse) {}
public:
    ZFListener runImpl;
    ZFListener cancelImpl;
    zfbool needCancel;
protected:
    zfoverride
    virtual void aniImplStart(void) {
        zfsuper::aniImplStart();
        if(this->cancelImpl) {
            zfself *owner = this;
            ZFLISTENER_1(implStop
                    , zfweakT<zfself>, owner
                    ) {
                owner->needCancel = zffalse;
                owner->aniImplNotifyStop();
            } ZFLISTENER_END()
            this->needCancel = zftrue;
            this->runImpl.execute(ZFArgs()
                    .sender(this)
                    .param0(zfobj<ZFTaskIdBasic>(implStop))
                    );
        }
        else {
            this->runImpl.execute(ZFArgs().sender(this));
            this->aniImplNotifyStop();
        }
    }
    zfoverride
    virtual void aniImplStop(void) {
        if(this->needCancel) {
            this->needCancel = zffalse;
            this->cancelImpl.execute(ZFArgs().sender(this));
        }
        zfsuper::aniImplStop();
    }
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        if(zffalse
                || !this->runImpl != another->runImpl
                || !this->cancelImpl != another->cancelImpl
                ) {
            return ZFCompareUncomparable;
        }
        return ZFCompareEqual;
    }
};
ZFMETHOD_DEFINE_2(ZFAniGroup, void, step
        , ZFMP_IN(const ZFListener &, runImpl)
        , ZFMP_IN_OPT(const ZFListener &, cancelImpl, zfnull)
        ) {
    if(runImpl) {
        zfobj<_ZFP_I_ZFAniGroupStep> step;
        step->runImpl = runImpl;
        step->cancelImpl = cancelImpl;
        this->child(step);
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFAniQueue)

ZF_NAMESPACE_GLOBAL_END

