#include "ZFTaskQueue.h"
#include "ZFTaskUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTaskQueue)

ZFEVENT_REGISTER(ZFTaskQueue, ChildOnStart)
ZFEVENT_REGISTER(ZFTaskQueue, ChildOnStop)

ZFMETHOD_DEFINE_1(ZFTaskQueue, void, child
        , ZFMP_IN(ZFTask *, child)
        ) {
    this->childArray()->add(child);
}
ZFMETHOD_DEFINE_2(ZFTaskQueue, void, child
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

ZFMETHOD_DEFINE_1(ZFTaskQueue, void, wait
        , ZFMP_IN(zftimet, duration)
        ) {
    this->child(zfobj<ZFWaitTask>(duration).to<ZFTask *>());
}

ZFMETHOD_DEFINE_0(ZFTaskQueue, zfindex, childCount) {
    return this->childArray()->count();
}
ZFMETHOD_DEFINE_1(ZFTaskQueue, zfanyT<ZFTask>, childAt
        , ZFMP_IN(zfindex, index)
        ) {
    return this->childArray()->get(index);
}
ZFMETHOD_DEFINE_1(ZFTaskQueue, zfautoT<ZFTask>, childRemoveAt
        , ZFMP_IN(zfindex, index)
        ) {
    zfautoT<ZFTask> ret = this->childArray()->get(index);
    this->childArray()->remove(index);
    return ret;
}
ZFMETHOD_DEFINE_0(ZFTaskQueue, void, childRemoveAll) {
    this->childArray()->removeAll();
}

void ZFTaskQueue::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->childArray()->isEmpty()) {
        this->stop(v_ZFResultType::e_Success);
        return;
    }
    zfobj<ZFArray> childQueue;
    childQueue->addFrom(this->childArray());
    this->objectTag("_ZFP_ZFTaskQueueImpl", childQueue);

    zfclassNotPOD _ZFP_startNext {
    public:
        static void a(
                ZF_IN ZFTaskQueue *owner
                , ZF_IN const zfautoT<ZFArray> &childQueue
                ) {
            ZFLISTENER_2(childOnStop
                    , zfweakT<ZFTaskQueue>, owner
                    , zfautoT<ZFArray>, childQueue
                    ) {
                ZFTask *child = zfargs.sender();
                owner->childOnStop(child);
                owner->observerNotify(zfself::E_ChildOnStop(), child);
                if(!child->canceled() && owner->started()) {
                    if(childQueue->getFirst() == child) {
                        childQueue->removeFirst();
                    }
                    if(child->success()) {
                        if(childQueue->isEmpty()) {
                            owner->stop(v_ZFResultType::e_Success);
                        }
                        else {
                            _ZFP_startNext::a(owner, childQueue);
                        }
                    }
                    else {
                        owner->stop(v_ZFResultType::e_Fail);
                    }
                }
            } ZFLISTENER_END()
            zfautoT<ZFTask> child = childQueue->getFirst();
            child->start(childOnStop);
            owner->childOnStart(child);
            owner->observerNotify(zfself::E_ChildOnStart(), child);
        }
    };
    _ZFP_startNext::a(this, childQueue);
}
void ZFTaskQueue::taskOnStop(void) {
    zfautoT<ZFArray> childQueue = this->objectTagRemoveAndGet("_ZFP_ZFTaskQueueImpl");
    if(childQueue != zfnull) {
        ZFTask *child = childQueue->getFirst();
        if(child != zfnull) {
            child->stop(v_ZFResultType::e_Cancel);
        }
    }
    zfsuper::taskOnStop();
}

void ZFTaskQueue::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
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

