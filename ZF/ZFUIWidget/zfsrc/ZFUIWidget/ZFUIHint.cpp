#include "ZFUIHint.h"

#include "ZFUIOnScreenKeyboardAutoResize.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_DEFINE(zftimet, ZFUIHintDuration, (zftimet)1500)

// ============================================================
static ZFArray *_ZFP_ZFUIHint_hintListForRead(ZF_IN ZFUISysWindow *inSysWindow) {
    if(inSysWindow == zfnull) {
        inSysWindow = ZFUISysWindow::mainWindow();
    }
    if(inSysWindow == zfnull) {
        return zfnull;
    }
    return inSysWindow->objectTag("_ZFP_ZFUIHint_hintList");
}
static ZFArray *_ZFP_ZFUIHint_hintListForWrite(ZF_IN ZFUISysWindow *inSysWindow) {
    if(inSysWindow == zfnull) {
        inSysWindow = ZFUISysWindow::mainWindow();
    }
    if(inSysWindow == zfnull) {
        return zfnull;
    }
    ZFArray *hintList = inSysWindow->objectTag("_ZFP_ZFUIHint_hintList");
    if(hintList == zfnull) {
        zfobj<ZFArray> hintListTmp;
        hintList = hintListTmp;
        inSysWindow->objectTag("_ZFP_ZFUIHint_hintList", hintList);
    }
    return hintList;
}

// ============================================================
zfclass _ZFP_ZFUIHintWindow : zfextend ZFUIWindow {
    ZFOBJECT_DECLARE(_ZFP_ZFUIHintWindow, ZFUIWindow)

    ZFPROPERTY_ON_INIT_INLINE(zfbool, focusable) {
        propertyValue = zffalse;
    }

protected:
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            ) {
        if(this->childCount() == 0) {
            ret = ZFUISizeZero();
        }
        else {
            ret = this->childAt(0)->layoutMeasure(sizeHint, sizeParam);
        }
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
        if(this->childCount() > 0) {
            this->childAt(0)->viewFrame(bounds);
        }
    }
};

// ============================================================
zfclassNotPOD _ZFP_ZFUIHintPrivate {
public:
    ZFUIHint *pimplOwner;
    _ZFP_ZFUIHintWindow *window;
    zfbool windowAutoResizeAttached;
    zfbool showing;
    zfbool delaying;
    ZFAnimation *started; // auto retain
    zfautoT<ZFTimer> showDelayTimer;
    ZFListener aniShowOnStopListener;
    ZFListener showDelayTimeoutListener;
    ZFListener aniHideOnStopListener;

public:
    _ZFP_ZFUIHintPrivate(void)
    : pimplOwner(zfnull)
    , window(zfnull)
    , windowAutoResizeAttached(zffalse)
    , showing(zffalse)
    , delaying(zffalse)
    , started(zfnull)
    , showDelayTimer()
    , aniShowOnStopListener()
    , showDelayTimeoutListener()
    , aniHideOnStopListener()
    {
    }

public:
    void hintDoDelay(void) {
        this->showing = zftrue;
        this->delaying = zftrue;
    }
    void hintDoShow(void) {
        this->pimplOwner->hintOnUpdate();
        this->pimplOwner->hintOnShow();
        this->showing = zftrue;
        this->delaying = zffalse;
        this->pimplOwner->window()->show();
        zfRetainChange(this->started, this->pimplOwner->aniShow());
        if(this->started != zfnull) {
            if(!this->aniShowOnStopListener) {
                ZFLISTENER_1(callback
                        , ZFUIHint *, pimplOwner
                        ) {
                    _ZFP_ZFUIHintPrivate::aniShowOnStop(zfargs, pimplOwner);
                } ZFLISTENER_END()
                this->aniShowOnStopListener = callback;
            }
            this->started->observerAddForOnce(
                ZFAnimation::E_AniOnStop(),
                this->aniShowOnStopListener);
            this->started->target(this->pimplOwner->window());
            this->started->start();
        }
        else {
            this->hintDoShowDelay();
        }
    }
    static void aniShowOnStop(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIHint *hint
            ) {
        hint->d->hintDoShowDelay();
    }
    void hintDoShowDelay(void) {
        zfRetainChange(this->started, zfnull);

        if(!this->showDelayTimeoutListener) {
            ZFLISTENER_1(callback
                    , ZFUIHint *, pimplOwner
                    ) {
                _ZFP_ZFUIHintPrivate::showDelayTimeout(zfargs, pimplOwner);
            } ZFLISTENER_END()
            this->showDelayTimeoutListener = callback;
        }
        this->showDelayTimer = ZFTimerOnce(
            this->pimplOwner->durationFixed(),
            this->showDelayTimeoutListener);
    }
    static void showDelayTimeout(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIHint *hint
            ) {
        hint->d->showDelayTimer = zfnull;
        hint->d->hintDoHide();
    }
    void hintDoHide(void) {
        zfRetainChange(this->started, this->pimplOwner->aniHide());
        if(this->started != zfnull) {
            if(!this->aniHideOnStopListener) {
                ZFLISTENER_1(callback
                        , ZFUIHint *, pimplOwner
                        ) {
                    _ZFP_ZFUIHintPrivate::aniHideOnStop(zfargs, pimplOwner);
                } ZFLISTENER_END()
                this->aniHideOnStopListener = callback;
            }
            this->started->observerAddForOnce(
                ZFAnimation::E_AniOnStop(),
                this->aniHideOnStopListener);
            this->started->target(this->pimplOwner->window());
            this->started->start();
        }
        else {
            this->hintDoFinish();
        }
    }
    static void aniHideOnStop(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIHint *hint
            ) {
        hint->d->hintDoFinish();
    }
    void hintDoFinish(void) {
        this->showing = zffalse;
        zfRetainChange(this->started, zfnull);
        ZFArray *hintList = _ZFP_ZFUIHint_hintListForWrite(this->pimplOwner->window()->ownerSysWindow());
        zfRetain(this->pimplOwner);
        zfscopeRelease(this->pimplOwner);
        hintList->removeElement(this->pimplOwner);
        this->pimplOwner->hintOnHide();
        this->pimplOwner->window()->hide();
        if(!hintList->isEmpty()) {
            ZFUIHint *hint = hintList->getFirst();
            if(hint->delaying()) {
                hint->d->hintDoShow();
            }
        }
        zfRelease(this->pimplOwner);
    }
};

