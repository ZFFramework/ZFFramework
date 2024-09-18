#include "ZFTimer.h"
#include "protocol/ZFProtocolZFTimer.h"

#include "ZFThread.h" // for timer thread register

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFTimerPrivate
zfclassNotPOD _ZFP_ZFTimerPrivate {
public:
    void *nativeTimer;
    zfbool timerStarted;
    unsigned short timerImplId;
    zfuint timerActivatedCount;
    void *timerThreadToken;

public:
    _ZFP_ZFTimerPrivate(void)
    : nativeTimer(zfnull)
    , timerStarted(zffalse)
    , timerImplId(0)
    , timerActivatedCount(0)
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
        zfblockedRelease(zfRetain(timer));
        ZFCoreMutexUnlock();
        timer->timerStop();
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
        , ZFMP_IN(zftimet, timerInterval)
        ) {
    this->objectOnInit();
    zfself::timerInterval(timerInterval);
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
    this->timerStop();
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_0(ZFTimer, void *, nativeTimer) {
    return d->nativeTimer;
}

ZFPROPERTY_ON_VERIFY_DEFINE(ZFTimer, zftimet, timerInterval) {
    ZFCoreAssert(!this->timerStarted());
    ZFCoreAssert(this->timerInterval() >= 0);
}

ZFMETHOD_DEFINE_0(ZFTimer, void, timerStart) {
    if(d->timerStarted) {
        return;
    }
    d->timerStarted = zftrue;

    zfRetain(this);

    d->timerActivatedCount = 0;
    ZFPROTOCOL_ACCESS(ZFTimer)->timerStart(this, (zfidentity)d->timerImplId);
}
ZFMETHOD_DEFINE_0(ZFTimer, void, timerStop) {
    if(d->timerStarted) {
        d->timerStarted = zffalse;
        ++(d->timerImplId);
        ZFPROTOCOL_ACCESS(ZFTimer)->timerStop(this);

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

ZFMETHOD_DEFINE_0(ZFTimer, zfbool, timerStarted) {
    return d->timerStarted;
}

ZFMETHOD_DEFINE_0(ZFTimer, zfindex, timerActivatedCount) {
    return (zfindex)d->timerActivatedCount;
}

zfidentity ZFTimer::timerImplId(void) {
    return (zfidentity)d->timerImplId;
}

void ZFTimer::_ZFP_ZFTimer_timerOnActivate(ZF_IN zfidentity timerImplId) {
    if((zfidentity)d->timerImplId != timerImplId) {
        return;
    }

    zfRetain(this);
    zfblockedRelease(this);

    ZFCoreMutexLock();
    if(d->timerActivatedCount == 0) {
        _ZFP_ZFTimerList().add(this);
        ZFCoreMutexUnlock();

        if(ZFThread::currentThread() == zfnull) {
            d->timerThreadToken = ZFThread::nativeThreadRegister("timer thread");
        }
        this->timerOnStart();
        ++(d->timerActivatedCount);
    }
    else {
        ++(d->timerActivatedCount);
        ZFCoreMutexUnlock();
    }

    {
        this->timerOnActivate();
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTimer>, ZFTimerStart
        , ZFMP_IN(zftimet, timerInterval)
        , ZFMP_IN(const ZFListener &, timerCallback)
        ) {
    zfobj<ZFTimer> ret;
    ret->timerInterval(timerInterval);
    ret->observerAdd(ZFTimer::EventTimerOnActivate(), timerCallback);
    ret->timerStart();
    return ret;
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTimer>, ZFTimerOnce
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(const ZFListener &, timerCallback)
        ) {
    zfobj<ZFTimer> ret;
    ret->timerInterval(delay);

    ZFLISTENER_2(implThreadCallback
            , zfautoT<ZFTimer>, ret
            , ZFListener, timerCallback
            ) {
        if(ret->timerStarted()) {
            timerCallback.execute(ZFArgs()
                    .sender(ret)
                );
        }
        ret->timerStop();
    } ZFLISTENER_END()

    zfautoT<ZFThread> currentThread = ZFThread::currentThread();
    ZFLISTENER_2(timerOnActivate
            , zfautoT<ZFThread>, currentThread
            , ZFListener, implThreadCallback
            ) {
        ZFThread::executeInThread(currentThread, implThreadCallback);
    } ZFLISTENER_END()
    ret->observerAddForOnce(ZFTimer::EventTimerOnActivate(), timerOnActivate);

    ret->timerStart();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

