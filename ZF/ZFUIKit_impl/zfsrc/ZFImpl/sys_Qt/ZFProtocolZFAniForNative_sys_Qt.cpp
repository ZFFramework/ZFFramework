#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAniForNative.h"

#if ZF_ENV_sys_Qt

#include <QAbstractAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsWidget>
#include <QTransform>

class _ZFP_ZFAniForNativeImpl_sys_Qt_Ani : public QAbstractAnimation {
    Q_OBJECT

public:
    typedef zffloat (*CurveFunc)(ZF_IN zffloat progress);

public:
    ZFAniForNative *ownerZFAnimation;

    // for ani impl
    ZFUIView *target;
    QGraphicsWidget *nativeAniTarget;
    zffloat nativeAniScale;
    zffloat aniProgress;
    CurveFunc curveFunc;
    zfbool started;

    // for ani transform
    QGraphicsOpacityEffect *alphaEffect;
    qreal alphaOrig;

public:
    _ZFP_ZFAniForNativeImpl_sys_Qt_Ani(ZF_IN ZFAniForNative *ownerZFAnimation)
    : QAbstractAnimation()
    , ownerZFAnimation(ownerZFAnimation)
    , target(NULL)
    , nativeAniTarget(NULL)
    , nativeAniScale(1)
    , aniProgress(0)
    , curveFunc(zfnull)
    , started(zffalse)
    , alphaEffect(NULL)
    , alphaOrig(1)
    {
        this->connect(this, SIGNAL(finished()), this, SLOT(nativeAniOnStop()));
    }
    ~_ZFP_ZFAniForNativeImpl_sys_Qt_Ani(void) {
        this->disconnect(this, SIGNAL(finished()), this, SLOT(nativeAniOnStop()));
    }

public slots:
    void nativeAniOnStop(void) {
        if(this->ownerZFAnimation->started()) {
            ZFPROTOCOL_ACCESS(ZFAniForNative)->notifyAniStop(this->ownerZFAnimation);
        }
    }

public:
    virtual int duration(void) const {
        return this->ownerZFAnimation->durationFixed();
    }
protected:
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState) {
        QAbstractAnimation::updateState(newState, oldState);
    }
    virtual void updateDirection(QAbstractAnimation::Direction direction) {
        QAbstractAnimation::updateDirection(direction);
    }
    virtual void updateCurrentTime(int currentTime) {
        if(!this->started) {
            return;
        }

        if(currentTime >= this->duration()) {
            this->aniProgress = 1;
            this->aniOnProgress();
            this->nativeAniCleanup();
            this->started = zffalse;
            this->stop();
        }
        else {
            this->aniProgress = this->curveFunc((zffloat)currentTime / this->duration());
            this->aniOnProgress();
        }
    }

public:
    void aniOnProgress(void) {
        ZFAniForNative *ani = this->ownerZFAnimation;
        zffloat progress = this->aniProgress;

        if(this->alphaEffect != NULL) {
            if(ani->alphaFrom() != 1 || ani->alphaTo() != 1) {
                this->alphaEffect->setOpacity((qreal)(
                        zfmApplyProgress(ani->alphaFrom() * this->alphaOrig, ani->alphaTo() * this->alphaOrig, progress)
                    ));
            }
        }

        QTransform transform;
        qreal width = this->nativeAniTarget->geometry().width();
        qreal height = this->nativeAniTarget->geometry().height();

        transform.translate(width / 2, height / 2);

        if(this->target->rotateZ() != 0) {
            transform.rotate(this->target->rotateZ());
        }
        if(ani->rotateZFrom() != 0 || ani->rotateZTo() != 0) {
            transform.rotate(zfmApplyProgress(ani->rotateZFrom(), ani->rotateZTo(), progress));
        }

        if(this->target->scaleX() != 1 || this->target->scaleY() != 1) {
            transform.scale(this->target->scaleX(), this->target->scaleY());
        }
        if(ani->scaleXFrom() != 1 || ani->scaleXTo() != 1
                || ani->scaleYFrom() != 1 || ani->scaleYTo() != 1
                ) {
            transform.scale(
                zfmApplyProgress(ani->scaleXFrom(), ani->scaleXTo(), progress),
                zfmApplyProgress(ani->scaleYFrom(), ani->scaleYTo(), progress)
                );
        }

        if(ani->translateXFrom() != 0 || ani->translateXTo() != 0
                || ani->translateYFrom() != 0 || ani->translateYTo() != 0
                ) {
            transform.translate(
                zfmApplyProgress(ani->translateXFrom(), ani->translateXTo(), progress) * this->nativeAniScale,
                zfmApplyProgress(ani->translateYFrom(), ani->translateYTo(), progress) * this->nativeAniScale
                );
        }

        transform.translate(-width / 2, -height / 2);

        this->nativeAniTarget->setTransform(transform);
    }

