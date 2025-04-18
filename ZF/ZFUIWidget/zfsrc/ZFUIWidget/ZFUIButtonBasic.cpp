#include "ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFUIButtonBasicPrivate
zfclassNotPOD _ZFP_ZFUIButtonBasicPrivate {
public:
    ZFUIButtonBasic *pimplOwner;
    ZFUITextView *labelView;
    ZFUIImageView *iconView;
    ZFUIImageView *bgView;

    ZFListener labelNormalOnUpdateListener;
    ZFListener labelHighlightedOnUpdateListener;
    ZFListener labelCheckedOnUpdateListener;
    ZFListener labelCheckedHighlightedOnUpdateListener;
    ZFListener labelDisabledOnUpdateListener;
    ZFListener iconNormalOnUpdateListener;
    ZFListener iconHighlightedOnUpdateListener;
    ZFListener iconCheckedOnUpdateListener;
    ZFListener iconCheckedHighlightedOnUpdateListener;
    ZFListener iconDisabledOnUpdateListener;
    ZFListener bgNormalOnUpdateListener;
    ZFListener bgHighlightedOnUpdateListener;
    ZFListener bgCheckedOnUpdateListener;
    ZFListener bgCheckedHighlightedOnUpdateListener;
    ZFListener bgDisabledOnUpdateListener;

public:
    _ZFP_ZFUIButtonBasicPrivate(void)
    : pimplOwner(zfnull)
    , labelView(zfnull)
    , iconView(zfnull)
    , bgView(zfnull)

    , labelNormalOnUpdateListener()
    , labelHighlightedOnUpdateListener()
    , labelCheckedOnUpdateListener()
    , labelCheckedHighlightedOnUpdateListener()
    , labelDisabledOnUpdateListener()
    , iconNormalOnUpdateListener()
    , iconHighlightedOnUpdateListener()
    , iconCheckedOnUpdateListener()
    , iconCheckedHighlightedOnUpdateListener()
    , iconDisabledOnUpdateListener()
    , bgNormalOnUpdateListener()
    , bgHighlightedOnUpdateListener()
    , bgCheckedOnUpdateListener()
    , bgCheckedHighlightedOnUpdateListener()
    , bgDisabledOnUpdateListener()
    {
    }

public:
    void labelViewPrepare(void) {
        if(this->labelView == zfnull) {
            this->labelView = zfRetain(this->pimplOwner->labelViewClass()->newInstance());
            ZFCoreAssert(this->labelView != zfnull);
            this->pimplOwner->internalBgViewAdd(this->labelView);
        }
    }
    void iconViewPrepare(void) {
        if(this->iconView == zfnull) {
            this->iconView = zfRetain(this->pimplOwner->iconViewClass()->newInstance());
            ZFCoreAssert(this->iconView != zfnull);
            this->pimplOwner->internalBgViewAdd(this->iconView);

            if(this->labelView != zfnull) {
                this->pimplOwner->internalBgViewRemove(this->labelView);
                this->pimplOwner->internalBgViewAdd(this->labelView);
            }
        }
    }
    void bgViewPrepare(void) {
        if(this->bgView == zfnull) {
            this->bgView = zfRetain(this->pimplOwner->bgViewClass()->newInstance());
            ZFCoreAssert(this->bgView != zfnull);
            this->pimplOwner->internalBgViewAdd(this->bgView);

            if(this->iconView != zfnull) {
                this->pimplOwner->internalBgViewRemove(this->iconView);
                this->pimplOwner->internalBgViewAdd(this->iconView);
            }
            if(this->labelView != zfnull) {
                this->pimplOwner->internalBgViewRemove(this->labelView);
                this->pimplOwner->internalBgViewAdd(this->labelView);
            }
        }
    }

public:
    void labelViewUpdate(void) {
        zfstring value = this->pimplOwner->label(this->pimplOwner->buttonState())->text();
        if(value == zfnull) {
            if(this->pimplOwner->buttonState() == v_ZFUIButtonState::e_Checked) {
                value = this->pimplOwner->labelHighlighted()->text();
            }
            else if(this->pimplOwner->buttonState() == v_ZFUIButtonState::e_CheckedHighlighted) {
                value = this->pimplOwner->labelChecked()->text();
                if(value == zfnull) {
                    value = this->pimplOwner->labelHighlighted()->text();
                }
            }
            if(value == zfnull) {
                value = this->pimplOwner->labelNormal()->text();
            }
        }
        this->labelView->text(value);
    }
    void iconViewUpdate(void) {
        ZFUIImage *value = this->pimplOwner->icon(this->pimplOwner->buttonState())->imageState();
        if(value == zfnull) {
            if(this->pimplOwner->buttonState() == v_ZFUIButtonState::e_Checked) {
                value = this->pimplOwner->iconHighlighted()->imageState();
            }
            else if(this->pimplOwner->buttonState() == v_ZFUIButtonState::e_CheckedHighlighted) {
                value = this->pimplOwner->iconChecked()->imageState();
                if(value == zfnull) {
                    value = this->pimplOwner->iconHighlighted()->imageState();
                }
            }
            if(value == zfnull) {
                value = this->pimplOwner->iconNormal()->imageState();
            }
        }
        this->iconView->image(value);
    }
    void bgViewUpdate(void) {
        ZFUIImage *value = this->pimplOwner->bg(this->pimplOwner->buttonState())->imageState();
        if(value == zfnull) {
            if(this->pimplOwner->buttonState() == v_ZFUIButtonState::e_Checked) {
                value = this->pimplOwner->bgHighlighted()->imageState();
            }
            else if(this->pimplOwner->buttonState() == v_ZFUIButtonState::e_CheckedHighlighted) {
                value = this->pimplOwner->bgChecked()->imageState();
                if(value == zfnull) {
                    value = this->pimplOwner->bgHighlighted()->imageState();
                }
            }
            if(value == zfnull) {
                value = this->pimplOwner->bgNormal()->imageState();
            }
        }
        this->bgView->image(value);
    }
};

