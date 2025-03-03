#include "ZFUIViewFocus.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFUIViewFocus_tag_nextFocus "_ZFP_ZFUIViewFocus_tag_nextFocus"
#define _ZFP_ZFUIViewFocus_tag_nextFocusOwner "_ZFP_ZFUIViewFocus_tag_nextFocusOwner"

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusNextSetupDataHolder, ZFLevelZFFrameworkEssential) {
    ZFLISTENER(nextFocusOnDealloc) {
        v_zfweak *nextFocusOwner = zfargs.sender()->objectTag(_ZFP_ZFUIViewFocus_tag_nextFocusOwner);
        if(nextFocusOwner == zfnull) {
            return;
        }
        nextFocusOwner->objectTagRemove(_ZFP_ZFUIViewFocus_tag_nextFocus);
        zfargs.sender()->observerRemove(ZFObject::E_ObjectBeforeDealloc(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusNextSetupDataHolder)->nextFocusOnDeallocListener);
        zfargs.sender()->objectTagRemove(_ZFP_ZFUIViewFocus_tag_nextFocusOwner);
    } ZFLISTENER_END()
    this->nextFocusOnDeallocListener = nextFocusOnDealloc;
}
public:
    ZFListener nextFocusOnDeallocListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusNextSetupDataHolder)

ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewFocusNextSetup
        , ZFMP_IN(ZFUIView *, from)
        , ZFMP_IN(ZFUIView *, nextFocus)
        ) {
    if(from == zfnull) {
        return;
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusNextSetupDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusNextSetupDataHolder);

    v_zfweak *nextFocusHolderOld = from->objectTag(_ZFP_ZFUIViewFocus_tag_nextFocus);
    if(nextFocusHolderOld != zfnull) {
        ZFObject *tmp = nextFocusHolderOld->zfv;
        if(tmp != zfnull) {
            tmp->observerRemove(ZFObject::E_ObjectBeforeDealloc(), d->nextFocusOnDeallocListener);
            tmp->objectTagRemove(_ZFP_ZFUIViewFocus_tag_nextFocusOwner);
        }
        from->objectTagRemove(_ZFP_ZFUIViewFocus_tag_nextFocus);
    }

    if(nextFocus != zfnull) {
        from->objectTag(_ZFP_ZFUIViewFocus_tag_nextFocus, zfobj<v_zfweak>(nextFocus));
        nextFocus->objectTag(_ZFP_ZFUIViewFocus_tag_nextFocusOwner, zfobj<v_zfweak>(from));
        nextFocus->observerAdd(ZFObject::E_ObjectBeforeDealloc(), d->nextFocusOnDeallocListener);
    }
}

// ============================================================
ZFEXPORT_VAR_DEFINE(ZFFilterForZFObject, ZFUIViewFocusNextFilter, ZFFilterForZFObject())
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusNextFilterDataHolder, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusNextFilterDataHolder) {
    ZFUIViewFocusNextFilter(ZFFilterForZFObject());
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusNextFilterDataHolder)

zfclassPOD _ZFP_ZFUIViewFocusData {
public:
    ZFUIView *view;
    ZFUIPoint center;
};
ZFOUTPUT_TYPE(_ZFP_ZFUIViewFocusData, {
    ZFUIPointToStringT(s, v.center);
    s += " ";
    ZFObjectInfoT(s, v.view);
})

static void _ZFP_ZFUIViewFocusNextFind(
        ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIViewFocusData> &focusDatas
        , ZF_IN ZFUIView *view
        , ZF_IN zfbool includingInternalViews
        , ZF_IN zffloat offsetX
        , ZF_IN zffloat offsetY
        ) {
    if(!ZFUIViewFocusNextFilter().filterPassed(view)) {
        return;
    }

    if(view->focusable()) {
        _ZFP_ZFUIViewFocusData focusData;
        focusData.view = view;
        focusData.center = ZFUIRectGetCenter(view->viewFrame());
        focusData.center.x += offsetX;
        focusData.center.y += offsetY;
        focusDatas.add(focusData);
    }

    for(zfindex i = 0; i < view->childCount(); ++i) {
        _ZFP_ZFUIViewFocusNextFind(
            focusDatas,
            view->childAt(i),
            includingInternalViews,
            offsetX + view->viewFrame().x,
            offsetY + view->viewFrame().y);
    }
    if(includingInternalViews) {
        ZFCoreArray<zfautoT<ZFUIView> > viewArray = view->internalBgViewArray();
        for(zfindex i = 0; i < viewArray.count(); ++i) {
            _ZFP_ZFUIViewFocusNextFind(
                focusDatas,
                viewArray[i],
                includingInternalViews,
                offsetX + view->viewFrame().x,
                offsetY + view->viewFrame().y);
        }
        viewArray = view->internalFgViewArray();
        for(zfindex i = 0; i < viewArray.count(); ++i) {
            _ZFP_ZFUIViewFocusNextFind(
                focusDatas,
                viewArray[i],
                includingInternalViews,
                offsetX + view->viewFrame().x,
                offsetY + view->viewFrame().y);
        }
    }
}

