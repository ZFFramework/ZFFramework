#include "ZFUIScrollThumbDefault.h"
#include "ZFUIWindow.h"
#include "ZFAnimationNativeView.h"

#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIScrollThumbDefault)

// ============================================================
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAni : zfextend ZFAnimationTimeLine {
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAni, ZFAnimationTimeLine)

private:
    ZFUIImageView *thumbView;

protected:
    virtual void objectOnInit(ZF_IN ZFUIImageView *thumbView) {
        this->objectOnInit();
        this->thumbView = thumbView;
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress) {
        zfsuper::aniTimeLineOnUpdate(progress);
        if(this->thumbView->viewVisible()) {
            this->thumbView->viewAlpha(1 - progress);
        }
    }
};
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView : zfextend ZFAnimationNativeView {
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView, ZFAnimationNativeView)

protected:
    virtual void objectOnInit(ZF_IN ZFUIImageView *thumbView) {
        this->objectOnInit();
        this->aniTarget(thumbView);
        this->aniAlphaTo(0);
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
};
zfclassNotPOD _ZFP_ZFUIScrollThumbDefaultPrivate {
public:
    ZFUIImageView *thumbView;
    ZFAnimation *thumbHideAni;
    ZFListener thumbHideAniAutoStopListener;
    zffloat lastPos;
    zffloat lastSize;
    zftimet lastVisibleTime;
public:
    _ZFP_ZFUIScrollThumbDefaultPrivate(void)
    : thumbView(zfnull)
    , thumbHideAni(zfnull)
    , thumbHideAniAutoStopListener()
    , lastPos(0)
    , lastSize(0)
    , lastVisibleTime(ZFTime::timestamp() - 1000)
    {
    }
public:
    static void aniOnStop(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIScrollThumbDefault *owner
            ) {
        if(owner != zfnull) {
            owner->d->thumbView->viewAlpha(1);
            owner->d->thumbView->viewVisible(zffalse);
        }
    }
    static void thumbHideAniAutoStop(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIScrollThumbDefault *owner
            ) {
        if(owner != zfnull) {
            owner->d->thumbHideAni->aniStop();
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIScrollThumbDefault)

void ZFUIScrollThumbDefault::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIScrollThumbDefaultPrivate);

    d->thumbView = zfAlloc(ZFUIImageView);

    if(ZFPROTOCOL_IS_AVAILABLE(ZFAnimationNativeView)) {
        d->thumbHideAni = zfAlloc(_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView, d->thumbView);
    }
    else {
        d->thumbHideAni = zfAlloc(_ZFP_ZFUIScrollThumbDefault_HideThumbAni, d->thumbView);
    }
}
void ZFUIScrollThumbDefault::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();

    ZFUIScrollThumbDefault *owner = this;
    ZFLISTENER_1(aniOnStop
            , ZFUIScrollThumbDefault *, owner
            ) {
        _ZFP_ZFUIScrollThumbDefaultPrivate::aniOnStop(zfargs, owner);
    } ZFLISTENER_END()
    d->thumbHideAni->observerAdd(ZFAnimation::EventAniOnStop(), aniOnStop);

    ZFLISTENER_1(thumbHideAniAutoStop
            , ZFUIScrollThumbDefault *, owner
            ) {
        _ZFP_ZFUIScrollThumbDefaultPrivate::thumbHideAniAutoStop(zfargs, owner);
    } ZFLISTENER_END()
    d->thumbHideAniAutoStopListener = thumbHideAniAutoStop;
    ZFGlobalObserver().observerAdd(ZFUIWindow::EventWindowOnHide(), d->thumbHideAniAutoStopListener);
}
void ZFUIScrollThumbDefault::objectOnDeallocPrepare(void) {
    ZFGlobalObserver().observerRemove(ZFUIWindow::EventWindowOnHide(), d->thumbHideAniAutoStopListener);
    zfsuper::objectOnDeallocPrepare();
}
void ZFUIScrollThumbDefault::objectOnDealloc(void) {
    d->thumbHideAni->aniStop();
    zfRelease(d->thumbHideAni);
    zfRelease(d->thumbView);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIScrollThumbDefault, zfanyT<ZFUIImage>, scrollThumbImageHorizontal) {
    if(this->scrollThumbHorizontal()) {
        d->thumbView->image(this->scrollThumbImageHorizontal());
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIScrollThumbDefault, zfanyT<ZFUIImage>, scrollThumbImageVertical) {
    if(!this->scrollThumbHorizontal()) {
        d->thumbView->image(this->scrollThumbImageVertical());
    }
}

void ZFUIScrollThumbDefault::scrollThumbInit(void) {
    d->lastPos = 0;
    d->lastSize = 0;

    this->scrollView()->internalFgViewAdd(d->thumbView);

    if(this->scrollThumbHorizontal()) {
        d->thumbView->layoutParam()->layoutAlign(ZFUIAlign::e_Left | ZFUIAlign::e_Bottom);
        d->thumbView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        d->thumbView->image(this->scrollThumbImageHorizontal());
    }
    else {
        d->thumbView->layoutParam()->layoutAlign(ZFUIAlign::e_Top | ZFUIAlign::e_Right);
        d->thumbView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        d->thumbView->image(this->scrollThumbImageVertical());
    }
    d->thumbView->layoutParam()->sizeHint(ZFUISizeZero());

    d->thumbView->viewVisible(zffalse);
}
void ZFUIScrollThumbDefault::scrollThumbDealloc(void) {
    d->thumbHideAni->aniStop();

    this->scrollView()->internalFgViewRemove(d->thumbView);
}

void ZFUIScrollThumbDefault::scrollThumbUpdate(void) {
    if(this->scrollView()->scrollViewState() != ZFUIScrollViewState::e_Idle) {
        d->thumbHideAni->aniStop();
        d->thumbView->viewAlpha(1);
    }

    if(d->thumbView->image() == zfnull) {
        d->thumbView->viewVisible(zffalse);
    }
    else {
        ZFUISize imageSize = d->thumbView->image()->imageSize();
        zffloat pos = 0;
        zffloat size = 0;
        zffloat sizeRange = 0;
        zfbool viewVisibleNew = zffalse;
        if(this->scrollThumbHorizontal()) {
            sizeRange = this->scrollView()->viewFrame().width
                - ZFUIMarginGetWidth(this->scrollView()->nativeImplViewMargin());
            ZFUIScrollThumb::scrollThumbPosFromViewPos(
                pos,
                size,
                sizeRange,
                this->scrollView()->scrollArea().width,
                this->scrollView()->scrollContentFrame().x,
                this->scrollView()->scrollContentFrame().width,
                imageSize.width,
                0,
                imageSize.width);
            sizeRange -= imageSize.width;
            pos += this->scrollView()->nativeImplViewMargin().left;
            if(size != 0) {
                viewVisibleNew = zftrue;
                d->thumbView->layoutParam()->layoutMargin(ZFUIMarginMake(
                    pos, 0, 0, this->scrollView()->nativeImplViewMargin().bottom));
                d->thumbView->layoutParam()->sizeHint(ZFUISizeMake(size, imageSize.height));
            }
        }
        else {
            sizeRange = this->scrollView()->viewFrame().height
                - ZFUIMarginGetHeight(this->scrollView()->nativeImplViewMargin());
            ZFUIScrollThumb::scrollThumbPosFromViewPos(
                pos,
                size,
                sizeRange,
                this->scrollView()->scrollArea().height,
                this->scrollView()->scrollContentFrame().y,
                this->scrollView()->scrollContentFrame().height,
                imageSize.height,
                0,
                0);
            pos += this->scrollView()->nativeImplViewMargin().top;
            if(size != 0) {
                viewVisibleNew = zftrue;
                d->thumbView->layoutParam()->layoutMargin(ZFUIMarginMake(
                    0, pos, this->scrollView()->nativeImplViewMargin().right, 0));
                d->thumbView->layoutParam()->sizeHint(ZFUISizeMake(imageSize.width, size));
            }
        }

        if(d->lastPos == 0 && d->lastSize == 0) {
            d->lastPos = pos;
            d->lastSize = size;
        }

        zftimet curTime = ZFTime::timestamp();
        if(viewVisibleNew) {
            if(!d->thumbView->viewVisible()
                    && pos == d->lastPos && size == d->lastSize
                    && (this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle || size == sizeRange)
                    ) {
                viewVisibleNew = zffalse;
            }
            else {
                if(curTime - d->lastVisibleTime >= 100
                        && this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle
                        && size == sizeRange
                        ) {
                    viewVisibleNew = zffalse;
                }
                d->lastPos = pos;
                d->lastSize = size;
            }
        }

        if(viewVisibleNew) {
            d->lastVisibleTime = curTime;
        }
        else {
            d->thumbHideAni->aniStop();
        }
        d->thumbView->viewVisible(viewVisibleNew);
    }

    // auto hide scroll thumb
    if(d->thumbView->viewVisible()
            && this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle) {
        d->thumbHideAni->aniStop();
        d->thumbView->viewVisible(zftrue);
        zftimet autoHideTime = (this->scrollThumbHorizontal() ? this->scrollThumbAutoHideDurationHorizontal() : this->scrollThumbAutoHideDurationVertical());
        if(autoHideTime > 0) {
            d->thumbHideAni->aniDelay(this->scrollThumbHorizontal() ? this->scrollThumbAutoHideDelayHorizontal() : this->scrollThumbAutoHideDelayVertical());
            d->thumbHideAni->aniDuration(autoHideTime);
            d->thumbHideAni->aniStart();
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