// ============================================================
// ZFUIButtonBasic
ZFOBJECT_REGISTER(ZFUIButtonBasic)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonBasic)

#define _ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(T_Type, T_Component, T_State, CkClass, CkProp) \
    ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonBasic, T_Type, T_Component##T_State) { \
        if(!d->T_Component##T_State##OnUpdateListener) { \
            ZFUIButtonBasic *button = this; \
            ZFLISTENER_1(callback \
                    , ZFUIButtonBasic *, button \
                    ) { \
                const ZFProperty *property = zfargs.param0().to<v_ZFProperty *>()->zfv; \
                button->d->T_Component##ViewPrepare(); \
                if(button->buttonState() == v_ZFUIButtonState::e_##T_State) { \
                    ZFPropertyCopy(property, button->T_Component##View()->toObject(), zfargs.sender()); \
                } \
                else if(property == ZFPropertyAccess(CkClass, CkProp)) { \
                    button->d->T_Component##ViewUpdate(); \
                } \
            } ZFLISTENER_END() \
            d->T_Component##T_State##OnUpdateListener = callback; \
        } \
        this->T_Component##T_State()->toObject()->observerAdd( \
            ZFObject::E_ObjectPropertyValueOnUpdate(), \
            d->T_Component##T_State##OnUpdateListener); \
    } \
    ZFPROPERTY_ON_DETACH_DEFINE(ZFUIButtonBasic, T_Type, T_Component##T_State) { \
        this->T_Component##T_State()->toObject()->observerRemove( \
            ZFObject::E_ObjectPropertyValueOnUpdate(), \
            d->T_Component##T_State##OnUpdateListener); \
    }

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUITextView>, label, Normal, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUITextView>, label, Highlighted, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUITextView>, label, Checked, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUITextView>, label, CheckedHighlighted, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUITextView>, label, Disabled, ZFUITextView, text)

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, icon, Normal, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, icon, Highlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, icon, Checked, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, icon, CheckedHighlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, icon, Disabled, ZFUIImageView, image)

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, bg, Normal, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, bg, Highlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, bg, Checked, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, bg, CheckedHighlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(zfanyT<ZFUIImageView>, bg, Disabled, ZFUIImageView, image)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUITextView>, labelNormal) {
    ZFUITextView *v = propertyValue;
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorNormal());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUITextView>, labelHighlighted) {
    ZFUITextView *v = propertyValue;
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorHighlighted());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUITextView>, labelChecked) {
    ZFUITextView *v = propertyValue;
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorChecked());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUITextView>, labelCheckedHighlighted) {
    ZFUITextView *v = propertyValue;
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorCheckedHighlighted());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUITextView>, labelDisabled) {
    ZFUITextView *v = propertyValue;
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorDisabled());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, iconNormal) {
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, iconHighlighted) {
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, iconChecked) {
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, iconCheckedHighlighted) {
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, iconDisabled) {
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, bgNormal) {
    d->bgViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, bgHighlighted) {
    d->bgViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, bgChecked) {
    d->bgViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, bgCheckedHighlighted) {
    d->bgViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, zfanyT<ZFUIImageView>, bgDisabled) {
    d->bgViewPrepare();
}

