#include "ZFUIViewPositionMap.h"
#include "protocol/ZFProtocolZFUIViewPositionMap.h"
#include "ZFUIWindow.h""

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(ZFUIPoint, ZFUIViewPositionMap
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIPoint &, localPos, ZFUIPointZero())
        ) {
    ZFUIPoint ret = ZFUIPointZero();
    ZFPROTOCOL_ACCESS(ZFUIViewPositionMap)->viewPositionMap(ret, view, ZFUIPointApplyScale(localPos, view->UIScaleFixed()));
    ZFUIRootWindow *rootWindow = ZFUIWindow::rootWindowForView(view);
    if(rootWindow) {
        ZFUIPointApplyScaleReverselyT(ret, ret, rootWindow->rootView()->UIScaleFixed());
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_1(ZFUIRect, ZFUIViewPositionOnScreen
        , ZFMP_IN(ZFUIView *, view)
        ) {
    ZFUIPoint pos = ZFUIViewPositionMap(view);
    return ZFUIRectCreate(pos.x, pos.y, view->width(), view->height());
}

ZF_NAMESPACE_GLOBAL_END

