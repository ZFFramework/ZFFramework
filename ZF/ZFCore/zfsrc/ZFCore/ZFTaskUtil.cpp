#include "ZFTaskUtil.h"
#include "ZFTimer.h"
#include "ZFThread_zfasync.h"
#include "ZFThread_zfpost.h"
#include "ZFThread_zfasyncIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFWaitTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFWaitTask
        , ZFMP_IN(zftimet, duration)
        ) {
    this->objectOnInit();
    this->duration(duration);
}

void ZFWaitTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->duration() > 0) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implOnStop
                , zfweakT<zfself>, owner
                ) {
            owner->_implTaskId = zfnull;
            owner->notifySuccess(zfargs.param0());
        } ZFLISTENER_END()
        this->_implTaskId = ZFTimerOnce(this->duration(), implOnStop);
    }
    else {
        this->notifySuccess();
    }
}
void ZFWaitTask::taskOnStop(ZF_IN ZFResultType resultType) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop(resultType);
}

// ============================================================
ZFOBJECT_REGISTER(ZFAsyncTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFAsyncTask
        , ZFMP_IN(const ZFListener &, impl)
        ) {
    this->objectOnInit();
    this->impl(impl);
}

void ZFAsyncTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->impl()) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implOnStop
                , zfweakT<zfself>, owner
                ) {
            owner->_implTaskId = zfnull;
            owner->notifySuccess(zfargs.param0());
        } ZFLISTENER_END()
        this->_implTaskId = zfasync(this->impl(), implOnStop);
    }
    else {
        this->notifySuccess();
    }
}
void ZFAsyncTask::taskOnStop(ZF_IN ZFResultType resultType) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop(resultType);
}

// ============================================================
ZFOBJECT_REGISTER(ZFPostTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFPostTask
        , ZFMP_IN(const ZFListener &, impl)
        ) {
    this->objectOnInit();
    this->impl(impl);
}

void ZFPostTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->impl()) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implRun
                , zfweakT<zfself>, owner
                ) {
            owner->_implTaskId = zfnull;
            owner->impl().execute(zfargs);
            owner->notifySuccess(zfargs.result());
        } ZFLISTENER_END()
        this->_implTaskId = zfpost(implRun);
    }
    else {
        this->notifySuccess();
    }
}
void ZFPostTask::taskOnStop(ZF_IN ZFResultType resultType) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop(resultType);
}

// ============================================================
ZFOBJECT_REGISTER(ZFAsyncIOCustomTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFAsyncIOCustomTask
        , ZFMP_IN(const ZFListener &, impl)
        ) {
    this->objectOnInit();
    this->impl(impl);
}

void ZFAsyncIOCustomTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->impl()) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implOnStop
                , zfweakT<zfself>, owner
                ) {
            owner->_implTaskId = zfnull;
            owner->notifySuccess(zfargs.param0());
        } ZFLISTENER_END()
        this->_implTaskId = zfasyncIOCustom(this->impl(), implOnStop);
    }
    else {
        this->notifySuccess();
    }
}
void ZFAsyncIOCustomTask::taskOnStop(ZF_IN ZFResultType resultType) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop(resultType);
}

// ============================================================
ZFOBJECT_REGISTER(ZFAsyncIOTask)
ZFOBJECT_ON_INIT_DEFINE_2(ZFAsyncIOTask
        , ZFMP_IN(const ZFOutput &, output)
        , ZFMP_IN(const ZFInput &, input)
        ) {
    this->objectOnInit();
    this->output(output);
    this->input(input);
}

void ZFAsyncIOTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->output() && this->input()) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implOnStop
                , zfweakT<zfself>, owner
                ) {
            owner->_implTaskId = zfnull;
            v_zfbool *result = zfargs.result();
            if(result->zfv) {
                owner->notifySuccess(result);
            }
            else {
                owner->notifyFail(zfstr("IO fail \"%s\" => \"%s\""
                            , owner->input()
                            , owner->output()
                            ), result);
            }
        } ZFLISTENER_END()
        this->_implTaskId = zfasyncIO(this->output(), this->input(), implOnStop);
    }
    else {
        this->notifySuccess();
    }
}
void ZFAsyncIOTask::taskOnStop(ZF_IN ZFResultType resultType) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop(resultType);
}

ZF_NAMESPACE_GLOBAL_END