ZFMETHOD_DEFINE_1(ZFUIButtonBasic, zfanyT<ZFUITextView>, label
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    switch(forState) {
        case v_ZFUIButtonState::e_Normal:
            return this->labelNormal();
        case v_ZFUIButtonState::e_Highlighted:
            return this->labelHighlighted();
        case v_ZFUIButtonState::e_Checked:
            return this->labelChecked();
        case v_ZFUIButtonState::e_CheckedHighlighted:
            return this->labelCheckedHighlighted();
        case v_ZFUIButtonState::e_Disabled:
            return this->labelDisabled();
        default:
            ZFCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
ZFMETHOD_DEFINE_1(ZFUIButtonBasic, zfanyT<ZFUIImageView>, icon
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    switch(forState) {
        case v_ZFUIButtonState::e_Normal:
            return this->iconNormal();
        case v_ZFUIButtonState::e_Highlighted:
            return this->iconHighlighted();
        case v_ZFUIButtonState::e_Checked:
            return this->iconChecked();
        case v_ZFUIButtonState::e_CheckedHighlighted:
            return this->iconCheckedHighlighted();
        case v_ZFUIButtonState::e_Disabled:
            return this->iconDisabled();
        default:
            ZFCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
ZFMETHOD_DEFINE_1(ZFUIButtonBasic, zfanyT<ZFUIImageView>, bg
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    switch(forState) {
        case v_ZFUIButtonState::e_Normal:
            return this->bgNormal();
        case v_ZFUIButtonState::e_Highlighted:
            return this->bgHighlighted();
        case v_ZFUIButtonState::e_Checked:
            return this->bgChecked();
        case v_ZFUIButtonState::e_CheckedHighlighted:
            return this->bgCheckedHighlighted();
        case v_ZFUIButtonState::e_Disabled:
            return this->bgDisabled();
        default:
            ZFCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}

ZFMETHOD_DEFINE_3(ZFUIButtonBasic, void, labelStyle
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const zfstring &, styleKey)
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    this->label(forState)->propStyle(propertyName, styleKey);
}
ZFMETHOD_DEFINE_3(ZFUIButtonBasic, void, iconStyle
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const zfstring &, styleKey)
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    this->icon(forState)->propStyle(propertyName, styleKey);
}
ZFMETHOD_DEFINE_3(ZFUIButtonBasic, void, bgStyle
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const zfstring &, styleKey)
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    this->bg(forState)->propStyle(propertyName, styleKey);
}

ZFMETHOD_DEFINE_3(ZFUIButtonBasic, void, labelProp
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    zfauto valueTmp;
    const ZFProperty *prop = this->label(forState)->classData()->propertyForName(propertyName);
    if(prop && ZFDI_implicitConvertT(valueTmp, prop->propertyTypeId(), value)) {
        prop->setterMethod()->methodInvoke(this->label(forState), valueTmp);
    }
}
ZFMETHOD_DEFINE_3(ZFUIButtonBasic, void, iconProp
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    zfauto valueTmp;
    const ZFProperty *prop = this->icon(forState)->classData()->propertyForName(propertyName);
    if(prop && ZFDI_implicitConvertT(valueTmp, prop->propertyTypeId(), value)) {
        prop->setterMethod()->methodInvoke(this->icon(forState), valueTmp);
    }
}
ZFMETHOD_DEFINE_3(ZFUIButtonBasic, void, bgProp
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(ZFObject *, value)
        , ZFMP_IN_OPT(ZFUIButtonState, forState, v_ZFUIButtonState::e_Normal)
        ) {
    zfauto valueTmp;
    const ZFProperty *prop = this->bg(forState)->classData()->propertyForName(propertyName);
    if(prop && ZFDI_implicitConvertT(valueTmp, prop->propertyTypeId(), value)) {
        prop->setterMethod()->methodInvoke(this->bg(forState), valueTmp);
    }
}

void ZFUIButtonBasic::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonBasicPrivate);
    d->pimplOwner = this;
}

void ZFUIButtonBasic::objectOnDealloc(void) {
    zfRelease(d->labelView);
    zfRelease(d->iconView);
    zfRelease(d->bgView);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIButtonBasic::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);

    if(this->labelView() != zfnull && !this->labelView()->text().isEmpty()) {
        zfstringAppend(ret, " \"%s\"", this->labelView()->text());
    }
}

