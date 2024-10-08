#include "ZFUIViewBlinkWhenFocus.h"
#include "ZFUIViewBlink.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkWhenFocusAutoApplyStart)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkWhenFocusAutoApplyStop)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkWhenFocusAutoApplyPause)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkWhenFocusAutoApplyResume)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkWhenFocusViewBlinkOn)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkWhenFocusViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
ZFEXPORT_VAR_DEFINE(ZFFilterForZFObject, ZFUIViewBlinkWhenFocusFilter, ZFFilterForZFObject())
ZFEXPORT_VAR_DEFINE(zfbool, ZFUIViewBlinkWhenFocusAutoApply, zftrue)
ZFEXPORT_VAR_DEFINE(zfauto, ZFUIViewBlinkWhenFocusMaskImage, zfnull)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocus_settingInit, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocus_settingInit) {
    ZFUIViewBlinkWhenFocusFilter().filterRemoveAll();
    ZFUIViewBlinkWhenFocusAutoApply(zftrue);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocus_settingInit)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusAutoApply_autoStart, ZFLevelAppLow) {
    if(ZFUIViewBlinkWhenFocusAutoApply()) {
        ZFUIViewBlinkWhenFocusAutoApplyStart();
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusAutoApply_autoStart) {
    ZFUIViewBlinkWhenFocusAutoApplyStop();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusAutoApply_autoStart)

// ============================================================
static void _ZFP_ZFUIViewBlinkWhenFocus_focusOnUpdate(ZF_IN const ZFArgs &zfargs);
static void _ZFP_ZFUIViewBlinkWhenFocus_mouseDown(ZF_IN const ZFArgs &zfargs);
static void _ZFP_ZFUIViewBlinkWhenFocus_viewOnDealloc(ZF_IN const ZFArgs &zfargs);
static zfbool _ZFP_ZFUIViewBlinkWhenFocus_started = zffalse;
static zffloat _ZFP_ZFUIViewBlinkWhenFocus_paused = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusDataHolder, ZFLevelZFFrameworkNormal) {
    this->focusOnUpdateListener = ZFCallbackForFunc(_ZFP_ZFUIViewBlinkWhenFocus_focusOnUpdate);
    this->mouseDownListener = ZFCallbackForFunc(_ZFP_ZFUIViewBlinkWhenFocus_mouseDown);
    this->viewOnDeallocListener = ZFCallbackForFunc(_ZFP_ZFUIViewBlinkWhenFocus_viewOnDealloc);

    this->viewBlinkOnListener = ZFCallbackForFunc(zfself::viewBlinkOn);
    this->viewBlinkOffListener = ZFCallbackForFunc(zfself::viewBlinkOff);
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::EventViewBlinkOn(), this->viewBlinkOnListener);
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::EventViewBlinkOff(), this->viewBlinkOffListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusDataHolder) {
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::EventViewBlinkOn(), this->viewBlinkOnListener);
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::EventViewBlinkOff(), this->viewBlinkOffListener);
    ZFUIViewBlinkWhenFocusMaskImage(zfnull);
}
public:
    zfautoT<ZFUIImage> maskImageCur;
    ZFListener viewBlinkOnListener;
    ZFListener viewBlinkOffListener;
    ZFListener focusOnUpdateListener;
    ZFListener mouseDownListener;
    ZFListener viewOnDeallocListener;
    ZFCoreArray<ZFUIView *> focusedViews;
    static void viewBlinkOn(ZF_IN const ZFArgs &zfargs) {
        ZFGlobalObserver().observerNotifyWithSender(
            zfargs.sender(), ZFGlobalEvent::EventViewBlinkWhenFocusViewBlinkOn());
    }
    static void viewBlinkOff(ZF_IN const ZFArgs &zfargs) {
        ZFGlobalObserver().observerNotifyWithSender(
            zfargs.sender(), ZFGlobalEvent::EventViewBlinkWhenFocusViewBlinkOff());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusDataHolder)

