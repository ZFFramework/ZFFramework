#include "ZFUIWindow.h"
#include "ZFUIRootView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_DEFINE(zfint, ZFUIWindowLevelBg, -1000)
ZFEXPORT_VAR_DEFINE(zfint, ZFUIWindowLevelNormal, 0)
ZFEXPORT_VAR_DEFINE(zfint, ZFUIWindowLevelFg, 1000)
ZFEXPORT_VAR_DEFINE(zfint, ZFUIWindowLevelDialog, 2000)
ZFEXPORT_VAR_DEFINE(zfint, ZFUIWindowLevelOverlay, 3000)
ZFEXPORT_VAR_DEFINE(zfint, ZFUIWindowLevelHint, 4000)

ZFSTYLE_DEFAULT_DEFINE(ZFUIWindow)

// ============================================================
// ZFUIWindowLayoutParam
ZFOBJECT_REGISTER(ZFUIWindowLayoutParam)

/* ZFTAG_TRICKS: util for chained call to build view tree */
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIWindowLayoutParam, zfanyT<ZFUIWindowLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIWindowLayoutParam, zfanyT<ZFUIWindowLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIWindow, zfanyT<ZFUIWindowLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIWindow, zfanyT<ZFUIWindowLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

// ============================================================
// _ZFP_ZFUIWindowPrivate
zfclassNotPOD _ZFP_ZFUIWindowPrivate {
public:
    ZFUIRootWindow *rootWindow;
    zfbool windowRemoveOverrideFlag;

public:
    _ZFP_ZFUIWindowPrivate(void)
    : rootWindow(zfnull)
    , windowRemoveOverrideFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUIWindow
ZFOBJECT_REGISTER(ZFUIWindow)

ZFEVENT_REGISTER(ZFUIWindow, RootWindowOnUpdate)
ZFEVENT_REGISTER(ZFUIWindow, WindowOnShow)
ZFEVENT_REGISTER(ZFUIWindow, WindowOnHide)
ZFEVENT_REGISTER(ZFUIWindow, WindowOwnerWindowOnRotate)

ZFMETHOD_DEFINE_1(ZFUIWindow, zfanyT<ZFUIWindow>, windowForView
        , ZFMP_IN(ZFUIView *, forView)
        ) {
    while(forView != zfnull && !forView->classData()->classIsTypeOf(ZFUIWindow::ClassData())) {
        forView = forView->parent();
    }
    return forView;
}

ZFMETHOD_DEFINE_1(ZFUIWindow, zfanyT<ZFUIRootWindow>, rootWindowForView
        , ZFMP_IN(ZFUIView *, view)
        ) {
    zfanyT<ZFUIWindow> window = ZFUIWindow::windowForView(view);
    return ((window != zfnull) ? window->rootWindow() : zfnull);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFUIWindow
        , ZFMP_IN(ZFUIRootWindow *, rootWindow)
        ) {
    if(rootWindow != zfnull) {
        this->rootWindow(rootWindow);
    }
}

void ZFUIWindow::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIWindowPrivate);
}
void ZFUIWindow::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
// properties
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIWindow, zfint, windowLevel) {
    ZFCoreAssertWithMessage(!this->showing(), "you must not change window level while it's showing");
}

