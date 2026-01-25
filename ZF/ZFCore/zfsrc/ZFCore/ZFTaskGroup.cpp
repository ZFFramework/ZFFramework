#include "ZFTaskGroup.h"
#include "ZFTaskUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFTaskGroup)

ZFEVENT_REGISTER(ZFTaskGroup, ChildOnStart)
ZFEVENT_REGISTER(ZFTaskGroup, ChildOnStop)

ZFMETHOD_DEFINE_1(ZFTaskGroup, void, child
        , ZFMP_IN(ZFTask *, child)
        ) {
    this->childArray()->add(child);
}
ZFMETHOD_DEFINE_2(ZFTaskGroup, void, child
        , ZFMP_IN(const ZFListener &, implStart)
        , ZFMP_IN_OPT(const ZFListener &, implStop, zfnull)
        ) {
    zfobj<ZFTask> child;
    if(implStart) {
        child->observerAdd(ZFTask::E_TaskOnStart(), implStart);
    }
    if(implStop) {
        child->observerAdd(ZFTask::E_TaskOnStop(), implStop);
    }
    this->childArray()->add(child);
}

ZFMETHOD_DEFINE_1(ZFTaskGroup, void, wait
        , ZFMP_IN(zftimet, duration)
        ) {
    this->child(zfobj<ZFWaitTask>(duration).to<ZFTask *>());
}

ZFMETHOD_DEFINE_0(ZFTaskGroup, zfindex, childCount) {
    return this->childArray()->count();
}
ZFMETHOD_DEFINE_1(ZFTaskGroup, zfanyT<ZFTask>, childAt
        , ZFMP_IN(zfindex, index)
        ) {
    return this->childArray()->get(index);
}
ZFMETHOD_DEFINE_1(ZFTaskGroup, zfautoT<ZFTask>, childRemoveAt
        , ZFMP_IN(zfindex, index)
        ) {
    zfautoT<ZFTask> ret = this->childArray()->get(index);
    this->childArray()->remove(index);
    return ret;
}
ZFMETHOD_DEFINE_0(ZFTaskGroup, void, childRemoveAll) {
    this->childArray()->removeAll();
}

void ZFTaskGroup::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->childArray()->isEmpty()) {
        this->stop(v_ZFResultType::e_Success);
        return;
    }
    zfindex concurrentCount = this->concurrentCount();
    if(concurrentCount == 0) {
        concurrentCount = zfindexMax();
    }
    zfobj<ZFArray> childRunning;
    zfobj<ZFArray> childQueued;
    if(concurrentCount == zfindexMax()) {
        childRunning->addFrom(this->childArray());
    }
    else {
        for(zfindex i = 0; i < concurrentCount; ++i) {
            childRunning->add(this->childAt(i));
        }
    }
    this->objectTag("_ZFP_ZFTaskGroup_running", childRunning);
    this->objectTag("_ZFP_ZFTaskGroup_queued", childQueued);

    zfclassNotPOD _ZFP_childOnStop {
    public:
        static ZFListener a(
                ZF_IN const zfweakT<ZFTaskGroup> &owner
                , ZF_IN zfindex concurrentCount
                , ZF_IN const zfautoT<ZFArray> &childRunning
                , ZF_IN const zfautoT<ZFArray> &childQueued
                ) {
            ZFLISTENER_4(childOnStop
                    , zfweakT<ZFTaskGroup>, owner
                    , zfindex, concurrentCount
                    , zfautoT<ZFArray>, childRunning
                    , zfautoT<ZFArray>, childQueued
                    ) {
                zfauto ownerHolder = owner;
                ZFTask *child = zfargs.sender();
                owner->childOnStop(child);
                owner->observerNotify(zfself::E_ChildOnStop(), child);
                if(!child->canceled() && owner->started()) {
                    childRunning->removeElement(child);
                    if(child->success()) {
                        while(zftrue
                                && !childQueued->isEmpty()
                                && childRunning->count() <concurrentCount
                                ) {
                            zfautoT<ZFTask> child = childQueued->removeFirstAndGet();
                            childRunning->add(child);
                            child->taskPending(zftrue);
                            child->start(_ZFP_childOnStop::a(owner, concurrentCount, childRunning, childQueued));
                            owner->childOnStart(child);
                            owner->observerNotify(zfself::E_ChildOnStart(), child);
                            child->taskPending(zffalse);
                        }
                        if(childRunning->isEmpty()
                                && childRunning == owner->objectTag("_ZFP_ZFTaskGroup_running")
                                ) {
                            owner->stop(v_ZFResultType::e_Success);
                        }
                    }
                    else {
                        owner->stop(v_ZFResultType::e_Fail);
                    }
                }
            } ZFLISTENER_END()
            return childOnStop;
        }
    };
    ZFListener childOnStop = _ZFP_childOnStop::a(this, concurrentCount, childRunning, childQueued);
    for(zfindex i = 0; i < childRunning->count(); ++i) {
        ZFTask *child = childRunning->get(i);
        child->taskPending(zftrue);
        child->start(childOnStop);
        this->childOnStart(child);
        this->observerNotify(zfself::E_ChildOnStart(), child);
        child->taskPending(zffalse);
    }
}
void ZFTaskGroup::taskOnStop(void) {
    zfautoT<ZFArray> childRunning = this->objectTagRemoveAndGet("_ZFP_ZFTaskGroup_running");
    zfautoT<ZFArray> childQueued = this->objectTagRemoveAndGet("_ZFP_ZFTaskGroup_queued");
    if(childRunning != zfnull) {
        for(zfindex i = 0; i < childRunning->count(); ++i) {
            ZFTask *child = childRunning->get(i);
            child->stop(v_ZFResultType::e_Cancel);
        }
    }
    zfsuper::taskOnStop();
}

void ZFTaskGroup::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    if(this->started()) {
        zfstringAppend(ret, "%s running"
                , this->classData()->className()
                );
    }
    else {
        zfstringAppend(ret, "%s %s"
                , this->classData()->className()
                , this->resultType()
                );
        if(!this->errorHint().isEmpty()) {
            ret += ":\"";
            ret += this->errorHint();
            ret += "\"";
        }
        ret += " ";
        this->childArray()->objectInfoOfContentT(ret);
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_GLOBAL_END

