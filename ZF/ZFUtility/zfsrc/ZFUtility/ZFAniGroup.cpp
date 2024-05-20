#include "ZFAniGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFAniQueue)

// ============================================================
// _ZFP_ZFAniGroupPrivate
zfclass _ZFP_ZFAniGroupPrivate : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFAniGroupPrivate, ZFObject)

public:
    ZFAniGroup *pimplOwner;
    zfobj<ZFArray> childAnis; // ZFAnimation
private:
    /*
     * for parallel group, stores all running child
     * for serial group, stores running child and all child to run
     */
    zfobj<ZFArray> childBuf; // ZFAnimation
    ZFListener cachedOnStartListener;
    ZFListener cachedOnStopListener;
    zfobj<ZFArray> childDelayTimers; // ZFTimer

public:
    void doStart(void) {
        if(this->childAnis->isEmpty()) {
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
                    timer->timerStop();
                }
            }
            this->childDelayTimers->removeAll();
        }
        if(!this->childBuf->isEmpty()) {
            zfobj<ZFArray> childToStop(this->childBuf);
            this->childBuf->removeAll();
            if(this->pimplOwner->_ZFP_ZFAniGroup_queueType()) {
                for(zfindex i = 0; i < childToStop->count(); ++i) {
                    this->childCleanup(childToStop->get(i));
                }
                ZFAnimation *childAni = childToStop->getFirst();
                childAni->aniStop();
            }
            else {
                for(zfindex i = 0; i < childToStop->count(); ++i) {
                    ZFAnimation *childAni = childToStop->get(i);
                    this->childCleanup(childAni);
                    childAni->aniStop();
                }
            }
        }
    }

private:
    void doStartGroup(void) {
        zfidentity aniId = this->pimplOwner->aniId();
        zfobj<ZFArray> tmpArray(this->childAnis);
        this->childBuf->addFrom(this->childAnis);
        for(zfindex i = 0; aniId == this->pimplOwner->aniId() && i < tmpArray->count(); ++i) {
            ZFAnimation *childAni = tmpArray->get(i);
            this->childSetup(childAni);
            childAni->aniStart();
        }
    }
    void doStartQueue(void) {
        this->childBuf->addFrom(this->childAnis);
        ZFAnimation *childAni = this->childBuf->getFirst();
        this->childSetup(childAni);
        childAni->aniStart();
    }

private:
    void childSetup(ZF_IN ZFAnimation *childAni) {
        childAni->observerAdd(ZFAnimation::EventAniOnStart(), this->cachedOnStartListener);
        childAni->observerAdd(ZFAnimation::EventAniOnStop(), this->cachedOnStopListener);
    }
    void childCleanup(ZF_IN ZFAnimation *childAni) {
        childAni->observerRemove(ZFAnimation::EventAniOnStart(), this->cachedOnStartListener);
        childAni->observerRemove(ZFAnimation::EventAniOnStop(), this->cachedOnStopListener);
    }