public:
    static zffloat _curveFuncLinear(ZF_IN zffloat progress) {
        return progress;
    }
    static zffloat _curveFuncEaseIn(ZF_IN zffloat progress) {
        return ZFBezierEaseIn().y_by_x(progress);
    }
    static zffloat _curveFuncEaseOut(ZF_IN zffloat progress) {
        return ZFBezierEaseOut().y_by_x(progress);
    }
    static zffloat _curveFuncEaseInOut(ZF_IN zffloat progress) {
        return ZFBezierEaseInOut().y_by_x(progress);
    }
    void nativeAniStart(void) {
        this->aniProgress = 0;
        switch(this->ownerZFAnimation->curve()) {
            case ZFAniForNativeCurve::e_Linear:
                this->curveFunc = _curveFuncLinear;
                break;
            case ZFAniForNativeCurve::e_EaseIn:
                this->curveFunc = _curveFuncEaseIn;
                break;
            case ZFAniForNativeCurve::e_EaseOut:
                this->curveFunc = _curveFuncEaseOut;
                break;
            case ZFAniForNativeCurve::e_EaseInOut:
                this->curveFunc = _curveFuncEaseInOut;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
        this->target = this->ownerZFAnimation->target()->to<ZFUIView *>();
        this->nativeAniTarget = (QGraphicsWidget *)this->target->nativeView();
        this->alphaOrig = 1;
        this->alphaEffect = qobject_cast<QGraphicsOpacityEffect *>(this->nativeAniTarget->graphicsEffect());
        if(this->alphaEffect != NULL) {
            this->alphaOrig = this->alphaEffect->opacity();
        }
        else if(this->ownerZFAnimation->alphaFrom() != 1 || this->ownerZFAnimation->alphaTo() != 1) {
            this->alphaEffect = new QGraphicsOpacityEffect(this->nativeAniTarget);
            this->nativeAniTarget->setGraphicsEffect(this->alphaEffect);
        }

        this->started = zftrue;
        this->start();
    }
    void nativeAniStop(void) {
        this->nativeAniCleanup();
        this->started = zffalse;
        this->stop();
    }
private:
    void nativeAniCleanup(void) {
        if(this->alphaOrig == 1) {
            this->nativeAniTarget->setGraphicsEffect(NULL);
        }
        else {
            QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect *>(this->nativeAniTarget->graphicsEffect());
            if(effect == zfnull) {
                effect = new QGraphicsOpacityEffect(this->nativeAniTarget);
                this->nativeAniTarget->setGraphicsEffect(effect);
            }
            effect->setOpacity(this->alphaOrig);
        }

        {
            QTransform t;
            t.translate(this->nativeAniTarget->geometry().width() / 2, this->nativeAniTarget->geometry().height() / 2);
            if(this->target->rotateZ() != 0) {
                t.rotate(this->target->rotateZ());
            }
            if(this->target->scaleX() != 1 || this->target->scaleY() != 1) {
                t.scale(this->target->scaleX(), this->target->scaleY());
            }
            t.translate(-this->nativeAniTarget->geometry().width() / 2, -this->nativeAniTarget->geometry().height() / 2);
            this->nativeAniTarget->setTransform(t);
        }

        this->alphaEffect = NULL;
        this->nativeAniTarget = NULL;
        this->curveFunc = zfnull;
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAniForNativeImpl_sys_Qt, ZFAniForNative, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QAbstractAnimation")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeAniCreate(ZF_IN ZFAniForNative *ani) {
        return new _ZFP_ZFAniForNativeImpl_sys_Qt_Ani(ani);
    }
    virtual void nativeAniDestroy(
            ZF_IN ZFAniForNative *ani
            , ZF_IN void *nativeAni
            ) {
        _ZFP_ZFAniForNativeImpl_sys_Qt_Ani *nativeAniTmp = (_ZFP_ZFAniForNativeImpl_sys_Qt_Ani *)nativeAni;
        nativeAniTmp->deleteLater();
    }

    virtual void nativeAniStart(
            ZF_IN ZFAniForNative *ani
            , ZF_IN zffloat nativeAniScale
            ) {
        _ZFP_ZFAniForNativeImpl_sys_Qt_Ani *nativeAni = (_ZFP_ZFAniForNativeImpl_sys_Qt_Ani *)ani->nativeAnimation();
        nativeAni->nativeAniScale = nativeAniScale;
        nativeAni->nativeAniStart();
    }
    virtual void nativeAniStop(ZF_IN ZFAniForNative *ani) {
        _ZFP_ZFAniForNativeImpl_sys_Qt_Ani *nativeAni = (_ZFP_ZFAniForNativeImpl_sys_Qt_Ani *)ani->nativeAnimation();
        nativeAni->nativeAniStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAniForNativeImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFAniForNative_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