static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparer_tab(
        ZF_IN _ZFP_ZFUIViewFocusData const &v0
        , ZF_IN _ZFP_ZFUIViewFocusData const &v1
        ) {
    if(v0.center.y < v1.center.y) {
        return ZFCompareSmaller;
    }
    else if(v0.center.y > v1.center.y) {
        return ZFCompareGreater;
    }
    else {
        return ZFComparerDefault(v0.center.x, v1.center.x);
    }
}
static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparer_x(
        ZF_IN _ZFP_ZFUIViewFocusData const &v0
        , ZF_IN _ZFP_ZFUIViewFocusData const &v1
        ) {
    if(v0.center.x < v1.center.x) {
        return ZFCompareSmaller;
    }
    else if(v0.center.x > v1.center.x) {
        return ZFCompareGreater;
    }
    else {
        return ZFComparerDefault(v0.center.y, v1.center.y);
    }
}
static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparer_y(
        ZF_IN _ZFP_ZFUIViewFocusData const &v0
        , ZF_IN _ZFP_ZFUIViewFocusData const &v1
        ) {
    if(v0.center.y < v1.center.y) {
        return ZFCompareSmaller;
    }
    else if(v0.center.y > v1.center.y) {
        return ZFCompareGreater;
    }
    else {
        return ZFComparerDefault(v0.center.x, v1.center.x);
    }
}
static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparerForFind(
        ZF_IN _ZFP_ZFUIViewFocusData const &v0
        , ZF_IN ZFUIView * const &v1
        ) {
    if(v0.view == v1) {
        return ZFCompareEqual;
    }
    else {
        return ZFCompareUncomparable;
    }
}

#define _ZFP_ZFUIViewFocusRatioFix 2
void _ZFP_ZFUIViewFocusDataRemoveForX(
        ZF_IN ZFCoreArray<_ZFP_ZFUIViewFocusData> &focusDatas
        , ZF_IN ZFUIView *refView
        , ZF_IN const ZFUIPoint &refCenter
        ) {
    for(zfindex i = focusDatas.count() - 1; i != zfindexMax(); --i) {
        if(focusDatas[i].center.x == refCenter.x
                || zfmAbs(focusDatas[i].center.y - refCenter.y) * _ZFP_ZFUIViewFocusRatioFix > zfmAbs(focusDatas[i].center.x - refCenter.x)
                ) {
            if(focusDatas[i].view != refView) {
                focusDatas.remove(i);
            }
            continue;
        }
        if(i > 0 && focusDatas[i].center.x == focusDatas[i - 1].center.x) {
            if(zfmAbs(focusDatas[i].center.y - refCenter.y) <= zfmAbs(focusDatas[i - 1].center.y - refCenter.y)) {
                focusDatas.remove(i - 1);
            }
            else {
                focusDatas.remove(i);
            }
        }
    }
}
void _ZFP_ZFUIViewFocusDataRemoveForY(
        ZF_IN ZFCoreArray<_ZFP_ZFUIViewFocusData> &focusDatas
        , ZF_IN ZFUIView *refView
        , ZF_IN const ZFUIPoint &refCenter
        ) {
    for(zfindex i = focusDatas.count() - 1; i != zfindexMax(); --i) {
        if(focusDatas[i].center.y == refCenter.y
                || zfmAbs(focusDatas[i].center.x - refCenter.x) > zfmAbs(focusDatas[i].center.y - refCenter.y) * _ZFP_ZFUIViewFocusRatioFix
                ) {
            if(focusDatas[i].view != refView) {
                focusDatas.remove(i);
            }
            continue;
        }
        if(i > 0 && focusDatas[i].center.y == focusDatas[i - 1].center.y) {
            if(zfmAbs(focusDatas[i].center.x - refCenter.x) <= zfmAbs(focusDatas[i - 1].center.x - refCenter.x)) {
                focusDatas.remove(i - 1);
            }
            else {
                focusDatas.remove(i);
            }
        }
    }
}

