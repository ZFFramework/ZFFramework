#include "ZFUIOnScreenKeyboardAutoFit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// #define _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG 1

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout)

// ============================================================
// _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate
zfclassNotPOD _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate {
public:
    ZFUIOnScreenKeyboardAutoFitLayout *pimplOwner;
    zfbool scrollEnableFlag;
    ZFUIView *autoFitFocusedView;
    ZFUIMargin autoFitMargin;
    ZFListener onScreenKeyboardStateOnUpdateListener;
    ZFListener focusOnUpdateListener;
    zfbool autoFitFocusedViewNeedUpdate;

public:
    void autoFitEnable(ZF_IN zfbool value) {
        if(!this->onScreenKeyboardStateOnUpdateListener) {
            ZFLISTENER_1(onScreenKeyboardStateOnUpdate
                    , ZFUIOnScreenKeyboardAutoFitLayout *, pimplOwner
                    ) {
                _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::onScreenKeyboardStateOnUpdate(zfargs, pimplOwner);
            } ZFLISTENER_END()
            this->onScreenKeyboardStateOnUpdateListener = onScreenKeyboardStateOnUpdate;
        }

        if(value) {
            ZFGlobalObserver().observerAdd(
                ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(),
                this->onScreenKeyboardStateOnUpdateListener);

            this->pimplOwner->layoutRequest();
        }
        else {
            this->scrollEnableFlag = zffalse;
            this->pimplOwner->scrollEnable(this->scrollEnableFlag && this->pimplOwner->autoFitScrollEnable());
            ZFGlobalObserver().observerRemove(
                ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(),
                this->onScreenKeyboardStateOnUpdateListener);

            this->pimplOwner->scrollContentFrame(ZFUIRectGetBounds(this->pimplOwner->viewFrame()));
            if(this->autoFitMargin == ZFUIMarginZero() || this->pimplOwner->layoutParam() == zfnull) {
                return;
            }
            this->autoFitMargin = ZFUIMarginZero();
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                ZFLogTrim() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin reset to " << this->autoFitMargin;
            #endif
            this->pimplOwner->layoutParam()->margin(this->autoFitMargin);
        }
    }
    void autoFitFocusedViewToVisible(ZF_IN zfbool value) {
        if(!this->focusOnUpdateListener) {
            ZFLISTENER_1(focusOnUpdate
                    , ZFUIOnScreenKeyboardAutoFitLayout *, pimplOwner
                    ) {
                _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::focusOnUpdate(zfargs, pimplOwner);
            } ZFLISTENER_END()
            this->focusOnUpdateListener = focusOnUpdate;
        }

        if(value) {
            ZFGlobalObserver().observerAdd(
                ZFUIView::E_ViewFocusOnUpdate(),
                this->focusOnUpdateListener);
        }
        else {
            ZFGlobalObserver().observerRemove(
                ZFUIView::E_ViewFocusOnUpdate(),
                this->focusOnUpdateListener);
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                ZFLogTrim() << "[ZFUIOnScreenKeyboardAutoFitLayout] focused view changed to " << (void *)zfnull;
            #endif
            this->autoFitFocusedView = zfnull;
        }
    }

public:
    void autoFitUpdateFrame(void) {
        if(this->pimplOwner->layoutParam() == zfnull) {
            return;
        }

        ZFUIRect orgRect = ZFUIViewPositionOnScreen(this->pimplOwner);
        if(orgRect.width != this->pimplOwner->viewFrame().width
                || orgRect.height != this->pimplOwner->viewFrame().height
                ) {
            return;
        }
        orgRect.x -= this->autoFitMargin.left;
        orgRect.width += this->autoFitMargin.left + this->autoFitMargin.right;
        orgRect.y -= this->autoFitMargin.top;
        orgRect.height += this->autoFitMargin.top + this->autoFitMargin.bottom;

        this->autoFitMarginCalc(this->autoFitMargin, orgRect, ZFUIOnScreenKeyboardState::instanceForView(this->pimplOwner));
        #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
            ZFLogTrim() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin changed to " << this->autoFitMargin;
        #endif
        this->pimplOwner->layoutParam()->margin(this->autoFitMargin);
    }
    void autoFitMarginCalc(
            ZF_OUT ZFUIMargin &ret
            , ZF_IN const ZFUIRect &orgRect
            , ZF_IN ZFUIOnScreenKeyboardState *keyboardState
            ) {
        ret = ZFUIMarginZero();
        if(keyboardState->keyboardShowing()) {
            ZFUIRect clientFrame = keyboardState->keyboardFixClientFrame();
            if(ZFUIRectGetLeft(clientFrame) > ZFUIRectGetLeft(orgRect)) {
                ret.left += ZFUIRectGetLeft(clientFrame) - ZFUIRectGetLeft(orgRect);
            }
            if(ZFUIRectGetTop(clientFrame) > ZFUIRectGetTop(orgRect)) {
                ret.top += ZFUIRectGetTop(clientFrame) - ZFUIRectGetTop(orgRect);
            }
            if(ZFUIRectGetRight(clientFrame) < ZFUIRectGetRight(orgRect)) {
                ret.right += ZFUIRectGetRight(orgRect) - ZFUIRectGetRight(clientFrame);
            }
            if(ZFUIRectGetBottom(clientFrame) < ZFUIRectGetBottom(orgRect)) {
                ret.bottom += ZFUIRectGetBottom(orgRect) - ZFUIRectGetBottom(clientFrame);
            }
        }
    }

public:
    static void onScreenKeyboardStateOnUpdate(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIOnScreenKeyboardAutoFitLayout *layout
            ) {
        ZFUIOnScreenKeyboardState *keyboardState = zfargs.sender();
        if(keyboardState->keyboardShowing()) {
            if(layout->autoFitFocusedView() != zfnull) {
                layout->d->autoFitFocusedViewNeedUpdate = zftrue;
            }
            layout->d->scrollEnableFlag = zftrue;
            layout->scrollEnable(layout->d->scrollEnableFlag && layout->autoFitScrollEnable());
            layout->layoutRequest();
        }
        else {
            layout->d->scrollEnableFlag = zffalse;
            layout->scrollEnable(layout->d->scrollEnableFlag && layout->autoFitScrollEnable());
            layout->d->autoFitMargin = ZFUIMarginZero();
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                ZFLogTrim() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin reset to " << layout->d->autoFitMargin;
            #endif
            layout->layoutParam()->margin(layout->d->autoFitMargin);
            layout->scrollContentFrame(ZFUIRectGetBounds(layout->viewFrame()));
        }
    }
    static void focusOnUpdate(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIOnScreenKeyboardAutoFitLayout *layout
            ) {
        ZFUIView *view = zfargs.sender();
        if(!view->focused()) {
            if(view == layout->d->autoFitFocusedView) {
                layout->d->autoFitFocusedView = zfnull;
            }
            return;
        }

        {
            ZFUIView *parent = view;
            while(parent != zfnull && parent != layout) {
                parent = parent->parent();
            }
            if(parent == zfnull) {
                return;
            }
        }

        #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
            ZFLogTrim() << "[ZFUIOnScreenKeyboardAutoFitLayout] focused view changed to " << view;
        #endif
        layout->d->autoFitFocusedView = view;
        layout->d->autoFitFocusedViewNeedUpdate = zftrue;
        layout->layoutRequest();
    }

public:
    _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate(void)
    : pimplOwner(zfnull)
    , scrollEnableFlag(zffalse)
    , autoFitFocusedView(zfnull)
    , autoFitMargin(ZFUIMarginZero())
    , onScreenKeyboardStateOnUpdateListener()
    , focusOnUpdateListener()
    , autoFitFocusedViewNeedUpdate(zffalse)
    {
    }
};

