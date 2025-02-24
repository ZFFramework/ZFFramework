#include "ZFUIDialog.h"
#include "ZFUIButton.h"
#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFEXPORT_VAR_DEFINE(ZFListener, ZFUIDialogDefaultLayoutParamCreator, zfnull)
ZFEXPORT_VAR_DEFINE(ZFListener, ZFUIDialogDefaultAniShowCreator, zfnull)
ZFEXPORT_VAR_DEFINE(ZFListener, ZFUIDialogDefaultAniHideCreator, zfnull)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogDefaultConfig, ZFLevelZFFrameworkHigh) {
    {
        ZFLISTENER(impl) {
            zfobj<ZFUILayoutParam> lp;
            lp->align(ZFUIAlign::e_Center);
            lp->margin(ZFUIMarginCreate(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
            zfargs.result(lp);
        } ZFLISTENER_END()
        ZFUIDialogDefaultLayoutParamCreator(impl);
    }
    {
        ZFLISTENER(impl) {
            zfargs.result(ZFAni("alpha", zfobj<v_zffloat>(0), zfobj<v_zffloat>(1)));
        } ZFLISTENER_END()
        ZFUIDialogDefaultAniShowCreator(impl);
    }
    {
        ZFLISTENER(impl) {
            zfargs.result(ZFAni("alpha", zfobj<v_zffloat>(1), zfobj<v_zffloat>(0)));
        } ZFLISTENER_END()
        ZFUIDialogDefaultAniHideCreator(impl);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogDefaultConfig)

// ============================================================
// ZFUIDialog
ZFSTYLE_DEFAULT_DEFINE(ZFUIDialog)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAllDialog, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAllDialog) {
    ZFCoreArray<ZFUIDialog *> tmp;
    tmp.copyFrom(this->allDialog);
    for(zfindex i = 0; i < tmp.count(); ++i) {
        if(tmp[i]->showing()) {
            tmp[i]->hideImmediately();
        }
    }
}
public:
    ZFCoreArray<ZFUIDialog *> allDialog;
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAllDialog)
#define _ZFP_ZFUIDialogAllDialog ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogAllDialog)->allDialog

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAutoHide, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAutoHide) {
    ZFCoreArray<ZFUIDialog *> tmp;
    tmp.copyFrom(_ZFP_ZFUIDialogAllDialog);
    for(zfindex i = 0; i < tmp.count(); ++i) {
        if(tmp[i]->showing()) {
            tmp[i]->hideImmediately();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAutoHide)

// ============================================================
// _ZFP_I_ZFUIDialogPrivate
/*
 * memo:
 *
 * _ZFP_I_ZFUIDialogPrivate (ZFUIWindow, handle all key event)
 * ^ dialogWindowBg (internal bg, window ani target)
 *   dialogClickMask (internal bg, focused if dialogContainer has no focusable child)
 *   _ZFP_ZFUIDialogContentHolder (Ui disable, supply layout logic)
 *   ^ dialogBg
 *     ^ dialogContainer (dialogContainer)
 *       ^ content
 *         subclass impl views
 */
zfclass _ZFP_I_ZFUIDialogPrivate : zfextend ZFUIWindow {
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIDialogPrivate, ZFUIWindow)

public:
    ZFUIDialog *pimplOwner;
    ZFUIView *dialogWindowBg;
    ZFAnimation *dialogWindowAniShow;
    ZFAnimation *dialogWindowAniHide;
    ZFUIButton *dialogClickMask;
    ZFUIImageView *dialogBg;
    ZFUIOnScreenKeyboardAutoFitLayout *dialogContainer;
    ZFListener aniShowOnStopListener;
    ZFListener aniHideOnStopListener;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->windowLevel(ZFUIWindowLevel::e_AppHigh);
        this->sysWindowMarginShouldApply(zffalse);
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
        if(this->pimplOwner->sysWindowMarginShouldApply()) {
            zfsuper::layoutOnLayout(ZFUIRectApplyMargin(bounds, this->ownerSysWindow()->sysWindowMargin()));
        }
        else {
            zfsuper::layoutOnLayout(bounds);
        }
    }

public:
    void stop(void) {
        if(this->dialogWindowAniShow != zfnull) {
            this->dialogWindowAniShow->stop();
        }
        if(this->dialogWindowAniHide != zfnull) {
            this->dialogWindowAniHide->stop();
        }
        if(this->pimplOwner->aniShow() != zfnull) {
            this->pimplOwner->aniShow()->stop();
        }
        if(this->pimplOwner->aniHide() != zfnull) {
            this->pimplOwner->aniHide()->stop();
        }
    }

public:
    ZFMETHOD_INLINE_1(void, dialogClickMaskOnClick
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        if(this->pimplOwner->hideWhenTouchOutside()) {
            this->pimplOwner->hide();
        }
    }
    ZFMETHOD_INLINE_1(void, aniShowOnStop
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        if(!this->dialogWindowAniShow->started()
                && !this->dialogWindowAniHide->started()
                && (this->pimplOwner->aniShow() == zfnull || !this->pimplOwner->aniShow()->started())
                && (this->pimplOwner->aniHide() == zfnull || !this->pimplOwner->aniHide()->started())
                ) {
            this->viewUIEnableTree(zftrue);
            if(this->pimplOwner->autoFocus()) {
                zfanyT<ZFUIView> v = this->pimplOwner->dialogFocusOnUpdate();
                if(this->pimplOwner->observerHasAdd(ZFUIDialog::E_DialogFocusOnUpdate())) {
                    this->pimplOwner->observerNotify(ZFUIDialog::E_DialogFocusOnUpdate(), zfobj<ZFObjectHolder>(v));
                }
                if(v != zfnull) {
                    v->focusRequest(zftrue);
                }
                else {
                    this->dialogClickMask->focusRequest(zftrue);
                }
            }
            else {
                this->dialogClickMask->focusRequest(zftrue);
            }
            this->pimplOwner->dialogAfterShow();
        }
    }
    ZFMETHOD_INLINE_1(void, aniHideOnStop
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        if(!this->dialogWindowAniShow->started()
                && !this->dialogWindowAniHide->started()
                && (this->pimplOwner->aniShow() == zfnull || !this->pimplOwner->aniShow()->started())
                && (this->pimplOwner->aniHide() == zfnull || !this->pimplOwner->aniHide()->started())
                ) {
            this->viewUIEnableTree(zftrue);
            this->hide();
            this->pimplOwner->dialogAfterHide();
            zfRelease(this->pimplOwner);
        }
    }

public:
    void layoutDialogPrepare(ZF_IN const ZFUIRect &bounds) {
        if(this->pimplOwner->bgImage() != zfnull) {
            this->dialogContainer->layoutParam()->margin(this->pimplOwner->bgImage()->imageNinePatch());
        }
    }
    void layoutDialog(ZF_IN const ZFUIRect &bounds) {
        ZFUIMargin margin;
        ZFUISizeParam sizeParam;
        ZFUIAlignFlags align;
        if(this->pimplOwner->layoutParam()) {
            margin = this->pimplOwner->layoutParam()->margin();
            sizeParam = this->pimplOwner->layoutParam()->sizeParam();
            align = this->pimplOwner->layoutParam()->align();
        }
        else {
            margin = ZFUIMarginCreate(ZFUIGlobalStyle::DefaultStyle()->itemMargin());
            sizeParam = ZFUISizeParamWrapWrap();
            align = ZFUIAlign::e_Center;
        }
        const ZFUIMargin &contentMargin = this->dialogContainer->layoutParam()->margin();

        ZFUIMargin totalMargin = ZFUIMarginInc(margin, contentMargin);

        this->dialogContainer->layoutMeasure(
            ZFUISizeApplyMargin(ZFUIRectGetSize(bounds), totalMargin),
            sizeParam);
        ZFUISize dialogContainerSize = this->dialogContainer->layoutMeasuredSize();

        ZFUISize dialogBgSize = dialogContainerSize;
        dialogBgSize.width += ZFUIMarginGetWidth(contentMargin);
        dialogBgSize.height += ZFUIMarginGetHeight(contentMargin);
        this->dialogBg->viewFrame(ZFUIAlignApply(
                    align,
                    ZFUIRectApplyMargin(bounds, margin),
                    dialogBgSize
                    ));
    }

    // event handle
protected:
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent) {
        // dialog would always resolve key event
        keyEvent->eventResolved(zftrue);

        if(!this->pimplOwner->hideWhenClickBack()
                || keyEvent->keyAction != ZFUIKeyAction::e_Up
                ) {
            return;
        }

        switch(keyEvent->keyCode) {
            case ZFUIKeyCode::e_kPhoneBack:
            case ZFUIKeyCode::e_kEsc:
                this->pimplOwner->hide();
                break;
            default:
                break;
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFUIDialogPrivate)

zfclass _ZFP_ZFUIDialogContentHolder : zfextend ZFUIView {
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogContentHolder, ZFUIView)

public:
    _ZFP_I_ZFUIDialogPrivate *pimplOwner;

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds) {
        this->pimplOwner->layoutDialogPrepare(bounds);
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
        this->pimplOwner->layoutDialog(bounds);
    }
};

