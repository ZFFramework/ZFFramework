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
    , timerImplId(zfidentityInvalid())
    {
    }
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
    ZFTimer *ownerZFTimer;
    zfidentity timerImplId;

public:
    void _ZFP_start(ZF_IN zfidentity timerImplId) {
        this->timerImplId = timerImplId;
        this->setSingleShot(false);

        this->setInterval(this->ownerZFTimer->interval());
        connect(this, SIGNAL(timeout()), this, SLOT(timerOnActivate()), Qt::DirectConnection);
        this->start();
    }
    void _ZFP_stop(void) {
        this->disconnect();
        this->stop();
    }

public slots:
    void timerOnActivate(void) {
        this->impl->notifyTimerActivate(this->ownerZFTimer, this->timerImplId);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_Qt, ZFTimer, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QTimer")
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = zfnew(_ZFP_ZFTimerImpl_sys_Qt_Timer);
        nativeTimer->impl = this;
        nativeTimer->ownerZFTimer = timer;
        return nativeTimer;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = (_ZFP_ZFTimerImpl_sys_Qt_Timer *)timer->nativeTimer();
        zfdelete(nativeTimer);
    }
    virtual void start(
            ZF_IN ZFTimer *timer
            , ZF_IN zfidentity timerImplId
            ) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = (_ZFP_ZFTimerImpl_sys_Qt_Timer *)timer->nativeTimer();
        nativeTimer->_ZFP_start(timerImplId);
    }
    virtual void stop(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = (_ZFP_ZFTimerImpl_sys_Qt_Timer *)timer->nativeTimer();
        nativeTimer->_ZFP_stop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFTimer_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

