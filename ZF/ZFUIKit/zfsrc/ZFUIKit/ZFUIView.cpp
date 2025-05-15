#include "ZFUIView.h"
#include "protocol/ZFProtocolZFUIView.h"
#include "protocol/ZFProtocolZFUIViewFocus.h"
#include "protocol/ZFProtocolZFUIViewTransform.h"
#include "ZFUIViewFocus.h"

#include "ZFCore/ZFSTLWrapper/zfstlset.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfuint _ZFP_ZFUIView_stateFlags = 0;
ZFSTYLE_DEFAULT_DEFINE(ZFUIView)

// ============================================================
// _ZFP_ZFUIViewPrivate
typedef zfstlset<zfstring> _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType;
zfclassNotPOD _ZFP_ZFUIViewPrivate {
public:
    void *nativeView;
    void *nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback;
    ZFUIMargin nativeImplViewMargin;

    ZFUIView *parent;
    ZFUIViewChildLayer viewLayer;
    zfautoT<ZFUILayoutParam> layoutParam;
    zfautoT<ZFUILayoutParam> serializableRefLayoutParam;
    zfautoT<ZFUILayoutParam> serializableRefLayoutParamCache;

    _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType internalViewAutoSerializeTags;

    zffloat UIScaleInherited;
    zffloat UIScaleForImpl;
    zffloat UIScaleFixed;

    zfautoT<ZFUIViewMeasureResult> measureResult;
    ZFUIRect viewFrame;
    ZFUIRect viewFramePrev; // also used to store state of user override of viewFrame,
                            // when viewFrame changed back to viewFramePrev,
                            // it's considered user has canceled viewFrame override,
                            // see also stateFlag_viewFrameOverrideFlag
    ZFUIRect nativeImplViewFrame;
    ZFCoreArray<zfautoT<ZFUIView> > layerInternalImpl;
    ZFCoreArray<zfautoT<ZFUIView> > layerInternalBg;
    ZFCoreArray<zfautoT<ZFUIView> > layerNormal;
    ZFCoreArray<zfautoT<ZFUIView> > layerInternalFg;
    ZFListener layoutParamOnUpdate;

    enum {
        stateFlag_nativeImplViewRequireVirtualIndex = 1 << 0,
        stateFlag_layoutRequested = 1 << 1,
        stateFlag_layoutRequestedRecursively = 1 << 2,
        stateFlag_layouting = 1 << 3,
        stateFlag_viewTreeInWindow = 1 << 4,
        stateFlag_viewTreeVisibleInternal = 1 << 5, // this flag detects visible only, without checking viewTreeInWindow
        stateFlag_viewFrameOverrideFlag = 1 << 6,
        stateFlag_viewFrameOverride_x = 1 << 7,
        stateFlag_viewFrameOverride_y = 1 << 8,
        stateFlag_viewFrameOverride_width = 1 << 9,
        stateFlag_viewFrameOverride_height = 1 << 10,
        stateFlag_viewFrameOverride_centerX = 1 << 11,
        stateFlag_viewFrameOverride_centerY = 1 << 12,
        stateFlag_viewFrameOverride_mask = 0
            | stateFlag_viewFrameOverride_x
            | stateFlag_viewFrameOverride_y
            | stateFlag_viewFrameOverride_width
            | stateFlag_viewFrameOverride_height
            | stateFlag_viewFrameOverride_centerX
            | stateFlag_viewFrameOverride_centerY
            ,
        stateFlag_UIScaleOnUpdate = 1 << 13,
        stateFlag_viewTransformUpdate = 1 << 14,
        stateFlag_viewTransformModified = 1 << 15,
        stateFlag_observerHasAddFlag_ViewTreeInWindowOnUpdate = 1 << 16,
        stateFlag_observerHasAddFlag_ViewTreeVisibleOnUpdate = 1 << 17,
        stateFlag_observerHasAddFlag_ViewChildOnUpdate = 1 << 18,
        stateFlag_observerHasAddFlag_ViewChildOnAdd = 1 << 19,
        stateFlag_observerHasAddFlag_ViewChildOnRemove = 1 << 20,
        stateFlag_observerHasAddFlag_ViewOnAddToParent = 1 << 21,
        stateFlag_observerHasAddFlag_ViewOnRemoveFromParent = 1 << 22,
        stateFlag_observerHasAddFlag_UIScaleOnUpdate = 1 << 23,
        stateFlag_observerHasAddFlag_ViewLayoutOnLayoutRequest = 1 << 24,
        stateFlag_observerHasAddFlag_ViewLayoutOnMeasure = 1 << 25,
        stateFlag_observerHasAddFlag_ViewLayoutOnLayoutPrepare = 1 << 26,
        stateFlag_observerHasAddFlag_ViewLayoutOnLayout = 1 << 27,
        stateFlag_observerHasAddFlag_ViewLayoutOnLayoutFinish = 1 << 28,
        stateFlag_observerHasAddFlag_ViewOnEvent = 1 << 29,
    };
    zfuint stateFlag;

public:
    _ZFP_ZFUIViewPrivate(void)
    : nativeView(zfnull)
    , nativeImplView(zfnull)
    , nativeImplViewDeleteCallback(zfnull)
    , nativeImplViewMargin(ZFUIMarginZero())
    , parent(zfnull)
    , viewLayer(v_ZFUIViewChildLayer::e_Normal)
    , layoutParam(zfnull)
    , serializableRefLayoutParam(zfnull)
    , serializableRefLayoutParamCache()
    , internalViewAutoSerializeTags()
    , UIScaleInherited(1)
    , UIScaleForImpl(1)
    , UIScaleFixed(1)
    , measureResult(zfnull)
    , viewFrame(ZFUIRectZero())
    , viewFramePrev(ZFUIRectZero())
    , nativeImplViewFrame(ZFUIRectZero())
    , layerInternalImpl()
    , layerInternalBg()
    , layerNormal()
    , layerInternalFg()
    , layoutParamOnUpdate()
    , stateFlag(0)
    {
        ZFBitSet(this->stateFlag, 0
                | _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested
                | _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively
                | _ZFP_ZFUIViewPrivate::stateFlag_viewTreeVisibleInternal
                );
    }

public:
    void viewTreeVisibleUpdate(
            ZF_IN ZFUIView *owner
            , ZF_IN zfbool viewTreeInWindow
            , ZF_IN zfbool viewTreeVisibleInternal
            ) {
        if(ZFBitTest(this->stateFlag, stateFlag_viewTreeInWindow) != viewTreeInWindow) {
            zfbool viewTreeVisibleOld = owner->viewTreeVisible();
            if(viewTreeInWindow) {
                ZFBitSet(this->stateFlag, stateFlag_viewTreeInWindow);
            }
            else {
                ZFBitUnset(this->stateFlag, stateFlag_viewTreeInWindow);
            }
            if(viewTreeVisibleInternal) {
                ZFBitSet(this->stateFlag, stateFlag_viewTreeVisibleInternal);
            }
            else {
                ZFBitUnset(this->stateFlag, stateFlag_viewTreeVisibleInternal);
            }
            owner->viewTreeInWindowOnUpdate();
            if(viewTreeVisibleOld != owner->viewTreeVisible()) {
                owner->viewTreeVisibleOnUpdate();
            }
            this->viewTreeVisibleUpdateRecursive(owner, viewTreeInWindow, viewTreeVisibleInternal);
        }
        else if(ZFBitTest(this->stateFlag, stateFlag_viewTreeVisibleInternal) != viewTreeVisibleInternal) {
            owner->viewTreeVisibleOnUpdate();
            this->viewTreeVisibleUpdateRecursive(owner, viewTreeInWindow, viewTreeVisibleInternal);
        }
    }
    void viewTreeVisibleUpdateRecursive(
            ZF_IN ZFUIView *owner
            , ZF_IN zfbool viewTreeInWindow
            , ZF_IN zfbool viewTreeVisibleInternal
            ) {
        for(zfindex i = 0; i < owner->d->layerInternalImpl.count(); ++i) {
            ZFUIView *v = owner->d->layerInternalImpl[i];
            v->d->viewTreeVisibleUpdate(v, viewTreeInWindow, viewTreeVisibleInternal && v->visible());
        }
        for(zfindex i = 0; i < owner->d->layerInternalBg.count(); ++i) {
            ZFUIView *v = owner->d->layerInternalBg[i];
            v->d->viewTreeVisibleUpdate(v, viewTreeInWindow, viewTreeVisibleInternal && v->visible());
        }
        for(zfindex i = 0; i < owner->d->layerNormal.count(); ++i) {
            ZFUIView *v = owner->d->layerNormal[i];
            v->d->viewTreeVisibleUpdate(v, viewTreeInWindow, viewTreeVisibleInternal && v->visible());
        }
        for(zfindex i = 0; i < owner->d->layerInternalFg.count(); ++i) {
            ZFUIView *v = owner->d->layerInternalFg[i];
            v->d->viewTreeVisibleUpdate(v, viewTreeInWindow, viewTreeVisibleInternal && v->visible());
        }
    }
    void layoutRequest(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool recursively
            ) {
        if(recursively) {
            if(!ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively)) {
                ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl = ZFPROTOCOL_ACCESS(ZFUIView);
                do {
                    ZFBitSet(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively);
                    if(!ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)) {
                        ZFBitSet(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
                        view->layoutOnLayoutRequest();
                        impl->layoutRequest(view);
                    }
                    view = view->parent();
                } while(view != zfnull && !ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively));
            }
        }
        else {
            if(!ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)) {
                ZFBitSet(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
                view->layoutOnLayoutRequest();
                ZFPROTOCOL_ACCESS(ZFUIView)->layoutRequest(view);
            }
        }
    }
    void layoutAction(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &bounds
            ) {
        ZFBitSet(this->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);

        // layout prepare
        view->layoutOnLayoutPrepare(bounds);
        if(ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutPrepare)
                || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutPrepare)
                ) {
            view->observerNotify(ZFUIView::E_ViewLayoutOnLayoutPrepare());
        }

        ZFUIRect nativeImplViewFrame = ZFUIRectZero();
        view->nativeImplViewOnLayout(nativeImplViewFrame, bounds, view->nativeImplViewMargin());
        if(view->d->nativeImplViewFrame != nativeImplViewFrame
                || ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleOnUpdate)
                ) {
            view->d->nativeImplViewFrame = nativeImplViewFrame;
            if(view->d->nativeImplView != zfnull) {
                ZFPROTOCOL_ACCESS(ZFUIView)->nativeImplViewFrame(
                    view,
                    ZFUIRectApplyScale(nativeImplViewFrame, view->UIScaleFixed()));
            }
        }

        // layout
        if(ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayout)
                || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayout)
                ) {
            zfobj<v_zfbool> done;
            view->observerNotify(ZFUIView::E_ViewLayoutOnLayout(), done);
            if(!done->zfv) {
                view->layoutOnLayout(bounds);
            }
        }
        else {
            view->layoutOnLayout(bounds);
        }

        // internal views
        view->internalViewOnLayout(bounds);

        // layout finish
        view->layoutOnLayoutFinish(bounds);
        if(ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutFinish)
                || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutFinish)
                ) {
            view->observerNotify(ZFUIView::E_ViewLayoutOnLayoutFinish());
        }

        // update transform
        if(ZFBitTest(this->stateFlag, stateFlag_viewTransformUpdate)) {
            this->viewTransformUpdateAction(view);
        }

        ZFBitUnset(this->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
    }
    void layoutParamUpdate(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUILayoutParam *newLayoutParam
            ) {
        if(!owner->d->layoutParamOnUpdate) {
            ZFLISTENER_1(layoutParamOnUpdate
                    , ZFUIView *, owner
                    ) {
                owner->layoutRequest();
            } ZFLISTENER_END()
            owner->d->layoutParamOnUpdate = layoutParamOnUpdate;
        }

        if(this->layoutParam) {
            this->layoutParam->_ZFP_LP_owner = zfnull;
        }

        if(newLayoutParam) {
            ZFCoreAssertWithMessageTrim(newLayoutParam->_ZFP_LP_owner == zfnull
                    , "layout param %s already attached to view: %s"
                    , newLayoutParam
                    , newLayoutParam->_ZFP_LP_owner
                    );
            newLayoutParam->_ZFP_LP_owner = owner;
            newLayoutParam->observerAdd(
                ZFUILayoutParam::E_LayoutParamOnUpdate(),
                owner->d->layoutParamOnUpdate);
        }
        if(this->layoutParam) {
            this->layoutParam->observerRemove(ZFUILayoutParam::E_LayoutParamOnUpdate(), owner->d->layoutParamOnUpdate);
        }
        this->layoutParam = newLayoutParam;
    }
    // return whether still keep in override state, or false to clear override state
    zfbool viewFrameOverrideFix(
            ZF_IN ZFUIView *owner
            , ZF_IN const ZFUIRect &viewFrameNew
            ) {
        ZFUIPoint viewCenter = ZFUIRectGetCenter(this->viewFrame);

        if(!ZFBitTest(this->stateFlag, stateFlag_viewFrameOverride_width)) {
            this->viewFrame.width = viewFrameNew.width;
            this->viewFramePrev.width = viewFrameNew.width;
        }
        if(ZFBitTest(this->stateFlag, stateFlag_viewFrameOverride_centerX)) {
            this->viewFrame.x = viewCenter.x - this->viewFrame.width / 2;
        }
        else if(!ZFBitTest(this->stateFlag, stateFlag_viewFrameOverride_x)) {
            this->viewFrame.x = viewFrameNew.x;
            this->viewFramePrev.x = viewFrameNew.x;
        }

        if(!ZFBitTest(this->stateFlag, stateFlag_viewFrameOverride_height)) {
            this->viewFrame.height = viewFrameNew.height;
            this->viewFramePrev.height = viewFrameNew.height;
        }
        if(ZFBitTest(this->stateFlag, stateFlag_viewFrameOverride_centerY)) {
            this->viewFrame.y = viewCenter.y - this->viewFrame.height / 2;
        }
        else if(!ZFBitTest(this->stateFlag, stateFlag_viewFrameOverride_y)) {
            this->viewFrame.y = viewFrameNew.y;
            this->viewFramePrev.y = viewFrameNew.y;
        }

        if(this->viewFrame == this->viewFramePrev) {
            owner->viewFrameReset();
            return zffalse;
        }

        return zftrue;
    }
    void viewFrameUpdate(ZF_IN const ZFUIRect &viewFrame) {
        if(!ZFBitTest(this->stateFlag, stateFlag_viewFrameOverrideFlag)) {
            this->viewFramePrev = this->viewFrame;
        }
        this->viewFrame = viewFrame;
        if(this->viewFrame.width < 0) {
            this->viewFrame.width = 0;
        }
        if(this->viewFrame.height < 0) {
            this->viewFrame.height = 0;
        }
    }
    void viewFrameUpdateForImpl(ZF_IN ZFUIView *view) {
        if(view->UIScale() == 1 || view->UIScale() == 0 || view->parent() == zfnull) {
            ZFPROTOCOL_ACCESS(ZFUIView)->viewFrame(
                    view,
                    ZFUIRectApplyScale(this->viewFrame, view->UIScaleFixed())
                );
        }
        else {
            ZFUIPoint viewCenter = ZFUIRectGetCenter(this->viewFrame);
            ZFPROTOCOL_ACCESS(ZFUIView)->viewFrame(
                    view,
                    ZFUIRectCreate(
                            viewCenter.x * this->UIScaleInherited * this->UIScaleForImpl - this->viewFrame.width * this->UIScaleFixed / 2,
                            viewCenter.y * this->UIScaleInherited * this->UIScaleForImpl - this->viewFrame.height * this->UIScaleFixed / 2,
                            this->viewFrame.width * this->UIScaleFixed,
                            this->viewFrame.height * this->UIScaleFixed
                        )
                );
        }
    }
    zfindex viewLayerPrevCount(ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer) {
        zfindex nativeImplFix = (this->nativeImplView && ZFBitTest(this->stateFlag, stateFlag_nativeImplViewRequireVirtualIndex) ? 1 : 0);
        if(&layer == &(this->layerNormal)) {
            return this->layerInternalImpl.count() + nativeImplFix
                + this->layerInternalBg.count();
        }
        else if(&layer == &(this->layerInternalImpl)) {
            return 0;
        }
        else if(&layer == &(this->layerInternalBg)) {
            return this->layerInternalImpl.count() + nativeImplFix;
        }
        else if(&layer == &(this->layerInternalFg)) {
            return this->layerInternalImpl.count() + nativeImplFix
                + this->layerInternalBg.count()
                + this->layerNormal.count();
        }
        else {
            ZFCoreCriticalShouldNotGoHere();
            return 0;
        }
    }
    static void UIScaleUpdateRecursively(ZF_IN ZFUIView *child) {
        ZFBitSet(child->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleOnUpdate);
        child->d->UIScaleFixed = child->d->UIScaleInherited * child->UIScale() * child->d->UIScaleForImpl;
        for(zfindex i = child->d->layerInternalImpl.count() - 1; i != zfindexMax(); --i) {
            UIScaleUpdateCheck(child, child->d->layerInternalImpl.get(i));
        }
        for(zfindex i = child->d->layerInternalBg.count() - 1; i != zfindexMax(); --i) {
            UIScaleUpdateCheck(child, child->d->layerInternalBg.get(i));
        }
        for(zfindex i = child->d->layerInternalFg.count() - 1; i != zfindexMax(); --i) {
            UIScaleUpdateCheck(child, child->d->layerInternalFg.get(i));
        }
        for(zfindex i = child->childCount() - 1; i != zfindexMax(); --i) {
            UIScaleUpdateCheck(child, child->childAt(i));
        }
        child->layoutRequest();
        child->UIScaleOnUpdate();
    }
    static void UIScaleUpdateCheck(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            ) {
        zffloat UIScaleInherited = parent->d->UIScaleInherited * parent->UIScale();
        if(child->d->UIScaleInherited == UIScaleInherited && child->d->UIScaleForImpl == parent->d->UIScaleForImpl) {
            return;
        }
        child->d->UIScaleInherited = UIScaleInherited;
        child->d->UIScaleForImpl = parent->d->UIScaleForImpl;
        UIScaleUpdateRecursively(child);
    }
    ZFUILayoutParam *child(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN ZFUIView *child
            , ZF_IN ZFUILayoutParam *layoutParam
            , ZF_IN zfindex atIndex
            ) {
        ZFCoreAssertWithMessageTrim(!owner->objectDeallocRunning(),
                "[ZFUIView] add child during parent's dealloc is not allowed");
        ZFCoreAssertWithMessageTrim(child != zfnull, "[ZFUIView] add null child");
        ZFCoreAssertWithMessageTrim(child->parent() == zfnull, "[ZFUIView] add child which already has parent, you should remove it first");
        zfblockedRelease(zfRetain(child));

        if(this->serializableRefLayoutParamCache == zfnull) {
            this->serializableRefLayoutParamCache = owner->layoutParamCreate();
        }
        child->serializableRefLayoutParam(this->serializableRefLayoutParamCache);

        if(layoutParam == zfnull) {
            layoutParam = child->layoutParam();
        }
        zfautoT<ZFUILayoutParam> layoutParamTmp = layoutParam;
        if(layoutParamTmp == zfnull) {
            layoutParamTmp = owner->layoutParamCreate();
            layoutParam = layoutParamTmp;
        }
        else {
            if(!layoutParam->classData()->classIsTypeOf(owner->layoutParamClass())) {
                layoutParamTmp = owner->layoutParamCreate();
                layoutParamTmp->styleableCopyFrom(layoutParam);
                layoutParam = layoutParamTmp;
            }
        }

        child->_ZFP_ZFUIView_parentOnUpdate(owner, layoutParam, childLayer);
        if(atIndex == zfindexMax()) {
            atIndex = layer.count();
        }
        layer.add(child, atIndex);
        owner->implChildOnAdd(child, this->viewLayerPrevCount(layer) + atIndex, childLayer, atIndex);

        owner->layoutRequest();
        child->layoutRequest();

        UIScaleUpdateCheck(owner, child);
        owner->viewChildOnAdd(child, childLayer);
        child->viewOnAddToParent(owner);
        owner->viewChildOnUpdate();

        return layoutParam;
    }
    void childRemove(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN ZFUIView *view
            ) {
        zfindex index = this->childFind(layer, view);
        if(index != zfindexMax()) {
            this->childRemoveAt(owner, childLayer, layer, index);
        }
    }
    void childRemoveAt(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN zfindex index
            ) {
        zfautoT<ZFUIView> child = layer.get(index);

        child->_ZFP_ZFUIView_parentOnUpdate(zfnull, zfnull, v_ZFUIViewChildLayer::e_Normal);
        layer.remove(index);
        owner->implChildOnRemove(child, this->viewLayerPrevCount(layer) + index, childLayer, index);

        owner->layoutRequest();

        owner->viewChildOnRemove(child, childLayer);
        child->viewOnRemoveFromParent(owner);
        owner->viewChildOnUpdate();
    }
    void childRemoveAll(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            ) {
        if(layer.isEmpty()) {
            return;
        }

        owner->layoutRequest();

        ZFCoreArray<zfautoT<ZFUIView> > tmp = layer;
        layer = ZFCoreArray<zfautoT<ZFUIView> >();

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i) {
            ZFUIView *child = tmp[i];
            child->_ZFP_ZFUIView_parentOnUpdate(zfnull, zfnull, v_ZFUIViewChildLayer::e_Normal);
            owner->implChildOnRemove(child, prevLayerCount + i, childLayer, i);
        }

        for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i) {
            zfautoT<ZFUIView> child = tmp[i];
            owner->viewChildOnRemove(child, childLayer);
            child->viewOnRemoveFromParent(owner);
        }

        owner->viewChildOnUpdate();
    }
    void childMove(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN zfindex fromIndex
            , ZF_IN zfindex toIndexOrIndexMax
            ) {
        ZFCoreAssertIndexRange(fromIndex, this->childCount(layer));
        if(toIndexOrIndexMax == zfindexMax()) {
            toIndexOrIndexMax = this->childCount(layer) - 1;
        }
        else {
            ZFCoreAssertIndexRange(toIndexOrIndexMax, this->childCount(layer));
        }
        if(fromIndex == toIndexOrIndexMax) {
            return;
        }

        ZFUIView *child = layer.get(fromIndex);
        layer.move(fromIndex, toIndexOrIndexMax);

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        owner->implChildOnRemove(child, prevLayerCount + fromIndex, childLayer, fromIndex);
        owner->implChildOnAdd(child, prevLayerCount + toIndexOrIndexMax, childLayer, toIndexOrIndexMax);

        owner->viewChildOnUpdate();
    }
    void childMove(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex toIndexOrIndexMax
            ) {
        zfindex fromIndex = this->childFind(layer, child);
        if(fromIndex != zfindexMax()) {
            this->childMove(owner, childLayer, layer, fromIndex, toIndexOrIndexMax);
        }
    }
    void childReplace(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN zfindex atIndex
            , ZF_IN ZFUIView *toReplace
            ) {
        ZFCoreAssert(atIndex < layer.count());
        ZFCoreAssert(toReplace != zfnull);
        ZFCoreAssert(toReplace->parent() == zfnull);

        zfautoT<ZFUIView> old = layer[atIndex];
        layer[atIndex] = toReplace;
        zfindex fixedIndex = this->viewLayerPrevCount(layer) + atIndex;
        owner->implChildOnRemove(old, fixedIndex, childLayer, atIndex);
        owner->implChildOnAdd(toReplace, fixedIndex, childLayer, atIndex);

        UIScaleUpdateCheck(owner, toReplace);
        toReplace->_ZFP_ZFUIView_parentOnUpdate(owner, old->layoutParam(), childLayer);
        old->_ZFP_ZFUIView_parentOnUpdate(zfnull, zfnull, v_ZFUIViewChildLayer::e_Normal);

        owner->viewChildOnRemove(old, childLayer);
        old->viewOnRemoveFromParent(owner);
        owner->viewChildOnAdd(toReplace, childLayer);
        toReplace->viewOnAddToParent(owner);
        owner->viewChildOnUpdate();
    }

    zfindex childCount(ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer) {
        return layer.count();
    }
    ZFUIView *childAt(
            ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN zfindex index
            ) {
        return layer.get(index);
    }
    zfindex childFind(
            ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN ZFUIView *view
            ) {
        return layer.find(view);
    }
    zfbool childArrayIsEqual(
            ZF_IN ZFUIView *view0
            , ZF_IN ZFUIView *view1
            , ZF_IN ZFUIViewChildLayer childLayer
            ) {
        const ZFCoreArray<zfautoT<ZFUIView> > *children0 = zfnull;
        const ZFCoreArray<zfautoT<ZFUIView> > *children1 = zfnull;
        switch(childLayer) {
            case v_ZFUIViewChildLayer::e_Normal:
                children0 = &(view0->d->layerNormal);
                children1 = &(view1->d->layerNormal);
                break;
            case v_ZFUIViewChildLayer::e_InternalImpl:
                children0 = &(view0->d->layerInternalImpl);
                children1 = &(view1->d->layerInternalImpl);
                break;
            case v_ZFUIViewChildLayer::e_InternalBg:
                children0 = &(view0->d->layerInternalBg);
                children1 = &(view1->d->layerInternalBg);
                break;
            case v_ZFUIViewChildLayer::e_InternalFg:
                children0 = &(view0->d->layerInternalFg);
                children1 = &(view1->d->layerInternalFg);
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        if(children0->count() != children1->count()) {
            return zffalse;
        }
        for(zfindex i = children0->count() - 1; i != zfindexMax(); --i) {
            if(ZFObjectCompare(children0->get(i), children1->get(i)) != ZFCompareEqual) {
                return zffalse;
            }
        }
        return zftrue;
    }
    ZFUILayoutParam *childLayoutParamAt(
            ZF_IN ZFCoreArray<zfautoT<ZFUIView> > &layer
            , ZF_IN zfindex index
            ) {
        return layer.get(index)->layoutParam();
    }

    zfbool serializeInternalViewFromCategoryData(
            ZF_IN ZFUIView *owner
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN const ZFSerializableData &categoryData
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
            ) {
        zfautoT<ZFUIView> internalView;
        if(!ZFObjectFromDataT(internalView, categoryData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(internalView == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "null view");
            return zffalse;
        }
        if(!internalView->classData()->classIsTypeOf(ZFUIView::ClassData())) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                internalView->objectInfoOfInstance(), ZFUIView::ClassData()->classNameFull());
            return zffalse;
        }
        ZFUIView *internalViewTmp = internalView;
        if(internalViewTmp->viewId().isEmpty()) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "auto serialized internal view %s has no viewId",
                internalViewTmp->objectInfoOfInstance());
            return zffalse;
        }
        ZFUIView *exist = zfnull;
        const ZFCoreArray<zfautoT<ZFUIView> > *views = zfnull;
        switch(childLayer) {
            case v_ZFUIViewChildLayer::e_Normal:
                ZFCoreCriticalShouldNotGoHere();
                break;
            case v_ZFUIViewChildLayer::e_InternalImpl:
                views = &(this->layerInternalImpl);
                break;
            case v_ZFUIViewChildLayer::e_InternalBg:
                views = &(this->layerInternalBg);
                break;
            case v_ZFUIViewChildLayer::e_InternalFg:
                views = &(this->layerInternalFg);
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        for(zfindex i = views->count() - 1; i != zfindexMax(); --i) {
            ZFUIView *tmp = views->get(i);
            if(tmp->viewId().compare(internalViewTmp->viewId()) == 0) {
                exist = tmp;
                break;
            }
        }
        if(exist == zfnull) {
            owner->internalViewAutoSerializeTagAdd(internalViewTmp->viewId());
            switch(childLayer) {
                case v_ZFUIViewChildLayer::e_Normal:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
                case v_ZFUIViewChildLayer::e_InternalImpl:
                    owner->internalImplViewAdd(internalViewTmp);
                    break;
                case v_ZFUIViewChildLayer::e_InternalBg:
                    owner->internalBgViewAdd(internalViewTmp);
                    break;
                case v_ZFUIViewChildLayer::e_InternalFg:
                    owner->internalFgViewAdd(internalViewTmp);
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
        else {
            exist->styleableCopyFrom(internalViewTmp);
        }
        return zftrue;
    }
    zfbool serializeInternalViewToCategoryData(
            ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFUIView *ref
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
            ) {
        const ZFCoreArray<zfautoT<ZFUIView> > *views = zfnull;
        const ZFCoreArray<zfautoT<ZFUIView> > *viewsRef = zfnull;
        const zfchar *categoryTag = zfnull;
        switch(childLayer) {
            case v_ZFUIViewChildLayer::e_Normal:
                ZFCoreCriticalShouldNotGoHere();
                break;
            case v_ZFUIViewChildLayer::e_InternalImpl:
                views = &(this->layerInternalImpl);
                if(ref != zfnull) {
                    viewsRef = &(ref->d->layerInternalImpl);
                }
                break;
                categoryTag = ZFSerializableKeyword_ZFUIView_internalImplView;
            case v_ZFUIViewChildLayer::e_InternalBg:
                views = &(this->layerInternalBg);
                if(ref != zfnull) {
                    viewsRef = &(ref->d->layerInternalBg);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalBgView;
                break;
            case v_ZFUIViewChildLayer::e_InternalFg:
                views = &(this->layerInternalFg);
                if(ref != zfnull) {
                    viewsRef = &(ref->d->layerInternalFg);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalFgView;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        if(ref == zfnull || viewsRef->count() == 0) {
            for(zfindex i = 0; i < views->count(); ++i) {
                ZFUIView *tmp = views->get(i);
                if(tmp->viewId().isEmpty()
                        || this->internalViewAutoSerializeTags.find(tmp->viewId()) == this->internalViewAutoSerializeTags.end()
                        ) {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToDataT(childData, tmp, outErrorHint)) {
                    return zffalse;
                }
                childData.category(categoryTag);
                serializableData.child(childData);
            }
        }
        else {
            for(zfindex i = 0; i < views->count(); ++i) {
                ZFUIView *tmp = views->get(i);
                if(tmp->viewId().isEmpty()
                        || this->internalViewAutoSerializeTags.find(tmp->viewId()) == this->internalViewAutoSerializeTags.end()
                        ) {
                    continue;
                }
                zfbool exist = zffalse;
                for(zfindex iRef = viewsRef->count() - 1; iRef != zfindexMax(); --iRef) {
                    if(viewsRef->get(iRef)->viewId().compare(tmp->viewId()) == 0) {
                        exist = zftrue;
                        break;
                    }
                }
                if(exist) {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToDataT(childData, tmp, outErrorHint)) {
                    return zffalse;
                }
                childData.category(categoryTag);
                serializableData.child(childData);
            }
        }
        return zftrue;
    }
    void viewTransformUpdate(ZF_IN ZFUIView *view) {
        ZFBitSet(this->stateFlag, stateFlag_viewTransformModified);
        if(ZFBitTest(this->stateFlag, stateFlag_layoutRequested)) {
            // update during layout
            ZFBitSet(this->stateFlag, stateFlag_viewTransformUpdate);
        }
        else {
            this->viewTransformUpdateAction(view);
        }
    }
    void viewTransformUpdateAction(ZF_IN ZFUIView *view) {
        ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewTransform) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewTransform);
        if(impl != zfnull) {
            ZFBitUnset(this->stateFlag, stateFlag_viewTransformUpdate);
            impl->viewTransform(view);
        }
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIView_stateFlags, ZFLevelZFFrameworkStatic) {
    _ZFP_ZFUIView_stateFlags = 0;
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewTreeInWindowOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeInWindowOnUpdate);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewTreeVisibleOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeVisibleOnUpdate);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewChildOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnUpdate);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewChildOnAdd(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnAdd);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewChildOnRemove(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnRemove);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewOnAddToParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnAddToParent);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewOnRemoveFromParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnRemoveFromParent);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_UIScaleOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_UIScaleOnUpdate);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewLayoutOnLayoutRequest(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutRequest);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewLayoutOnMeasure(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnMeasure);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewLayoutOnLayoutPrepare(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutPrepare);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewLayoutOnLayout(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayout);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewLayoutOnLayoutFinish(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutFinish);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::E_ViewOnEvent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnEvent);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIView_stateFlags) {
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewTreeInWindowOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeInWindowOnUpdate);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewTreeVisibleOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeVisibleOnUpdate);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewChildOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnUpdate);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewChildOnAdd(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnAdd);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewChildOnRemove(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnRemove);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewOnAddToParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnAddToParent);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewOnRemoveFromParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnRemoveFromParent);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_UIScaleOnUpdate(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_UIScaleOnUpdate);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewLayoutOnLayoutRequest(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutRequest);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewLayoutOnMeasure(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnMeasure);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewLayoutOnLayoutPrepare(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutPrepare);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewLayoutOnLayout(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayout);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewLayoutOnLayoutFinish(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutFinish);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::E_ViewOnEvent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnEvent);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIView_stateFlags)