ZFUIView *_ZFP_ZFUIViewFocusDataFindNext(
        ZF_IN const ZFCoreArray<_ZFP_ZFUIViewFocusData> &focusDatas
        , ZF_IN zfbool ascending
        , ZF_IN ZFUIView *view
        ) {
    if(ascending) {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax()) {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[0].view);
        }
        if(index != focusDatas.count() - 1) {
            return focusDatas[index + 1].view;
        }
        return zfnull;
    }
    else {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax()) {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[focusDatas.count() - 1].view);
        }
        if(index != 0) {
            return focusDatas[index - 1].view;
        }
        return zfnull;
    }
}
ZFUIView *_ZFP_ZFUIViewFocusDataFindFirst(
        ZF_IN const ZFCoreArray<_ZFP_ZFUIViewFocusData> &focusDatas
        , ZF_IN zfbool ascending
        , ZF_IN ZFUIView *view
        ) {
    if(ascending) {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax()) {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[0].view);
        }
        if(index != 0) {
            return focusDatas[0].view;
        }
        return zfnull;
    }
    else {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax()) {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[focusDatas.count() - 1].view);
        }
        if(index != focusDatas.count() - 1) {
            return focusDatas[focusDatas.count() - 1].view;
        }
        return zfnull;
    }
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIViewFocusNextParam, ZFUIViewFocusNextParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewFocusNextParam, ZFUIOrientationFlags, focusDirection)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewFocusNextParam, zfbool, focusLoopMode)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewFocusNextParam, zfanyT<ZFUIView>, focusEndParent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewFocusNextParam, zfbool, focusInternalViews)

void ZFUIViewFocusNextParam::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFUIViewFocusNextParam focusDirection:%s focusLoopMode:%s focusEndParent:%s focusInternalViews:%s>"
            , this->focusDirection()
            , this->focusLoopMode()
            , this->focusEndParent()
            , this->focusInternalViews()
            );
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfanyT<ZFUIView>, ZFUIViewFocusNextFind
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIViewFocusNextParam &, param, ZFUIViewFocusNextParam())
        ) {
    if(view == zfnull) {
        return zfnull;
    }

    {
        v_zfweak *t = view->objectTag(_ZFP_ZFUIViewFocus_tag_nextFocus);
        if(t != zfnull) {
            return t->zfv;
        }
    }

    ZFUIView *root = view;
    ZFUIPoint viewCenter = ZFUIRectGetCenter(view->viewFrame());
    while(root->parent() != zfnull
            && !root->classData()->classIsTypeOf(ZFUIWindow::ClassData())
            && root != param.focusEndParent()
            ) {
        viewCenter.x += root->viewFrame().x;
        viewCenter.y += root->viewFrame().y;
        root = root->parent();
    }
    ZFCoreArray<_ZFP_ZFUIViewFocusData> focusDatas;
    _ZFP_ZFUIViewFocusNextFind(focusDatas, root, param.focusInternalViews(), 0, 0);

    ZFUIView *ret = zfnull;
    if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Right) && ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Bottom)) { // tab
        focusDatas.sort(0, zfindexMax(), _ZFP_ZFUIViewFocusNextFind_comparer_tab);
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatas, zftrue, view);
        if(ret != zfnull) {
            return ret;
        }
        return _ZFP_ZFUIViewFocusDataFindFirst(focusDatas, zftrue, view);
    }
    if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Left) && ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Top)) { // shift tab
        focusDatas.sort(0, zfindexMax(), _ZFP_ZFUIViewFocusNextFind_comparer_tab);
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatas, zffalse, view);
        if(ret != zfnull) {
            return ret;
        }
        return _ZFP_ZFUIViewFocusDataFindFirst(focusDatas, zffalse, view);
    }

    ZFCoreArray<_ZFP_ZFUIViewFocusData> focusDatasX = focusDatas;
    ZFCoreArray<_ZFP_ZFUIViewFocusData> focusDatasY;
    focusDatasY.copyFrom(focusDatas);

    focusDatasX.sort(0, zfindexMax(), _ZFP_ZFUIViewFocusNextFind_comparer_x);
    _ZFP_ZFUIViewFocusDataRemoveForX(focusDatasX, view, viewCenter);

    focusDatasY.sort(0, zfindexMax(), _ZFP_ZFUIViewFocusNextFind_comparer_y);
    _ZFP_ZFUIViewFocusDataRemoveForY(focusDatasY, view, viewCenter);

    if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Right)) {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasX, zftrue, view);
        if(ret != zfnull) {
            return ret;
        }
    }
    if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Bottom)) {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasY, zftrue, view);
        if(ret != zfnull) {
            return ret;
        }
    }
    if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Left)) {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasX, zffalse, view);
        if(ret != zfnull) {
            return ret;
        }
    }
    if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Top)) {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasY, zffalse, view);
        if(ret != zfnull) {
            return ret;
        }
    }

    if(param.focusLoopMode()) {
        if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Right)) {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasX, zftrue, view);
            if(ret != zfnull) {
                return ret;
            }
        }
        if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Bottom)) {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasY, zftrue, view);
            if(ret != zfnull) {
                return ret;
            }
        }
        if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Left)) {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasX, zffalse, view);
            if(ret != zfnull) {
                return ret;
            }
        }
        if(ZFBitTest(param.focusDirection(), v_ZFUIOrientation::e_Top)) {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasY, zffalse, view);
            if(ret != zfnull) {
                return ret;
            }
        }
    }

    return zfnull;
}

