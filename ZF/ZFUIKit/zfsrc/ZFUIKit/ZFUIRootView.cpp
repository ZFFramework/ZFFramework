#include "ZFUIRootView.h"
#include "ZFUIRootWindow.h"
#include "ZFUIWindow.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIRootView)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIRootView, zfanyT<ZFUIRootWindow> const &, rootWindow)

ZFMETHOD_DEFINE_0(ZFUIRootView, const ZFCoreArray<zfautoT<ZFUIWindow> > &, windowList) {
    return _ZFP_ZFUIRootView_windowList;
}

void ZFUIRootView::viewOnAddToParent(ZF_IN ZFUIView *parent) {
    ZFCoreCriticalMessage(
        "you must not add ZFUIRootView to another parent, parent: %s",
        parent);
}
void ZFUIRootView::viewOnRemoveFromParent(ZF_IN ZFUIView *parent) {
    ZFCoreCriticalMessage(
        "you must not add ZFUIRootView to another parent, parent: %s",
        parent);
}

void ZFUIRootView::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ret = sizeHint;
}

static void _ZFP_ZFUIRootView_layoutParamApplyT(
        ZF_OUT ZFUIRect &ret
        , ZF_IN const ZFUIRect &rect
        , ZF_IN ZFUIView *child
        , ZF_IN ZFUILayoutParam *lp
        , ZF_IN const ZFUIMargin &windowMargin
        ) {
    ZFUIMargin totalMargin = ZFUIMarginInc(lp->margin(), windowMargin);
    ZFUISize refSizeTmp = ZFUIRectGetSize(ZFUIRectApplyMargin(rect, totalMargin));
    if(refSizeTmp.width < 0) {
        refSizeTmp.width = 0;
    }
    if(refSizeTmp.height < 0) {
        refSizeTmp.height = 0;
    }

    if(lp->sizeHint().width >= 0) {
        refSizeTmp.width = zfmMin(lp->sizeHint().width, refSizeTmp.width);
    }
    if(lp->sizeHint().height >= 0) {
        refSizeTmp.height = zfmMin(lp->sizeHint().height, refSizeTmp.height);
    }
    child->layoutMeasure(refSizeTmp, lp->sizeParam());
    ZFUIAlignApplyT(
            ret,
            lp->align(),
            ZFUIRectApplyMargin(rect, totalMargin),
            child->layoutMeasuredSize()
        );
}
void ZFUIRootView::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    for(zfindex i = 0; i < this->childCount(); ++i) {
        zfanyT<ZFUIView> child = this->childAt(i);
        ZFUIWindow *window = child;
        if(window == zfnull || !window->windowMarginShouldApply()) {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    child->layoutParam()));
            continue;
        }

        ZFUIRect result = ZFUIRectZero();
        _ZFP_ZFUIRootView_layoutParamApplyT(
            result,
            bounds,
            child,
            child->layoutParam(),
            window->rootWindow()->windowMargin());
        child->viewFrame(result);
    }
}

ZF_NAMESPACE_GLOBAL_END