// ============================================================
// ZFUIView
ZFOBJECT_REGISTER(ZFUIView)

ZFEVENT_REGISTER(ZFUIView, ViewTreeInWindowOnUpdate)
ZFEVENT_REGISTER(ZFUIView, ViewTreeVisibleOnUpdate)
ZFEVENT_REGISTER(ZFUIView, ViewChildOnUpdate)
ZFEVENT_REGISTER(ZFUIView, ViewChildOnAdd)
ZFEVENT_REGISTER(ZFUIView, ViewChildOnRemove)
ZFEVENT_REGISTER(ZFUIView, ViewOnAddToParent)
ZFEVENT_REGISTER(ZFUIView, ViewOnRemoveFromParent)
ZFEVENT_REGISTER(ZFUIView, UIScaleOnUpdate)
ZFEVENT_REGISTER(ZFUIView, ViewFocusOnUpdate)
ZFEVENT_REGISTER(ZFUIView, ViewOnEvent)
ZFEVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutRequest)
ZFEVENT_REGISTER(ZFUIView, ViewLayoutOnMeasure)
ZFEVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutPrepare)
ZFEVENT_REGISTER(ZFUIView, ViewLayoutOnLayout)
ZFEVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutFinish)
ZFEVENT_REGISTER(ZFUIView, NativeImplViewMarginOnUpdate)