// ============================================================
static ZFCoreArray<ZFUIHint *> _ZFP_ZFUIHint_allHint;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIHintAutoFinish, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIHintAutoFinish) {
    ZFCoreArray<ZFUIHint *> t = _ZFP_ZFUIHint_allHint;
    _ZFP_ZFUIHint_allHint = ZFCoreArray<ZFUIHint *>();
    for(zfindex i = 0; i < t.count(); ++i) {
        t[i]->hide();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIHintAutoFinish)

// ============================================================
ZFOBJECT_REGISTER(ZFUIHint)

ZFEVENT_REGISTER(ZFUIHint, HintOnInit)
ZFEVENT_REGISTER(ZFUIHint, HintOnUpdate)
ZFEVENT_REGISTER(ZFUIHint, HintOnShow)
ZFEVENT_REGISTER(ZFUIHint, HintOnHide)

ZFMETHOD_DEFINE_1(ZFUIHint, ZFCoreArray<zfautoT<ZFUIHint> >, hintList
        , ZFMP_IN_OPT(ZFUISysWindow *, inSysWindow, zfnull)
        ) {
    ZFCoreArray<zfautoT<ZFUIHint> > ret;

    ZFArray *hintList = _ZFP_ZFUIHint_hintListForRead(inSysWindow);
    if(hintList == zfnull) {
        return ret;
    }

    for(zfindex i = 0; i < hintList->count(); ++i) {
        ret.add(hintList->get(i));
    }

    return ret;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIHint, zfanyT<ZFUIView>, content) {
    if(this->content() != zfnull) {
        this->window()->child(this->content());
    }
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIHint, zfanyT<ZFUIView>, content) {
    if(this->content() != zfnull) {
        this->content()->removeFromParent();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIHint, zfbool, windowAutoResize) {
    if(this->windowAutoResize()) {
        if(!d->windowAutoResizeAttached) {
            d->windowAutoResizeAttached = zftrue;
            ZFUIOnScreenKeyboardAutoResizeStart(this->window());
        }
    }
    else {
        if(d->windowAutoResizeAttached) {
            d->windowAutoResizeAttached = zffalse;
            ZFUIOnScreenKeyboardAutoResizeStop(this->window());
        }
    }
}

ZFMETHOD_DEFINE_0(ZFUIHint, ZFUIWindow *, window) {
    return d->window;
}

ZFMETHOD_DEFINE_0(ZFUIHint, zftimet, durationFixed) {
    return (this->duration() > 0 ? this->duration() : ZFUIHintDuration());
}

ZFMETHOD_DEFINE_0(ZFUIHint, void, show) {
    if(this->showing()) {
        return;
    }
    d->showing = zftrue;
    zfRetain(this);

    ZFArray *hintList = _ZFP_ZFUIHint_hintListForWrite(this->window()->ownerSysWindow());
    hintList->add(this);
    if(hintList->count() == 1) {
        d->hintDoShow();
    }
    else {
        d->hintDoDelay();
    }
}
ZFMETHOD_DEFINE_0(ZFUIHint, void, hide) {
    if(d->showing) {
        if(d->delaying) {
            d->showing = zffalse;
            d->delaying = zffalse;
            zfRetain(this);
            zfscopeRelease(this);
            ZFArray *hintList = _ZFP_ZFUIHint_hintListForWrite(this->window()->ownerSysWindow());
            hintList->removeElement(this);
            zfRelease(this);
        }
        else {
            if(d->showDelayTimer != zfnull) {
                d->showDelayTimer->stop();
                d->showDelayTimer = zfnull;
            }
            if(d->started != zfnull) {
                d->started->observerRemove(ZFAnimation::E_AniOnStop(), d->aniShowOnStopListener);
                d->started->observerRemove(ZFAnimation::E_AniOnStop(), d->aniHideOnStopListener);
                ZFAnimation *hintAniTmp = d->started;
                d->started = zfnull;
                hintAniTmp->stop();
                zfRelease(hintAniTmp);
            }
            d->showing = zffalse;
            d->delaying = zffalse;
            this->hintOnHide();
            zfRelease(this);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIHint, zfbool, showing) {
    return d->showing;
}
ZFMETHOD_DEFINE_0(ZFUIHint, zfbool, delaying) {
    return d->delaying;
}
ZFMETHOD_DEFINE_0(ZFUIHint, ZFAnimation *, started) {
    return d->started;
}

void ZFUIHint::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIHintPrivate);

    d->pimplOwner = this;

    d->window = zfAlloc(_ZFP_ZFUIHintWindow);
    d->window->windowLevel(v_ZFUIWindowLevel::e_ZFFrameworkFgHighest);
    d->window->viewSizeMin(ZFUISizeCreate(ZFUIGlobalStyle::DefaultStyle()->itemSizeText()));
    d->window->windowLayoutParam()->align(v_ZFUIAlign::e_Center);
    d->window->windowLayoutParam()->sizeParam(ZFUISizeParamWrapWrap());
    d->window->windowLayoutParam()->margin(ZFUIMarginCreate(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
    d->window->viewUIEnableTree(zffalse);

    ZFUIHint *hint = this;
    ZFLISTENER_1(windowOnUpdate
            , ZFUIHint *, hint
            ) {
        ZFCoreAssertWithMessage(!hint->showing(), "you must not change ZFUIHint's window while it's showing or delaying");
        ZFUISysWindow *sysWindowOld = zfargs.param0();
        ZFArray *hintListOld = _ZFP_ZFUIHint_hintListForWrite(sysWindowOld);
        ZFArray *hintListNew = _ZFP_ZFUIHint_hintListForWrite(hint->window()->ownerSysWindow());
        hintListNew->add(hint);
        hintListOld->removeElement(hint);
        if(!hintListOld->isEmpty()) {
            ZFUIHint *hint = hintListOld->getFirst();
            if(hint->delaying()) {
                hint->d->hintDoShow();
            }
        }
        if(!hintListNew->isEmpty()) {
            ZFUIHint *tmp = hintListNew->getFirst();
            if(!tmp->d->showing) {
                tmp->d->hintDoShow();
            }
        }
    } ZFLISTENER_END()
    d->window->observerAdd(ZFUIWindow::E_WindowOwnerSysWindowOnUpdate(), windowOnUpdate);

    ZFLISTENER_1(windowOnLayoutPrepare
            , ZFUIHint *, hint
            ) {
        hint->hintOnUpdate();
    } ZFLISTENER_END()
    d->window->observerAdd(ZFUIView::E_ViewLayoutOnLayoutPrepare(), windowOnLayoutPrepare);

    _ZFP_ZFUIHint_allHint.add(this);
}
void ZFUIHint::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();

    // access to init auto resize logic
    (void)this->windowAutoResize();

    this->hintOnInit();
}
void ZFUIHint::objectOnDeallocPrepare(void) {
    this->windowAutoResize(zffalse);
    if(d->started != zfnull) {
        d->started->stop();
        zfRetainChange(d->started, zfnull);
    }
    if(d->showDelayTimer != zfnull) {
        d->showDelayTimer->stop();
        d->showDelayTimer = zfnull;
    }
    d->window->hide();
    zfsuper::objectOnDeallocPrepare();
}
void ZFUIHint::objectOnDealloc(void) {
    _ZFP_ZFUIHint_allHint.removeElement(this);
    zfRetainChange(d->window, zfnull);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

