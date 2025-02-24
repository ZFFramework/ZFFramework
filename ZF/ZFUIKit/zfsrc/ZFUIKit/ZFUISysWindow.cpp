#include "ZFUISysWindow.h"
#include "protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIRootView.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(SysWindowMainWindowOnAttach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfclassNotPOD _ZFP_ZFUISysWindowPrivate {
public:
    zfautoT<ZFUISysWindowEmbedImpl> embedImpl;
    void *nativeWindow;
    zfautoT<ZFUIRootView> windowRootView;
    ZFUIOrientationFlags sysWindowOrientationFlags;
    ZFUISysWindow *modalWindowOwner;
    zfautoT<ZFUISysWindow> modalWindowShowing;
    zfautoT<ZFUILayoutParam> sysWindowLayoutParam;
    ZFListener sysWindowLayoutParamOnUpdateListener;
    zfbool nativeWindowCreated;
    zfbool nativeWindowResumed;
    ZFUISize sysWindowSize;
    ZFUIMargin sysWindowMargin;

public:
    _ZFP_ZFUISysWindowPrivate(void)
    : embedImpl(zfnull)
    , nativeWindow(zfnull)
    , windowRootView(zfnull)
    , sysWindowOrientationFlags(ZFUIOrientation::e_Top)
    , modalWindowOwner(zfnull)
    , modalWindowShowing(zfnull)
    , sysWindowLayoutParam(zfnull)
    , sysWindowLayoutParamOnUpdateListener()
    , nativeWindowCreated(zffalse)
    , nativeWindowResumed(zffalse)
    , sysWindowSize()
    , sysWindowMargin()
    {
    }
};

ZFOBJECT_REGISTER(ZFUISysWindow)

ZFEVENT_REGISTER(ZFUISysWindow, SysWindowOnCreate)
ZFEVENT_REGISTER(ZFUISysWindow, SysWindowOnDestroy)
ZFEVENT_REGISTER(ZFUISysWindow, SysWindowOnResume)
ZFEVENT_REGISTER(ZFUISysWindow, SysWindowOnPause)
ZFEVENT_REGISTER(ZFUISysWindow, SysWindowOnRotate)
ZFEVENT_REGISTER(ZFUISysWindow, SysWindowMarginOnUpdate)
ZFEVENT_REGISTER(ZFUISysWindow, SysWindowOnKeyEvent)

zfautoT<ZFUISysWindow> ZFUISysWindow::nativeWindowEmbed(ZF_IN ZFUISysWindowEmbedImpl *embedImpl) {
    zfautoT<ZFUISysWindow> tmp = ZFUISysWindow::ClassData()->newInstance();
    ZFUISysWindow *ret = tmp;
    ret->d->embedImpl = embedImpl;
    embedImpl->_ZFP_ownerZFUISysWindow = ret;
    embedImpl->sysWindowLayoutParamOnInit(ret);
    return tmp;
}
ZFUISysWindowEmbedImpl *ZFUISysWindow::nativeWindowEmbedImpl(void) {
    return d->embedImpl;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, void, nativeWindowEmbedImplDestroy) {
    if(this->nativeWindowEmbedImpl() != zfnull) {
        if(this->nativeWindowIsResumed()) {
            this->nativeWindowEmbedImpl()->notifyOnPause(this);
        }
        if(this->nativeWindowIsCreated()) {
            this->nativeWindowEmbedImpl()->notifyOnDestroy(this);
        }
    }
}

// ============================================================
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindowBuiltin = zfnull;
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindow = zfnull;
static zfweakT<ZFUISysWindow> _ZFP_ZFUISysWindow_keyWindow;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUISysWindowMainWindowCleanup, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUISysWindowMainWindowCleanup) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUISysWindow) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUISysWindow);
    if(impl != zfnull) {
        impl->mainWindowOnCleanup();
    }
    zfblockedRelease(_ZFP_ZFUISysWindow_mainWindowRegistered);
    zfblockedRelease(_ZFP_ZFUISysWindow_keyWindow);

    _ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
    _ZFP_ZFUISysWindow_mainWindowBuiltin = zfnull;
    _ZFP_ZFUISysWindow_mainWindow = zfnull;
    _ZFP_ZFUISysWindow_keyWindow = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUISysWindowMainWindowCleanup)

void ZFUISysWindow::mainWindowRegister(ZF_IN ZFUISysWindow *window) {
    ZFCoreAssertWithMessage(_ZFP_ZFUISysWindow_mainWindow == zfnull,
        "[ZFUISysWindow] mainWindowRegister must be called before accessing mainWindow");
    ZFCoreAssertWithMessage(window != zfnull,
        "[ZFUISysWindow] mainWindowRegister called with null window");

    zfblockedRelease(_ZFP_ZFUISysWindow_mainWindowRegistered);
    _ZFP_ZFUISysWindow_mainWindowRegistered = zfRetain(window);
    _ZFP_ZFUISysWindow_mainWindow = _ZFP_ZFUISysWindow_mainWindowRegistered;

    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_SysWindowMainWindowOnAttach());
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, zfanyT<ZFUISysWindow>, mainWindow) {
    if(_ZFP_ZFUISysWindow_mainWindow == zfnull) {
        _ZFP_ZFUISysWindow_mainWindowBuiltin = ZFPROTOCOL_ACCESS(ZFUISysWindow)->mainWindow();
        _ZFP_ZFUISysWindow_mainWindow = _ZFP_ZFUISysWindow_mainWindowBuiltin;
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_SysWindowMainWindowOnAttach());
    }
    return _ZFP_ZFUISysWindow_mainWindow;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, zfbool, mainWindowAttached) {
    return (_ZFP_ZFUISysWindow_mainWindow != zfnull);
}