// ============================================================
// serialize
void ZFUIView::serializableRefLayoutParam(ZF_IN ZFUILayoutParam *serializableRefLayoutParam) {
    d->serializableRefLayoutParam = serializableRefLayoutParam;
}
ZFUILayoutParam *ZFUIView::serializableRefLayoutParam(void) {
    return d->serializableRefLayoutParam;
}

zfbool ZFUIView::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    if(this->serializableOnCheckNeedSerializeChildren()) {
        this->childRemoveAll();
    }
    d->layoutParamUpdate(this, zfnull);

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &categoryData = serializableData.childAt(i);
        if(categoryData.resolved()) {continue;}
        zfstring category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfstringIsEqual(category, ZFSerializableKeyword_ZFUIView_child)) {
            zfauto element;
            if(!ZFObjectFromDataT(element, categoryData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            if(element == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                    "null view");
                return zffalse;
            }
            if(!element->classData()->classIsTypeOf(ZFUIView::ClassData())) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                    "%s not type of %s",
                    element->objectInfoOfInstance(), ZFUIView::ClassData()->classNameFull());
                return zffalse;
            }
            ZFUIView *child = element;
            this->childWithParam(child, child->layoutParam());

            categoryData.resolveMark();
        }
        else if(zfstringIsEqual(category, ZFSerializableKeyword_ZFUIView_layoutParam)) {
            zfauto layoutParam;
            if(!ZFObjectFromDataT(layoutParam, categoryData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            if(layoutParam == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                    "null layoutParam");
                return zffalse;
            }
            if(!layoutParam->classData()->classIsTypeOf(ZFUILayoutParam::ClassData())) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                    "%s not type of %s",
                    layoutParam->objectInfoOfInstance(), ZFUILayoutParam::ClassData()->classNameFull());
                return zffalse;
            }

            d->layoutParamUpdate(this, layoutParam);
        }
        else if(zfstringIsEqual(category, ZFSerializableKeyword_ZFUIView_internalImplView)) {
            if(!d->serializeInternalViewFromCategoryData(this,
                        v_ZFUIViewChildLayer::e_InternalImpl,
                        categoryData,
                        outErrorHint,
                        outErrorPos)
                        ) {
                return zffalse;
            }
        }
        else if(zfstringIsEqual(category, ZFSerializableKeyword_ZFUIView_internalBgView)) {
            if(!d->serializeInternalViewFromCategoryData(this,
                        v_ZFUIViewChildLayer::e_InternalBg,
                        categoryData,
                        outErrorHint,
                        outErrorPos)
                        ) {
                return zffalse;
            }
        }
        else if(zfstringIsEqual(category, ZFSerializableKeyword_ZFUIView_internalFgView)) {
            if(!d->serializeInternalViewFromCategoryData(this,
                        v_ZFUIViewChildLayer::e_InternalFg,
                        categoryData,
                        outErrorHint,
                        outErrorPos)
                        ) {
                return zffalse;
            }
        }
    }
    return zftrue;
}
zfbool ZFUIView::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    // layoutParam
    if(d->layoutParam != zfnull && this->parent() != zfnull) {
        ZFUILayoutParam *refLayoutParam = ((ref == zfnull) ? zfnull : ref->d->layoutParam);
        if(refLayoutParam == zfnull) {
            refLayoutParam = d->serializableRefLayoutParam;
        }
        if(refLayoutParam != zfnull) {
            ZFSerializableData categoryData;
            if(!ZFObjectToDataT(categoryData, d->layoutParam, outErrorHint, refLayoutParam)) {
                return zffalse;
            }
            if(categoryData.attrCount() > 0 || categoryData.childCount() > 0) {
                categoryData.category(ZFSerializableKeyword_ZFUIView_layoutParam);
                serializableData.child(categoryData);
            }
        }
    }

    { // internal views
        if(!d->internalViewAutoSerializeTags.empty()) {
            if(!d->serializeInternalViewToCategoryData(v_ZFUIViewChildLayer::e_InternalImpl, serializableData, ref, outErrorHint)) {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(v_ZFUIViewChildLayer::e_InternalBg, serializableData, ref, outErrorHint)) {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(v_ZFUIViewChildLayer::e_InternalFg, serializableData, ref, outErrorHint)) {
                return zffalse;
            }
        }
    }

    // all children
    if(this->serializableOnCheckNeedSerializeChildren()) {
        if(ref == zfnull || ref->childCount() == 0) {
            for(zfindex i = 0, count = this->childCount(); i < count; ++i) {
                ZFSerializableData childData;
                if(!ZFObjectToDataT(childData, this->childAt(i), outErrorHint)) {
                    return zffalse;
                }
                childData.category(ZFSerializableKeyword_ZFUIView_child);
                serializableData.child(childData);
            }
        }
        else {
            if(!d->childArrayIsEqual(this, ref, v_ZFUIViewChildLayer::e_Normal)) {
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "child mismatch, this: %s, ref: %s",
                    this->objectInfoOfInstance(), ref->objectInfoOfInstance());
                return zffalse;
            }
        }
    }
    return zftrue;
}