zfclass _ZFP_I_ZFUIDialog_DialogClickMask : zfextend ZFUIButton {
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIDialog_DialogClickMask, ZFUIButton)
};

// ============================================================
// ZFUIDialog
ZFOBJECT_REGISTER(ZFUIDialog)

ZFEVENT_REGISTER(ZFUIDialog, DialogBeforeShow)
ZFEVENT_REGISTER(ZFUIDialog, DialogAfterShow)
ZFEVENT_REGISTER(ZFUIDialog, DialogBeforeHide)
ZFEVENT_REGISTER(ZFUIDialog, DialogAfterHide)
ZFEVENT_REGISTER(ZFUIDialog, DialogFocusOnUpdate)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, ZFUIColor, dialogWindowColor) {
    d->dialogWindowBg->bgColor(this->dialogWindowColor());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, zfanyT<ZFUIImage>, bgImage) {
    d->dialogBg->image(this->bgImage());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, zfanyT<ZFUIView>, content) {
    if(this->content() != zfnull) {
        d->dialogContainer->child(this->content())->c_alignCenter();
    }
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIDialog, zfanyT<ZFUIView>, content) {
    if(this->content() != zfnull) {
        d->dialogContainer->childRemove(this->content());
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, zfbool, dialogWindowAutoResize) {
    if(this->dialogWindowAutoResize()) {
        this->dialogWindowAutoFit(zffalse);
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, zfbool, dialogWindowAutoFit) {
    if(this->dialogWindowAutoFit()) {
        this->dialogWindowAutoResize(zffalse);
    }
    this->dialogWindowAutoFitLayout()->autoFitEnable(this->dialogWindowAutoFit());
}

ZFMETHOD_DEFINE_0(ZFUIDialog, ZFUIOnScreenKeyboardAutoFitLayout *, dialogWindowAutoFitLayout) {
    return d->dialogContainer;
}

ZFMETHOD_DEFINE_0(ZFUIDialog, zfbool, showing) {
    return d->showing();
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, show) {
    if(d->showing()) {
        return;
    }
    zfRetain(this);
    d->stop();
    d->show();

    if(this->dialogWindowAutoResize()) {
        ZFUIOnScreenKeyboardAutoResizeStart(this->dialogWindow());
    }

    d->viewUIEnableTree(zffalse);
    this->dialogBeforeShow();
    if(this->dialogWindowColor() != ZFUIColorZero() && this->dialogWindowAutoDim()) {
        d->dialogWindowAniShow->target(d->dialogWindowBg);
        d->dialogWindowAniShow->observerAddForOnce(
            ZFAnimation::E_AniOnStop(),
            d->aniShowOnStopListener);
        d->dialogWindowAniShow->start();
    }
    if(this->aniShow() != zfnull) {
        this->aniShow()->target(d->dialogBg);
        this->aniShow()->observerAddForOnce(
            ZFAnimation::E_AniOnStop(),
            d->aniShowOnStopListener);
        this->aniShow()->start();
    }
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, hide) {
    if(!d->showing()) {
        return;
    }
    d->stop();
    if(!d->showing()) {
        return;
    }

    if(this->dialogWindowAutoResize()) {
        ZFUIOnScreenKeyboardAutoResizeStop(this->dialogWindow());
    }

    d->viewUIEnableTree(zffalse);
    this->dialogBeforeHide();
    if(this->dialogWindowColor() != ZFUIColorZero() && this->dialogWindowAutoDim()) {
        d->dialogWindowAniHide->target(d->dialogWindowBg);
        d->dialogWindowAniHide->observerAddForOnce(
            ZFAnimation::E_AniOnStop(),
            d->aniHideOnStopListener);
        d->dialogWindowAniHide->start();
    }
    if(this->aniHide() != zfnull) {
        this->aniHide()->target(d->dialogBg);
        this->aniHide()->observerAddForOnce(
            ZFAnimation::E_AniOnStop(),
            d->aniHideOnStopListener);
        this->aniHide()->start();
    }
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, hideImmediately) {
    if(d->showing()) {
        zfRetain(this);
        d->stop();
        if(d->showing()) {
            this->dialogBeforeHide();
            d->hide();
            this->dialogAfterHide();
        }
        zfRelease(this);
    }
}

ZFMETHOD_DEFINE_1(ZFUIDialog, void, autoHideForButton
        , ZFMP_IN(ZFUIButton *, button)
        ) {
    if(button == zfnull) {
        return;
    }
    ZFUIDialog *owner = this;
    ZFLISTENER_1(buttonOnClick
            , ZFUIDialog *, owner
            ) {
        owner->hide();
    } ZFLISTENER_END()
    button->observerAdd(
            ZFUIButton::E_ButtonOnClick(),
            buttonOnClick,
            ZFLevelZFFrameworkPostNormal
        );
}

ZFMETHOD_DEFINE_0(ZFUIDialog, ZFUIWindow *, dialogWindow) {
    return d;
}

ZFMETHOD_DEFINE_0(ZFUIDialog, ZFUIView *, dialogContainer) {
    return d->dialogContainer;
}
ZFMETHOD_DEFINE_0(ZFUIDialog, ZFUIView *, dialogBackgroundContainer) {
    return d->dialogBg;
}

zfanyT<ZFUIView> ZFUIDialog::dialogFocusOnUpdate(void) {
    return ZFUIViewFocusNextMove(this->dialogContainer(),
        ZFUIViewFocusNextParam().focusDirection(ZFUIOrientation::e_Left | ZFUIOrientation::e_Top));
}

void ZFUIDialog::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_I_ZFUIDialogPrivate);
    d->pimplOwner = this;

    d->dialogWindowBg = zfAlloc(ZFUIView);
    d->internalBgViewAdd(d->dialogWindowBg)->c_sizeFill();

    d->dialogClickMask = zfAlloc(_ZFP_I_ZFUIDialog_DialogClickMask);
    d->internalBgViewAdd(d->dialogClickMask)->c_sizeFill();
    d->dialogClickMask->observerAdd(ZFUIButton::E_ButtonOnClick(),
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, dialogClickMaskOnClick)));

    zfobj<_ZFP_ZFUIDialogContentHolder> dialogContentHolder;
    d->child(dialogContentHolder)->c_sizeFill();
    dialogContentHolder->pimplOwner = d;
    dialogContentHolder->viewUIEnable(zffalse);

    d->dialogBg = zfAlloc(ZFUIImageView);
    dialogContentHolder->child(d->dialogBg);
    d->dialogBg->viewUIEnableTree(zftrue);
    d->dialogBg->viewUIEnable(zftrue);

    d->dialogContainer = zfAlloc(ZFUIOnScreenKeyboardAutoFitLayout);
    d->dialogBg->child(d->dialogContainer)->c_sizeFill();

    d->dialogWindowAniShow = zfRetain(ZFAni("alpha", zfobj<v_zffloat>(0), zfobj<v_zffloat>(1)));
    d->dialogWindowAniHide = zfRetain(ZFAni("alpha", zfobj<v_zffloat>(1), zfobj<v_zffloat>(0)));

    d->aniShowOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, aniShowOnStop));
    d->aniHideOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, aniHideOnStop));

    d->dialogWindowBg->bgColor(this->dialogWindowColor());
    d->dialogBg->image(this->bgImage());

    _ZFP_ZFUIDialogAllDialog.add(this);
}
void ZFUIDialog::objectOnDealloc(void) {
    _ZFP_ZFUIDialogAllDialog.removeElement(this);

    zfRetainChange(d->dialogWindowAniShow, zfnull);
    zfRetainChange(d->dialogWindowAniHide, zfnull);

    zfRelease(d->dialogContainer);
    zfRelease(d->dialogBg);
    zfRelease(d->dialogClickMask);
    zfRelease(d->dialogWindowBg);
    zfRelease(d);
    d = zfnull;

    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