ZFMETHOD_DEFINE_1(ZFUIWindow, void, rootWindow
        , ZFMP_IN(ZFUIRootWindow *, rootWindow)
        ) {
    if(d->rootWindow != rootWindow) {
        ZFCoreAssertWithMessage(!this->showing(), "you must not change window's owner while it's showing");
        ZFCoreAssertWithMessage(rootWindow != zfnull, "null owner root window");

        ZFUIRootWindow *rootWindowOld = d->rootWindow;
        d->rootWindow = rootWindow;
        if(!(rootWindowOld == rootWindow
                    || (rootWindowOld == zfnull && ZFUIRootWindow::mainWindowAttached() && rootWindow == ZFUIRootWindow::mainWindow()))
                    ) {
            this->rootWindowOnUpdate(rootWindowOld);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfanyT<ZFUIRootWindow>, rootWindow) {
    if(d->rootWindow == zfnull) {
        d->rootWindow = ZFUIRootWindow::keyWindow();
    }
    return d->rootWindow;
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, show) {
    if(this->showing()) {
        return;
    }
    zfindex windowIndex = 0;
    zfindex addToIndex = 0;
    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->rootWindow()->rootView()->childArray();
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
    this->rootWindow()->rootView()->childWithParam(this, this->layoutParam(), addToIndex);
    this->rootWindow()->rootView()->_ZFP_ZFUIRootView_windowList.add(this, windowIndex);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, hide) {
    if(!this->showing()) {
        return;
    }

    zfobjReleaseInScope(zfobjRetain(this));
    ZFUIRootView *rootView = this->parent();
    if(rootView != zfnull) {
        rootView->_ZFP_ZFUIRootView_windowList.removeElement(this);
    }
    d->windowRemoveOverrideFlag = zftrue;
    this->removeFromParent();
    d->windowRemoveOverrideFlag = zffalse;
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfbool, showing) {
    return (this->parent() != zfnull);
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToTop) {
    if(!this->showing()) {
        return;
    }

    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->rootWindow()->rootView()->childArray();
    zfint selfWindowLevel = this->windowLevel();
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
        this->rootWindow()->rootView()->childMove(selfIndex, topIndex);
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToBottom) {
    if(!this->showing()) {
        return;
    }

    ZFCoreArray<zfautoT<ZFUIView> > tmpArray = this->rootWindow()->rootView()->childArray();
    zfint selfWindowLevel = this->windowLevel();
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
        this->rootWindow()->rootView()->childMove(selfIndex, bottomIndex);
    }
}

void ZFUIWindow::layoutParam(ZF_IN ZFUILayoutParam *layoutParam) {
    zfsuper::layoutParam(layoutParam);
}
zfanyT<ZFUILayoutParam> ZFUIWindow::layoutParam(void) {
    zfanyT<ZFUILayoutParam> ret = zfsuper::layoutParam();
    if(ret == zfnull) {
        zfobj<ZFUILayoutParam> tmp;
        tmp->sizeParam(ZFUISizeParamFillFill());
        zfsuper::layoutParam(tmp);
        return tmp;
    }
    else {
        return ret;
    }
}

void ZFUIWindow::viewOnAddToParent(ZF_IN ZFUIView *parent) {
    ZFCoreAssertWithMessage(parent->classData()->classIsTypeOf(ZFUIRootView::ClassData()), "you must not add a window to another view");
    zfsuper::viewOnAddToParent(parent);

    this->windowOnShow();
}
void ZFUIWindow::viewOnRemoveFromParent(ZF_IN ZFUIView *parent) {
    // should not check remove here,
    // since it's all right to remove this window when owner root window deallocated or when hide window
    zfsuper::viewOnRemoveFromParent(parent);

    if(d->windowRemoveOverrideFlag) {
        this->windowOnHide();
    }
    else {
        this->hide();
    }
}

void ZFUIWindow::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ret = sizeHint;
}
void ZFUIWindow::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    const ZFUIMargin &safeAreaOrig = this->rootWindow()->safeArea();
    const ZFUIRect &viewFrame = this->viewFrame();
    ZFUIMargin safeAreaFixed;
    safeAreaFixed.left = zfmMax<zffloat>(0, safeAreaOrig.left - viewFrame.x);
    safeAreaFixed.top = zfmMax<zffloat>(0, safeAreaOrig.top - viewFrame.y);
    safeAreaFixed.right = zfmMax<zffloat>(0, safeAreaOrig.right - (this->parent()->width() - ZFUIRectGetRight(viewFrame)));
    safeAreaFixed.bottom = zfmMax<zffloat>(0, safeAreaOrig.bottom - (this->parent()->height() - ZFUIRectGetBottom(viewFrame)));
    for(zfindex i = 0; i < this->childCount(); ++i) {
        ZFUIView *child = this->childAt(i);
        ZFUIWindowLayoutParam *lp = child->layoutParam();
        ZFUIRect childFrame;
        if(lp->safeAreaAdapt()) {
            child->layoutMeasure(
                    ZFUISizeCreate(
                        bounds.width - safeAreaFixed.left - safeAreaFixed.right
                        , bounds.height - safeAreaFixed.top - safeAreaFixed.bottom
                        )
                    , lp->sizeParam()
                    );
            ZFUIAlignApplyT(
                    childFrame
                    , lp->align()
                    , ZFUIRectApplyMargin(ZFUIRectApplyMargin(bounds, safeAreaFixed), lp->margin())
                    , child->layoutMeasuredSize()
                    );
        }
        else {
            ZFUILayoutParam::layoutParamApplyT(childFrame, bounds, child);
        }
        child->viewFrame(childFrame);
    }
}

ZF_NAMESPACE_GLOBAL_END

