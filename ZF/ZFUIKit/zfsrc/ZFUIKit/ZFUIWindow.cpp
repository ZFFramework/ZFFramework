#include "ZFUIWindow.h"
#include "ZFUIRootView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIWindowLevel)

ZFSTYLE_DEFAULT_DEFINE(ZFUIWindow)

// ============================================================
// _ZFP_ZFUIWindowPrivate
zfclassNotPOD _ZFP_ZFUIWindowPrivate {
public:
    ZFUISysWindow *ownerSysWindow;
    ZFUILayoutParam *windowLayoutParam;
    zfbool windowRemoveOverrideFlag;

public:
    _ZFP_ZFUIWindowPrivate(void)
    : ownerSysWindow(zfnull)
    , windowLayoutParam(zfnull)
    , windowRemoveOverrideFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUIWindow
ZFOBJECT_REGISTER(ZFUIWindow)

ZFEVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnUpdate)
ZFEVENT_REGISTER(ZFUIWindow, WindowOnShow)
ZFEVENT_REGISTER(ZFUIWindow, WindowOnHide)
ZFEVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnRotate)

ZFMETHOD_DEFINE_1(ZFUIWindow, zfanyT<ZFUIWindow>, windowForView
        , ZFMP_IN(ZFUIView *, forView)
        ) {
    while(forView != zfnull && !forView->classData()->classIsTypeOf(ZFUIWindow::ClassData())) {
        forView = forView->parent();
    }
    return forView;
}

ZFMETHOD_DEFINE_1(ZFUIWindow, zfanyT<ZFUISysWindow>, sysWindowForView
        , ZFMP_IN(ZFUIView *, view)
        ) {
    zfanyT<ZFUIWindow> window = ZFUIWindow::windowForView(view);
    return ((window != zfnull) ? window->ownerSysWindow() : zfnull);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFUIWindow
        , ZFMP_IN(ZFUISysWindow *, ownerSysWindow)
        ) {
    this->objectOnInit();
    if(ownerSysWindow != zfnull) {
        this->ownerSysWindow(ownerSysWindow);
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
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIWindow, ZFUIWindowLevel, windowLevel) {
    ZFCoreAssertWithMessage(!this->showing(), "you must not change window level while it's showing");
}

ZFMETHOD_DEFINE_1(ZFUIWindow, void, ownerSysWindow
        , ZFMP_IN(ZFUISysWindow *, ownerSysWindow)
        ) {
    if(d->ownerSysWindow != ownerSysWindow) {
        ZFCoreAssertWithMessage(!this->showing(), "you must not change window's owner while it's showing");
        ZFCoreAssertWithMessage(ownerSysWindow != zfnull, "null owner sys window");

        ZFUISysWindow *oldSysWindow = d->ownerSysWindow;
        d->ownerSysWindow = ownerSysWindow;
        if(!(oldSysWindow == ownerSysWindow
                    || (oldSysWindow == zfnull && ZFUISysWindow::mainWindowAttached() && ownerSysWindow == ZFUISysWindow::mainWindow()))
                    ) {
            this->ownerSysWindowOnUpdate(oldSysWindow);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfanyT<ZFUISysWindow>, ownerSysWindow) {
    if(d->ownerSysWindow == zfnull) {
        d->ownerSysWindow = ZFUISysWindow::keyWindow();
    }
    return d->ownerSysWindow;
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, show) {
    if(this->showing()) {
        return;
    }
    zfindex windowIndex = 0;
    zfindex addToIndex = 0;
    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->ownerSysWindow()->rootView()->childArray();
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
    this->ownerSysWindow()->rootView()->childWithParam(this, d->windowLayoutParam, addToIndex);
    this->ownerSysWindow()->rootView()->_ZFP_ZFUIRootView_windowList.add(this, windowIndex);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, hide) {
    if(!this->showing()) {
        return;
    }

    zfRetain(this);
    ZFUIRootView *rootView = this->parent();
    if(rootView != zfnull) {
        rootView->_ZFP_ZFUIRootView_windowList.removeElement(this);
    }
    d->windowRemoveOverrideFlag = zftrue;
    this->removeFromParent();
    d->windowRemoveOverrideFlag = zffalse;
    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfbool, showing) {
    return (this->parent() != zfnull);
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToTop) {
    if(!this->showing()) {
        return;
    }

    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->ownerSysWindow()->rootView()->childArray();
    ZFUIWindowLevel selfWindowLevel = this->windowLevel();
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
        this->ownerSysWindow()->rootView()->childMove(selfIndex, topIndex);
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToBottom) {
    if(!this->showing()) {
        return;
    }

    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->ownerSysWindow()->rootView()->childArray();
    ZFUIWindowLevel selfWindowLevel = this->windowLevel();
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
        this->ownerSysWindow()->rootView()->childMove(selfIndex, bottomIndex);
    }
}

ZFMETHOD_DEFINE_0(ZFUIWindow, zfanyT<ZFUILayoutParam>, windowLayoutParam) {
    return d->windowLayoutParam;
}

void ZFUIWindow::viewOnAddToParent(ZF_IN ZFUIView *parent) {
    ZFCoreAssertWithMessage(parent->classData()->classIsTypeOf(ZFUIRootView::ClassData()), "you must not add a window to another view");
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
        this->hide();
    }
}

ZF_NAMESPACE_GLOBAL_END

