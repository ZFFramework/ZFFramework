#include "ZFUIHint.h"

#include "ZFUIOnScreenKeyboardAutoResize.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_DEFINE(zftimet, ZFUIHintDurationDefault, (zftimet)1500)

// ============================================================
static ZFArray *_ZFP_ZFUIHint_hintListForRead(ZF_IN ZFUISysWindow *inSysWindow)
{
    if(inSysWindow == zfnull)
    {
        inSysWindow = ZFUISysWindow::mainWindow();
    }
    if(inSysWindow == zfnull)
    {
        return zfnull;
    }
    return inSysWindow->objectTag<ZFArray *>("_ZFP_ZFUIHint_hintList");
}
static ZFArray *_ZFP_ZFUIHint_hintListForWrite(ZF_IN ZFUISysWindow *inSysWindow)
{
    if(inSysWindow == zfnull)
    {
        inSysWindow = ZFUISysWindow::mainWindow();
    }
    if(inSysWindow == zfnull)
    {
        return zfnull;
    }
    ZFArray *hintList = inSysWindow->objectTag<ZFArray *>("_ZFP_ZFUIHint_hintList");
    if(hintList == zfnull)
    {
        zfblockedAlloc(ZFArray, hintListTmp);
        hintList = hintListTmp;
        inSysWindow->objectTag("_ZFP_ZFUIHint_hintList", hintList);
    }
    return hintList;
}

// ============================================================
zfclass _ZFP_ZFUIHintWindow : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIHintWindow, ZFUIWindow)

    ZFPROPERTY_ON_INIT_INLINE(zfbool, viewFocusable)
    {
        propertyValue = zffalse;
    }

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam)
    {
        if(this->childCount() == 0)
        {
            ret = ZFUISizeZero();
        }
        else
        {
            ret = this->childAt(0)->layoutMeasure(sizeHint, sizeParam);
        }
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        if(this->childCount() > 0)
        {
            this->childAt(0)->viewFrame(bounds);
        }
    }
};

// ============================================================
zfclassNotPOD _ZFP_ZFUIHintPrivate
{
public:
    ZFUIHint *pimplOwner;
    _ZFP_ZFUIHintWindow *hintWindow;
    zfbool hintShowing;
    zfbool hintDelaying;
    ZFAnimation *hintAnimating; // auto retain
    zfautoObjectT<ZFTimer *> hintShowDelayTimer;
    ZFListener hintAniShowOnStopListener;
    ZFListener hintShowDelayTimeoutListener;
    ZFListener hintAniHideOnStopListener;

public:
    _ZFP_ZFUIHintPrivate(void)
    : pimplOwner(zfnull)
    , hintWindow(zfnull)
    , hintShowing(zffalse)
    , hintDelaying(zffalse)
    , hintAnimating(zfnull)
    , hintShowDelayTimer()
    , hintAniShowOnStopListener()
    , hintShowDelayTimeoutListener()
    , hintAniHideOnStopListener()
    {
    }

public:
    void hintDoDelay(void)
    {
        this->hintShowing = zftrue;
        this->hintDelaying = zftrue;
    }
    void hintDoShow(void)
    {
        this->pimplOwner->hintOnUpdate();
        this->pimplOwner->hintOnShow();
        this->hintShowing = zftrue;
        this->hintDelaying = zffalse;
        this->pimplOwner->hintWindow()->windowShow();
        zfRetainChange(this->hintAnimating, this->pimplOwner->hintAniShow());
        if(this->hintAnimating != zfnull)
        {
            if(!this->hintAniShowOnStopListener)
            {
                ZFLISTENER_1(callback
                        , ZFUIHint *, pimplOwner
                        ) {
                    _ZFP_ZFUIHintPrivate::hintAniShowOnStop(zfargs, pimplOwner);
                } ZFLISTENER_END(callback)
                this->hintAniShowOnStopListener = callback;
            }
            this->hintAnimating->observerAddForOnce(
                ZFAnimation::EventAniOnStopOrInvalid(),
                this->hintAniShowOnStopListener);
            this->hintAnimating->aniTarget(this->pimplOwner->hintWindow());
            this->hintAnimating->aniStart();
        }
        else
        {
            this->hintDoShowDelay();
        }
    }
    static void hintAniShowOnStop(ZF_IN const ZFArgs &zfargs, ZF_IN ZFUIHint *hint)
    {
        hint->d->hintDoShowDelay();
    }
    void hintDoShowDelay(void)
    {
        zfRetainChange(this->hintAnimating, zfnull);

        if(!this->hintShowDelayTimeoutListener)
        {
            ZFLISTENER_1(callback
                    , ZFUIHint *, pimplOwner
                    ) {
                _ZFP_ZFUIHintPrivate::hintShowDelayTimeout(zfargs, pimplOwner);
            } ZFLISTENER_END(callback)
            this->hintShowDelayTimeoutListener = callback;
        }
        this->hintShowDelayTimer = ZFTimerOnce(
            this->pimplOwner->hintDurationFixed(),
            this->hintShowDelayTimeoutListener);
    }
    static void hintShowDelayTimeout(ZF_IN const ZFArgs &zfargs, ZF_IN ZFUIHint *hint)
    {
        hint->d->hintShowDelayTimer = zfnull;
        hint->d->hintDoHide();
    }
    void hintDoHide(void)
    {
        zfRetainChange(this->hintAnimating, this->pimplOwner->hintAniHide());
        if(this->hintAnimating != zfnull)
        {
            if(!this->hintAniHideOnStopListener)
            {
                ZFLISTENER_1(callback
                        , ZFUIHint *, pimplOwner
                        ) {
                    _ZFP_ZFUIHintPrivate::hintAniHideOnStop(zfargs, pimplOwner);
                } ZFLISTENER_END(callback)
                this->hintAniHideOnStopListener = callback;
            }
            this->hintAnimating->observerAddForOnce(
                ZFAnimation::EventAniOnStopOrInvalid(),
                this->hintAniHideOnStopListener);
            this->hintAnimating->aniTarget(this->pimplOwner->hintWindow());
            this->hintAnimating->aniStart();
        }
        else
        {
            this->hintDoFinish();
        }
    }
    static void hintAniHideOnStop(ZF_IN const ZFArgs &zfargs, ZF_IN ZFUIHint *hint)
    {
        hint->d->hintDoFinish();
    }
    void hintDoFinish(void)
    {
        this->hintShowing = zffalse;
        zfRetainChange(this->hintAnimating, zfnull);
        ZFArray *hintList = _ZFP_ZFUIHint_hintListForWrite(this->pimplOwner->hintWindow()->windowOwnerSysWindow());
        zfRetain(this->pimplOwner);
        zfblockedRelease(this->pimplOwner);
        hintList->removeElement(this->pimplOwner, ZFComparerCheckEqual);
        this->pimplOwner->hintOnHide();
        this->pimplOwner->hintWindow()->windowHide();
        if(!hintList->isEmpty())
        {
            ZFUIHint *hint = hintList->getFirst<ZFUIHint *>();
            if(hint->hintDelaying())
            {
                hint->d->hintDoShow();
            }
        }
        zfRelease(this->pimplOwner);
    }
};

