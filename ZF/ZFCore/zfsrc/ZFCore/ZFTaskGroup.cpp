#include "ZFTaskGroup.h"
#include "ZFTaskUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFTaskGroup)

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
    zfobj<ZFArray> childRunning;
    for(zfindex i = this->childArray()->count() - 1; i != zfindexMax(); --i) {
        childRunning->add(this->childArray()->get(i));
    }
    this->objectTag("_ZFP_ZFTaskGroupImpl", childRunning);
    zfweakT<zfself> owner = this;
    ZFLISTENER_2(childOnStop
            , zfweakT<zfself>, owner
            , zfautoT<ZFArray>, childRunning
            ) {
        ZFTask *child = zfargs.sender();
        if(!child->canceled()) {
            childRunning->removeElement(child);
            if(childRunning->isEmpty()) {
                owner->stop(v_ZFResultType::e_Success);
            }
        }
    } ZFLISTENER_END()
    for(zfindex i = childRunning->count() - 1; i != zfindexMax(); --i) {
        ZFTask *child = childRunning->get(i);
        child->start(childOnStop);
    }
}
void ZFTaskGroup::taskOnStop(void) {
    zfautoT<ZFArray> childRunning = this->objectTagRemoveAndGet("_ZFP_ZFTaskGroupImpl");
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

