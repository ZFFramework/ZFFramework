#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"
#include "ZFCore/ZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFTimerImpl_default_Timer : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_ZFTimerImpl_default_Timer, ZFObject)

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
    ZFTimer *timer;
private:
    zfbool timerThreadStarted;
    zfbool timerThreadStartNotified;
    zfidentity threadCallbackTaskId;
    ZFThread *timerThread;
protected:
    _ZFP_ZFTimerImpl_default_Timer(void)
    : impl(zfnull)
    , timer(zfnull)
    , timerThreadStarted(zffalse)
    , timerThreadStartNotified(zffalse)
    , threadCallbackTaskId(zfidentityInvalid())
    , timerThread(zfnull)
    {
    }

public:
    zffinal void timerStart(void) {
        zfRetain(this);
        ++(this->threadCallbackTaskId);
        this->timerThread = zfAlloc(ZFThread, ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, threadCallback)));
        this->timerThread->threadName("ZFTimerImpl_default");
        this->timerThread->threadStart(zfobj<v_zfidentity>(this->threadCallbackTaskId));
    }
    zffinal void timerStop(void) {
        ++(this->threadCallbackTaskId);
        this->timerThread->threadStop();
        zfRelease(this->timerThread);
        if(this->timerThreadStarted) {
            this->timerThreadStarted = zffalse;
            this->impl->notifyTimerStop(this->timer);
        }
        zfRelease(this);
    }

public:
    ZFMETHOD_INLINE_1(void, threadCallback
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        zfRetain(this);
        zfblockedRelease(this);
        zfidentity const &curId = zfargs.param0().zfv();
        ZFThread *curThread = ZFThread::currentThread();

        // delay
        if(curId != this->threadCallbackTaskId || curThread->threadStopRequested()) {return;}
        if(this->timer->timerDelay() > 0) {
            ZFThread::sleep(this->timer->timerDelay());
        }

        // start
        if(curId != this->threadCallbackTaskId || curThread->threadStopRequested()) {return;}
        this->timerThreadStarted = zftrue;
        if(curId != this->threadCallbackTaskId || curThread->threadStopRequested()) {return;}

        this->timerThreadStartNotified = zftrue;
        this->impl->notifyTimerStart(this->timer);

        // timer
        if(this->timer->timerActivateOnMainThread()) {
            zfself *owner = this;
            ZFLISTENER_2(timerMainThread
                    , zfautoT<zfself>, owner
                    , zfidentity, curId
                    ) {
                owner->_ZFP_timerMainThread(curId);
            } ZFLISTENER_END()
            while(curId == this->threadCallbackTaskId && !curThread->threadStopRequested()) {
                ZFThread::sleep(this->timer->timerInterval());
                if(this->threadCallbackTaskId == curId && !curThread->threadStopRequested()) {
                    ZFThread::mainThread()->taskQueueAdd(timerMainThread);
                }
            }
        }
        else {
            while(curId == this->threadCallbackTaskId && !curThread->threadStopRequested()) {
                ZFThread::sleep(this->timer->timerInterval());
                if(this->threadCallbackTaskId == curId && !curThread->threadStopRequested()) {
                    this->impl->notifyTimerActivate(this->timer);
                }
            }
        }
    }
public:
    void _ZFP_timerMainThread(ZF_IN zfidentity curId) {
        if(this->threadCallbackTaskId == curId) {
             this->impl->notifyTimerActivate(this->timer);
        }
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_default, ZFTimer, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFFramework:ZFThread")
    ZFPROTOCOL_IMPLEMENTATION_CHECK({
            return ZFThread::implAvailable();
        })

public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_default_Timer *token = zfAlloc(_ZFP_ZFTimerImpl_default_Timer);
        token->impl = this;
        token->timer = timer;
        return token;
    }
    virtual void nativeTimerDestroy(
            ZF_IN ZFTimer *timer
            , ZF_IN void *nativeTimer
            ) {
        _ZFP_ZFTimerImpl_default_Timer *token = (_ZFP_ZFTimerImpl_default_Timer *)nativeTimer;
        zfRelease(token);
    }
    virtual void timerStart(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_default_Timer *token = (_ZFP_ZFTimerImpl_default_Timer *)timer->nativeTimer();
        token->impl = this;
        token->timer = timer;
        token->timerStart();
    }
    virtual void timerStop(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_default_Timer *token = (_ZFP_ZFTimerImpl_default_Timer *)timer->nativeTimer();
        token->timerStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_default)

ZF_NAMESPACE_GLOBAL_END

