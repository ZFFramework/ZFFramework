#include "ZFUIScrollerDefault.h"

#include <cmath> // for sqrt

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIScrollerDefault_scrollAniBounceMax ((zffloat)40)
#define _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax ((zffloat)800)
#define _ZFP_ZFUIScrollerDefault_scrollAniStartTolerance ((zffloat)500)

#define _ZFP_ZFUIScrollerDefault_aniByPointDurationMin ((zftimet)250)
#define _ZFP_ZFUIScrollerDefault_aniByPointDurationMax ((zftimet)1500)

#define _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal ((zffloat)3000) // pixels per second
#define _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxBounce ((zffloat)500) // pixels per second
#define _ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal ((zffloat)500) // reduce how much speed per second
#define _ZFP_ZFUIScrollerDefault_aniBySpeedGravityBounce ((zffloat)2000) // reduce how much speed per second

// ============================================================
typedef enum {
    _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle,
    _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint,
    _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed,
} _ZFP_ZFUIScrollerDefaultPrivateAniState;
zfclassNotPOD _ZFP_ZFUIScrollerDefaultPrivate {
public:
    _ZFP_ZFUIScrollerDefaultPrivate(void)
    : pimplOwner(zfnull)
    , contentOffset(0)
    , contentSize(0)
    , ownerSize(0)
    , scrollBounce(zftrue)
    , scrollBounceAlways(zffalse)
    , scrollAlignToPage(zffalse)
    , scrollAlignToPageOverrideScrollFlag(zffalse)
    , scrollRequireFocus(zffalse)
    , contentBounceTailFix(0)
    , scrollDragPrevPrevPos(0)
    , scrollDragPrevPrevTime(0)
    , scrollDragPrevPos(0)
    , scrollDragPrevTime(0)
    , scrollDragCurPos(0)
    , scrollDragCurTime(0)
    , aniState(_ZFP_ZFUIScrollerDefaultPrivateAniStateIdle)
    , aniLastTime(0)
    , aniByPointStartPos(0)
    , aniByPointStartTime(0)
    , aniByPointStopPos(0)
    , aniByPointStopTime(0)
    , aniBySpeedCurSpeed(0)
    , aniBySpeedLastTime(0)
    {
    }

public:
    ZFUIScrollerDefault *pimplOwner;

    zffloat contentOffset;
    zffloat contentSize;
    zffloat ownerSize;
    zfbool scrollBounce;
    zfbool scrollBounceAlways;

    zfbool scrollAlignToPage;
    zfbool scrollAlignToPageOverrideScrollFlag;

    zfbool scrollRequireFocus;

    // contentOffset + fix should be 0 if no bounce at tail,
    // and should be < 0 if bounce,
    // this value would be 0 if (contentSize < ownerSize),
    // and should be (contentSize - ownerSize) if (contentSize > ownerSize)
    zffloat contentBounceTailFix;

    zffloat scrollDragPrevPrevPos;
    zftimet scrollDragPrevPrevTime;
    zffloat scrollDragPrevPos;
    zftimet scrollDragPrevTime;
    zffloat scrollDragCurPos;
    zftimet scrollDragCurTime;

    _ZFP_ZFUIScrollerDefaultPrivateAniState aniState;
    zftimet aniLastTime;

    zffloat aniByPointStartPos;
    zftimet aniByPointStartTime;
    zffloat aniByPointStopPos;
    zftimet aniByPointStopTime;

    // when not bouncing, speed would keep decreased until reach 0 or start bouncing
    // when bouncing out, speed would be decreased at higher speed until reach 0
    // when bouncing back, speed would be increased at higher speed until end bouncing
    zffloat aniBySpeedCurSpeed;
    zftimet aniBySpeedLastTime;

public:
    void contentBounceTailFixUpdate(void) {
        if(this->contentSize < this->ownerSize) {
            this->contentBounceTailFix = 0;
        }
        else {
            this->contentBounceTailFix = this->contentSize - this->ownerSize;
        }
    }
    inline zfbool scrollBounceEnable(void) {
        return (this->scrollBounceAlways || (this->scrollBounce && this->contentSize > this->ownerSize));
    }
    inline zffloat calcBounceFromDrag(ZF_IN zffloat dragOffset) {
        if(dragOffset >= _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax) {
            return _ZFP_ZFUIScrollerDefault_scrollAniBounceMax;
        }
        else {
            zffloat ret = (zffloat)(ZFBezierEaseOut().y_by_x((zffloat)dragOffset / _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax) * _ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
            return zfmMin(ret, dragOffset);
        }
    }
    inline zffloat calcDragFromBounce(ZF_IN zffloat scrollBounce) {
        if(scrollBounce >= _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) {
            return _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax;
        }
        else {
            zffloat ret = (zffloat)(ZFBezierEaseOut().x_by_y((zffloat)scrollBounce / _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) * _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax);
            return zfmMax(ret, scrollBounce);
        }
    }
    inline zffloat calcBounceHeadFromContentOffset(ZF_IN zffloat contentOffset) {
        if(contentOffset > 0) {
            return contentOffset;
        }
        else {
            return 0;
        }
    }
    inline zffloat calcContentOffsetFromBounceHead(ZF_IN zffloat scrollBounceHead) {
        return scrollBounceHead;
    }
    inline zffloat calcBounceTailFromContentOffset(ZF_IN zffloat contentOffset) {
        if(contentOffset + this->contentBounceTailFix < 0) {
            return -(contentOffset + this->contentBounceTailFix);
        }
        else {
            return 0;
        }
    }
    inline zffloat calcContentOffsetFromBounceTail(ZF_IN zffloat scrollBounceTail) {
        return (-this->contentBounceTailFix - scrollBounceTail);
    }
    inline zffloat calcContentOffsetFromBounce(
            ZF_IN zffloat scrollBounce
            , ZF_IN zfbool bounceAtHead
            ) {
        return (bounceAtHead
                ? this->calcContentOffsetFromBounceHead(scrollBounce)
                : this->calcContentOffsetFromBounceTail(scrollBounce));
    }
    inline zffloat calcBounceFromContentOffset(
            ZF_IN zffloat contentOffset
            , ZF_IN zfbool bounceAtHead
            ) {
        return (bounceAtHead
                ? this->calcBounceHeadFromContentOffset(contentOffset)
                : this->calcBounceTailFromContentOffset(contentOffset));
    }

    zffloat calcScrollOffset(
            ZF_IN zffloat dragOffset
            , ZF_IN zftimet timeOffset
            ) {
        zffloat offset = 0;
        if(timeOffset > 0) {
            zfdouble speed = (zfdouble)((zft_zfdouble)dragOffset * 1000 / timeOffset);
            zfdouble speedFix = zfmAbs(speed);
            zfdouble g = (zfdouble)200;
            zfdouble t = (zfdouble)(speedFix / g);
            offset = (zffloat)(speedFix * t - (g * t * t) / 2);
            if(speed < 0) {
                offset = -offset;
            }
        }
        return offset;
    }

    void scrollAniNotifyStart(void) {
        ZFArgs zfargs;
        this->pimplOwner->scrollAniStartCallback.execute(zfargs);
        this->aniLastTime = zfargs.result().to<v_zftimet *>()->zfv;
    }
    void scrollAniNotifyStop(void) {
        this->scrollRequireFocus = zffalse;

        this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle;
        this->pimplOwner->scrollAniStopCallback.execute();
    }
    void scrollToFitRange(void) {
        if(this->calcBounceHeadFromContentOffset(this->contentOffset) > 0
                || this->calcBounceTailFromContentOffset(this->contentOffset) > 0
                ) {
            this->aniBySpeedCurSpeed = 0;
            this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed;
            this->scrollAniNotifyStart();
            this->aniBySpeedLastTime = this->aniLastTime;
            this->scrollAlignToPageCheckUpdate();
        }
        else {
            if(!this->scrollAlignToPageCheckUpdate()) {
                this->scrollAniNotifyStop();
            }
        }
    }

    // ============================================================
    // aniByPoint logic
    void aniByPointUpdate(ZF_IN zftimet curTime) {
        if(curTime >= this->aniByPointStopTime) {
            this->contentOffset = aniByPointStopPos;
            this->scrollToFitRange();
            return;
        }

        zffloat progress = (zffloat)(curTime - this->aniByPointStartTime) / (this->aniByPointStopTime - this->aniByPointStartTime);
        zffloat offsetProgress = ZFBezierEaseOut().y_by_x(progress);

        this->contentOffset = this->aniByPointStartPos + (this->aniByPointStopPos - this->aniByPointStartPos) * offsetProgress;
        if(zfmAbs(this->contentOffset - this->aniByPointStopPos) <= 1) {
            this->contentOffset = aniByPointStopPos;
            this->scrollToFitRange();
            return;
        }
    }
    zftimet aniByPointDurationForOffset(ZF_IN zffloat offset) {
        offset = zfmAbs(offset);
        if(offset <= 2) {
            return 0;
        }

        if(offset >= _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax) {
            return _ZFP_ZFUIScrollerDefault_aniByPointDurationMax;
        }
        zftimet ret = (zft_zftimet)(ZFBezierLinear().y_by_x((zft_zffloat)offset / _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax)
            * _ZFP_ZFUIScrollerDefault_aniByPointDurationMax);
        if(ret < _ZFP_ZFUIScrollerDefault_aniByPointDurationMin) {
            return _ZFP_ZFUIScrollerDefault_aniByPointDurationMin;
        }
        else {
            return ret;
        }
    }
    void aniByPointStart(
            ZF_IN zffloat stopPos
            , ZF_IN_OPT zftimet duration = 0
            ) {
        // fix stopPos in bounce range
        if(this->calcBounceHeadFromContentOffset(stopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) {
            stopPos = this->calcContentOffsetFromBounceHead(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
        else if(this->calcBounceTailFromContentOffset(stopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) {
            stopPos = this->calcContentOffsetFromBounceTail(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }

        if(duration <= 0) {
            duration = this->aniByPointDurationForOffset(stopPos - this->contentOffset);
        }
        if(duration <= 0) {
            this->contentOffset = stopPos;
            this->aniByPointStopPos = stopPos;
            this->scrollToFitRange();
            return;
        }

        this->aniByPointStartPos = this->contentOffset;
        this->aniByPointStopPos = stopPos;
        this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint;
        this->scrollAniNotifyStart();
        this->aniByPointStartTime = this->aniLastTime;
        this->aniByPointStopTime = this->aniByPointStartTime + duration;
    }
    void aniByPointScrollOwnerSizeOnUpdate(void) {
        if(this->calcBounceHeadFromContentOffset(this->aniByPointStartPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) {
            this->aniByPointStartPos = this->calcContentOffsetFromBounceHead(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
        else if(this->calcBounceTailFromContentOffset(this->aniByPointStartPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) {
            this->aniByPointStartPos = this->calcContentOffsetFromBounceTail(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }

        if(this->calcBounceHeadFromContentOffset(this->aniByPointStopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) {
            this->aniByPointStopPos = this->calcContentOffsetFromBounceHead(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
        else if(this->calcBounceTailFromContentOffset(this->aniByPointStopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) {
            this->aniByPointStopPos = this->calcContentOffsetFromBounceTail(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
    }

    // ============================================================
    // aniBySpeed logic
    void aniBySpeedScrollOwnerSizeOnUpdate(void) {
        // nothing to do
    }
    void aniBySpeedCalcSpeed(void) {
        zffloat prevSpeed = 0;
        zffloat curSpeed = 0;
        if(this->scrollDragPrevTime > this->scrollDragPrevPrevTime) {
            prevSpeed = (zffloat)((this->scrollDragPrevPos - this->scrollDragPrevPrevPos) * 1000 / (this->scrollDragPrevTime - this->scrollDragPrevPrevTime));
        }
        if(this->scrollDragCurTime > this->scrollDragPrevTime) {
            curSpeed = (zffloat)((this->scrollDragCurPos - this->scrollDragPrevPos) * 1000 / (this->scrollDragCurTime - this->scrollDragPrevTime));
        }
        if((prevSpeed > 0 && curSpeed > 0) || (prevSpeed < 0 && curSpeed < 0)) {
            this->aniBySpeedCurSpeed = (prevSpeed + curSpeed) / 2;
        }
        else {
            this->aniBySpeedCurSpeed = curSpeed / 2;
        }
        if(zfmAbs(this->aniBySpeedCurSpeed) > _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal) {
            if(this->aniBySpeedCurSpeed > 0) {
                this->aniBySpeedCurSpeed = _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal;
            }
            else {
                this->aniBySpeedCurSpeed = -_ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal;
            }
        }
    }
    void aniBySpeedStart(void) {
        if(this->aniBySpeedShouldStop()) {
            this->scrollToFitRange();
            return;
        }
        this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed;
        this->scrollAniNotifyStart();
        this->aniBySpeedLastTime = this->aniLastTime;
    }
    void aniBySpeedUpdate(ZF_IN zftimet curTime) {
        zffloat v = zfmAbs(this->aniBySpeedCurSpeed);
        zftimet t = (curTime - this->aniBySpeedLastTime);

        zfbool bounceAtHead = (this->calcBounceHeadFromContentOffset(this->contentOffset) > 0);
        zfbool bounceAtTail = (this->calcBounceTailFromContentOffset(this->contentOffset) > 0);
        if(bounceAtHead || bounceAtTail) {
            zfbool bounceAway = ((bounceAtHead && this->aniBySpeedCurSpeed > 0) || (bounceAtTail && this->aniBySpeedCurSpeed < 0));
            zffloat a = (bounceAway ? (zffloat)-_ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal : _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal);
            zffloat bounceOffset = this->calcBounceFromContentOffset(this->contentOffset, bounceAtHead);
            if(bounceAway) {
                v = zfmMin(v, _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxBounce);
                zftimet tDelta = (zft_zftimet)(-v * 1000 / a);
                zfbool reachEnd = (t - 10 >= tDelta);
                this->contentOffset = this->calcContentOffsetFromBounce(
                    bounceOffset + this->aniBySpeed_calcOffset(v, reachEnd ? tDelta : t, a),
                    bounceAtHead);
                if(reachEnd) {
                    // next time, we'll bounce back
                    this->aniBySpeedCurSpeed = 0;
                }
                else {
                    // reduce speed
                    this->aniBySpeedCurSpeed = (zffloat)(v + a * t / 1000);
                    if(bounceAtTail) {
                        this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
                    }
                }
            }
            else { // bounce back
                zffloat offset = this->aniBySpeed_calcOffset(v, t, a);
                if(offset >= bounceOffset - offset / 4) {
                    this->contentOffset = this->calcContentOffsetFromBounce(0, bounceAtHead);
                    this->scrollAniNotifyStop();
                }
                else {
                    this->contentOffset = this->calcContentOffsetFromBounce(
                        bounceOffset - offset,
                        bounceAtHead);
                    this->aniBySpeedCurSpeed = (zffloat)(v + a * t / 1000);
                    if(bounceAtHead) {
                        this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
                    }
                }
            }
            this->aniBySpeedLastTime = curTime;
            return;
        }

        // no bounce
        zffloat a = -_ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal;
        zffloat offset = this->aniBySpeed_calcOffset(v, t, a);
        zfbool scrollToHead = (this->aniBySpeedCurSpeed > 0);
        if(this->calcBounceFromContentOffset(
                    scrollToHead ? this->contentOffset + offset : this->contentOffset - offset,
                    scrollToHead) > 0
                    ) { // begin to bounce
            zffloat offsetDelta = (scrollToHead
                ? -this->contentOffset
                : this->contentOffset + this->contentBounceTailFix - this->ownerSize);
            zftimet tDelta = this->aniBySpeed_calcTime(v, offsetDelta, a);
            this->contentOffset = this->calcContentOffsetFromBounce(1, scrollToHead);
            this->aniBySpeedLastTime += tDelta;
            this->aniBySpeedCurSpeed = (zffloat)(v + a * t / 1000);
            if(!scrollToHead) {
                this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
            }
            if(this->aniBySpeedShouldStop()) {
                this->contentOffset = this->calcContentOffsetFromBounce(0, scrollToHead);
                this->scrollAniNotifyStop();
            }
            else {
                this->aniBySpeedUpdate(curTime);
            }
        }
        else { // not begin to bounce
            if(scrollToHead) {
                this->contentOffset += offset;
            }
            else {
                this->contentOffset -= offset;
            }
            this->aniBySpeedLastTime = curTime;
            this->aniBySpeedCurSpeed = (zffloat)(v + a * t / 1000);
            if(!scrollToHead) {
                this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
            }
            if(this->aniBySpeedShouldStop()) {
                this->scrollAniNotifyStop();
            }
        }
    }
    zffloat anibySpeedEndPointPredicted(void) {
        zftimet t = (zft_zftimet)(zfmAbs(this->aniBySpeedCurSpeed) * 1000 / _ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal);
        zffloat offset = this->aniBySpeed_calcOffset(zfmAbs(this->aniBySpeedCurSpeed), t, -_ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal);
        if(this->aniBySpeedCurSpeed > 0) {
            return this->contentOffset + offset;
        }
        else {
            return this->contentOffset - offset;
        }
    }

private:
    zfbool aniBySpeedShouldStop(void) {
        return (zfmAbs(this->aniBySpeedCurSpeed) <= _ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal * 20 / 1000);
    }
    zffloat aniBySpeed_calcOffset(
            ZF_IN zffloat v
            , ZF_IN zftimet t
            , ZF_IN zffloat a
            ) {
        return (zffloat)(v * t / 1000 + a * t * t / 1000 / 1000 / 2);
    }
    zftimet aniBySpeed_calcTime(
            ZF_IN zffloat v
            , ZF_IN zffloat offset
            , ZF_IN zffloat a
            ) {
        zftimet t = (zft_zftimet)((-v + sqrt((double)((long)v * v + (long)2 * a * offset))) * 1000 / a);
        return zfmMax((zftimet)0, t);
    }

    // ============================================================
    // align to page logic
public:
    zfbool scrollAlignToPageCheckUpdate(void) {
        if(!this->scrollAlignToPage || this->scrollAlignToPageOverrideScrollFlag) {
            return zffalse;
        }

        zffloat endPos = this->pimplOwner->scrollEndPointPredicted();
        zffloat endPosAligned = this->scrollAlignToPageEndPosAligned(endPos);
        if(endPos != endPosAligned) {
            this->scrollRequireFocus = zftrue;
            this->scrollAlignToPageOverrideScrollFlag = zftrue;
            this->pimplOwner->scrollByPoint(endPosAligned);
            this->scrollAlignToPageOverrideScrollFlag = zffalse;
            return zftrue;
        }
        return zffalse;
    }
private:
    zffloat scrollAlignToPageEndPosAligned(ZF_IN zffloat endPos) {
        if(this->ownerSize == 0) {
            return endPos;
        }
        if(this->calcBounceHeadFromContentOffset(endPos) > 0) {
            return this->calcContentOffsetFromBounceHead(0);
        }
        else if(this->calcBounceTailFromContentOffset(endPos) > 0) {
            return this->calcContentOffsetFromBounceTail(0);
        }
        else {
            zffloat tmp = (((zffloat)((endPos - this->ownerSize / 2) / this->ownerSize)) * this->ownerSize);
            if(this->contentSize >= this->ownerSize && tmp + this->contentSize < this->ownerSize) {
                return this->calcContentOffsetFromBounceTail(0);
            }
            return tmp;
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIScrollerDefault)

void ZFUIScrollerDefault::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIScrollerDefaultPrivate);
    d->pimplOwner = this;
}
void ZFUIScrollerDefault::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIScrollerDefault::scrollOwnerSizeOnUpdate(ZF_IN zffloat ownerSize) {
    d->ownerSize = ownerSize;
    d->contentBounceTailFixUpdate();

    if(d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint) {
        d->aniByPointScrollOwnerSizeOnUpdate();
    }
    else if(d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed) {
        d->aniBySpeedScrollOwnerSizeOnUpdate();
    }
    else {
        if(d->calcBounceHeadFromContentOffset(d->contentOffset) > 0) {
            d->contentOffset = d->calcContentOffsetFromBounceHead(0);
        }
        else if(d->calcBounceTailFromContentOffset(d->contentOffset) > 0) {
            d->contentOffset = d->calcContentOffsetFromBounceTail(0);
        }
    }
    d->scrollAlignToPageCheckUpdate();
}
void ZFUIScrollerDefault::scrollBounceOnUpdate(
        ZF_IN zfbool scrollBounce
        , ZF_IN zfbool scrollBounceAlways
        ) {
    d->scrollBounce = scrollBounce;
    d->scrollBounceAlways = scrollBounceAlways;
}

void ZFUIScrollerDefault::scrollAlignToPageOnUpdate(ZF_IN zfbool scrollAlignToPage) {
    d->scrollAlignToPage = scrollAlignToPage;
    d->scrollAlignToPageCheckUpdate();
}

void ZFUIScrollerDefault::scrollContentOnUpdate(
        ZF_IN zffloat contentOffset
        , ZF_IN zffloat contentSize
        ) {
    d->contentOffset = contentOffset;
    d->contentSize = contentSize;
    d->contentBounceTailFixUpdate();

    // scrollBySpeed have no need to update, simply keep current speed
    // scrollByPoint need update src and dst position
    if(d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint) {
        this->scrollByPoint(d->aniByPointStopPos);
    }
}

void ZFUIScrollerDefault::scrollToFitRange(void) {
    d->scrollToFitRange();
}
void ZFUIScrollerDefault::scrollWithoutAnimation(ZF_IN zffloat contentOffset) {
    d->scrollAniNotifyStop();
    d->contentOffset = contentOffset;
}
void ZFUIScrollerDefault::scrollByPoint(ZF_IN zffloat point) {
    d->aniByPointStart(point);
    d->scrollAlignToPageCheckUpdate();
}
zffloat ZFUIScrollerDefault::scrollByPointEndPoint(void) {
    return (d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint ? d->aniByPointStopPos : d->contentOffset);
}

zffloat ZFUIScrollerDefault::scrollEndPointPredicted(void) {
    switch(d->aniState) {
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle:
            return d->contentOffset;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint:
            return d->aniByPointStopPos;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed:
            return d->anibySpeedEndPointPredicted();
        default:
            ZFCoreCriticalShouldNotGoHere();
            return 0;
    }
}

void ZFUIScrollerDefault::scrollBySpeed(ZF_IN zffloat speed) {
    d->aniBySpeedCurSpeed = speed;
    d->aniBySpeedStart();
    d->scrollAlignToPageCheckUpdate();
}
zffloat ZFUIScrollerDefault::scrollBySpeedCurrentSpeed(void) {
    return (d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed ? d->aniBySpeedCurSpeed : (zffloat)0);
}
zffloat ZFUIScrollerDefault::scrollBySpeedEndPointPredicted(void) {
    return (d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed
        ? d->anibySpeedEndPointPredicted()
        : (zffloat)0);
}

zfbool ZFUIScrollerDefault::scrollRequireFocus(void) {
    return d->scrollRequireFocus;
}

zffloat ZFUIScrollerDefault::scrollOwnerSize(void) {
    return d->ownerSize;
}
zffloat ZFUIScrollerDefault::scrollContentOffset(void) {
    return d->contentOffset;
}
zffloat ZFUIScrollerDefault::scrollContentSize(void) {
    return d->contentSize;
}

void ZFUIScrollerDefault::scrollOnDragBegin(
        ZF_IN zffloat mousePos
        , ZF_IN zftimet mouseTime
        ) {
    d->scrollAniNotifyStop();

    d->scrollDragPrevPrevPos = mousePos;
    d->scrollDragPrevPrevTime = mouseTime;
    d->scrollDragPrevPos = mousePos;
    d->scrollDragPrevTime = mouseTime;
    d->scrollDragCurPos = mousePos;
    d->scrollDragCurTime = mouseTime;

    d->aniByPointStopPos = mousePos;
}
void ZFUIScrollerDefault::scrollOnDrag(
        ZF_IN zffloat mousePos
        , ZF_IN zftimet mouseTime
        ) {
    d->scrollDragPrevPrevPos = d->scrollDragPrevPos;
    d->scrollDragPrevPrevTime = d->scrollDragPrevTime;
    d->scrollDragPrevPos = d->scrollDragCurPos;
    d->scrollDragPrevTime = d->scrollDragCurTime;
    d->scrollDragCurPos = mousePos;
    d->scrollDragCurTime = mouseTime;

    d->aniByPointStopPos = mousePos;

    zffloat dragOffset = d->scrollDragCurPos - d->scrollDragPrevPos;
    zffloat offset = 0;
    if(d->calcBounceHeadFromContentOffset(d->contentOffset) > 0) {
        offset = d->calcContentOffsetFromBounceHead(d->calcDragFromBounce(d->calcBounceHeadFromContentOffset(d->contentOffset)));
    }
    else if(d->calcBounceTailFromContentOffset(d->contentOffset) > 0) {
        offset = d->calcContentOffsetFromBounceTail(d->calcDragFromBounce(d->calcBounceTailFromContentOffset(d->contentOffset)));
    }
    else {
        offset = d->contentOffset;
    }
    offset += dragOffset;
    if(d->calcBounceHeadFromContentOffset(offset) > 0) {
        if(!d->scrollBounceEnable()) {
            offset = d->calcContentOffsetFromBounceHead(0);
        }
        d->contentOffset = d->calcContentOffsetFromBounceHead(d->calcBounceFromDrag(d->calcBounceHeadFromContentOffset(offset)));
    }
    else if(d->calcBounceTailFromContentOffset(offset) > 0) {
        if(!d->scrollBounceEnable()) {
            offset = d->calcContentOffsetFromBounceTail(0);
        }
        d->contentOffset = d->calcContentOffsetFromBounceTail(d->calcBounceFromDrag(d->calcBounceTailFromContentOffset(offset)));
    }
    else {
        d->contentOffset = offset;
    }
}
void ZFUIScrollerDefault::scrollOnDragEnd(
        ZF_IN zftimet mouseTime
        , ZF_IN zfbool needScrollAni
        ) {
    if(!needScrollAni) {
        d->scrollToFitRange();
        return;
    }

    if(d->scrollDragCurTime - d->scrollDragPrevTime <= 5
            || mouseTime - d->scrollDragPrevTime >= _ZFP_ZFUIScrollerDefault_scrollAniStartTolerance
            ) { // no need animation
        d->scrollToFitRange();
        return;
    }

    d->aniBySpeedCalcSpeed();
    d->aniBySpeedStart();
    d->scrollAlignToPageCheckUpdate();
}

void ZFUIScrollerDefault::scrollAniOnUpdate(ZF_IN zftimet time) {
    d->aniLastTime = time;
    switch(d->aniState) {
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint:
            d->aniByPointUpdate(time);
            break;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed:
            d->aniBySpeedUpdate(time);
            break;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle:
            ZFCoreCriticalShouldNotGoHere();
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
}

ZF_NAMESPACE_GLOBAL_END

