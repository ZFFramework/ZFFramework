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
void ZFWaitTask::taskOnStop(void) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop();
}

// ============================================================
ZFOBJECT_REGISTER(ZFAsyncTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFAsyncTask
        , ZFMP_IN(const ZFListener &, impl)
        ) {
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
void ZFAsyncTask::taskOnStop(void) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop();
}

// ============================================================
ZFOBJECT_REGISTER(ZFPostTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFPostTask
        , ZFMP_IN(const ZFListener &, impl)
        ) {
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
void ZFPostTask::taskOnStop(void) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop();
}

// ============================================================
ZFOBJECT_REGISTER(ZFAsyncIOCustomTask)
ZFOBJECT_ON_INIT_DEFINE_1(ZFAsyncIOCustomTask
        , ZFMP_IN(const ZFListener &, impl)
        ) {
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
        this->notifyFail("null impl");
    }
}
void ZFAsyncIOCustomTask::taskOnStop(void) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop();
}

// ============================================================
ZFOBJECT_REGISTER(ZFAsyncIOTask)
ZFOBJECT_ON_INIT_DEFINE_2(ZFAsyncIOTask
        , ZFMP_IN(const ZFOutput &, output)
        , ZFMP_IN(const ZFInput &, input)
        ) {
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
            v_zfindex *result = zfargs.param0();
            if(result->zfv != zfindexMax()) {
                owner->output(zfnull);
                owner->input(zfnull);
                owner->notifySuccess(result);
            }
            else {
                zfstring errorHint = zfstr("IO fail \"%s\" => \"%s\""
                        , owner->input()
                        , owner->output()
                        );
                owner->output(zfnull);
                owner->input(zfnull);
                owner->notifyFail(errorHint, result);
            }
        } ZFLISTENER_END()
        this->_implTaskId = zfasyncIO(this->output(), this->input(), implOnStop);
    }
    else {
        if(!this->output()) {
            this->notifyFail("null output");
        }
        else if(!this->input()) {
            this->notifyFail("null input");
        }
        else {
            this->notifyFail("null impl");
        }
    }
}
void ZFAsyncIOTask::taskOnStop(void) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop();
}

ZF_NAMESPACE_GLOBAL_END
