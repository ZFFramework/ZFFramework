#include "ZFUIViewUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFUIViewUtil)

ZFMETHOD_FUNC_DEFINE_1(zfanyT<ZFUIView>, viewRoot
        , ZFMP_IN(ZFUIView *, view)
        ) {
    if(view != zfnull) {
        while(view->parent() != zfnull) {
            view = view->parent();
        }
    }
    return view;
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, isChildOf
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIView *, parentToCheck)
        ) {
    if(view != zfnull) {
        ZFUIView *tmp = parentToCheck;
        do {
            if(tmp == view) {
                return zftrue;
            }
            tmp = tmp->parent();
        } while(tmp != zfnull);
    }
    return zffalse;
}

ZFMETHOD_FUNC_DEFINE_4(zfanyT<ZFUIView>, viewAtPos
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(const ZFUIPoint &, pos)
        , ZFMP_IN_OPT(zfbool, filterDisabledView, zffalse)
        , ZFMP_IN_OPT(zfbool, filterInternalView, zftrue)
        ) {
    if(view == zfnull
            || (filterDisabledView && !view->viewUIEnableTree())
            ) {
        return zfnull;
    }
    if(pos.x < 0 || pos.y < 0 || pos.x > view->viewFrame().width || pos.y > view->viewFrame().height) {
        return zfnull;
    }

    ZFCoreArray<zfautoT<ZFUIView> > childList = view->internalFgViewArray();
    for(zfindex i = childList.count() - 1; i != zfindexMax(); --i) {
        ZFUIView *child = childList[i];
        ZFUIView *tmp = ZFUIViewUtil::viewAtPos(child, ZFUIPointCreate(
                pos.x - child->viewFrame().x,
                pos.y - child->viewFrame().y
            ),
            filterDisabledView,
            filterInternalView);
        if(tmp != zfnull) {
            return tmp;
        }
    }

    ZFUIPoint layoutChildOffset = view->layoutChildOffset();
    childList = view->childArray();
    for(zfindex i = childList.count() - 1; i != zfindexMax(); --i) {
        ZFUIView *child = childList[i];
        ZFUIView *tmp = ZFUIViewUtil::viewAtPos(child, ZFUIPointCreate(
                pos.x - (child->viewFrame().x + layoutChildOffset.x),
                pos.y - (child->viewFrame().y + layoutChildOffset.y)
            ),
            filterDisabledView,
            filterInternalView);
        if(tmp != zfnull) {
            return tmp;
        }
    }

    childList = view->internalBgViewArray();
    for(zfindex i = childList.count() - 1; i != zfindexMax(); --i) {
        ZFUIView *child = childList[i];
        ZFUIView *tmp = ZFUIViewUtil::viewAtPos(child, ZFUIPointCreate(
                pos.x - child->viewFrame().x,
                pos.y - child->viewFrame().y
            ),
            filterDisabledView,
            filterInternalView);
        if(tmp != zfnull) {
            return tmp;
        }
    }

    if(filterDisabledView && !view->viewUIEnable()) {
        return zfnull;
    }

    return view;
}

ZFMETHOD_FUNC_DEFINE_3(void, viewRectToParentT
        , ZFMP_OUT(ZFUIRect &, rect)
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIView *, parent)
        ) {
    if(view == zfnull || parent == zfnull) {
        rect = ZFUIRectZero();
        return;
    }
    rect = view->viewFrame();
    while(view->parent() != zfnull && view != parent) {
        view = view->parent();
        ZFUIPoint layoutChildOffset = view->layoutChildOffset();
        rect.x += layoutChildOffset.x;
        rect.y += layoutChildOffset.y;
    }
    if(view != parent) {
        rect = ZFUIRectZero();
    }
}
ZFMETHOD_FUNC_DEFINE_2(ZFUIRect, viewRectToParent
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIView *, parent)
        ) {
    ZFUIRect ret = ZFUIRectZero();
    ZFUIViewUtil::viewRectToParentT(ret, view, parent);
    return ret;
}

ZF_NAMESPACE_END(ZFUIViewUtil)
ZF_NAMESPACE_GLOBAL_END