// ============================================================
// properties
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zfbool, visible) {
    if(propertyValue != propertyValueOld) {
        ZFPROTOCOL_ACCESS(ZFUIView)->visible(this, this->visible());
        d->viewTreeVisibleUpdate(
                this
                , this->viewTreeInWindow()
                , this->visible() && (this->parent()
                    ? ZFBitTest(this->parent()->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewTreeVisibleInternal)
                    : zftrue)
                );
        this->layoutRequest();
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewTreeInWindow) {
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewTreeInWindow);
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewTreeVisible) {
    // for public access, checks both viewTreeInWindow and viewTreeVisible
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewTreeInWindow)
        && ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewTreeVisibleInternal);
}
void ZFUIView::_ZFP_ZFUIView_viewTreeInWindow(ZF_IN zfbool viewTreeInWindow) {
    d->viewTreeVisibleUpdate(this
            , viewTreeInWindow
            , ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewTreeVisibleInternal)
            );
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, alpha) {
    propertyValue = zfmApplyRange<zffloat>(propertyValue, 0, 1);
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zffloat, alpha) {
    if(propertyValue != propertyValueOld) {
        ZFPROTOCOL_ACCESS(ZFUIView)->alpha(this, this->alpha());
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zfbool, focusable) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull) {
        impl->focusable(this, this->focusable());
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewUIEnable) {
    ZFPROTOCOL_ACCESS(ZFUIView)->viewUIEnable(this, this->viewUIEnable());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewUIEnableTree) {
    ZFPROTOCOL_ACCESS(ZFUIView)->viewUIEnableTree(this, this->viewUIEnableTree());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zfbool, mouseHoverEnable) {
    ZFPROTOCOL_ACCESS(ZFUIView)->mouseHoverEnable(this, this->mouseHoverEnable());
}

ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewUIEnableFixed) {
    return this->viewUIEnable() && this->viewUIEnableTreeFixed();
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewUIEnableTreeFixed) {
    if(!this->viewUIEnableTree()) {
        return zffalse;
    }
    ZFUIView *parent = this->parent();
    while(parent != zfnull) {
        if(!parent->viewUIEnableTree()) {
            return zffalse;
        }
        parent = parent->parent();
    }
    return zftrue;
}

ZFMETHOD_DEFINE_1(ZFUIView, void, viewSizeFixed
        , ZFMP_IN(const ZFUISize &, size)
        ) {
    this->viewSizeMin(size);
    this->viewSizeMax(size);
}
ZFMETHOD_DEFINE_2(ZFUIView, void, viewSizeFixed
        , ZFMP_IN(zffloat, width)
        , ZFMP_IN(zffloat, height)
        ) {
    ZFUISize size = ZFUISizeCreate(width, height);
    this->viewSizeMin(size);
    this->viewSizeMax(size);
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizeMin) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizeMax) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, ZFUIColor, bgColor) {
    ZFPROTOCOL_ACCESS(ZFUIView)->bgColor(this, this->bgColor());
}

ZFMETHOD_DEFINE_0(ZFUIView, ZFUITransformFlags, transformAvailable) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewTransform) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewTransform);
    return impl ? impl->transformAvailable() : v_ZFUITransform::e_Unavailable;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zffloat, translateX) {
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zffloat, translateY) {
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zffloat, translateZ) {
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, scaleX) {
    if(propertyValue < 0) {
        propertyValue = 0;
    }
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, scaleY) {
    if(propertyValue < 0) {
        propertyValue = 0;
    }
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, scaleZ) {
    if(propertyValue < 0) {
        propertyValue = 0;
    }
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, rotateX) {
    while(propertyValue < 0) {
        propertyValue += 360;
    }
    while(propertyValue >= 360) {
        propertyValue -= 360;
    }
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, rotateY) {
    while(propertyValue < 0) {
        propertyValue += 360;
    }
    while(propertyValue >= 360) {
        propertyValue -= 360;
    }
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, rotateZ) {
    while(propertyValue < 0) {
        propertyValue += 360;
    }
    while(propertyValue >= 360) {
        propertyValue -= 360;
    }
    if(propertyValue != propertyValueOld) {
        d->viewTransformUpdate(this);
    }
}

ZFMETHOD_DEFINE_2(ZFUIView, void, bind
        , ZFMP_IN(ZFObject *, owner)
        , ZFMP_IN(const zfstring &, name)
        ) {
    if(owner) {
        owner->invoke(name, this);
    }
    if(!this->viewId()) {
        this->viewId(name);
    }
}

