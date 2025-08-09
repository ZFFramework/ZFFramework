#include "ZFTimer.h"
#include "protocol/ZFProtocolZFTimer.h"

#include "ZFThread.h" // for timer thread register

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFTimerPrivate
zfclassNotPOD _ZFP_ZFTimerPrivate {
public:
    void *nativeTimer;
    zfbool started;
    unsigned short timerImplId;
    zfuint activatedCount;
    void *timerThreadToken;

public:
    _ZFP_ZFTimerPrivate(void)
    : nativeTimer(zfnull)
    , started(zffalse)
    , timerImplId(0)
    , activatedCount(0)
    , timerThreadToken(zfnull)
    {
    }
};

static ZFCoreArray<ZFTimer *> &_ZFP_ZFTimerList(void) {
    static ZFCoreArray<ZFTimer *> d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTimerList, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTimerList) {
    ZFCoreMutexLock();
    ZFCoreArray<ZFTimer *> &d = _ZFP_ZFTimerList();
    while(!d.isEmpty()) {
        ZFTimer *timer = d.removeLastAndGet();
        zfscopeRelease(zfRetain(timer));
        ZFCoreMutexUnlock();
        timer->stop();
        ZFCoreMutexLock();
    }
    ZFCoreMutexUnlock();
}
ZF_GLOBAL_INITIALIZER_END(ZFTimerList)

// ============================================================
// ZFTimer
ZFOBJECT_REGISTER(ZFTimer)

ZFEVENT_REGISTER(ZFTimer, TimerOnStart)
ZFEVENT_REGISTER(ZFTimer, TimerOnActivate)
ZFEVENT_REGISTER(ZFTimer, TimerOnStop)

ZFOBJECT_ON_INIT_DEFINE_1(ZFTimer
        , ZFMP_IN(zftimet, interval)
        ) {
    zfself::interval(interval);
}
void ZFTimer::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTimerPrivate);
    d->nativeTimer = ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerCreate(this);
}
void ZFTimer::objectOnDealloc(void) {
    ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerDestroy(this, d->nativeTimer);
    d->nativeTimer = zfnull;
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFTimer::objectOnDeallocPrepare(void) {
    this->stop();
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_0(ZFTimer, void *, nativeTimer) {
    return d->nativeTimer;
}

ZFPROPERTY_ON_UPDATE_DEFINE(ZFTimer, zftimet, interval) {
    ZFCoreAssert(!this->started());
    ZFCoreAssert(this->interval() >= 0);
}

ZFMETHOD_DEFINE_0(ZFTimer, void, start) {
    if(d->started) {
        return;
    }
    d->started = zftrue;

    zfRetain(this);

    d->activatedCount = 0;
    ZFPROTOCOL_ACCESS(ZFTimer)->start(this, (zfidentity)d->timerImplId);
}
ZFMETHOD_DEFINE_0(ZFTimer, void, stop) {
    if(d->started) {
        d->started = zffalse;
        ++(d->timerImplId);
        ZFPROTOCOL_ACCESS(ZFTimer)->stop(this);

        ZFCoreMutexLock();
        _ZFP_ZFTimerList().removeElement(this);
        ZFCoreMutexUnlock();
        this->timerOnStop();
        if(d->timerThreadToken != zfnull) {
            ZFThread::nativeThreadUnregister(d->timerThreadToken);
            d->timerThreadToken = zfnull;
        }
        zfRelease(this);
    }
}

ZFMETHOD_DEFINE_0(ZFTimer, zfbool, started) {
    return d->started;
}

ZFMETHOD_DEFINE_0(ZFTimer, zfindex, activatedCount) {
    return (zfindex)d->activatedCount;
}

zfidentity ZFTimer::timerImplId(void) {
    return (zfidentity)d->timerImplId;
}

void ZFTimer::_ZFP_ZFTimer_timerOnActivate(ZF_IN zfidentity timerImplId) {
    if((zfidentity)d->timerImplId != timerImplId) {
        return;
    }

    zfRetain(this);
    zfscopeRelease(this);

    ZFCoreMutexLock();
    if(d->activatedCount == 0) {
        _ZFP_ZFTimerList().add(this);
        ZFCoreMutexUnlock();

        if(ZFThread::currentThread() == zfnull) {
            d->timerThreadToken = ZFThread::nativeThreadRegister("timer thread");
        }
        this->timerOnStart();
        ++(d->activatedCount);
    }
    else {
        ++(d->activatedCount);
        ZFCoreMutexUnlock();
    }

    {
        this->timerOnActivate();
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTimer>, ZFTimerStart
        , ZFMP_IN(zftimet, interval)
        , ZFMP_IN(const ZFListener &, timerCallback)
        ) {
    zfobj<ZFTimer> ret;
    ret->interval(interval);
    ret->observerAdd(ZFTimer::E_TimerOnActivate(), timerCallback);
    ret->start();
    return ret;
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTimer>, ZFTimerOnce
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(const ZFListener &, timerCallback)
        ) {
    zfobj<ZFTimer> ret;
    ret->interval(delay);

    ZFLISTENER_2(implThreadCallback
            , zfautoT<ZFTimer>, ret
            , ZFListener, timerCallback
            ) {
        if(ret->started()) {
            timerCallback.execute(ZFArgs()
                    .sender(ret)
                );
        }
        ret->stop();
    } ZFLISTENER_END()

    zfautoT<ZFThread> currentThread = ZFThread::currentThread();
    ZFLISTENER_2(timerOnActivate
            , zfautoT<ZFThread>, currentThread
            , ZFListener, implThreadCallback
            ) {
        ZFThread::executeInThread(currentThread, implThreadCallback);
    } ZFLISTENER_END()
    ret->observerAddForOnce(ZFTimer::E_TimerOnActivate(), timerOnActivate);

    ret->start();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

