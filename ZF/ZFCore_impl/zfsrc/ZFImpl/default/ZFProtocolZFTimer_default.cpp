#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"
#include "ZFCore/ZFThread.h"
#include "ZFCore/ZFThread_zfpost.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFTimerImpl_default_Timer : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_ZFTimerImpl_default_Timer, ZFObject)

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
    zfautoT<ZFTimer> timer;
private:
    ZFThread *timerThread;
protected:
    _ZFP_ZFTimerImpl_default_Timer(void)
    : impl(zfnull)
    , timer(zfnull)
    , timerThread(zfnull)
    {
    }

public:
    zffinal void start(ZF_IN zfidentity timerImplId) {
        zfobjRetain(this);
        this->timerThread = zfobjAlloc(ZFThread, ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, threadCallback)));
        this->timerThread->threadName("ZFTimerImpl_default");
        this->timerThread->threadStart(zfobj<v_zfidentity>(this->timer->timerImplId()));
    }
    zffinal void stop(void) {
        this->timerThread->threadStop();
        zfobjRelease(this->timerThread);
        zfobjRelease(this);
    }

public:
    ZFMETHOD_INLINE_1(void, threadCallback
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        zfobjRetain(this);
        zfobjReleaseInScope(this);
        zfidentity curId = zfargs.param0().to<v_zfidentity *>()->zfv;
        ZFThread *curThread = ZFThread::currentThread();

        // start
        if(curId != this->timer->timerImplId() || curThread->threadStopRequested()) {return;}
        if(curId != this->timer->timerImplId() || curThread->threadStopRequested()) {return;}

        // timer
        if(this->timer->eventOnMainThread()) {
            zfself *owner = this;
            ZFLISTENER_2(timerMainThread
                    , zfautoT<zfself>, owner
                    , zfidentity, curId
                    ) {
                owner->_ZFP_timerMainThread(curId);
            } ZFLISTENER_END()
            while(curId == this->timer->timerImplId() && !curThread->threadStopRequested()) {
                ZFThread::sleep(this->timer->interval());
                if(this->timer->timerImplId() == curId && !curThread->threadStopRequested()) {
                    zfpost(timerMainThread);
                }
            }
        }
        else {
            while(curId == this->timer->timerImplId() && !curThread->threadStopRequested()) {
                ZFThread::sleep(this->timer->interval());
                if(this->timer->timerImplId() == curId && !curThread->threadStopRequested()) {
                    this->impl->notifyTimerActivate(this->timer, curId);
                }
            }
        }
    }
public:
    void _ZFP_timerMainThread(ZF_IN zfidentity curId) {
        this->impl->notifyTimerActivate(this->timer, curId);
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_default, ZFTimer, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFFramework:ZFThread")
    ZFPROTOCOL_IMPLEMENTATION_CHECK({
            return ZFThread::implAvailable();
        })

public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_default_Timer *token = zfobjAlloc(_ZFP_ZFTimerImpl_default_Timer);
        token->impl = this;
        token->timer = timer;
        return token;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_default_Timer *token = (_ZFP_ZFTimerImpl_default_Timer *)timer->nativeTimer();
        zfobjRelease(token);
    }
    virtual void start(
            ZF_IN ZFTimer *timer
            , ZF_IN zfidentity timerImplId
            ) {
        _ZFP_ZFTimerImpl_default_Timer *token = (_ZFP_ZFTimerImpl_default_Timer *)timer->nativeTimer();
        token->impl = this;
        token->timer = timer;
        token->start(timerImplId);
    }
    virtual void stop(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_default_Timer *token = (_ZFP_ZFTimerImpl_default_Timer *)timer->nativeTimer();
        token->stop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_default)

ZF_NAMESPACE_GLOBAL_END