ZFMETHOD_DEFINE_1(ZFUISysWindow, void, keyWindow
        , ZFMP_IN(ZFUISysWindow *, window)
        ) {
    _ZFP_ZFUISysWindow_keyWindow = window;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, zfanyT<ZFUISysWindow>, keyWindow) {
    ZFUISysWindow *ret = zfnull;
    if(_ZFP_ZFUISysWindow_keyWindow != zfnull) {
        ret = _ZFP_ZFUISysWindow_keyWindow;
    }
    if(ret != zfnull) {
        return ret;
    }
    else {
        return ZFUISysWindow::mainWindow();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUISysWindow, const ZFUISize &, sysWindowSize) {
    return d->sysWindowSize;
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUISysWindow, const ZFUIMargin &, sysWindowMargin) {
    return d->sysWindowMargin;
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_sysWindowMargin(ZF_IN const ZFUIMargin &sysWindowMargin) {
    ZFUIMargin sysWindowMarginOld = d->sysWindowMargin;
    ZFUIMarginApplyScaleReverselyT(d->sysWindowMargin, sysWindowMargin, this->rootView()->UIScaleFixed());
    if(d->sysWindowMargin != sysWindowMarginOld) {
        this->sysWindowMarginOnUpdate(sysWindowMarginOld);
    }
}
void ZFUISysWindow::sysWindowMarginOnUpdate(ZF_IN const ZFUIMargin &sysWindowMarginOld) {
    if(this->observerHasAdd(ZFUISysWindow::E_SysWindowMarginOnUpdate())) {
        this->observerNotify(ZFUISysWindow::E_SysWindowMarginOnUpdate(), zfobj<v_ZFUIMargin>(sysWindowMarginOld));
    }
}

void ZFUISysWindow::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUISysWindowPrivate);
    d->windowRootView = ZFUIRootView::ClassData()->newInstance();
    d->windowRootView->_ZFP_ZFUIRootView_rootViewOwnerSysWindow = this;
    d->sysWindowLayoutParam = zflineAlloc(ZFUILayoutParam);
    d->sysWindowLayoutParam->sizeParam(ZFUISizeParamFillFill());
}
void ZFUISysWindow::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowLayoutParamOnInit(this);

    ZFUISysWindow *owner = this;
    ZFLISTENER_1(sysWindowLayoutParamOnUpdate
            , ZFUISysWindow *, owner
            ) {
        owner->_ZFP_ZFUISysWindow_sysWindowLayoutUpdate();
    } ZFLISTENER_END()
    d->sysWindowLayoutParamOnUpdateListener = sysWindowLayoutParamOnUpdate;
    d->sysWindowLayoutParam->observerAdd(ZFUILayoutParam::E_LayoutParamOnUpdate(), d->sysWindowLayoutParamOnUpdateListener);
}
void ZFUISysWindow::objectOnDeallocPrepare(void) {
    if(d->nativeWindowResumed) {
        if(d->embedImpl != zfnull) {
            d->embedImpl->notifyOnPause(this);
        }
        else {
            ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnPause(this);
        }
    }
    if(d->nativeWindowCreated) {
        if(d->embedImpl != zfnull) {
            d->embedImpl->notifyOnDestroy(this);
        }
        else {
            ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnDestroy(this);
        }
    }

    if(d->embedImpl != zfnull) {
        d->embedImpl->nativeWindowOnCleanup(this);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->nativeWindowOnCleanup(this);
    }
    zfsuper::objectOnDeallocPrepare();
}
void ZFUISysWindow::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, void *, nativeWindow) {
    return d->nativeWindow;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, zfbool, nativeWindowIsCreated) {
    return d->nativeWindowCreated;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, zfbool, nativeWindowIsResumed) {
    return d->nativeWindowResumed;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUIOrientation, sysWindowOrientation) {
    if(d->nativeWindowCreated) {
        if(d->embedImpl != zfnull) {
            return d->embedImpl->sysWindowOrientation(this);
        }
        else {
            return ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowOrientation(this);
        }
    }
    else {
        return ZFUIOrientation::e_Top;
    }
}
ZFMETHOD_DEFINE_1(ZFUISysWindow, void, sysWindowOrientationFlags
        , ZFMP_IN(const ZFUIOrientationFlags &, sysWindowOrientationFlags)
        ) {
    zfuint tmp = 0;
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Left)) {
        ZFBitSet(tmp, ZFUIOrientation::e_Left);
    }
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Top)) {
        ZFBitSet(tmp, ZFUIOrientation::e_Top);
    }
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Right)) {
        ZFBitSet(tmp, ZFUIOrientation::e_Right);
    }
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Bottom)) {
        ZFBitSet(tmp, ZFUIOrientation::e_Bottom);
    }
    if(d->sysWindowOrientationFlags != tmp) {
        d->sysWindowOrientationFlags = tmp;
        if(d->embedImpl != zfnull) {
            d->embedImpl->sysWindowOrientationFlags(this, tmp);
        }
        else {
            if(this->nativeWindow() != zfnull) {
                ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowOrientationFlags(this, tmp);
            }
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, const ZFUIOrientationFlags &, sysWindowOrientationFlags) {
    return d->sysWindowOrientationFlags;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUISysWindow, zfbool, preferFullscreen) {
    if(this->nativeWindowIsResumed()) {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowLayoutParamOnUpdate(this);
    }
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, zfautoT<ZFUISysWindow>, modalWindowShow) {
    ZFCoreAssertWithMessage(d->modalWindowShowing == zfnull, "already has a showing modal window, you must finish it first");

    zfautoT<ZFUISysWindow> modalWindow;
    if(d->embedImpl != zfnull) {
        modalWindow = d->embedImpl->modalWindowShow(this);
    }
    else {
        modalWindow = ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowShow(this);
    }

    ZFCoreAssertWithMessage(modalWindow != zfnull, "modalWindowShow not available on embeded window");

    d->modalWindowShowing = modalWindow;
    d->modalWindowShowing->d->modalWindowOwner = this;
    return modalWindow;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, void, modalWindowFinish) {
    ZFCoreAssertWithMessage(d->modalWindowOwner != zfnull, "not a modal window");

    if(d->embedImpl != zfnull) {
        d->embedImpl->modalWindowFinish(d->modalWindowOwner, this);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowFinish(d->modalWindowOwner, this);
    }
    zfblockedRelease(zfRetain(d->modalWindowOwner->d->modalWindowShowing));
    d->modalWindowOwner->d->modalWindowShowing = zfnull;
    d->modalWindowOwner = zfnull;

    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, zfanyT<ZFUISysWindow>, modalWindowShowing) {
    return d->modalWindowShowing;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, zfanyT<ZFUISysWindow>, modalWindowOwner) {
    return d->modalWindowOwner;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, zfanyT<ZFUILayoutParam>, sysWindowLayoutParam) {
    return d->sysWindowLayoutParam;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, zfanyT<ZFUIRootView>, rootView) {
    return d->windowRootView;
}

ZFUIRect ZFUISysWindow::_ZFP_ZFUISysWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect) {
    // use UIScaleForImpl instead of UIScaleFixed, to ensure native window's size unit
    ZFUIRect ret = ZFUILayoutParam::layoutParamApply(
        ZFUIRectApplyScaleReversely(rootRefRect, this->rootView()->UIScaleForImpl()),
        this->rootView(),
        this->sysWindowLayoutParam());
    d->sysWindowSize.width = ret.width;
    d->sysWindowSize.height = ret.height;
    return ZFUIRectApplyScale(ret, this->rootView()->UIScaleForImpl());
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onCreate(ZF_IN void *nativeWindow) {
    ZFCoreAssertWithMessage(!d->nativeWindowCreated, "window already created");

    d->nativeWindow = nativeWindow;
    d->nativeWindowCreated = zftrue;

    void *nativeParentView = zfnull;
    if(d->embedImpl != zfnull) {
        d->embedImpl->sysWindowOrientationFlags(this, d->sysWindowOrientationFlags);
        d->embedImpl->nativeWindowRootViewOnAdd(this, nativeParentView);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowOrientationFlags(this, d->sysWindowOrientationFlags);
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->nativeWindowRootViewOnAdd(this, nativeParentView);
    }
    ZFUIView::_ZFP_ZFUIView_nativeViewNotifyAdd(this->rootView(), nativeParentView);

    this->observerNotify(ZFUISysWindow::E_SysWindowOnCreate());
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onDestroy(void) {
    if(d->nativeWindowResumed) {
        this->_ZFP_ZFUISysWindow_onPause();
    }
    if(!d->nativeWindowCreated) {
        return;
    }
    d->nativeWindowCreated = zffalse;

    if(d->embedImpl != zfnull) {
        d->embedImpl->nativeWindowRootViewOnRemove(this);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->nativeWindowRootViewOnRemove(this);
    }

    ZFUIView::_ZFP_ZFUIView_nativeViewNotifyRemove(this->rootView());

    this->observerNotify(ZFUISysWindow::E_SysWindowOnDestroy());

    if(this == _ZFP_ZFUISysWindow_mainWindowBuiltin) {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->mainWindowOnDestroy();
    }
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onResume(void) {
    ZFCoreAssertWithMessage(d->nativeWindowCreated, "window not created");
    if(d->nativeWindowResumed) {
        return;
    }

    d->nativeWindowResumed = zftrue;
    this->observerNotify(ZFUISysWindow::E_SysWindowOnResume());
    this->rootView()->_ZFP_ZFUIView_viewTreeInWindow(zftrue);

    ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowLayoutParamOnUpdate(this);
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onPause(void) {
    if(!d->nativeWindowResumed) {
        return;
    }

    d->nativeWindowResumed = zffalse;
    this->observerNotify(ZFUISysWindow::E_SysWindowOnPause());
    this->rootView()->_ZFP_ZFUIView_viewTreeInWindow(zffalse);
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onRotate(void) {
    ZFCoreAssertWithMessage(d->nativeWindowCreated, "window not created");
    ZFCoreAssertWithMessage(d->nativeWindowResumed, "window not resumed");
    this->observerNotify(ZFUISysWindow::E_SysWindowOnRotate());

    for(zfindex i = this->rootView()->childCount() - 1; i != zfindexMax(); --i) {
        ZFUIWindow *window = this->rootView()->childAt(i);
        if(window != zfnull) {
            window->ownerSysWindowOnRotate();
        }
    }
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_sysWindowLayoutUpdate(void) {
    if(this->nativeWindowIsResumed()) {
        if(this->nativeWindowEmbedImpl() != zfnull) {
            this->nativeWindowEmbedImpl()->sysWindowLayoutParamOnUpdate(this);
        }
        else {
            ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowLayoutParamOnUpdate(this);
        }
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFUISysWindowEmbedImpl)

void ZFUISysWindowEmbedImpl::nativeWindowRootViewOnAdd(
        ZF_IN ZFUISysWindow *sysWindow
        , ZF_OUT_OPT void *&nativeParentView
        ) {
}
void ZFUISysWindowEmbedImpl::nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow) {
}

zfautoT<ZFUISysWindow> ZFUISysWindowEmbedImpl::modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner) {
    return ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowShow(sysWindowOwner);
}
void ZFUISysWindowEmbedImpl::modalWindowFinish(
        ZF_IN ZFUISysWindow *sysWindowOwner
        , ZF_IN ZFUISysWindow *sysWindowToFinish
        ) {
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowFinish(sysWindowOwner, sysWindowToFinish);
}

ZF_NAMESPACE_GLOBAL_END