ZFMETHOD_FUNC_DEFINE_2(zfanyT<ZFUIView>, ZFUIViewFocusNextMove
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIViewFocusNextParam &, param, ZFUIViewFocusNextParam())
        ) {
    zfanyT<ZFUIView> next = ZFUIViewFocusNextFind(view, param);
    if(next != zfnull) {
        next->focusRequest(zftrue);
    }
    return next;
}

// ============================================================
static zfbool _ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed = zffalse;
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFUIViewFocusResolveKeyEvent
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIKeyEvent *, keyEvent)
        , ZFMP_OUT_OPT(zfauto *, nextFocus, zfnull)
        , ZFMP_IN_OPT(ZFUIView *, endParent, zfnull)
        ) {
    if(nextFocus != zfnull) {
        *nextFocus = zfnull;
    }
    ZFUIViewFocusNextParam param;
    switch(keyEvent->keyCode) {
        case v_ZFUIKeyCode::e_kLeft:
            param.focusDirection(v_ZFUIOrientation::e_Left);
            break;
        case v_ZFUIKeyCode::e_kUp:
            param.focusDirection(v_ZFUIOrientation::e_Top);
            break;
        case v_ZFUIKeyCode::e_kRight:
            param.focusDirection(v_ZFUIOrientation::e_Right);
            break;
        case v_ZFUIKeyCode::e_kDown:
            param.focusDirection(v_ZFUIOrientation::e_Bottom);
            break;
        case v_ZFUIKeyCode::e_kTab: {
            param.focusLoopMode(zftrue);
            if(_ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed) {
                param.focusDirection(v_ZFUIOrientation::e_Left | v_ZFUIOrientation::e_Top);
            }
            else {
                param.focusDirection(v_ZFUIOrientation::e_Right | v_ZFUIOrientation::e_Bottom);
            }
        }
            break;
        case v_ZFUIKeyCode::e_kShift:
            switch(keyEvent->keyAction) {
                case v_ZFUIKeyAction::e_Down:
                case v_ZFUIKeyAction::e_Repeat:
                    _ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed = zftrue;
                    break;
                case v_ZFUIKeyAction::e_Up:
                case v_ZFUIKeyAction::e_Cancel:
                    _ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed = zffalse;
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }
            keyEvent->eventResolved(zftrue);
            return zffalse;
        default:
            return zffalse;
    }
    keyEvent->eventResolved(zftrue);
    if(keyEvent->keyAction != v_ZFUIKeyAction::e_Repeat && keyEvent->keyAction != v_ZFUIKeyAction::e_Up) {
        return zftrue;
    }
    param.focusEndParent(endParent);
    zfanyT<ZFUIView> next = ZFUIViewFocusNextMove(view, param);
    if(nextFocus != zfnull) {
        *nextFocus = next;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