ZFMETHOD_DEFINE_3(ZFUIView, void, bindEvent
        , ZFMP_IN(const zfstring &, eventName)
        , ZFMP_IN(ZFObject *, owner)
        , ZFMP_IN(const zfstring &, methodName)
        ) {
    zfidentity eventId = ZFIdMapIdForName(eventName);
    if(eventId != zfidentityInvalid()) {
        return this->bindEvent(eventId, owner, methodName);
    }
    eventId = ZFIdMapIdForName(zfstr("%s.E_%s", this->classData()->classNameFull(), eventName));
    if(eventId != zfidentityInvalid()) {
        return this->bindEvent(eventId, owner, methodName);
    }
    ZFCoreArray<const ZFClass *> allParent = this->classData()->parentGetAll();
    for(zfindex i = 0; i < allParent.count(); ++i) {
        eventId = ZFIdMapIdForName(zfstr("%s.E_%s", this->classData()->classNameFull(), eventName));
        if(eventId != zfidentityInvalid()) {
            return this->bindEvent(eventId, owner, methodName);
        }
    }
    ZFCoreLogTrim("no such event \"%s\" for class: %s", eventName, this->classData()->classNameFull());
}
ZFMETHOD_DEFINE_3(ZFUIView, void, bindEvent
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(ZFObject *, owner)
        , ZFMP_IN(const zfstring &, methodName)
        ) {
    if(eventId == zfidentityInvalid()
            || owner == zfnull
            ) {
        return;
    }
    ZFCoreArray<const ZFMethod *> allMethods = owner->classData()->methodForNameGetAll(methodName);
    const ZFMethod *methodToBind = zfnull;
    for(zfindex i = 0; i < allMethods.count(); ++i) {
        const ZFMethod *m = allMethods[i];
        if(m->paramCount() == 0
                || (m->paramCount() == 1 && m->paramTypeIdAt(0) == ZFTypeId_ZFArgs())
                || m->paramCount() == 2
                ) {
            methodToBind = m;
            break;
        }
    }
    if(methodToBind == zfnull) {
        ZFCoreLogTrim("no such method \"%s\" for class: %s, while bindEvent to %s"
                , methodName
                , owner->classData()->classNameFull()
                , ZFIdMapNameForId(eventId)
                );
        return;
    }
    ZFLISTENER_3(wrap
            , zfidentity, eventId
            , zfweak, owner
            , const ZFMethod *, methodToBind
            ) {
        zfauto ret;
        if(!methodToBind->methodInvokeT(ret, zfnull, owner)) {
            ZFCoreLogTrim("failed to invoke method %s::%s(%s) for bindEvent %s"
                    , owner ? owner->classData()->classNameFull().cString() : ZFTOKEN_zfnull
                    , methodToBind->methodName()
                    , methodToBind->paramInfo()
                    , ZFIdMapNameForId(eventId)
                    );
        }
    } ZFLISTENER_END()
    this->observerAdd(eventId, wrap);
}

// ============================================================
// init and dealloc
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewNativeViewCache, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewNativeViewCache) {
    while(!this->nativeViewCache.isEmpty()) {
        ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewDestroy(this->nativeViewCache.removeLastAndGet());
    }
}
ZFCoreArray<void *> nativeViewCache;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewNativeViewCache)

void ZFUIView::objectOnInit(void) {
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIViewPrivate);

    d->measureResult = zflineAlloc(ZFUIViewMeasureResult);
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkNormal) != ZFFrameworkStateAvailable) {
        d->nativeView = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCreate(this);
    }
    else {
        ZFCoreArray<void *> &nativeViewCache = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache;
        if(nativeViewCache.isEmpty()) {
            d->nativeView = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCreate(this);
        }
        else {
            d->nativeView = nativeViewCache.removeLastAndGet();
            ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCacheOnRestore(this, d->nativeView);
        }
    }
}
void ZFUIView::objectOnDealloc(void) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewTransformModified)) {
        ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewTransform) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewTransform);
        if(impl != zfnull) {
            impl->viewTransformReset(this);
        }
    }

    this->nativeImplView(zfnull, zfnull, zffalse);
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkNormal) == ZFFrameworkStateAvailable
            && ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCacheOnSave(d->nativeView)
            && ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache.count() < 100
            ) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache.add(d->nativeView);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewDestroy(d->nativeView);
    }
    d->nativeView = zfnull;

    d->layoutParamUpdate(this, zfnull);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIView::objectOnDeallocPrepare(void) {
    // directly remove all children, better performance
    this->implChildOnRemoveAllForDealloc();

    ZFCoreArray<zfautoT<ZFUIView> > layerNormal;
    ZFCoreArray<zfautoT<ZFUIView> > layerInternalFg;
    ZFCoreArray<zfautoT<ZFUIView> > layerInternalBg;
    ZFCoreArray<zfautoT<ZFUIView> > layerInternalImpl;
    d->layerNormal.swap(layerNormal);
    d->layerInternalFg.swap(layerInternalFg);
    d->layerInternalBg.swap(layerInternalBg);
    d->layerInternalImpl.swap(layerInternalImpl);

    for(zfindex i = layerNormal.count() - 1; i != zfindexMax(); --i) {
        zfautoT<ZFUIView> child = layerNormal[i];
        child->_ZFP_ZFUIView_parentOnUpdate(zfnull, zfnull, v_ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, v_ZFUIViewChildLayer::e_Normal);
        child->viewOnRemoveFromParent(this);
    }
    for(zfindex i = layerInternalFg.count() - 1; i != zfindexMax(); --i) {
        zfautoT<ZFUIView> child = layerInternalFg[i];
        child->_ZFP_ZFUIView_parentOnUpdate(zfnull, zfnull, v_ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, v_ZFUIViewChildLayer::e_InternalFg);
        child->viewOnRemoveFromParent(this);
    }
    for(zfindex i = layerInternalBg.count() - 1; i != zfindexMax(); --i) {
        zfautoT<ZFUIView> child = layerInternalBg[i];
        child->_ZFP_ZFUIView_parentOnUpdate(zfnull, zfnull, v_ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, v_ZFUIViewChildLayer::e_InternalBg);
        child->viewOnRemoveFromParent(this);
    }
    for(zfindex i = layerInternalImpl.count() - 1; i != zfindexMax(); --i) {
        zfautoT<ZFUIView> child = layerInternalImpl[i];
        child->_ZFP_ZFUIView_parentOnUpdate(zfnull, zfnull, v_ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, v_ZFUIViewChildLayer::e_InternalImpl);
        child->viewOnRemoveFromParent(this);
    }

    layerNormal.removeAll();
    layerInternalFg.removeAll();
    layerInternalBg.removeAll();
    layerInternalImpl.removeAll();
    this->viewChildOnUpdate();

    zfsuper::objectOnDeallocPrepare();
}

void ZFUIView::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);

    if(!this->viewId().isEmpty()) {
        zfstringAppend(ret, " [%s]", this->viewId());
    }

    ret += " ";
    ZFUIRectToStringT(ret, this->viewFrame());
    if(this->UIScale() != 1) {
        zfstringAppend(ret, "(UIScale:%s)", this->UIScale());
    }

    if(!this->visible()) {
        ret += " Hidden";
    }

    if(!this->viewUIEnableTree()) {
        ret += " UITreeDisabled";
    }
    else if(!this->viewUIEnable()) {
        ret += " UIDisabled";
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, void *, nativeImplView) {
    return d->nativeImplView;
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, nativeImplViewRequireVirtualIndex) {
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_nativeImplViewRequireVirtualIndex);
}
void ZFUIView::nativeImplView(
        ZF_IN void *nativeImplView
        , ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback
        , ZF_IN zfbool nativeImplViewRequireVirtualIndex
        ) {
    void *nativeImplViewOld = d->nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallbackOld = d->nativeImplViewDeleteCallback;
    d->nativeImplView = nativeImplView;
    d->nativeImplViewDeleteCallback = nativeImplViewDeleteCallback;
    if(nativeImplViewRequireVirtualIndex) {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_nativeImplViewRequireVirtualIndex);
    }
    else {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_nativeImplViewRequireVirtualIndex);
    }
    ZFPROTOCOL_ACCESS(ZFUIView)->nativeImplView(this, nativeImplViewOld, d->nativeImplView, d->layerInternalImpl.count(), nativeImplViewRequireVirtualIndex);
    if(nativeImplViewOld && nativeImplViewDeleteCallbackOld) {
        nativeImplViewDeleteCallbackOld(this, nativeImplViewOld);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIMargin &, nativeImplViewMargin) {
    return d->nativeImplViewMargin;
}
ZFMETHOD_DEFINE_0(ZFUIView, void, nativeImplViewMarginUpdate) {
    ZFUIMargin newValue = this->nativeImplViewMarginCustom();
    this->nativeImplViewMarginImplUpdate(newValue);
    if(d->nativeImplViewMargin != newValue) {
        d->nativeImplViewMargin = newValue;
        this->layoutRequest();
        this->nativeImplViewMarginOnUpdate();
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIRect &, nativeImplViewFrame) {
    return d->nativeImplViewFrame;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, ZFUIMargin, nativeImplViewMarginCustom) {
    if(propertyValue != propertyValueOld) {
        this->nativeImplViewMarginUpdate();
    }
}

// ============================================================
void ZFUIView::implChildOnAdd(
        ZF_IN ZFUIView *child
        , ZF_IN zfindex virtualIndex
        , ZF_IN ZFUIViewChildLayer childLayer
        , ZF_IN zfindex childLayerIndex
        ) {
    ZFPROTOCOL_ACCESS(ZFUIView)->child(this, child, virtualIndex, childLayer, childLayerIndex);
}
void ZFUIView::implChildOnRemove(
        ZF_IN ZFUIView *child
        , ZF_IN zfindex virtualIndex
        , ZF_IN ZFUIViewChildLayer childLayer
        , ZF_IN zfindex childLayerIndex
        ) {
    ZFPROTOCOL_ACCESS(ZFUIView)->childRemove(this, child, virtualIndex, childLayer, childLayerIndex);
}
void ZFUIView::implChildOnRemoveAllForDealloc(void) {
    ZFPROTOCOL_ACCESS(ZFUIView)->childRemoveAllForDealloc(this);
}

// ============================================================
// parent
void ZFUIView::_ZFP_ZFUIView_parentOnUpdate(
        ZF_IN ZFUIView *parent
        , ZF_IN ZFUILayoutParam *layoutParam
        , ZF_IN ZFUIViewChildLayer viewLayer
        ) {
    if(parent == zfnull) {
        d->parent = zfnull;
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag)) {
            d->viewFramePrev = ZFUIRectZero();
        }
    }
    else {
        d->parent = parent;
    }

    d->layoutParamUpdate(this, layoutParam);
    if(layoutParam == zfnull) {
        this->serializableRefLayoutParam(zfnull);
    }

    d->viewLayer = viewLayer;

    if(parent) {
        d->viewTreeVisibleUpdate(
                this
                , parent->viewTreeInWindow()
                , this->visible() && ZFBitTest(parent->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewTreeVisibleInternal)
                );
    }
    else {
        d->viewTreeVisibleUpdate(this, zffalse, this->visible());
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, void *, nativeView) {
    return d->nativeView;
}
void ZFUIView::_ZFP_ZFUIView_nativeViewNotifyAdd(
        ZF_IN ZFUIView *view
        , ZF_IN void *nativeParentView
        ) {
    ZFCoreAssert(view != zfnull);

    zfRetain(view);
    zffloat UIScaleForImpl = ZFPROTOCOL_ACCESS(ZFUIView)->UIScaleForImpl(nativeParentView != zfnull ? nativeParentView : view->nativeView());
    if(view->d->UIScaleForImpl != UIScaleForImpl) {
        view->d->UIScaleForImpl = UIScaleForImpl;
        _ZFP_ZFUIViewPrivate::UIScaleUpdateRecursively(view);
    }
}
void ZFUIView::_ZFP_ZFUIView_nativeViewNotifyRemove(ZF_IN ZFUIView *view) {
    ZFCoreAssert(view != zfnull);
    zfRelease(view);
}

