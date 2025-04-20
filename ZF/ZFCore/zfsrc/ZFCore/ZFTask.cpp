#include "ZFTask.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFTask)

ZFEVENT_REGISTER(ZFTask, TaskOnStart)
ZFEVENT_REGISTER(ZFTask, TaskOnStop)

ZFMETHOD_DEFINE_1(ZFTask, void, start
        , ZFMP_IN_OPT(const ZFListener &, onStop, zfnull)
        ) {
    this->stop();
    _ZFP_started = zftrue;
    _ZFP_onStop = onStop;

    this->result(zfnull);
    this->resultType(v_ZFResultType::e_Success);
    this->errorHint("");

    zfRetain(this);
    this->taskOnStart();
    this->observerNotify(zfself::E_TaskOnStart());
}
ZFMETHOD_DEFINE_1(ZFTask, void, stop
        , ZFMP_IN_OPT(ZFResultType, resultType, v_ZFResultType::e_Cancel)
        ) {
    if(!_ZFP_started) {
        return;
    }
    _ZFP_started = zffalse;
    ZFListener onStopSaved = _ZFP_onStop;
    _ZFP_onStop = zfnull;
    this->resultType(resultType);
    this->taskOnStop(resultType);
    this->observerNotify(zfself::E_TaskOnStop());
    if(onStopSaved) {
        onStopSaved.execute(ZFArgs()
                .sender(this)
                .eventId(zfself::E_TaskOnStop())
                .param0(zfobj<v_ZFResultType>(resultType))
                );
    }
    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFTask, zfbool, started) {
    return _ZFP_started;
}

ZFMETHOD_DEFINE_1(ZFTask, void, notifySuccess
        , ZFMP_IN_OPT(ZFObject *, result, zfnull)
        ) {
    if(_ZFP_started) {
        this->result(result);
        this->stop(v_ZFResultType::e_Success);
    }
}
ZFMETHOD_DEFINE_2(ZFTask, void, notifyFail
        , ZFMP_IN(const zfstring &, errorHint)
        , ZFMP_IN_OPT(ZFObject *, result, zfnull)
        ) {
    if(_ZFP_started) {
        this->errorHint(errorHint);
        this->result(result);
        this->stop(v_ZFResultType::e_Fail);
    }
}


ZFOBJECT_ON_INIT_DEFINE_2(ZFTask
        , ZFMP_IN(const ZFListener &, implOnStart)
        , ZFMP_IN_OPT(const ZFListener &, implOnStop, zfnull)
        ) {
    if(implOnStart) {
        this->on(zfself::E_TaskOnStart(), implOnStart);
    }
    if(implOnStop) {
        this->on(zfself::E_TaskOnStop(), implOnStop);
    }
}

void ZFTask::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
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

ZF_NAMESPACE_GLOBAL_END

