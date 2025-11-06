#include "ZFUIRootWindow.h"
#include "protocol/ZFProtocolZFUIRootWindow.h"
#include "ZFUIRootView.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(MainWindowOnAttach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfclassNotPOD _ZFP_ZFUIRootWindowPrivate {
public:
    zfautoT<ZFUIRootWindowEmbedImpl> embedImpl;
    void *nativeWindow;
    zfautoT<ZFUIRootView> windowRootView;
    ZFUIOrientationFlags windowOrientationFlags;
    ZFUIRootWindow *modalWindowOwner;
    zfautoT<ZFUIRootWindow> modalWindowShowing;
    zfautoT<ZFUILayoutParam> layoutParam;
    ZFListener layoutParamOnUpdateListener;
    zfbool nativeWindowCreated;
    zfbool nativeWindowResumed;
    zfbool nativeWindowLayoutUpdated;
    ZFUISize windowSize;
    ZFUIMargin windowMargin;

public:
    _ZFP_ZFUIRootWindowPrivate(void)
    : embedImpl(zfnull)
    , nativeWindow(zfnull)
    , windowRootView(zfnull)
    , windowOrientationFlags(v_ZFUIOrientation::e_Top)
    , modalWindowOwner(zfnull)
    , modalWindowShowing(zfnull)
    , layoutParam(zfnull)
    , layoutParamOnUpdateListener()
    , nativeWindowCreated(zffalse)
    , nativeWindowResumed(zffalse)
    , nativeWindowLayoutUpdated(zftrue)
    , windowSize()
    , windowMargin()
    {
    }
};

ZFOBJECT_REGISTER(ZFUIRootWindow)

ZFEVENT_REGISTER(ZFUIRootWindow, WindowOnCreate)
ZFEVENT_REGISTER(ZFUIRootWindow, WindowOnDestroy)
ZFEVENT_REGISTER(ZFUIRootWindow, WindowOnResume)
ZFEVENT_REGISTER(ZFUIRootWindow, WindowOnPause)
ZFEVENT_REGISTER(ZFUIRootWindow, WindowOnRotate)
ZFEVENT_REGISTER(ZFUIRootWindow, WindowMarginOnUpdate)
ZFEVENT_REGISTER(ZFUIRootWindow, WindowOnKeyEvent)

zfautoT<ZFUIRootWindow> ZFUIRootWindow::nativeWindowEmbed(ZF_IN ZFUIRootWindowEmbedImpl *embedImpl) {
    zfautoT<ZFUIRootWindow> tmp = ZFUIRootWindow::ClassData()->newInstance();
    ZFUIRootWindow *ret = tmp;
    ret->d->embedImpl = embedImpl;
    embedImpl->_ZFP_ownerZFUIRootWindow = ret;
    embedImpl->layoutParamOnInit(ret);
    return tmp;
}
ZFUIRootWindowEmbedImpl *ZFUIRootWindow::nativeWindowEmbedImpl(void) {
    return d->embedImpl;
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, void, nativeWindowEmbedImplDestroy) {
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
static ZFUIRootWindow *_ZFP_ZFUIRootWindow_mainWindowRegistered = zfnull;
static ZFUIRootWindow *_ZFP_ZFUIRootWindow_mainWindowBuiltin = zfnull;
static ZFUIRootWindow *_ZFP_ZFUIRootWindow_mainWindow = zfnull;
static zfweakT<ZFUIRootWindow> _ZFP_ZFUIRootWindow_keyWindow;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIRootWindowMainWindowCleanup, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIRootWindowMainWindowCleanup) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIRootWindow) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIRootWindow);
    if(impl != zfnull) {
        impl->mainWindowOnCleanup();
    }
    zfscopeRelease(_ZFP_ZFUIRootWindow_mainWindowRegistered);
    zfscopeRelease(_ZFP_ZFUIRootWindow_keyWindow);

    _ZFP_ZFUIRootWindow_mainWindowRegistered = zfnull;
    _ZFP_ZFUIRootWindow_mainWindowBuiltin = zfnull;
    _ZFP_ZFUIRootWindow_mainWindow = zfnull;
    _ZFP_ZFUIRootWindow_keyWindow = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIRootWindowMainWindowCleanup)

void ZFUIRootWindow::mainWindowRegister(ZF_IN ZFUIRootWindow *window) {
    ZFCoreAssertWithMessage(_ZFP_ZFUIRootWindow_mainWindow == zfnull,
        "[ZFUIRootWindow] mainWindowRegister must be called before accessing mainWindow");
    ZFCoreAssertWithMessage(window != zfnull,
        "[ZFUIRootWindow] mainWindowRegister called with null window");

    zfscopeRelease(_ZFP_ZFUIRootWindow_mainWindowRegistered);
    _ZFP_ZFUIRootWindow_mainWindowRegistered = zfRetain(window);
    _ZFP_ZFUIRootWindow_mainWindow = _ZFP_ZFUIRootWindow_mainWindowRegistered;

    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_MainWindowOnAttach());
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfanyT<ZFUIRootWindow>, mainWindow) {
    if(_ZFP_ZFUIRootWindow_mainWindow == zfnull) {
        _ZFP_ZFUIRootWindow_mainWindowBuiltin = ZFPROTOCOL_ACCESS(ZFUIRootWindow)->mainWindow();
        _ZFP_ZFUIRootWindow_mainWindow = _ZFP_ZFUIRootWindow_mainWindowBuiltin;
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_MainWindowOnAttach());
    }
    return _ZFP_ZFUIRootWindow_mainWindow;
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfbool, mainWindowAttached) {
    return (_ZFP_ZFUIRootWindow_mainWindow != zfnull);
}

ZFMETHOD_DEFINE_1(ZFUIRootWindow, void, keyWindow
        , ZFMP_IN(ZFUIRootWindow *, window)
        ) {
    _ZFP_ZFUIRootWindow_keyWindow = window;
}
ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfanyT<ZFUIRootWindow>, keyWindow) {
    ZFUIRootWindow *ret = zfnull;
    if(_ZFP_ZFUIRootWindow_keyWindow != zfnull) {
        ret = _ZFP_ZFUIRootWindow_keyWindow;
    }
    if(ret != zfnull) {
        return ret;
    }
    else {
        return ZFUIRootWindow::mainWindow();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIRootWindow, const ZFUISize &, windowSize) {
    return d->windowSize;
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIRootWindow, const ZFUIMargin &, windowMargin) {
    return d->windowMargin;
}
void ZFUIRootWindow::_ZFP_ZFUIRootWindow_windowMargin(ZF_IN const ZFUIMargin &windowMargin) {
    ZFUIMargin windowMarginOld = d->windowMargin;
    ZFUIMarginApplyScaleReverselyT(d->windowMargin, windowMargin, this->rootView()->UIScaleFixed());
    if(d->windowMargin != windowMarginOld) {
        this->windowMarginOnUpdate(windowMarginOld);
    }
}
void ZFUIRootWindow::windowMarginOnUpdate(ZF_IN const ZFUIMargin &windowMarginOld) {
    if(this->observerHasAdd(ZFUIRootWindow::E_WindowMarginOnUpdate())) {
        this->observerNotify(ZFUIRootWindow::E_WindowMarginOnUpdate(), zfobj<v_ZFUIMargin>(windowMarginOld));
    }
}

void ZFUIRootWindow::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIRootWindowPrivate);
    d->windowRootView = ZFUIRootView::ClassData()->newInstance();
    d->windowRootView->_ZFP_ZFUIRootView_rootWindow = this;
    d->layoutParam = zfobj<ZFUILayoutParam>();
    d->layoutParam->sizeParam(ZFUISizeParamFillFill());
}
void ZFUIRootWindow::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->layoutParamOnInit(this);

    ZFUIRootWindow *owner = this;
    ZFLISTENER_1(layoutParamOnUpdate
            , ZFUIRootWindow *, owner
            ) {
        owner->_ZFP_ZFUIRootWindow_layoutUpdate();
    } ZFLISTENER_END()
    d->layoutParamOnUpdateListener = layoutParamOnUpdate;
    d->layoutParam->observerAdd(ZFUILayoutParam::E_LayoutParamOnUpdate(), d->layoutParamOnUpdateListener);
}
void ZFUIRootWindow::objectOnDeallocPrepare(void) {
    if(d->nativeWindowResumed) {
        if(d->embedImpl != zfnull) {
            d->embedImpl->notifyOnPause(this);
        }
        else {
            ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnPause(this);
        }
    }
    if(d->nativeWindowCreated) {
        if(d->embedImpl != zfnull) {
            d->embedImpl->notifyOnDestroy(this);
        }
        else {
            ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnDestroy(this);
        }
    }

    if(d->embedImpl != zfnull) {
        d->embedImpl->nativeWindowOnCleanup(this);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUIRootWindow)->nativeWindowOnCleanup(this);
    }
    zfsuper::objectOnDeallocPrepare();
}
void ZFUIRootWindow::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, void *, nativeWindow) {
    return d->nativeWindow;
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfbool, nativeWindowIsCreated) {
    return d->nativeWindowCreated;
}
ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfbool, nativeWindowIsResumed) {
    return d->nativeWindowResumed;
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, ZFUIOrientation, windowOrientation) {
    if(d->nativeWindowCreated) {
        if(d->embedImpl != zfnull) {
            return d->embedImpl->windowOrientation(this);
        }
        else {
            return ZFPROTOCOL_ACCESS(ZFUIRootWindow)->windowOrientation(this);
        }
    }
    else {
        return v_ZFUIOrientation::e_Top;
    }
}
ZFMETHOD_DEFINE_1(ZFUIRootWindow, void, windowOrientationFlags
        , ZFMP_IN(const ZFUIOrientationFlags &, windowOrientationFlags)
        ) {
    zfuint tmp = 0;
    if(ZFBitTest(windowOrientationFlags.enumValue(), v_ZFUIOrientation::e_Left)) {
        ZFBitSet(tmp, v_ZFUIOrientation::e_Left);
    }
    if(ZFBitTest(windowOrientationFlags.enumValue(), v_ZFUIOrientation::e_Top)) {
        ZFBitSet(tmp, v_ZFUIOrientation::e_Top);
    }
    if(ZFBitTest(windowOrientationFlags.enumValue(), v_ZFUIOrientation::e_Right)) {
        ZFBitSet(tmp, v_ZFUIOrientation::e_Right);
    }
    if(ZFBitTest(windowOrientationFlags.enumValue(), v_ZFUIOrientation::e_Bottom)) {
        ZFBitSet(tmp, v_ZFUIOrientation::e_Bottom);
    }
    if(d->windowOrientationFlags != tmp) {
        d->windowOrientationFlags = tmp;
        if(d->embedImpl != zfnull) {
            d->embedImpl->windowOrientationFlags(this, tmp);
        }
        else {
            if(this->nativeWindow() != zfnull) {
                ZFPROTOCOL_ACCESS(ZFUIRootWindow)->windowOrientationFlags(this, tmp);
            }
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIRootWindow, const ZFUIOrientationFlags &, windowOrientationFlags) {
    return d->windowOrientationFlags;
}

ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIRootWindow, zfbool, preferFullscreen) {
    if(propertyValue != propertyValueOld) {
        this->_ZFP_ZFUIRootWindow_layoutUpdate();
    }
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfautoT<ZFUIRootWindow>, modalWindowShow) {
    ZFCoreAssertWithMessage(d->modalWindowShowing == zfnull, "already has a showing modal window, you must finish it first");

    zfautoT<ZFUIRootWindow> modalWindow;
    if(d->embedImpl != zfnull) {
        modalWindow = d->embedImpl->modalWindowShow(this);
    }
    else {
        modalWindow = ZFPROTOCOL_ACCESS(ZFUIRootWindow)->modalWindowShow(this);
    }

    ZFCoreAssertWithMessage(modalWindow != zfnull, "modalWindowShow not available on embeded window");

    d->modalWindowShowing = modalWindow;
    d->modalWindowShowing->d->modalWindowOwner = this;
    return modalWindow;
}
ZFMETHOD_DEFINE_0(ZFUIRootWindow, void, modalWindowHide) {
    ZFCoreAssertWithMessage(d->modalWindowOwner != zfnull, "not a modal window");

    if(d->embedImpl != zfnull) {
        d->embedImpl->modalWindowHide(d->modalWindowOwner, this);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUIRootWindow)->modalWindowHide(d->modalWindowOwner, this);
    }
    zfscopeRelease(zfRetain(d->modalWindowOwner->d->modalWindowShowing));
    d->modalWindowOwner->d->modalWindowShowing = zfnull;
    d->modalWindowOwner = zfnull;

    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfanyT<ZFUIRootWindow>, modalWindowShowing) {
    return d->modalWindowShowing;
}
ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfanyT<ZFUIRootWindow>, modalWindowOwner) {
    return d->modalWindowOwner;
}

ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfanyT<ZFUILayoutParam>, layoutParam) {
    return d->layoutParam;
}
ZFMETHOD_DEFINE_0(ZFUIRootWindow, zfanyT<ZFUIRootView>, rootView) {
    return d->windowRootView;
}