// ============================================================
// view focus
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, focused) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull) {
        return impl->focused(this);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_DEFINE_1(ZFUIView, void, focusRequest
        , ZFMP_IN(zfbool, focus)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull) {
        if(!focus || this->focusable()) {
            impl->focusRequest(this, focus);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zfanyT<ZFUIView>, focusFind) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl == zfnull) {
        return zfnull;
    }
    else {
        return impl->focusFind(this);
    }
}

// ============================================================
// parent
ZFMETHOD_DEFINE_0(ZFUIView, zfanyT<ZFUIView>, parent) {
    return d->parent;
}

ZFMETHOD_DEFINE_0(ZFUIView, void, removeFromParent) {
    if(this->parent() != zfnull) {
        switch(this->viewLayer()) {
            case v_ZFUIViewChildLayer::e_Normal:
                this->parent()->childRemove(this);
                break;
            case v_ZFUIViewChildLayer::e_InternalImpl:
                this->parent()->internalImplViewRemove(this);
                break;
            case v_ZFUIViewChildLayer::e_InternalBg:
                this->parent()->internalBgViewRemove(this);
                break;
            case v_ZFUIViewChildLayer::e_InternalFg:
                this->parent()->internalFgViewRemove(this);
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
    }
}

// ============================================================
// scale settings
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIView, zffloat, UIScale) {
    if(propertyValue < 0) {
        propertyValue = 0;
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIView, zffloat, UIScale) {
    if(propertyValue != propertyValueOld) {
        _ZFP_ZFUIViewPrivate::UIScaleUpdateRecursively(this);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleInherited) {
    return d->UIScaleInherited;
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleForImpl) {
    return d->UIScaleForImpl;
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleForPixel) {
    return this->UIScale() * this->UIScaleInherited() * ZFPROTOCOL_ACCESS(ZFUIView)->UIScaleForPixel(this->nativeView());
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleFixed) {
    return d->UIScaleFixed;
}
void ZFUIView::UIScaleOnUpdate(void) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_UIScaleOnUpdate)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_UIScaleOnUpdate)
            ) {
        this->observerNotify(ZFUIView::E_UIScaleOnUpdate());
    }
}

// ============================================================
// layout logic
ZFMETHOD_DEFINE_0(ZFUIView, zfautoT<ZFUILayoutParam>, layoutParamCreate) {
    zfautoT<ZFUILayoutParam> layoutParam = this->layoutParamClass()->newInstance();
    if(layoutParam == zfnull || !layoutParam->classData()->classIsTypeOf(ZFUILayoutParam::ClassData())) {
        return zfnull;
    }
    this->layoutParamOnUpdate(layoutParam);
    return layoutParam;
}
const ZFClass *ZFUIView::layoutParamClass(void) {
    return ZFUILayoutParam::ClassData();
}

ZFMETHOD_DEFINE_1(ZFUIView, void, layoutParam
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        ) {
    if(this->parent() != zfnull && layoutParam != zfnull && !layoutParam->classData()->classIsTypeOf(this->layoutParamClass())) {
        zfautoT<ZFUILayoutParam> layoutParamTmp = this->layoutParamCreate();
        layoutParamTmp->styleableCopyFrom(layoutParam);
        d->layoutParamUpdate(this, layoutParamTmp);
    }
    else {
        d->layoutParamUpdate(this, layoutParam);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zfanyT<ZFUILayoutParam>, layoutParam) {
    return d->layoutParam;
}

ZFMETHOD_DEFINE_0(ZFUIView, void, layoutRequest) {
    d->layoutRequest(this, zftrue);
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, layoutRequested) {
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, layouting) {
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
}
ZFMETHOD_DEFINE_2(ZFUIView, const ZFUISize &, layoutMeasure
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(const ZFUISizeParam &, sizeParam)
        ) {
    if(zftrue
            && ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_width)
            && ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_height)
            ) {
        d->measureResult->measuredSize.width = d->viewFrame.width;
        d->measureResult->measuredSize.height = d->viewFrame.height;
        return d->measureResult->measuredSize;
    }

    ZFUISize sizeHintFixed = ZFUILayoutParam::sizeHintMerge(sizeHint, this->viewSizeMax());

    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
            || d->measureResult->sizeHint != sizeHintFixed
            || d->measureResult->sizeParam != sizeParam
            || ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_width)
            || ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_height)
            ) {
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
                && d->measureResult->sizeParam == sizeParam
                && !ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_width)
                && !ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_height)
                ) {
            // if all these cond matchs, result is ensured not changed
            // * prev measure with wrap, and result a size less than size hint
            // * cur measure with a larger size hint than prev
            if(zftrue
                    && (d->measureResult->sizeHint.width == sizeHintFixed.width
                        || (sizeParam.width == v_ZFUISizeType::e_Wrap && (
                                (d->measureResult->sizeHint.width < 0 && (sizeHintFixed.width < 0 || d->measureResult->measuredSize.width <= sizeHintFixed.width))
                                || (d->measureResult->measuredSize.width < d->measureResult->sizeHint.width && (sizeHintFixed.width == -1 || sizeHintFixed.width >= d->measureResult->sizeHint.width))
                                )))
                    && (d->measureResult->sizeHint.height == sizeHintFixed.height
                        || (sizeParam.height == v_ZFUISizeType::e_Wrap && (
                                (d->measureResult->sizeHint.height < 0 && (sizeHintFixed.height < 0 || d->measureResult->measuredSize.height <= sizeHintFixed.height))
                                || (d->measureResult->measuredSize.height < d->measureResult->sizeHint.height && (sizeHintFixed.height == -1 || sizeHintFixed.height >= d->measureResult->sizeHint.height))
                                )))
                                ) {
                return d->measureResult->measuredSize;
            }
        }

        d->measureResult->measuredSize = ZFUISizeInvalid();
        d->measureResult->sizeHint = sizeHintFixed;
        d->measureResult->sizeParam = sizeParam;
        if(sizeParam.width == v_ZFUISizeType::e_Fill && sizeParam.height == v_ZFUISizeType::e_Fill) {
            d->measureResult->sizeHint = sizeHintFixed;
            d->measureResult->measuredSize.width = zfmMax((zffloat)0, d->measureResult->measuredSize.width);
            d->measureResult->measuredSize.height = zfmMax((zffloat)0, d->measureResult->measuredSize.height);
        }
        else {
            this->layoutOnMeasure(d->measureResult->measuredSize, sizeHintFixed, sizeParam);
        }

        this->layoutOnMeasureFinish(d->measureResult->measuredSize, sizeHintFixed, sizeParam);
        if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnMeasure)
                || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnMeasure)
                ) {
            this->observerNotify(ZFUIView::E_ViewLayoutOnMeasure(), d->measureResult);
        }

        ZFUILayoutParam::sizeHintApplyT(d->measureResult->measuredSize, d->measureResult->measuredSize, sizeHintFixed, sizeParam);
        ZFUISizeApplyRangeT(d->measureResult->measuredSize, d->measureResult->measuredSize, this->viewSizeMin(), this->viewSizeMax());

        if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_width)) {
            d->measureResult->measuredSize.width = d->viewFrame.width;
        }
        if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_height)) {
            d->measureResult->measuredSize.height = d->viewFrame.height;
        }
    }
    return d->measureResult->measuredSize;
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUISize &, layoutMeasuredSize) {
    return d->measureResult->measuredSize;
}

ZFMETHOD_DEFINE_0(ZFUIView, ZFUIRect const &, viewFrame) {
    return d->viewFrame;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewFrame
        , ZFMP_IN(ZFUIRect const &, viewFrame)
        ) {
    if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting)
            && (d->parent == zfnull || !ZFBitTest(d->parent->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting))
            ) { // changed by user or animation
        if(d->viewFrame != viewFrame) {
            if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag)) {
                ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag);
                if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_mask)) {
                    ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_mask);
                }
                d->viewFramePrev = d->viewFrame;
            }
            else {
                if(viewFrame == d->viewFramePrev) {
                    // viewFrame restored to prev, considered as user canceled viewFrame override
                    this->viewFrameReset();
                    d->measureResult->sizeHint = ZFUISizeInvalid();
                }
            }
            d->viewFrameUpdate(viewFrame);
            if(d->viewFrame.width != d->viewFramePrev.width
                    || d->viewFrame.height != d->viewFramePrev.height
                    ) {
                // request layout only for the changed view
                d->layoutRequest(this, zffalse);
            }
            d->viewFrameUpdateForImpl(this);
        }
        return;
    }
    // else, changed by parent layout step

    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag)
            && d->viewFrameOverrideFix(this, viewFrame)
            ) { // user has set viewFrame, ignore parent layout logic
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting)) {
            d->layoutAction(this, ZFUIRectGetBounds(d->viewFrame));
        }
        d->viewFrameUpdateForImpl(this);
    }
    else {
        if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
                || d->viewFrame.width != viewFrame.width
                || d->viewFrame.height != viewFrame.height
                ) {
            d->viewFrameUpdate(viewFrame);
            if(d->viewFrame != d->viewFramePrev || ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleOnUpdate)) {
                d->viewFrameUpdateForImpl(this);
            }

            ZFUIRect bounds = ZFUIRectGetBounds(d->viewFrame);

            d->layoutAction(this, bounds);
        }
        else if(d->viewFrame != viewFrame || ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleOnUpdate)) {
            d->viewFrameUpdate(viewFrame);
            d->viewFrameUpdateForImpl(this);
        }
    }
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively);
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleOnUpdate);
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIRect &, viewFramePrev) {
    return d->viewFramePrev;
}

ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewFrameOverrided) {
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag
            | _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_mask
            );
}

