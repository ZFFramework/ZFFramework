#include "ZFUIWindow.h"
#include "ZFUIRootView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIWindowLevel)

ZFSTYLE_DEFAULT_DEFINE(ZFUIWindow)

// ============================================================
// _ZFP_ZFUIWindowPrivate
zfclassNotPOD _ZFP_ZFUIWindowPrivate {
public:
    ZFUISysWindow *windowOwnerSysWindow;
    ZFUILayoutParam *windowLayoutParam;
    zfbool windowRemoveOverrideFlag;

public:
    _ZFP_ZFUIWindowPrivate(void)
    : windowOwnerSysWindow(zfnull)
    , windowLayoutParam(zfnull)
    , windowRemoveOverrideFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUIWindow
ZFOBJECT_REGISTER(ZFUIWindow)

ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOnShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOnHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnRotate)

ZFMETHOD_DEFINE_1(ZFUIWindow, zfanyT<ZFUIWindow>, windowForView
        , ZFMP_IN(ZFUIView *, forView)
        ) {
    while(forView != zfnull && !forView->classData()->classIsTypeOf(ZFUIWindow::ClassData())) {
        forView = forView->viewParent();
    }
    return forView;
}

ZFMETHOD_DEFINE_1(ZFUIWindow, zfanyT<ZFUISysWindow>, sysWindowForView
        , ZFMP_IN(ZFUIView *, view)
        ) {
    zfanyT<ZFUIWindow> window = ZFUIWindow::windowForView(view);
    return ((window != zfnull) ? window->windowOwnerSysWindow() : zfnull);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFUIWindow
        , ZFMP_IN(ZFUISysWindow *, windowOwnerSysWindow)
        ) {
    this->objectOnInit();
    if(windowOwnerSysWindow != zfnull) {
        this->windowOwnerSysWindow(windowOwnerSysWindow);
    }
}

void ZFUIWindow::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIWindowPrivate);
    d->windowLayoutParam = zfAlloc(ZFUILayoutParam);
    d->windowLayoutParam->sizeParam(ZFUISizeParamFillFill());
}
void ZFUIWindow::objectOnDealloc(void) {
    zfRelease(d->windowLayoutParam);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
// properties
ZFPROPERTY_ON_VERIFY_DEFINE(ZFUIWindow, ZFUIWindowLevelEnum, windowLevel) {
    zfCoreAssertWithMessage(!this->windowShowing(), "you must not change window level while it's showing");
}

ZFMETHOD_DEFINE_1(ZFUIWindow, void, windowOwnerSysWindow
        , ZFMP_IN(ZFUISysWindow *, windowOwnerSysWindow)
        ) {
    if(d->windowOwnerSysWindow != windowOwnerSysWindow) {
        zfCoreAssertWithMessage(!this->windowShowing(), "you must not change window's owner while it's showing");
        zfCoreAssertWithMessage(windowOwnerSysWindow != zfnull, "null owner sys window");

        ZFUISysWindow *oldSysWindow = d->windowOwnerSysWindow;
        d->windowOwnerSysWindow = windowOwnerSysWindow;
        if(!(oldSysWindow == windowOwnerSysWindow
                    || (oldSysWindow == zfnull && ZFUISysWindow::mainWindowAttached() && windowOwnerSysWindow == ZFUISysWindow::mainWindow()))
                    ) {
            this->windowOwnerSysWindowOnChange(oldSysWindow);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfanyT<ZFUISysWindow>, windowOwnerSysWindow) {
    if(d->windowOwnerSysWindow == zfnull) {
        d->windowOwnerSysWindow = ZFUISysWindow::keyWindow();
    }
    return d->windowOwnerSysWindow;
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowShow) {
    if(this->windowShowing()) {
        return;
    }
    zfindex windowIndex = 0;
    zfindex addToIndex = 0;
    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    for(zfindex i = 0; i < tmpArray.count(); ++i) {
        ZFUIWindow *tmpWindow = tmpArray[i];
        if(tmpWindow != zfnull) {
            if(tmpWindow->windowLevel() <= this->windowLevel()) {
                ++windowIndex;
                addToIndex = i + 1;
            }
            else {
                break;
            }
        }
    }
    this->windowOwnerSysWindow()->rootView()->childAddWithParam(this, d->windowLayoutParam, addToIndex);
    this->windowOwnerSysWindow()->rootView()->_ZFP_ZFUIRootView_windowList.add(windowIndex, this);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowHide) {
    if(!this->windowShowing()) {
        return;
    }

    zfRetain(this);
    ZFUIRootView *rootView = this->viewParent();
    if(rootView != zfnull) {
        rootView->_ZFP_ZFUIRootView_windowList.removeElement(this);
    }
    d->windowRemoveOverrideFlag = zftrue;
    this->viewRemoveFromParent();
    d->windowRemoveOverrideFlag = zffalse;
    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfbool, windowShowing) {
    return (this->viewParent() != zfnull);
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToTop) {
    if(!this->windowShowing()) {
        return;
    }

    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    ZFUIWindowLevelEnum selfWindowLevel = this->windowLevel();
    zfindex topIndex = zfindexMax();
    zfindex selfIndex = zfindexMax();
    for(zfindex i = tmpArray.count() - 1; i != zfindexMax(); --i) {
        ZFUIWindow *tmpWindow = tmpArray[i];
        if(tmpWindow != zfnull) {
            if(topIndex == zfindexMax() && tmpWindow->windowLevel() == selfWindowLevel) {
                topIndex = i;
            }
            if(tmpWindow == this) {
                selfIndex = i;
                break;
            }
        }
    }
    if(topIndex != selfIndex && topIndex != zfindexMax() && selfIndex != zfindexMax()) {
        this->windowOwnerSysWindow()->rootView()->childMove(selfIndex, topIndex);
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToBottom) {
    if(!this->windowShowing()) {
        return;
    }

    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    ZFUIWindowLevelEnum selfWindowLevel = this->windowLevel();
    zfindex bottomIndex = zfindexMax();
    zfindex selfIndex = zfindexMax();
    for(zfindex i = 0; i < tmpArray.count(); ++i) {
        ZFUIWindow *tmpWindow = tmpArray[i];
        if(tmpWindow != zfnull) {
            if(bottomIndex == zfindexMax() && tmpWindow->windowLevel() == selfWindowLevel) {
                bottomIndex = i;
            }
            if(tmpWindow == this) {
                selfIndex = i;
                break;
            }
        }
    }
    if(bottomIndex != selfIndex && bottomIndex != zfindexMax() && selfIndex != zfindexMax()) {
        this->windowOwnerSysWindow()->rootView()->childMove(selfIndex, bottomIndex);
    }
}

ZFMETHOD_DEFINE_0(ZFUIWindow, zfanyT<ZFUILayoutParam>, windowLayoutParam) {
    return d->windowLayoutParam;
}

void ZFUIWindow::viewOnAddToParent(ZF_IN ZFUIView *parent) {
    zfCoreAssertWithMessage(parent->classData()->classIsTypeOf(ZFUIRootView::ClassData()), "you must not add a window to another view");
    zfsuper::viewOnAddToParent(parent);

    this->windowOnShow();
}
void ZFUIWindow::viewOnRemoveFromParent(ZF_IN ZFUIView *parent) {
    // should not check remove here,
    // since it's all right to remove this window when owner sys window deallocated or when hide window
    zfsuper::viewOnRemoveFromParent(parent);

    if(d->windowRemoveOverrideFlag) {
        this->windowOnHide();
    }
    else {
        this->windowHide();
    }
}

ZF_NAMESPACE_GLOBAL_END

