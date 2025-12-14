#include "ZFUIRootView.h"
#include "ZFUIRootWindow.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIRootView)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIRootView, zfanyT<ZFUIRootWindow> const &, rootWindow)

ZFMETHOD_DEFINE_0(ZFUIRootView, const ZFCoreArray<zfautoT<ZFUIWindow> > &, windowList) {
    return _ZFP_ZFUIRootView_windowList;
}

void ZFUIRootView::layoutParam(ZF_IN ZFUILayoutParam *layoutParam) {
    ZFCoreCriticalMessage(
        "manually changing root view's layoutParam is not allowed, root view: %s, layout param: %s"
        , this
        , layoutParam
        );
}
zfanyT<ZFUILayoutParam> ZFUIRootView::layoutParam(void) {
    return _ZFP_ZFUIRootView_rootWindow ? _ZFP_ZFUIRootView_rootWindow->layoutParam() : zfnull;
}

void ZFUIRootView::viewOnAddToParent(ZF_IN ZFUIView *parent) {
    ZFCoreCriticalMessage(
        "manually adding root view to another parent is not allowed, root view: %s, parent: %s"
        , this
        , parent
        );
}
void ZFUIRootView::viewOnRemoveFromParent(ZF_IN ZFUIView *parent) {
    ZFCoreCriticalMessage(
        "manually removing root view is not allowed, root view: %s, parent: %s"
        , this
        , parent
        );
}

void ZFUIRootView::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ret = sizeHint;
}

void ZFUIRootView::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    for(zfindex i = 0; i < this->childCount(); ++i) {
        zfanyT<ZFUIView> child = this->childAt(i);
        ZFUIWindow *window = child;
        child->viewFrame(ZFUILayoutParam::layoutParamApply(
                    (window == zfnull || !window->windowMarginShouldApply())
                        ? bounds
                        : ZFUIRectApplyMargin(bounds, window->rootWindow()->windowMargin())
                    , child
                    ));
    }
}

ZF_NAMESPACE_GLOBAL_END