ZFMETHOD_DEFINE_0(ZFUIView, void, viewFrameReset) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag
                | _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_mask
                )) {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag
                | _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_mask
                );
        if(this->parent() != zfnull) {
            this->parent()->layoutRequest();
        }
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, zffloat, x) {
    return this->viewFrame().x;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, x
        , ZFMP_IN(zffloat const &, propertyValue)
        ) {
    if(propertyValue != this->viewFrame().x) {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_x);
        ZFUIRect viewFrame = this->viewFrame();
        viewFrame.x = propertyValue;
        this->viewFrame(viewFrame);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, y) {
    return this->viewFrame().y;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, y
        , ZFMP_IN(zffloat const &, propertyValue)
        ) {
    if(propertyValue != this->viewFrame().y) {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_y);
        ZFUIRect viewFrame = this->viewFrame();
        viewFrame.y = propertyValue;
        this->viewFrame(viewFrame);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, width) {
    return this->viewFrame().width;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, width
        , ZFMP_IN(zffloat const &, propertyValue)
        ) {
    if(propertyValue != this->viewFrame().width) {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_width);
        ZFUIRect viewFrame = this->viewFrame();
        viewFrame.width = propertyValue;
        this->viewFrame(viewFrame);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, height) {
    return this->viewFrame().height;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, height
        , ZFMP_IN(zffloat const &, propertyValue)
        ) {
    if(propertyValue != this->viewFrame().height) {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_height);
        ZFUIRect viewFrame = this->viewFrame();
        viewFrame.height = propertyValue;
        this->viewFrame(viewFrame);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, centerX) {
    return ZFUIRectGetCenterX(this->viewFrame());
}
ZFMETHOD_DEFINE_1(ZFUIView, void, centerX
        , ZFMP_IN(zffloat const &, propertyValue)
        ) {
    if(propertyValue != ZFUIRectGetCenterX(this->viewFrame())) {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_centerX);
        ZFUIRect viewFrame = this->viewFrame();
        viewFrame.x = propertyValue - viewFrame.width / 2;
        this->viewFrame(viewFrame);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, centerY) {
    return ZFUIRectGetCenterY(this->viewFrame());
}
ZFMETHOD_DEFINE_1(ZFUIView, void, centerY
        , ZFMP_IN(zffloat const &, propertyValue)
        ) {
    if(propertyValue != ZFUIRectGetCenterY(this->viewFrame())) {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverride_centerY);
        ZFUIRect viewFrame = this->viewFrame();
        viewFrame.y = propertyValue - viewFrame.height / 2;
        this->viewFrame(viewFrame);
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, void, layoutIfNeed) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)) {
        this->_ZFP_ZFUIView_notifyLayoutView(this->viewFrame());
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, ZFUIPoint, layoutChildOffset) {
    ZFUIPoint ret = ZFUIPointZero();
    this->layoutChildOffsetOnUpdate(ret);
    return ret;
}

void ZFUIView::_ZFP_ZFUIView_notifyLayoutView(ZF_IN const ZFUIRect &viewFrame) {
    ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
    this->viewFrame(viewFrame);
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
}

void ZFUIView::layoutOnLayoutRequest(void) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutRequest)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutRequest)
            ) {
        this->observerNotify(ZFUIView::E_ViewLayoutOnLayoutRequest());
    }
}
void ZFUIView::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    for(zfindex i = 0; i < this->childCount(); ++i) {
        ZFUIView *child = this->childAt(i);
        child->viewFrame(
            ZFUILayoutParam::layoutParamApply(
                bounds,
                child,
                child->layoutParam()));
    }
}

// ============================================================
// child views
ZFMETHOD_DEFINE_3(ZFUIView, zfanyT<ZFUIView>, childFindById
        , ZFMP_IN(const zfstring &, viewId)
        , ZFMP_IN_OPT(zfbool, findRecursively, zftrue)
        , ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse)
        ) {
    if(viewId == zfnull || *viewId == '\0') {
        return zfnull;
    }
    ZFLISTENER_1(action
            , const zfstring &, viewId
            ) {
        ZFUIView *child = zfargs.param0();
        if(zfstringIsEqual(child->viewId(), viewId)) {
            zfargs.eventFiltered(zftrue);
            zfargs.result(child);
        }
    } ZFLISTENER_END()
    return this->childForEach(action, findRecursively, includeInternalViews);
}
ZFMETHOD_DEFINE_3(ZFUIView, zfanyT<ZFUIView>, childFindByClass
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN_OPT(zfbool, findRecursively, zftrue)
        , ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse)
        ) {
    if(cls == zfnull) {
        return zfnull;
    }
    ZFLISTENER_1(action
            , const ZFClass *, cls
            ) {
        ZFUIView *child = zfargs.param0();
        if(child->classData() == cls) {
            zfargs.eventFiltered(zftrue);
            zfargs.result(child);
        }
    } ZFLISTENER_END()
    return this->childForEach(action, findRecursively, includeInternalViews);
}
ZFMETHOD_DEFINE_3(ZFUIView, zfauto, childForEach
        , ZFMP_IN(const ZFListener &, impl)
        , ZFMP_IN_OPT(zfbool, findRecursively, zftrue)
        , ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse)
        ) {
    if(!impl) {
        return zfnull;
    }

    ZFArgs zfargs;
    zfargs.sender(this);

    if(!findRecursively) {
        for(zfindex i = 0; i < d->layerNormal.count(); ++i) {
            zfargs.param0(d->layerNormal[i]);
            impl.execute(zfargs);
            if(zfargs.eventFiltered()) {
                return zfargs.result();
            }
        }
        if(includeInternalViews) {
            for(zfindex i = 0; i < d->layerInternalImpl.count(); ++i) {
                zfargs.param0(d->layerInternalImpl[i]);
                impl.execute(zfargs);
                if(zfargs.eventFiltered()) {
                    return zfargs.result();
                }
            }
            for(zfindex i = 0; i < d->layerInternalBg.count(); ++i) {
                zfargs.param0(d->layerInternalBg[i]);
                impl.execute(zfargs);
                if(zfargs.eventFiltered()) {
                    return zfargs.result();
                }
            }
            for(zfindex i = 0; i < d->layerInternalFg.count(); ++i) {
                zfargs.param0(d->layerInternalFg[i]);
                impl.execute(zfargs);
                if(zfargs.eventFiltered()) {
                    return zfargs.result();
                }
            }
        }
        return zfnull;
    }

    ZFCoreQueuePOD<ZFUIView *> toFind;
    toFind.add(this);
    while(!toFind.isEmpty()) {
        ZFUIView *view = toFind.take();
        zfargs.param0(view);
        impl.execute(zfargs);
        if(zfargs.eventFiltered()) {
            return zfargs.result();
        }
        toFind.addFrom(view->childArray());
        if(includeInternalViews) {
            toFind.addFrom(view->d->layerInternalImpl);
            toFind.addFrom(view->d->layerInternalBg);
            toFind.addFrom(view->d->layerInternalFg);
        }
    }
    return zfnull;
}

ZFMETHOD_DEFINE_3(ZFUIView, zfanyT<ZFUILayoutParam>, childWithParam
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        ) {
    return d->child(this, v_ZFUIViewChildLayer::e_Normal, d->layerNormal, view, layoutParam, atIndex);
}

zfanyT<ZFUILayoutParam> ZFUIView::child(
        ZF_IN const zfany &view
        , ZF_IN_OPT zfindex atIndex /* = zfindexMax() */
        ) {
    ZFUIView *tmp = view->classData()->classIsTypeOf(ZFUILayoutParam::ClassData())
        ? zfcast(ZFUILayoutParam *, view)->ownerLayout()
        : zfcast(ZFUIView *, view);
    ZFCoreAssertWithMessageTrim(tmp
            , "invalid view: %s, must be type of ZFUIView or ZFUILayoutParam"
            , view
            );
    return this->childWithParam(tmp, zfnull, atIndex);
}
/* ZFTAG_TRICKS: util for chained call to build view tree */
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIView, ZFUILayoutParam *, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIView, ZFUILayoutParam *, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

ZFMETHOD_DEFINE_1(ZFUIView, void, childRemove
        , ZFMP_IN(ZFUIView *, view)
        ) {
    d->childRemove(this, v_ZFUIViewChildLayer::e_Normal, d->layerNormal, view);
}
ZFMETHOD_DEFINE_1(ZFUIView, void, childRemoveAt
        , ZFMP_IN(zfindex, index)
        ) {
    d->childRemoveAt(this, v_ZFUIViewChildLayer::e_Normal, d->layerNormal, index);
}
ZFMETHOD_DEFINE_0(ZFUIView, void, childRemoveAll) {
    d->childRemoveAll(this, v_ZFUIViewChildLayer::e_Normal, d->layerNormal);
}

ZFMETHOD_DEFINE_2(ZFUIView, void, childMove
        , ZFMP_IN(zfindex, fromIndex)
        , ZFMP_IN(zfindex, toIndexOrIndexMax)
        ) {
    d->childMove(this, v_ZFUIViewChildLayer::e_Normal, d->layerNormal, fromIndex, toIndexOrIndexMax);
}
ZFMETHOD_DEFINE_2(ZFUIView, void, childMove
        , ZFMP_IN(ZFUIView *, child)
        , ZFMP_IN(zfindex, toIndexOrIndexMax)
        ) {
    d->childMove(this, v_ZFUIViewChildLayer::e_Normal, d->layerNormal, child, toIndexOrIndexMax);
}
ZFMETHOD_DEFINE_2(ZFUIView, void, childReplaceAt
        , ZFMP_IN(zfindex, atIndex)
        , ZFMP_IN(ZFUIView *, toReplace)
        ) {
    d->childReplace(this, v_ZFUIViewChildLayer::e_Normal, d->layerNormal, atIndex, toReplace);
}

ZFMETHOD_DEFINE_0(ZFUIView, zfindex, childCount) {
    return d->childCount(d->layerNormal);
}
ZFMETHOD_DEFINE_1(ZFUIView, zfanyT<ZFUIView>, childAt
        , ZFMP_IN(zfindex, index)
        ) {
    return d->childAt(d->layerNormal, index);
}
ZFMETHOD_DEFINE_1(ZFUIView, zfindex, childFind
        , ZFMP_IN(ZFUIView *, view)
        ) {
    return d->childFind(d->layerNormal, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfautoT<ZFUIView> >, childArray) {
    return d->layerNormal;
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIViewChildLayer, viewLayer) {
    return d->viewLayer;
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfautoT<ZFUIView> >, childRawArray) {
    ZFCoreArray<zfautoT<ZFUIView> > ret;
    ret.capacity(
        d->layerInternalBg.count()
        + d->layerInternalBg.count()
        + this->childCount()
        + d->layerInternalFg.count());
    ret.addFrom(d->layerInternalImpl);
    ret.addFrom(d->layerInternalBg);
    ret.addFrom(d->layerNormal);
    ret.addFrom(d->layerInternalFg);
    return ret;
}

// ============================================================
// events
void ZFUIView::viewTreeInWindowOnUpdate(void) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeInWindowOnUpdate)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeInWindowOnUpdate)
            ) {
        this->observerNotify(ZFUIView::E_ViewTreeInWindowOnUpdate());
    }
}
void ZFUIView::viewTreeVisibleOnUpdate(void) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeVisibleOnUpdate)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeVisibleOnUpdate)
            ) {
        this->observerNotify(ZFUIView::E_ViewTreeVisibleOnUpdate());
    }
}
void ZFUIView::viewChildOnUpdate(void) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnUpdate)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnUpdate)
            ) {
        this->observerNotify(ZFUIView::E_ViewChildOnUpdate());
    }
}
void ZFUIView::viewChildOnAdd(
        ZF_IN ZFUIView *child
        , ZF_IN ZFUIViewChildLayer childLayer
        ) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnAdd)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnAdd)
            ) {
        zfobj<v_ZFUIViewChildLayer> t(childLayer);
        this->observerNotify(ZFUIView::E_ViewChildOnAdd(), child, t);
    }
}
void ZFUIView::viewChildOnRemove(
        ZF_IN ZFUIView *child
        , ZF_IN ZFUIViewChildLayer childLayer
        ) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnRemove)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnRemove)
            ) {
        zfobj<v_ZFUIViewChildLayer> t(childLayer);
        this->observerNotify(ZFUIView::E_ViewChildOnRemove(), child, t);
    }
}
void ZFUIView::viewOnAddToParent(ZF_IN ZFUIView *parent) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnAddToParent)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnAddToParent)
            ) {
        this->observerNotify(ZFUIView::E_ViewOnAddToParent(), parent);
    }
}
void ZFUIView::viewOnRemoveFromParent(ZF_IN ZFUIView *parent) {
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnRemoveFromParent)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnRemoveFromParent)
            ) {
        this->observerNotify(ZFUIView::E_ViewOnRemoveFromParent(), parent);
    }
}