// ============================================================
// ZFUIOnScreenKeyboardAutoFitLayout
ZFOBJECT_REGISTER(ZFUIOnScreenKeyboardAutoFitLayout)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitEnable) {
    if(propertyValue != propertyValueOld) {
        d->autoFitEnable(this->autoFitEnable());
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitFocusedViewToVisible) {
    if(propertyValue != propertyValueOld) {
        d->autoFitFocusedViewToVisible(this->autoFitFocusedViewToVisible());
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitScrollEnable) {
    if(propertyValue != propertyValueOld) {
        this->scrollEnable(d->scrollEnableFlag && this->autoFitScrollEnable());
    }
}

ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardAutoFitLayout, zfanyT<ZFUIView>, autoFitFocusedView) {
    return d->autoFitFocusedView;
}

void ZFUIOnScreenKeyboardAutoFitLayout::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate);
    d->pimplOwner = this;
    d->scrollEnableFlag = zffalse;
    this->scrollEnable(d->scrollEnableFlag && this->autoFitScrollEnable());
}
void ZFUIOnScreenKeyboardAutoFitLayout::objectOnDealloc(void) {
    d->autoFitEnable(zffalse);
    d->autoFitFocusedViewToVisible(zffalse);

    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIOnScreenKeyboardAutoFitLayout::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();
    d->autoFitEnable(this->autoFitEnable());
    d->autoFitFocusedViewToVisible(this->autoFitFocusedViewToVisible());
}