void ZFUIButtonBasic::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ZFUISize sizeHintTmp = sizeHint;
    if(sizeHintTmp.width >= 0) {
        sizeHintTmp.width -= this->contentMargin().left + this->contentMargin().right;
        if(sizeHintTmp.width < 0) {
            sizeHintTmp.width = 0;
        }
    }
    if(sizeHintTmp.height >= 0) {
        sizeHintTmp.height -= this->contentMargin().top + this->contentMargin().bottom;
        if(sizeHintTmp.height < 0) {
            sizeHintTmp.height = 0;
        }
    }

    ZFUISize iconSize = ZFUISizeZero();
    if(d->iconView != zfnull && d->iconView->visible() && d->iconView->imageState() != zfnull) {
        iconSize = d->iconView->layoutMeasure(sizeHintTmp, ZFUISizeParamWrapWrap());
    }

    ZFUISize labelSize = ZFUISizeZero();
    if(d->labelView != zfnull && d->labelView->visible() && !d->labelView->text().isEmpty()) {
        ZFUISize sizeHintForLabel = sizeHintTmp;
        switch(this->iconPosition()) {
            case v_ZFUIOrientation::e_Left:
            case v_ZFUIOrientation::e_Right:
                if(iconSize.width > 0) {
                    ZFUILayoutParam::sizeHintOffsetT(sizeHintForLabel.width, sizeHintTmp.width, -(iconSize.width + this->contentSpace()));
                }
                break;
            case v_ZFUIOrientation::e_Top:
            case v_ZFUIOrientation::e_Bottom:
                if(iconSize.height > 0) {
                    ZFUILayoutParam::sizeHintOffsetT(sizeHintForLabel.height, sizeHintTmp.height, -(iconSize.height + this->contentSpace()));
                }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
        labelSize = d->labelView->layoutMeasure(sizeHintForLabel, ZFUISizeParamWrapWrap());
    }

    ZFUISize bgSize = ZFUISizeZero();
    if(d->bgView != zfnull && d->bgView->visible() && d->bgView->imageState() != zfnull) {
        ZFUISize sizeHintBg = sizeHint;
        if(sizeHintBg.width >= 0) {
            sizeHintBg.width -= this->contentMargin().left + this->contentMargin().right;
            if(sizeHintBg.width < 0) {
                sizeHintBg.width = 0;
            }
        }
        if(sizeHintBg.height >= 0) {
            sizeHintBg.height -= this->contentMargin().top + this->contentMargin().bottom;
            if(sizeHintBg.height < 0) {
                sizeHintBg.height = 0;
            }
        }
        bgSize = d->bgView->layoutMeasure(sizeHintBg, ZFUISizeParamWrapWrap());
    }

    ZFUISize contentSize = ZFUISizeZero();
    zffloat contentSpace = 0;
    switch(this->iconPosition()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0) {
                contentSpace = this->contentSpace();
            }
            contentSize.width = iconSize.width + contentSpace + labelSize.width;
            contentSize.height = zfmMax(iconSize.height, labelSize.height);
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0) {
                contentSpace = this->contentSpace();
            }
            contentSize.width = zfmMax(iconSize.width, labelSize.width);
            contentSize.height = iconSize.height + contentSpace + labelSize.height;
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }

    if(bgSize.width > 0) {
        bgSize.width += ZFUIMarginGetWidth(this->bgMargin());
    }
    if(bgSize.height > 0) {
        bgSize.height += ZFUIMarginGetHeight(this->bgMargin());
    }
    if(contentSize.width > 0) {
        contentSize.width += ZFUIMarginGetWidth(this->contentMargin());
    }
    if(contentSize.height > 0) {
        contentSize.height += ZFUIMarginGetHeight(this->contentMargin());
    }
    ret.width = zfmMax(bgSize.width, contentSize.width);
    ret.height = zfmMax(bgSize.height, contentSize.height);
}
void ZFUIButtonBasic::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);

    if(d->bgView != zfnull) {
        d->bgView->viewFrame(ZFUIRectApplyMargin(bounds, this->bgMargin()));
    }

    ZFUISize sizeHint = ZFUISizeApplyMargin(ZFUIRectGetSize(bounds), this->contentMargin());

    ZFUISize labelSize = ZFUISizeZero();
    if(d->labelView != zfnull && d->labelView->visible() && !d->labelView->text().isEmpty()) {
        labelSize = d->labelView->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    }

    ZFUISize iconSize = ZFUISizeZero();
    if(d->iconView != zfnull && d->iconView->visible() && d->iconView->imageState() != zfnull) {
        iconSize = d->iconView->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    }

    ZFUIRect contentFrame = ZFUIRectZero();
    zffloat contentSpace = 0;
    switch(this->iconPosition()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0) {
                contentSpace = this->contentSpace();
            }
            ZFUIAlignApplyT(
                contentFrame,
                this->contentAlign(),
                ZFUIRectApplyMargin(bounds, this->contentMargin()),
                ZFUISizeCreate(iconSize.width + contentSpace + labelSize.width, zfmMax(iconSize.height, labelSize.height))
                );
            if(this->iconPosition() == v_ZFUIOrientation::e_Left) {
                if(d->iconView != zfnull) {
                    d->iconView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Left, contentFrame, iconSize));
                }
                if(d->labelView != zfnull) {
                    d->labelView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Right, contentFrame, labelSize));
                }
            }
            else {
                if(d->iconView != zfnull) {
                    d->iconView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Right, contentFrame, iconSize));
                }
                if(d->labelView != zfnull) {
                    d->labelView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Left, contentFrame, labelSize));
                }
            }
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0) {
                contentSpace = this->contentSpace();
            }
            ZFUIAlignApplyT(
                contentFrame,
                this->contentAlign(),
                ZFUIRectApplyMargin(bounds, this->contentMargin()),
                ZFUISizeCreate(zfmMax(iconSize.width, labelSize.width), iconSize.height + contentSpace + labelSize.height)
                );
            if(this->iconPosition() == v_ZFUIOrientation::e_Top) {
                if(d->iconView != zfnull) {
                    d->iconView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Top, contentFrame, iconSize));
                }
                if(d->labelView != zfnull) {
                    d->labelView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Bottom, contentFrame, labelSize));
                }
            }
            else {
                if(d->iconView != zfnull) {
                    d->iconView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Bottom, contentFrame, iconSize));
                }
                if(d->labelView != zfnull) {
                    d->labelView->viewFrame(ZFUIAlignApply(v_ZFUIAlign::e_Top, contentFrame, labelSize));
                }
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