private:
    ZFAnimation *checkChild(
            ZF_IN ZFObject *childAni
            , ZF_IN zfbool autoRemove = zffalse
            ) {
        for(zfindex i = 0; i < this->childBuf->count(); ++i) {
            ZFAnimation *tmp = this->childBuf->get(i);
            if(tmp == childAni) {
                if(autoRemove) { // retained in childAnis, safe to be removed directly
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
        ZFAnimation *childAni = this->checkChild(zfargs.sender());
        if(childAni == zfnull) {
            return;
        }
        this->pimplOwner->childAniOnStart(childAni);
    }
    ZFMETHOD_INLINE_1(void, onChildStop_group
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        ZFAnimation *childAni = this->checkChild(zfargs.sender(), zfHint("autoRemove")zftrue);
        if(childAni == zfnull) {
            return;
        }
        this->childCleanup(childAni);
        this->pimplOwner->childAniOnStop(childAni);

        if(this->childBuf->isEmpty()) {
            this->pimplOwner->aniImplNotifyStop();
        }
    }
    ZFMETHOD_INLINE_1(void, onChildStart_queue
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        ZFAnimation *childAni = this->childBuf->getFirst();
        if(childAni == zfnull || childAni != zfargs.sender()) {
            return;
        }
        this->pimplOwner->childAniOnStart(childAni);
    }
    ZFMETHOD_INLINE_1(void, onChildStop_queue
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        ZFAnimation *childAni = this->childBuf->getFirst();
        if(childAni == zfnull || childAni != zfargs.sender()) {
            return;
        }
        this->childBuf->removeFirst();
        this->childCleanup(childAni);
        this->pimplOwner->childAniOnStop(childAni);

        if(this->childBuf->isEmpty()) {
            this->pimplOwner->aniImplNotifyStop();
        }
        else {
            childAni = this->childBuf->getFirst();
            this->childSetup(childAni);
            childAni->aniStart();
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniGroup)

ZFEVENT_REGISTER(ZFAniGroup, ChildAniOnStart)
ZFEVENT_REGISTER(ZFAniGroup, ChildAniOnStop)

// ============================================================
// serialize
zfbool ZFAniGroup::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->aniRemoveAll();

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &categoryData = serializableData.childAt(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfstringIsEqual(category, ZFSerializableKeyword_ZFAniGroup_child)) {
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
            this->ani(element);

            categoryData.resolveMark();
        }
    }
    return zftrue;
}
zfbool ZFAniGroup::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);

    if(ref == zfnull) {
        for(zfindex i = 0; i < d->childAnis->count(); ++i) {
            ZFSerializableData elementData;
            if(this->autoUpdateDuration()) {
                ZFAnimation *child = d->childAnis->get(i);
                ZFPropertyValueReset(ZFPropertyAccess(ZFAnimation, aniDuration), child);
            }
            if(!ZFObjectToDataT(elementData, d->childAnis->get(i), outErrorHint)) {
                return zffalse;
            }
            elementData.category(ZFSerializableKeyword_ZFAniGroup_child);
            serializableData.childAdd(elementData);
        }
    }
    else {
        zfbool mismatch = zffalse;
        if(this->aniCount() != ref->aniCount()) {
            mismatch = zftrue;
        }
        else {
            for(zfindex i = 0; i < d->childAnis->count(); ++i) {
                if(ZFObjectCompare(d->childAnis->get(i), ref->d->childAnis->get(i)) != ZFCompareTheSame) {
                    mismatch = zftrue;
                    break;
                }
            }
        }
        if(mismatch) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "animation group contents mismatch, this: %s, ref: %s",
                d->childAnis->objectInfoOfContent(), ref->d->childAnis->objectInfoOfContent());
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
    this->aniStop();
    this->aniRemoveAll();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFAniGroup::objectHash(void) {
    return d->childAnis->objectHash();
}
ZFCompareResult ZFAniGroup::objectCompare(ZF_IN ZFObject *anotherObj) {
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame) {
        return ZFCompareUncomparable;
    }
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    return ZFObjectCompare(d->childAnis, another->d->childAnis);
}

// ============================================================
// child animation control
ZFMETHOD_DEFINE_2(ZFAniGroup, void, ani
        , ZFMP_IN(ZFAnimation *, ani)
        , ZFMP_IN_OPT(zfindex, index, zfindexMax())
        ) {
    if(ani == zfnull) {
        return;
    }
    zfCoreAssertWithMessage(!this->aniRunning(), "you must not modify child animation while group is running");
    d->childAnis->add(ani, index);
}
ZFMETHOD_DEFINE_0(ZFAniGroup, zfindex, aniCount) {
    return d->childAnis->count();
}
ZFMETHOD_DEFINE_1(ZFAniGroup, ZFAnimation *, aniAt
        , ZFMP_IN(zfindex, index)
        ) {
    return d->childAnis->get(index);
}
ZFMETHOD_DEFINE_1(ZFAniGroup, void, aniRemoveAt
        , ZFMP_IN(zfindex, index)
        ) {
    d->childAnis->remove(index);
}
ZFMETHOD_DEFINE_0(ZFAniGroup, void, aniRemoveAll) {
    d->childAnis->removeAll();
}

// ============================================================
// start stop
zfbool ZFAniGroup::aniImplCheckValid(void) {
    if(!zfsuper::aniImplCheckValid()) {
        return zffalse;
    }
    for(zfindex i = 0; i < d->childAnis->count(); ++i) {
        ZFAnimation *childAni = d->childAnis->get(i);
        if(this->autoUpdateTarget() && this->aniTarget() != zfnull) {
            childAni->aniTarget(this->aniTarget());
        }
        if(this->autoUpdateDuration()) {
            childAni->aniDuration(this->aniDuration());
        }
        if(!childAni->aniValid()) {
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

void ZFAniGroup::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    d->childAnis->objectInfoT(ret);
}

ZF_NAMESPACE_GLOBAL_END