// ============================================================
// internal views
ZFMETHOD_DEFINE_3(ZFUIView, zfanyT<ZFUILayoutParam>, internalImplViewAdd
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull)
        , ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue)
        ) {
    return d->child(this, v_ZFUIViewChildLayer::e_InternalImpl, d->layerInternalImpl, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalImplViewRemove
        , ZFMP_IN(ZFUIView *, view)
        ) {
    d->childRemove(this, v_ZFUIViewChildLayer::e_InternalImpl, d->layerInternalImpl, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfautoT<ZFUIView> >, internalImplViewArray) {
    return d->layerInternalImpl;
}

ZFMETHOD_DEFINE_3(ZFUIView, zfanyT<ZFUILayoutParam>, internalBgViewAdd
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull)
        , ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue)
        ) {
    return d->child(this, v_ZFUIViewChildLayer::e_InternalBg, d->layerInternalBg, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalBgViewRemove
        , ZFMP_IN(ZFUIView *, view)
        ) {
    d->childRemove(this, v_ZFUIViewChildLayer::e_InternalBg, d->layerInternalBg, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfautoT<ZFUIView> >, internalBgViewArray) {
    return d->layerInternalBg;
}

ZFMETHOD_DEFINE_3(ZFUIView, zfanyT<ZFUILayoutParam>, internalFgViewAdd
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull)
        , ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue)
        ) {
    return d->child(this, v_ZFUIViewChildLayer::e_InternalFg, d->layerInternalFg, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalFgViewRemove
        , ZFMP_IN(ZFUIView *, view)
        ) {
    d->childRemove(this, v_ZFUIViewChildLayer::e_InternalFg, d->layerInternalFg, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfautoT<ZFUIView> >, internalFgViewArray) {
    return d->layerInternalFg;
}

// ============================================================
// other internal view logic
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagAdd
        , ZFMP_IN(const zfstring &, tag)
        ) {
    if(tag) {
        d->internalViewAutoSerializeTags.insert(tag);
    }
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagRemove
        , ZFMP_IN(const zfstring &, tag)
        ) {
    if(tag) {
        d->internalViewAutoSerializeTags.erase(tag);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, void, internalViewAutoSerializeTagRemoveAll) {
    d->internalViewAutoSerializeTags.clear();
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        ) {
    ret.capacity(ret.count() + d->internalViewAutoSerializeTags.size());
    for(_ZFP_ZFUIViewInternalViewAutoSerializeTagMapType::iterator it = d->internalViewAutoSerializeTags.begin();
            it != d->internalViewAutoSerializeTags.end();
            ++it
            ) {
        ret.add(*it);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfstring>, internalViewAutoSerializeTagGetAll) {
    ZFCoreArray<zfstring> ret;
    this->internalViewAutoSerializeTagGetAllT(ret);
    return ret;
}

void ZFUIView::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    for(zfindex i = 0; i < d->childCount(d->layerInternalImpl); ++i) {
        ZFUIView *child = d->childAt(d->layerInternalImpl, i);
        if(this->internalViewShouldLayout(child)) {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAt(d->layerInternalImpl, i)));
        }
    }
    for(zfindex i = 0; i < d->childCount(d->layerInternalBg); ++i) {
        ZFUIView *child = d->childAt(d->layerInternalBg, i);
        if(this->internalViewShouldLayout(child)) {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAt(d->layerInternalBg, i)));
        }
    }
    for(zfindex i = 0; i < d->childCount(d->layerInternalFg); ++i) {
        ZFUIView *child = d->childAt(d->layerInternalFg, i);
        if(this->internalViewShouldLayout(child)) {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAt(d->layerInternalFg, i)));
        }
    }
}

// ============================================================
// events
ZFMETHOD_DEFINE_1(ZFUIView, void, viewEventSend
        , ZFMP_IN(ZFUIEvent *, event)
        ) {
    if(event == zfnull) {
        return;
    }

    zfblockedRelease(zfRetain(this));

    this->viewEventOnEvent(event);
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnEvent)
            || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnEvent)
            ) {
        this->observerNotify(ZFUIView::E_ViewOnEvent(), event);
    }
}
void ZFUIView::viewEventOnEvent(ZF_IN ZFUIEvent *event) {
    const ZFClass *eventClass = event->classData();
    if(eventClass->classIsTypeOf(ZFUIMouseEvent::ClassData())) {
        ZFUIMouseEvent *mouseEvent = zfcast(ZFUIMouseEvent *, event);
        this->viewEventOnMouseEvent(mouseEvent);
        mouseEvent->eventResolved(zftrue);
    }
    else if(eventClass->classIsTypeOf(ZFUIKeyEvent::ClassData())) {
        ZFUIKeyEvent *eventTmp = zfcast(ZFUIKeyEvent *, event);
        this->viewEventOnKeyEvent(eventTmp);
        ZFUIView *view = this->parent();
        while(view != zfnull && !event->eventResolved()) {
            view->viewEventOnKeyEvent(eventTmp);
            view = view->parent();
        }
    }
    else if(eventClass->classIsTypeOf(ZFUIWheelEvent::ClassData())) {
        ZFUIWheelEvent *eventTmp = zfcast(ZFUIWheelEvent *, event);
        this->viewEventOnWheelEvent(eventTmp);
        ZFUIView *view = this->parent();
        while(view != zfnull && !event->eventResolved()) {
            view->viewEventOnWheelEvent(eventTmp);
            view = view->parent();
        }
    }
}

void ZFUIView::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent) {
    if(this->viewUIEnable()) {
        if(mouseEvent->mouseAction == v_ZFUIMouseAction::e_Up
                && this->focusable()
                && this->focusObtainWhenClick()
                ) {
            this->focusRequest(zftrue);
        }
        mouseEvent->eventResolved(zftrue);
    }
}

void ZFUIView::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent) {
    this->viewEventOnKeyEventResolveFocus(keyEvent);
}
void ZFUIView::viewEventOnKeyEventResolveFocus(ZF_IN ZFUIKeyEvent *keyEvent) {
    ZFUIViewFocusResolveKeyEvent(this, keyEvent);
}

void ZFUIView::viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent) {
    // nothing to do
}

// ============================================================
// override
void ZFUIView::styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    zfsuperI(ZFStyleable)::styleableOnCopyFrom(anotherStyleable);
    ZFUIView *ref = zfcast(zfself *, anotherStyleable);
    if(ref == zfnull || this->childCount() != 0) {
        return;
    }
    for(zfindex i = 0; i < ref->childCount(); ++i) {
        zfautoT<ZFUIView> child = ref->childAt(i)->copy();
        zfautoT<ZFUILayoutParam> childLayoutParam = ref->childAt(i)->layoutParam()->copy();
        this->childWithParam(child, childLayoutParam);
    }
}

void ZFUIView::observerOnAdd(ZF_IN zfidentity eventId) {
    zfsuper::observerOnAdd(eventId);
    if(zffalse) {
    }
    else if(eventId == ZFUIView::E_ViewOnEvent()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnEvent);}
    else if(eventId == ZFUIView::E_ViewLayoutOnMeasure()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnMeasure);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayoutRequest()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutRequest);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayoutPrepare()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutPrepare);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayout()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayout);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayoutFinish()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutFinish);}
    else if(eventId == ZFUIView::E_ViewTreeInWindowOnUpdate()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeInWindowOnUpdate);}
    else if(eventId == ZFUIView::E_ViewTreeVisibleOnUpdate()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeVisibleOnUpdate);}
    else if(eventId == ZFUIView::E_ViewChildOnUpdate()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnUpdate);}
    else if(eventId == ZFUIView::E_ViewChildOnAdd()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnAdd);}
    else if(eventId == ZFUIView::E_ViewChildOnRemove()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnRemove);}
    else if(eventId == ZFUIView::E_ViewOnAddToParent()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnAddToParent);}
    else if(eventId == ZFUIView::E_ViewOnRemoveFromParent()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnRemoveFromParent);}
    else if(eventId == ZFUIView::E_UIScaleOnUpdate()) {ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_UIScaleOnUpdate);}
}
void ZFUIView::observerOnRemove(ZF_IN zfidentity eventId) {
    zfsuper::observerOnRemove(eventId);
    if(zffalse) {
    }
    else if(eventId == ZFUIView::E_ViewOnEvent()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnEvent);}
    else if(eventId == ZFUIView::E_ViewLayoutOnMeasure()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnMeasure);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayoutRequest()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutRequest);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayoutPrepare()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutPrepare);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayout()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayout);}
    else if(eventId == ZFUIView::E_ViewLayoutOnLayoutFinish()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewLayoutOnLayoutFinish);}
    else if(eventId == ZFUIView::E_ViewTreeInWindowOnUpdate()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeInWindowOnUpdate);}
    else if(eventId == ZFUIView::E_ViewTreeVisibleOnUpdate()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewTreeVisibleOnUpdate);}
    else if(eventId == ZFUIView::E_ViewChildOnUpdate()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnUpdate);}
    else if(eventId == ZFUIView::E_ViewChildOnAdd()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnAdd);}
    else if(eventId == ZFUIView::E_ViewChildOnRemove()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewChildOnRemove);}
    else if(eventId == ZFUIView::E_ViewOnAddToParent()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnAddToParent);}
    else if(eventId == ZFUIView::E_ViewOnRemoveFromParent()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_ViewOnRemoveFromParent);}
    else if(eventId == ZFUIView::E_UIScaleOnUpdate()) {ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_UIScaleOnUpdate);}
}

ZF_NAMESPACE_GLOBAL_END