zfbool ZFUIButtonBasic::internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
    if((d->labelView != zfnull && d->labelView->toObject() == internalView)
            || (d->iconView != zfnull && d->iconView->toObject() == internalView)
            || (d->bgView != zfnull && d->bgView->toObject() == internalView)
            ) {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}

void ZFUIButtonBasic::buttonStateOnUpdate(void) {
    zfsuper::buttonStateOnUpdate();

    #define _ZFP_ZFUIButtonBasic_buttonStateOn(T_State) \
        if(d->labelView != zfnull) { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, label##T_State), this)) { \
                d->labelView->styleableCopyFrom(this->label##T_State()); \
            } \
            d->labelViewUpdate(); \
        } \
        if(d->iconView != zfnull) { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, icon##T_State), this)) { \
                d->iconView->styleableCopyFrom(this->icon##T_State()); \
            } \
            d->iconViewUpdate(); \
        } \
        if(d->bgView != zfnull) { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bg##T_State), this)) { \
                d->bgView->styleableCopyFrom(this->bg##T_State()); \
            } \
            d->bgViewUpdate(); \
        }
    switch(this->buttonState()) {
        case v_ZFUIButtonState::e_Normal:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Normal)
            break;
        case v_ZFUIButtonState::e_Highlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Highlighted)
            break;
        case v_ZFUIButtonState::e_Checked:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Checked)
            break;
        case v_ZFUIButtonState::e_CheckedHighlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(CheckedHighlighted)
            break;
        case v_ZFUIButtonState::e_Disabled:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Disabled)
            break;
        default:
            break;
    }
}

const ZFClass *ZFUIButtonBasic::labelViewClass(void) {
    return ZFUITextView::ClassData();
}
const ZFClass *ZFUIButtonBasic::iconViewClass(void) {
    return ZFUIImageView::ClassData();
}
const ZFClass *ZFUIButtonBasic::bgViewClass(void) {
    return ZFUIImageView::ClassData();
}

ZFMETHOD_DEFINE_0(ZFUIButtonBasic, zfanyT<ZFUITextView>, labelView) {
    d->labelViewPrepare();
    return d->labelView;
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, zfanyT<ZFUIImageView>, iconView) {
    d->iconViewPrepare();
    return d->iconView;
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, zfanyT<ZFUIImageView>, bgView) {
    d->bgViewPrepare();
    return d->bgView;
}

ZF_NAMESPACE_GLOBAL_END

