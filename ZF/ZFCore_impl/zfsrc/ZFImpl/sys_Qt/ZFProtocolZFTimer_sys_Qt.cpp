#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_Qt
#include <QTimer>
#include <QThread>
#include <QCoreApplication>

class _ZFP_ZFTimerImpl_sys_Qt_Timer : public QTimer {
    Q_OBJECT
public:
    _ZFP_ZFTimerImpl_sys_Qt_Timer(void)
    : QTimer()
    , impl(zfnull)
    , ownerZFTimer(zfnull)
    , delaying(zffalse)
    , firstTime(zftrue)
    {
    }
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
    ZFTimer *ownerZFTimer;
    zfbool delaying;
    zfbool firstTime;

public:
    void timerStart(void) {
        if(this->ownerZFTimer->timerDelay() > 0) {
            this->delaying = zftrue;
            this->setSingleShot(true);
            this->setInterval(this->ownerZFTimer->timerDelay());
            this->start();
            return;
        }
        this->delaying = zffalse;
        this->timerDoStart();
    }
    void timerStop(void) {
        this->disconnect();
        this->stop();
        this->impl->notifyTimerStop(this->ownerZFTimer);
    }
private:
    void timerDoStart(void) {
        this->delaying = zffalse;
        this->setSingleShot(false);
        this->firstTime = zftrue;

        this->setInterval(this->ownerZFTimer->timerInterval());
        connect(this, SIGNAL(timeout()), this, SLOT(timerOnActivate()), Qt::DirectConnection);
        this->start();
    }

public slots:
    void timerOnActivate(void) {
        if(this->delaying) {
            this->timerDoStart();
            return;
        }
        if(this->firstTime) {
            this->firstTime = zffalse;
            this->impl->notifyTimerStart(this->ownerZFTimer);
        }
        this->impl->notifyTimerActivate(this->ownerZFTimer);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_Qt, ZFTimer, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QTimer")
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = zfnew(_ZFP_ZFTimerImpl_sys_Qt_Timer);
        nativeTimer->impl = this;
        nativeTimer->ownerZFTimer = timer;
        return nativeTimer;
    }
    virtual void nativeTimerDestroy(
            ZF_IN ZFTimer *timer
            , ZF_IN void *nativeTimer
            ) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimerTmp = (_ZFP_ZFTimerImpl_sys_Qt_Timer *)nativeTimer;
        zfdelete(nativeTimerTmp);
    }
    virtual void timerStart(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = (_ZFP_ZFTimerImpl_sys_Qt_Timer *)timer->nativeTimer();
        nativeTimer->timerStart();
    }
    virtual void timerStop(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = (_ZFP_ZFTimerImpl_sys_Qt_Timer *)timer->nativeTimer();
        nativeTimer->timerStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTimerImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFTimer_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