// ============================================================
static void _ZFP_ZFUIViewBlinkWhenFocus_focusOnUpdate(ZF_IN const ZFArgs &zfargs) {
    ZFUIView *view = zfargs.sender();

    if(view->objectIsInternal()) {
        return;
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);

    if(view->focused()) {
        if(!ZFUIViewBlinkWhenFocusFilter().filterPassed(view)) {
            return;
        }
        d->focusedViews.add(view);
        view->observerAdd(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerAdd(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
        if(!ZFUIViewBlinkWhenFocusAutoApplyPaused()) {
            ZFUIViewBlink(view, d->maskImageCur);
        }
    }
    else {
        d->focusedViews.removeElement(view);
        view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
    }
}
static void _ZFP_ZFUIViewBlinkWhenFocus_mouseDown(ZF_IN const ZFArgs &zfargs) {
    ZFUIView *view = zfargs.sender();
    if(view->objectIsInternal()) {
        return;
    }

    ZFUIMouseEvent *event = zfargs.param0();
    if(event == zfnull || event->mouseAction != ZFUIMouseAction::e_Down) {
        return;
    }
    if(!ZFUIViewBlinkWhenFocusFilter().filterPassed(view)) {
        return;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);
    if(!ZFUIViewBlinkWhenFocusAutoApplyPaused()) {
        ZFUIViewBlink(view, d->maskImageCur);
    }
}
static void _ZFP_ZFUIViewBlinkWhenFocus_viewOnDealloc(ZF_IN const ZFArgs &zfargs) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);
    ZFUIView *view = zfargs.sender();
    d->focusedViews.removeElement(view);
    view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
    view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFUIViewBlinkWhenFocusAutoApplyStart
        , ZFMP_IN_OPT(ZFUIImage *, img, zfnull)
        ) {
    ZFUIViewBlinkWhenFocusAutoApplyStop();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);

    _ZFP_ZFUIViewBlinkWhenFocus_started = zftrue;
    d->maskImageCur = ((img != zfnull) ? img : ZFUIViewBlinkWhenFocusMaskImage().toObject());

    ZFGlobalObserver().observerAdd(ZFUIView::EventViewFocusOnUpdate(), d->focusOnUpdateListener);
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyStart());
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewBlinkWhenFocusAutoApplyStop) {
    ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel();
    if(!_ZFP_ZFUIViewBlinkWhenFocus_started) {
        return;
    }
    _ZFP_ZFUIViewBlinkWhenFocus_started = zffalse;

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);
    ZFGlobalObserver().observerRemove(ZFUIView::EventViewFocusOnUpdate(), d->focusOnUpdateListener);
    for(zfindex i = 0; i < d->focusedViews.count(); ++i) {
        ZFUIView *view = d->focusedViews[i];
        view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
    }
    d->focusedViews.removeAll();
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyStop());
}
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFUIViewBlinkWhenFocusAutoApplyStarted) {
    return _ZFP_ZFUIViewBlinkWhenFocus_started;
}

ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewBlinkWhenFocusAutoApplyPause) {
    ++_ZFP_ZFUIViewBlinkWhenFocus_paused;
    if(_ZFP_ZFUIViewBlinkWhenFocus_paused == 1) {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyPause());
    }
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewBlinkWhenFocusAutoApplyResume) {
    ZFCoreAssert(_ZFP_ZFUIViewBlinkWhenFocus_paused > 0);
    --_ZFP_ZFUIViewBlinkWhenFocus_paused;
    if(_ZFP_ZFUIViewBlinkWhenFocus_paused == 0) {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyResume());
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfindex, ZFUIViewBlinkWhenFocusAutoApplyPaused) {
    return (!_ZFP_ZFUIViewBlinkWhenFocus_started || _ZFP_ZFUIViewBlinkWhenFocus_paused);
}

// ============================================================
// ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder, ZFLevelZFFrameworkHigh) {
    this->started = zffalse;
    this->delayTimer = zfnull;
    this->doStopListener = ZFCallbackForFunc(zfself::doStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder) {
    zfRelease(this->delayTimer);
}
public:
    zfbool started;
    ZFTimer *delayTimer;
    zftimet endTime;
    ZFListener doStopListener;
    static void doStop(ZF_IN const ZFArgs &zfargs) {
        ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder)

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
        , ZFMP_IN_OPT(zftimet, time, 0)
        ) {
    if(ZFUIViewBlinkWhenFocusAutoApplyPaused()) {
        return;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder);

    time = zfmMax(time, (zftimet)100);
    zftimet endTime = ZFTime::timestamp() + time;
    if(d->started && endTime <= d->endTime + 100) {
        return;
    }

    if(d->delayTimer == zfnull) {
        d->delayTimer = zfAlloc(ZFTimer);
        d->delayTimer->observerAdd(ZFTimer::EventTimerOnActivate(), d->doStopListener);
    }
    d->delayTimer->stop();

    d->endTime = endTime;
    d->delayTimer->interval(time);
    ZFUIViewBlinkWhenFocusAutoApplyPause();
    d->started = zftrue;
    d->delayTimer->start();
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder);
    if(!d->started) {
        return;
    }
    d->started = zffalse;
    d->delayTimer->stop();
    ZFUIViewBlinkWhenFocusAutoApplyResume();
}

// ZFUIViewBlinkWhenFocusAutoApplyPauseForTime auto cleanup
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeAutoCleanup, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeAutoCleanup) {
    ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeAutoCleanup)

ZF_NAMESPACE_GLOBAL_END