// ============================================================
static ZFCoreArrayPOD<ZFUIHint *> _ZFP_ZFUIHint_allHint;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIHintAutoFinish, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIHintAutoFinish)
{
    ZFCoreArrayPOD<ZFUIHint *> t = _ZFP_ZFUIHint_allHint;
    _ZFP_ZFUIHint_allHint = ZFCoreArrayPOD<ZFUIHint *>();
    for(zfindex i = 0; i < t.count(); ++i)
    {
        t[i]->hintHide();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIHintAutoFinish)

// ============================================================
ZFOBJECT_REGISTER(ZFUIHint)

ZFOBSERVER_EVENT_REGISTER(ZFUIHint, HintOnInit)
ZFOBSERVER_EVENT_REGISTER(ZFUIHint, HintOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIHint, HintOnShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIHint, HintOnHide)

ZFMETHOD_DEFINE_1(ZFUIHint, ZFCoreArrayPOD<ZFUIHint *>, hintList,
                  ZFMP_IN_OPT(ZFUISysWindow *, inSysWindow, zfnull))
{
    ZFCoreArrayPOD<ZFUIHint *> ret;

    ZFArray *hintList = _ZFP_ZFUIHint_hintListForRead(inSysWindow);
    if(hintList == zfnull)
    {
        return ret;
    }

    for(zfindex i = 0; i < hintList->count(); ++i)
    {
        ret.add(hintList->get<ZFUIHint *>(i));
    }

    return ret;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIHint, ZFUIView *, hintContent)
{
    if(this->hintContent() != zfnull)
    {
        this->hintWindow()->childAdd(this->hintContent());
    }
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIHint, ZFUIView *, hintContent)
{
    if(this->hintContent() != zfnull)
    {
        this->hintContent()->viewRemoveFromParent();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIHint, zfbool, hintWindowAutoResize)
{
    if(this->hintWindowAutoResize() != propertyValueOld)
    {
        if(this->hintWindowAutoResize())
        {
            ZFUIOnScreenKeyboardAutoResizeStart(this->hintWindow());
        }
        else
        {
            ZFUIOnScreenKeyboardAutoResizeStop(this->hintWindow());
        }
    }
}

ZFMETHOD_DEFINE_0(ZFUIHint, ZFUIWindow *, hintWindow)
{
    return d->hintWindow;
}

ZFMETHOD_DEFINE_0(ZFUIHint, zftimet, hintDurationFixed)
{
    return (this->hintDuration() > 0 ? this->hintDuration() : ZFUIHintDurationDefault());
}

ZFMETHOD_DEFINE_0(ZFUIHint, void, hintShow)
{
    if(this->hintShowing())
    {
        return ;
    }
    d->hintShowing = zftrue;
    zfRetain(this);

    ZFArray *hintList = _ZFP_ZFUIHint_hintListForWrite(this->hintWindow()->windowOwnerSysWindow());
    hintList->add(this);
    if(hintList->count() == 1)
    {
        d->hintDoShow();
    }
    else
    {
        d->hintDoDelay();
    }
}
ZFMETHOD_DEFINE_0(ZFUIHint, void, hintHide)
{
    if(d->hintShowing)
    {
        if(d->hintDelaying)
        {
            d->hintShowing = zffalse;
            d->hintDelaying = zffalse;
            zfRetain(this);
            zfblockedRelease(this);
            ZFArray *hintList = _ZFP_ZFUIHint_hintListForWrite(this->hintWindow()->windowOwnerSysWindow());
            hintList->removeElement(this, ZFComparerCheckEqual);
            zfRelease(this);
        }
        else
        {
            if(d->hintShowDelayTimer != zfnull)
            {
                d->hintShowDelayTimer->timerStop();
                d->hintShowDelayTimer = zfnull;
            }
            if(d->hintAnimating != zfnull)
            {
                d->hintAnimating->observerRemove(ZFAnimation::EventAniOnStopOrInvalid(), d->hintAniShowOnStopListener);
                d->hintAnimating->observerRemove(ZFAnimation::EventAniOnStopOrInvalid(), d->hintAniHideOnStopListener);
                ZFAnimation *hintAniTmp = d->hintAnimating;
                d->hintAnimating = zfnull;
                hintAniTmp->aniStop();
                zfRelease(hintAniTmp);
            }
            d->hintShowing = zffalse;
            d->hintDelaying = zffalse;
            this->hintOnHide();
            zfRelease(this);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIHint, zfbool, hintShowing)
{
    return d->hintShowing;
}
ZFMETHOD_DEFINE_0(ZFUIHint, zfbool, hintDelaying)
{
    return d->hintDelaying;
}
ZFMETHOD_DEFINE_0(ZFUIHint, ZFAnimation *, hintAnimating)
{
    return d->hintAnimating;
}

void ZFUIHint::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIHintPrivate);

    d->pimplOwner = this;

    d->hintWindow = zfAlloc(_ZFP_ZFUIHintWindow);
    d->hintWindow->windowLevel(ZFUIWindowLevel::e_ZFFrameworkFgHighest);
    d->hintWindow->viewSizeMin(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeText()));
    d->hintWindow->windowLayoutParam()->layoutAlign(ZFUIAlign::e_Center);
    d->hintWindow->windowLayoutParam()->sizeParam(ZFUISizeParamWrapWrap());
    d->hintWindow->windowLayoutParam()->layoutMargin(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
    d->hintWindow->viewUIEnableTree(zffalse);

    ZFUIHint *hint = this;
    ZFLISTENER_1(hintWindowChanged
            , ZFUIHint *, hint
            ) {
        zfCoreAssertWithMessage(!hint->hintShowing(), "you must not change ZFUIHint's window while it's showing or delaying");
        ZFUISysWindow *sysWindowOld = zfargs.param0T();
        ZFArray *hintListOld = _ZFP_ZFUIHint_hintListForWrite(sysWindowOld);
        ZFArray *hintListNew = _ZFP_ZFUIHint_hintListForWrite(hint->hintWindow()->windowOwnerSysWindow());
        hintListNew->add(hint);
        hintListOld->removeElement(hint, ZFComparerCheckEqual);
        if(!hintListOld->isEmpty())
        {
            ZFUIHint *hint = hintListOld->getFirst<ZFUIHint *>();
            if(hint->hintDelaying())
            {
                hint->d->hintDoShow();
            }
        }
        if(!hintListNew->isEmpty())
        {
            ZFUIHint *tmp = hintListNew->getFirst<ZFUIHint *>();
            if(!tmp->d->hintShowing)
            {
                tmp->d->hintDoShow();
            }
        }
    } ZFLISTENER_END(hintWindowChanged)
    d->hintWindow->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnChange(), hintWindowChanged);

    ZFLISTENER_1(hintWindowOnLayoutPrepare
            , ZFUIHint *, hint
            ) {
        hint->hintOnUpdate();
    } ZFLISTENER_END(hintWindowOnLayoutPrepare)
    d->hintWindow->observerAdd(ZFUIView::EventViewLayoutOnLayoutPrepare(), hintWindowOnLayoutPrepare);

    _ZFP_ZFUIHint_allHint.add(this);
}
void ZFUIHint::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    if(this->hintWindowAutoResize())
    {
        ZFUIOnScreenKeyboardAutoResizeStart(this->hintWindow());
    }
    this->hintOnInit();
}
void ZFUIHint::objectOnDealloc(void)
{
    _ZFP_ZFUIHint_allHint.removeElement(this);
    this->hintWindowAutoResize(zffalse);
    if(d->hintAnimating != zfnull)
    {
        d->hintAnimating->aniStop();
        zfRetainChange(d->hintAnimating, zfnull);
    }
    if(d->hintShowDelayTimer != zfnull)
    {
        d->hintShowDelayTimer->timerStop();
        d->hintShowDelayTimer = zfnull;
    }
    d->hintWindow->windowHide();
    zfRetainChange(d->hintWindow, zfnull);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