ZFUIRect ZFUIRootWindow::_ZFP_ZFUIRootWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect) {
    // use UIScaleForImpl instead of UIScaleFixed, to ensure native window's size unit
    ZFUIRect ret = ZFUILayoutParam::layoutParamApply(
        ZFUIRectApplyScaleReversely(rootRefRect, this->rootView()->UIScaleForImpl()),
        this->rootView(),
        this->layoutParam());
    d->windowSize.width = ret.width;
    d->windowSize.height = ret.height;
    return ZFUIRectApplyScale(ret, this->rootView()->UIScaleForImpl());
}
void ZFUIRootWindow::_ZFP_ZFUIRootWindow_onCreate(ZF_IN void *nativeWindow) {
    ZFCoreAssertWithMessage(!d->nativeWindowCreated, "window already created");

    d->nativeWindow = nativeWindow;
    d->nativeWindowCreated = zftrue;

    void *nativeParentView = zfnull;
    if(d->embedImpl != zfnull) {
        d->embedImpl->windowOrientationFlags(this, d->windowOrientationFlags);
        d->embedImpl->nativeWindowRootViewOnAdd(this, nativeParentView);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUIRootWindow)->windowOrientationFlags(this, d->windowOrientationFlags);
        ZFPROTOCOL_ACCESS(ZFUIRootWindow)->nativeWindowRootViewOnAdd(this, nativeParentView);
    }
    ZFUIView::_ZFP_ZFUIView_nativeViewNotifyAdd(this->rootView(), nativeParentView);
    this->rootView()->_ZFP_ZFUIView_viewTreeInWindow(zftrue);

    this->observerNotify(ZFUIRootWindow::E_WindowOnCreate());
}
void ZFUIRootWindow::_ZFP_ZFUIRootWindow_onDestroy(void) {
    if(d->nativeWindowResumed) {
        this->_ZFP_ZFUIRootWindow_onPause();
    }
    if(!d->nativeWindowCreated) {
        return;
    }
    d->nativeWindowCreated = zffalse;

    if(d->embedImpl != zfnull) {
        d->embedImpl->nativeWindowRootViewOnRemove(this);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUIRootWindow)->nativeWindowRootViewOnRemove(this);
    }
    this->rootView()->_ZFP_ZFUIView_viewTreeInWindow(zffalse);
    ZFUIView::_ZFP_ZFUIView_nativeViewNotifyRemove(this->rootView());

    this->observerNotify(ZFUIRootWindow::E_WindowOnDestroy());

    if(this == _ZFP_ZFUIRootWindow_mainWindowBuiltin) {
        ZFPROTOCOL_ACCESS(ZFUIRootWindow)->mainWindowOnDestroy();
    }
}
void ZFUIRootWindow::_ZFP_ZFUIRootWindow_onResume(void) {
    ZFCoreAssertWithMessage(d->nativeWindowCreated, "window not created");
    if(d->nativeWindowResumed) {
        return;
    }

    d->nativeWindowResumed = zftrue;
    this->observerNotify(ZFUIRootWindow::E_WindowOnResume());
    if(d->nativeWindowLayoutUpdated) {
        this->_ZFP_ZFUIRootWindow_layoutUpdate();
    }
}
void ZFUIRootWindow::_ZFP_ZFUIRootWindow_onPause(void) {
    if(!d->nativeWindowResumed) {
        return;
    }

    d->nativeWindowResumed = zffalse;
    this->observerNotify(ZFUIRootWindow::E_WindowOnPause());
}
void ZFUIRootWindow::_ZFP_ZFUIRootWindow_onRotate(void) {
    ZFCoreAssertWithMessage(d->nativeWindowCreated, "window not created");
    ZFCoreAssertWithMessage(d->nativeWindowResumed, "window not resumed");
    this->observerNotify(ZFUIRootWindow::E_WindowOnRotate());
    this->_ZFP_ZFUIRootWindow_layoutUpdate();

    for(zfindex i = this->rootView()->childCount() - 1; i != zfindexMax(); --i) {
        ZFUIWindow *window = this->rootView()->childAt(i);
        if(window != zfnull) {
            window->ownerWindowOnRotate();
        }
    }
}
void ZFUIRootWindow::_ZFP_ZFUIRootWindow_layoutUpdate(void) {
    if(this->nativeWindowIsResumed()) {
        d->nativeWindowLayoutUpdated = zffalse;
        if(this->nativeWindowEmbedImpl() != zfnull) {
            this->nativeWindowEmbedImpl()->layoutParamOnUpdate(this);
        }
        else {
            ZFPROTOCOL_ACCESS(ZFUIRootWindow)->layoutParamOnUpdate(this);
        }
    }
    else {
        d->nativeWindowLayoutUpdated = zftrue;
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFUIRootWindowEmbedImpl)

void ZFUIRootWindowEmbedImpl::nativeWindowRootViewOnAdd(
        ZF_IN ZFUIRootWindow *rootWindow
        , ZF_OUT_OPT void *&nativeParentView
        ) {
}
void ZFUIRootWindowEmbedImpl::nativeWindowRootViewOnRemove(ZF_IN ZFUIRootWindow *rootWindow) {
}

zfautoT<ZFUIRootWindow> ZFUIRootWindowEmbedImpl::modalWindowShow(ZF_IN ZFUIRootWindow *owner) {
    return ZFPROTOCOL_ACCESS(ZFUIRootWindow)->modalWindowShow(owner);
}
void ZFUIRootWindowEmbedImpl::modalWindowHide(
        ZF_IN ZFUIRootWindow *owner
        , ZF_IN ZFUIRootWindow *toHide
        ) {
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->modalWindowHide(owner, toHide);
}

ZF_NAMESPACE_GLOBAL_END