ZFSerializablePropertyType ZFUIOnScreenKeyboardAutoFitLayout::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property) {
    if(property == ZFPropertyAccess(ZFUIScrollView, scrollContentFrame)
            || property == ZFPropertyAccess(ZFUIScrollView, scrollEnable)
            ) {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    else {
        return zfsuperI(ZFSerializable)::serializableOnCheckPropertyType(property);
    }
}

void ZFUIOnScreenKeyboardAutoFitLayout::layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            ) {
    ret = ZFUISizeZero();
    ZFUISize sizeHintTmp = ZFUILayoutParam::sizeHintOffset(sizeHint,
        ZFUISizeCreate(ZFUIMarginGetWidth(d->autoFitMargin), ZFUIMarginGetHeight(d->autoFitMargin)));
    for(zfindex i = 0; i < this->childCount(); ++i) {
        ZFUIView *child = this->childAt(i);
        child->layoutMeasure(sizeHintTmp, child->layoutParam()->sizeParam());
        ret.width = zfmMax<zffloat>(ret.width, child->layoutMeasuredSize().width + ZFUIMarginGetWidth(child->layoutParam()->margin()));
        ret.height = zfmMax<zffloat>(ret.height, child->layoutMeasuredSize().height + ZFUIMarginGetWidth(child->layoutParam()->margin()));
    }
}
void ZFUIOnScreenKeyboardAutoFitLayout::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds) {
    zfsuper::layoutOnLayoutPrepare(bounds);
    if(!this->autoFitEnable()
            || (this->viewFrame().width == 0 && this->viewFrame().height == 0)
            || this->viewFrame().width != bounds.width || this->viewFrame().height != bounds.height
            || !d->scrollEnableFlag
            ) {
        this->scrollContentFrame(bounds);
    }
    else {
        ZFUIRect scrollContentFrame = this->scrollContentFrame();
        scrollContentFrame.width = bounds.width + ZFUIMarginGetWidth(d->autoFitMargin);
        scrollContentFrame.height = bounds.height + ZFUIMarginGetHeight(d->autoFitMargin);
        this->scrollContentFrameUpdate(scrollContentFrame);
        d->autoFitUpdateFrame();
        if(d->autoFitFocusedViewNeedUpdate) {
            d->autoFitFocusedViewNeedUpdate = zffalse;

            ZFUIOnScreenKeyboardAutoFitLayout *owner = this;
            ZFLISTENER_1(scrollFocusedViewToVisibleDelay
                    , ZFUIOnScreenKeyboardAutoFitLayout *, owner
                    ) {
                if(owner->autoFitFocusedView() != zfnull) {
                    #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                        ZFLogTrim() << "[ZFUIOnScreenKeyboardAutoFitLayout] scrollFocusedViewToVisible " << owner->autoFitFocusedView();
                    #endif
                    owner->scrollChildToVisible(owner->autoFitFocusedView());
                }
            } ZFLISTENER_END()
            zfpost(scrollFocusedViewToVisibleDelay);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

