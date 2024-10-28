#include "ZFTask.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFTask)

ZFEVENT_REGISTER(ZFTask, TaskOnStart)
ZFEVENT_REGISTER(ZFTask, TaskOnStop)

ZFMETHOD_DEFINE_1(ZFTask, void, start
        , ZFMP_IN_OPT(const ZFListener &, onStop, zfnull)
        ) {
    if(_ZFP_started) {
        return;
    }
    _ZFP_started = zftrue;
    _ZFP_onStop = onStop;

    this->implData(zfnull);
    this->result(zfnull);
    this->resultType(ZFResultType::e_Success);
    this->errorHint("");

    this->taskOnStart();
    this->observerNotify(zfself::EventTaskOnStart());
}
ZFMETHOD_DEFINE_1(ZFTask, void, stop
        , ZFMP_IN_OPT(ZFResultTypeEnum, resultType, ZFResultType::e_Cancel)
        ) {
    if(!_ZFP_started) {
        return;
    }
    _ZFP_started = zffalse;
    this->resultType(resultType);
    this->taskOnStop();
    this->observerNotify(zfself::EventTaskOnStop());
    if(_ZFP_onStop) {
        ZFListener tmp = _ZFP_onStop;
        _ZFP_onStop = zfnull;
        tmp.execute(ZFArgs()
                .sender(this)
                );
    }
}
ZFMETHOD_DEFINE_0(ZFTask, zfbool, started) {
    return _ZFP_started;
}

ZFMETHOD_DEFINE_1(ZFTask, void, notifySuccess
        , ZFMP_IN_OPT(ZFObject *, result, zfnull)
        ) {
    this->result(result);
    this->stop(ZFResultType::e_Success);
}
ZFMETHOD_DEFINE_1(ZFTask, void, notifyFail
        , ZFMP_IN(const zfstring &, errorHint)
        ) {
    this->errorHint(errorHint);
    this->stop(ZFResultType::e_Fail);
}

void ZFTask::objectInfoT(ZF_IN_OUT zfstring &ret) {
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
        if(this->result() != zfnull) {
            ret += ", result: ";
            this->result()->objectInfoT(ret);
        }
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

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
        child->observerAdd(ZFTask::EventTaskOnStart(), implStart);
    }
    if(implStop) {
        child->observerAdd(ZFTask::EventTaskOnStop(), implStop);
    }
    this->childArray()->add(child);
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
        this->stop(ZFResultType::e_Success);
        return;
    }
    zfobj<ZFArray> childRunning;
    childRunning->addFrom(this->childArray());
    this->implData(childRunning);
    zfweakT<zfself> owner = this;
    ZFLISTENER_2(childOnStop
            , zfweakT<zfself>, owner
            , zfautoT<ZFArray>, childRunning
            ) {
        ZFTask *child = zfargs.sender();
        if(child->resultType() != ZFResultType::e_Cancel) {
            childRunning->removeElement(child);
            if(childRunning->isEmpty()) {
                owner->stop(ZFResultType::e_Success);
            }
        }
    } ZFLISTENER_END()
    for(zfindex i = 0; i < childRunning->count(); ++i) {
        ZFTask *child = childRunning->get(i);
        child->start(childOnStop);
    }
}
void ZFTaskGroup::taskOnStop(void) {
    ZFArray *childRunning = this->implData();
    if(childRunning != zfnull) {
        for(zfindex i = 0; i < childRunning->count(); ++i) {
            ZFTask *child = childRunning->get(i);
            child->stop();
        }
    }
    zfsuper::taskOnStop();
}

void ZFTaskGroup::objectInfoT(ZF_IN_OUT zfstring &ret) {
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

// ============================================================
ZFOBJECT_REGISTER(ZFTaskQueue)

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
        child->observerAdd(ZFTask::EventTaskOnStart(), implStart);
    }
    if(implStop) {
        child->observerAdd(ZFTask::EventTaskOnStop(), implStop);
    }
    this->childArray()->add(child);
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

static void _ZFP_ZFTaskQueue_startNext(
        ZF_IN ZFTaskQueue *owner
        ) {
    ZFLISTENER_1(childOnStop
            , zfweakT<ZFTaskQueue>, owner
            ) {
        ZFTask *child = zfargs.sender();
        if(child->resultType() != ZFResultType::e_Cancel) {
            ZFArray *childQueue = owner->implData();
            if(childQueue->getFirst() == child) {
                childQueue->removeFirst();
            }
            if(childQueue->isEmpty()) {
                owner->stop(ZFResultType::e_Success);
            }
            else {
                _ZFP_ZFTaskQueue_startNext(owner);
            }
        }
    } ZFLISTENER_END()
    ZFArray *childQueue = owner->implData();
    zfautoT<ZFTask> child = childQueue->getFirst();
    child->start(childOnStop);
}
void ZFTaskQueue::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->childArray()->isEmpty()) {
        this->stop(ZFResultType::e_Success);
        return;
    }
    zfobj<ZFArray> childQueue;
    childQueue->addFrom(this->childArray());
    this->implData(childQueue);
    _ZFP_ZFTaskQueue_startNext(this);
}
void ZFTaskQueue::taskOnStop(void) {
    ZFArray *childQueue = this->implData();
    if(childQueue != zfnull) {
        ZFTask *child = childQueue->getFirst();
        if(child != zfnull) {
            child->stop();
        }
    }
    zfsuper::taskOnStop();
}

void ZFTaskQueue::objectInfoT(ZF_IN_OUT zfstring &ret) {
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
