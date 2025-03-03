#include "ZFUIScrollThumbDefault.h"
#include "ZFUIWindow.h"
#include "ZFAniForNative.h"

#include "ZFUIKit/protocol/ZFProtocolZFAniForNative.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIScrollThumbDefault)

// ============================================================
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAni : zfextend ZFAniForTimer {
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAni, ZFAniForTimer)

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
    virtual void aniTimerOnUpdate(ZF_IN zffloat progress) {
        zfsuper::aniTimerOnUpdate(progress);
        if(this->thumbView->visible()) {
            this->thumbView->alpha(1 - progress);
        }
    }
};
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView : zfextend ZFAniForNative {
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView, ZFAniForNative)

protected:
    virtual void objectOnInit(ZF_IN ZFUIImageView *thumbView) {
        this->objectOnInit();
        this->target(thumbView);
        this->alphaTo(0);
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
};
zfclassNotPOD _ZFP_ZFUIScrollThumbDefaultPrivate {
public:
    zfautoT<ZFUIImageView> thumbView;
    zfautoT<ZFAniQueue> thumbHideAni; // [delay, actualAni]
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
            owner->d->thumbView->alpha(1);
            owner->d->thumbView->visible(zffalse);
        }
    }
    static void thumbHideAniAutoStop(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIScrollThumbDefault *owner
            ) {
        if(owner != zfnull) {
            owner->d->thumbHideAni->stop();
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIScrollThumbDefault)

void ZFUIScrollThumbDefault::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIScrollThumbDefaultPrivate);

    d->thumbView = zfobj<ZFUIImageView>();
    d->thumbHideAni = zfobj<ZFAniQueue>();
    d->thumbHideAni->wait(0);

    if(ZFPROTOCOL_IS_AVAILABLE(ZFAniForNative)) {
        d->thumbHideAni->child(zfobj<_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView>(d->thumbView));
    }
    else {
        d->thumbHideAni->child(zfobj<_ZFP_ZFUIScrollThumbDefault_HideThumbAni>(d->thumbView));
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
    d->thumbHideAni->observerAdd(ZFAnimation::E_AniOnStop(), aniOnStop);

    ZFLISTENER_1(thumbHideAniAutoStop
            , ZFUIScrollThumbDefault *, owner
            ) {
        _ZFP_ZFUIScrollThumbDefaultPrivate::thumbHideAniAutoStop(zfargs, owner);
    } ZFLISTENER_END()
    d->thumbHideAniAutoStopListener = thumbHideAniAutoStop;
    ZFGlobalObserver().observerAdd(ZFUIWindow::E_WindowOnHide(), d->thumbHideAniAutoStopListener);
}
void ZFUIScrollThumbDefault::objectOnDeallocPrepare(void) {
    ZFGlobalObserver().observerRemove(ZFUIWindow::E_WindowOnHide(), d->thumbHideAniAutoStopListener);
    zfsuper::objectOnDeallocPrepare();
}
void ZFUIScrollThumbDefault::objectOnDealloc(void) {
    d->thumbHideAni->stop();
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIScrollThumbDefault, zfanyT<ZFUIImage>, thumbImageHorizontal) {
    if(this->scrollThumbHorizontal()) {
        d->thumbView->image(this->thumbImageHorizontal());
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIScrollThumbDefault, zfanyT<ZFUIImage>, thumbImageVertical) {
    if(!this->scrollThumbHorizontal()) {
        d->thumbView->image(this->thumbImageVertical());
    }
}

void ZFUIScrollThumbDefault::scrollThumbInit(void) {
    d->lastPos = 0;
    d->lastSize = 0;

    this->scrollView()->internalFgViewAdd(d->thumbView);

    if(this->scrollThumbHorizontal()) {
        d->thumbView->layoutParam()->align(v_ZFUIAlign::e_Left | v_ZFUIAlign::e_Bottom);
        d->thumbView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        d->thumbView->image(this->thumbImageHorizontal());
    }
    else {
        d->thumbView->layoutParam()->align(v_ZFUIAlign::e_Top | v_ZFUIAlign::e_Right);
        d->thumbView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        d->thumbView->image(this->thumbImageVertical());
    }
    d->thumbView->layoutParam()->sizeHint(ZFUISizeZero());

    d->thumbView->visible(zffalse);
}
void ZFUIScrollThumbDefault::scrollThumbDealloc(void) {
    d->thumbHideAni->stop();

    this->scrollView()->internalFgViewRemove(d->thumbView);
}

void ZFUIScrollThumbDefault::scrollThumbUpdate(void) {
    if(this->scrollView()->scrollViewState() != v_ZFUIScrollViewState::e_Idle) {
        d->thumbHideAni->stop();
        d->thumbView->alpha(1);
    }

    if(d->thumbView->imageState() == zfnull) {
        d->thumbView->visible(zffalse);
    }
    else {
        ZFUISize imageSize = d->thumbView->imageState()->imageSize();
        zffloat pos = 0;
        zffloat size = 0;
        zffloat sizeRange = 0;
        zfbool visibleNew = zffalse;
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
                visibleNew = zftrue;
                d->thumbView->layoutParam()->margin(ZFUIMarginCreate(
                    pos, 0, 0, this->scrollView()->nativeImplViewMargin().bottom));
                d->thumbView->layoutParam()->sizeHint(ZFUISizeCreate(size, imageSize.height));
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
                visibleNew = zftrue;
                d->thumbView->layoutParam()->margin(ZFUIMarginCreate(
                    0, pos, this->scrollView()->nativeImplViewMargin().right, 0));
                d->thumbView->layoutParam()->sizeHint(ZFUISizeCreate(imageSize.width, size));
            }
        }

        if(d->lastPos == 0 && d->lastSize == 0) {
            d->lastPos = pos;
            d->lastSize = size;
        }

        zftimet curTime = ZFTime::timestamp();
        if(visibleNew) {
            if(!d->thumbView->visible()
                    && pos == d->lastPos && size == d->lastSize
                    && (this->scrollView()->scrollViewState() == v_ZFUIScrollViewState::e_Idle || size == sizeRange)
                    ) {
                visibleNew = zffalse;
            }
            else {
                if(curTime - d->lastVisibleTime >= 100
                        && this->scrollView()->scrollViewState() == v_ZFUIScrollViewState::e_Idle
                        && size == sizeRange
                        ) {
                    visibleNew = zffalse;
                }
                d->lastPos = pos;
                d->lastSize = size;
            }
        }

        if(visibleNew) {
            d->lastVisibleTime = curTime;
        }
        else {
            d->thumbHideAni->stop();
        }
        d->thumbView->visible(visibleNew);
    }

    // auto hide scroll thumb
    if(d->thumbView->visible()
            && this->scrollView()->scrollViewState() == v_ZFUIScrollViewState::e_Idle) {
        d->thumbHideAni->stop();
        d->thumbView->visible(zftrue);
        zftimet autoHideTime = (this->scrollThumbHorizontal() ? this->autoHideDurationHorizontal() : this->autoHideDurationVertical());
        if(autoHideTime > 0) {
            d->thumbHideAni->childAt(0)->duration(this->scrollThumbHorizontal() ? this->autoHideDelayHorizontal() : this->autoHideDelayVertical());
            d->thumbHideAni->childAt(1)->duration(autoHideTime);
            d->thumbHideAni->start();
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

