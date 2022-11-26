#include "ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFUIButtonBasicPrivate
zfclassNotPOD _ZFP_ZFUIButtonBasicPrivate
{
public:
    ZFUIButtonBasic *pimplOwner;
    ZFUITextView *labelView;
    ZFUIImageView *iconView;
    ZFUIImageView *backgroundView;

    ZFListener labelNormalChangedListener;
    ZFListener labelHighlightedChangedListener;
    ZFListener labelCheckedChangedListener;
    ZFListener labelCheckedHighlightedChangedListener;
    ZFListener labelDisabledChangedListener;
    ZFListener iconNormalChangedListener;
    ZFListener iconHighlightedChangedListener;
    ZFListener iconCheckedChangedListener;
    ZFListener iconCheckedHighlightedChangedListener;
    ZFListener iconDisabledChangedListener;
    ZFListener backgroundNormalChangedListener;
    ZFListener backgroundHighlightedChangedListener;
    ZFListener backgroundCheckedChangedListener;
    ZFListener backgroundCheckedHighlightedChangedListener;
    ZFListener backgroundDisabledChangedListener;

public:
    _ZFP_ZFUIButtonBasicPrivate(void)
    : pimplOwner(zfnull)
    , labelView(zfnull)
    , iconView(zfnull)
    , backgroundView(zfnull)

    , labelNormalChangedListener()
    , labelHighlightedChangedListener()
    , labelCheckedChangedListener()
    , labelCheckedHighlightedChangedListener()
    , labelDisabledChangedListener()
    , iconNormalChangedListener()
    , iconHighlightedChangedListener()
    , iconCheckedChangedListener()
    , iconCheckedHighlightedChangedListener()
    , iconDisabledChangedListener()
    , backgroundNormalChangedListener()
    , backgroundHighlightedChangedListener()
    , backgroundCheckedChangedListener()
    , backgroundCheckedHighlightedChangedListener()
    , backgroundDisabledChangedListener()
    {
    }

public:
    void labelViewPrepare(void)
    {
        if(this->labelView == zfnull)
        {
            ZFObject *obj = zfRetain(this->pimplOwner->buttonLabelClass()->newInstance().toObject());
            this->labelView = ZFCastZFObject(ZFUITextView *, obj);
            zfCoreAssert(this->labelView != zfnull);
            this->pimplOwner->internalBgViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, this->labelView));
        }
    }
    void iconViewPrepare(void)
    {
        if(this->iconView == zfnull)
        {
            ZFObject *obj = zfRetain(this->pimplOwner->buttonIconClass()->newInstance().toObject());
            this->iconView = ZFCastZFObject(ZFUIImageView *, obj);
            zfCoreAssert(this->iconView != zfnull);
            this->pimplOwner->internalBgViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, this->iconView));

            if(this->labelView != zfnull)
            {
                ZFUIView *t = this->labelView->to<ZFUIView *>();
                this->pimplOwner->internalBgViewRemove(t);
                this->pimplOwner->internalBgViewAdd(t);
            }
        }
    }
    void backgroundViewPrepare(void)
    {
        if(this->backgroundView == zfnull)
        {
            ZFObject *obj = zfRetain(this->pimplOwner->buttonBackgroundClass()->newInstance().toObject());
            this->backgroundView = ZFCastZFObject(ZFUIImageView *, obj);
            zfCoreAssert(this->backgroundView != zfnull);
            this->pimplOwner->internalBgViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, this->backgroundView));

            if(this->iconView != zfnull)
            {
                ZFUIView *t = this->iconView->to<ZFUIView *>();
                this->pimplOwner->internalBgViewRemove(t);
                this->pimplOwner->internalBgViewAdd(t);
            }
            if(this->labelView != zfnull)
            {
                ZFUIView *t = this->labelView->to<ZFUIView *>();
                this->pimplOwner->internalBgViewRemove(t);
                this->pimplOwner->internalBgViewAdd(t);
            }
        }
    }

public:
    void labelViewUpdate(void)
    {
        const zfchar *value = this->pimplOwner->label(this->pimplOwner->buttonState())->text();
        if(*value == '\0')
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->labelHighlighted()->text();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->labelChecked()->text();
                if(*value == '\0')
                {
                    value = this->pimplOwner->labelHighlighted()->text();
                }
            }
            if(*value == '\0')
            {
                value = this->pimplOwner->labelNormal()->text();
            }
        }
        this->labelView->text(value);
    }
    void iconViewUpdate(void)
    {
        ZFUIImage *value = this->pimplOwner->icon(this->pimplOwner->buttonState())->image();
        if(value == zfnull)
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->iconHighlighted()->image();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->iconChecked()->image();
                if(value == zfnull)
                {
                    value = this->pimplOwner->iconHighlighted()->image();
                }
            }
            if(value == zfnull)
            {
                value = this->pimplOwner->iconNormal()->image();
            }
        }
        this->iconView->image(value);
    }
    void backgroundViewUpdate(void)
    {
        ZFUIImage *value = this->pimplOwner->background(this->pimplOwner->buttonState())->image();
        if(value == zfnull)
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->backgroundHighlighted()->image();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->backgroundChecked()->image();
                if(value == zfnull)
                {
                    value = this->pimplOwner->backgroundHighlighted()->image();
                }
            }
            if(value == zfnull)
            {
                value = this->pimplOwner->backgroundNormal()->image();
            }
        }
        this->backgroundView->image(value);
    }
};

// ============================================================
// ZFUIButtonBasic
ZFOBJECT_REGISTER(ZFUIButtonBasic)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonBasic)

#define _ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(T_Type, T_Component, T_State, CkClass, CkProp) \
    ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonBasic, T_Type, T_Component##T_State) \
    { \
        if(!d->T_Component##T_State##ChangedListener) \
        { \
            ZFUIButtonBasic *button = this; \
            ZFLISTENER_1(callback \
                    , ZFUIButtonBasic *, button \
                    ) { \
                const ZFProperty *property = zfargs.param0()->to<v_ZFProperty *>()->zfv; \
                button->d->T_Component##ViewPrepare(); \
                if(button->buttonState() == ZFUIButtonState::e_##T_State) \
                { \
                    ZFPropertyCopy(property, button->T_Component##View()->toObject(), zfargs.sender()); \
                } \
                else if(property == ZFPropertyAccess(CkClass, CkProp)) \
                { \
                    button->d->T_Component##ViewUpdate(); \
                } \
            } ZFLISTENER_END(callback) \
            d->T_Component##T_State##ChangedListener = callback; \
        } \
        this->T_Component##T_State()->toObject()->observerAdd( \
            ZFObject::EventObjectPropertyValueOnUpdate(), \
            d->T_Component##T_State##ChangedListener); \
    } \
    ZFPROPERTY_ON_DETACH_DEFINE(ZFUIButtonBasic, T_Type, T_Component##T_State) \
    { \
        this->T_Component##T_State()->toObject()->observerRemove( \
            ZFObject::EventObjectPropertyValueOnUpdate(), \
            d->T_Component##T_State##ChangedListener); \
    }

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, label, Normal, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, label, Highlighted, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, label, Checked, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, label, CheckedHighlighted, ZFUITextView, text)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, label, Disabled, ZFUITextView, text)

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, icon, Normal, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, icon, Highlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, icon, Checked, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, icon, CheckedHighlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, icon, Disabled, ZFUIImageView, image)

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, background, Normal, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, background, Highlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, background, Checked, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, background, CheckedHighlighted, ZFUIImageView, image)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, background, Disabled, ZFUIImageView, image)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, labelNormal)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorNormal());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, labelHighlighted)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorHighlighted());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, labelChecked)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorChecked());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, labelCheckedHighlighted)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorCheckedHighlighted());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, labelDisabled)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorDisabled());

    d->labelViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, iconNormal)
{
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, iconHighlighted)
{
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, iconChecked)
{
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, iconCheckedHighlighted)
{
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, iconDisabled)
{
    d->iconViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, backgroundNormal)
{
    d->backgroundViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, backgroundHighlighted)
{
    d->backgroundViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, backgroundChecked)
{
    d->backgroundViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, backgroundCheckedHighlighted)
{
    d->backgroundViewPrepare();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, backgroundDisabled)
{
    d->backgroundViewPrepare();
}

ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUITextView *, label,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    switch(forState)
    {
        case ZFUIButtonState::e_Normal:
            return this->labelNormal();
        case ZFUIButtonState::e_Highlighted:
            return this->labelHighlighted();
        case ZFUIButtonState::e_Checked:
            return this->labelChecked();
        case ZFUIButtonState::e_CheckedHighlighted:
            return this->labelCheckedHighlighted();
        case ZFUIButtonState::e_Disabled:
            return this->labelDisabled();
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUIImageView *, icon,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    switch(forState)
    {
        case ZFUIButtonState::e_Normal:
            return this->iconNormal();
        case ZFUIButtonState::e_Highlighted:
            return this->iconHighlighted();
        case ZFUIButtonState::e_Checked:
            return this->iconChecked();
        case ZFUIButtonState::e_CheckedHighlighted:
            return this->iconCheckedHighlighted();
        case ZFUIButtonState::e_Disabled:
            return this->iconDisabled();
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUIImageView *, background,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    switch(forState)
    {
        case ZFUIButtonState::e_Normal:
            return this->backgroundNormal();
        case ZFUIButtonState::e_Highlighted:
            return this->backgroundHighlighted();
        case ZFUIButtonState::e_Checked:
            return this->backgroundChecked();
        case ZFUIButtonState::e_CheckedHighlighted:
            return this->backgroundCheckedHighlighted();
        case ZFUIButtonState::e_Disabled:
            return this->backgroundDisabled();
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}

void ZFUIButtonBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonBasicPrivate);
    d->pimplOwner = this;
}

void ZFUIButtonBasic::objectOnDealloc(void)
{
    zfRelease(d->labelView);
    zfRelease(d->iconView);
    zfRelease(d->backgroundView);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIButtonBasic::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(this->labelView() != zfnull && !this->labelView()->text().isEmpty())
    {
        zfstringAppend(ret, " \"%s\"", this->labelView()->text().cString());
    }
}

void ZFUIButtonBasic::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                      ZF_IN const ZFUISize &sizeHint,
                                      ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUIView *labelView = ZFCastZFObjectUnchecked(ZFUIView *, d->labelView);
    ZFUIView *iconView = ZFCastZFObjectUnchecked(ZFUIView *, d->iconView);
    ZFUIView *backgroundView = ZFCastZFObjectUnchecked(ZFUIView *, d->backgroundView);
    ZFUISize sizeHintTmp = sizeHint;
    if(sizeHintTmp.width >= 0)
    {
        sizeHintTmp.width -= this->contentMargin().left + this->contentMargin().right;
        if(sizeHintTmp.width < 0)
        {
            sizeHintTmp.width = 0;
        }
    }
    if(sizeHintTmp.height >= 0)
    {
        sizeHintTmp.height -= this->contentMargin().top + this->contentMargin().bottom;
        if(sizeHintTmp.height < 0)
        {
            sizeHintTmp.height = 0;
        }
    }

    ZFUISize labelSize = ZFUISizeZero();
    if(labelView != zfnull && labelView->viewVisible() && !d->labelView->text().isEmpty())
    {
        labelSize = labelView->layoutMeasure(sizeHintTmp, ZFUISizeParamWrapWrap());
    }

    ZFUISize iconSize = ZFUISizeZero();
    if(iconView != zfnull && iconView->viewVisible() && d->iconView->image() != zfnull)
    {
        iconSize = iconView->layoutMeasure(sizeHintTmp, ZFUISizeParamWrapWrap());
    }

    ZFUISize backgroundSize = ZFUISizeZero();
    if(backgroundView != zfnull && backgroundView->viewVisible() && d->backgroundView->image() != zfnull)
    {
        ZFUISize sizeHintBg = sizeHint;
        if(sizeHintBg.width >= 0)
        {
            sizeHintBg.width -= this->contentMargin().left + this->contentMargin().right;
            if(sizeHintBg.width < 0)
            {
                sizeHintBg.width = 0;
            }
        }
        if(sizeHintBg.height >= 0)
        {
            sizeHintBg.height -= this->contentMargin().top + this->contentMargin().bottom;
            if(sizeHintBg.height < 0)
            {
                sizeHintBg.height = 0;
            }
        }
        backgroundSize = backgroundView->layoutMeasure(sizeHintBg, ZFUISizeParamWrapWrap());
    }

    ZFUISize contentSize = ZFUISizeZero();
    zffloat contentSpace = 0;
    switch(this->iconPosition())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0)
            {
                contentSpace = this->contentSpace();
            }
            contentSize.width = iconSize.width + contentSpace + labelSize.width;
            contentSize.height = zfmMax(iconSize.height, labelSize.height);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0)
            {
                contentSpace = this->contentSpace();
            }
            contentSize.width = zfmMax(iconSize.width, labelSize.width);
            contentSize.height = iconSize.height + contentSpace + labelSize.height;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }

    if(backgroundSize.width > 0)
    {
        backgroundSize.width += ZFUIMarginGetWidth(this->backgroundMargin());
    }
    if(backgroundSize.height > 0)
    {
        backgroundSize.height += ZFUIMarginGetHeight(this->backgroundMargin());
    }
    if(contentSize.width > 0)
    {
        contentSize.width += ZFUIMarginGetWidth(this->contentMargin());
    }
    if(contentSize.height > 0)
    {
        contentSize.height += ZFUIMarginGetHeight(this->contentMargin());
    }
    ret.width = zfmMax(backgroundSize.width, contentSize.width);
    ret.height = zfmMax(backgroundSize.height, contentSize.height);
}
void ZFUIButtonBasic::internalViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalViewOnLayout(bounds);

    ZFUIView *backgroundView = ZFCastZFObjectUnchecked(ZFUIView *, d->backgroundView);
    if(backgroundView != zfnull)
    {
        backgroundView->viewFrame(ZFUIRectApplyMargin(bounds, this->backgroundMargin()));
    }

    ZFUISize sizeHint = ZFUISizeApplyMargin(ZFUIRectGetSize(bounds), this->contentMargin());

    ZFUIView *labelView = ZFCastZFObjectUnchecked(ZFUIView *, d->labelView);
    ZFUIView *iconView = ZFCastZFObjectUnchecked(ZFUIView *, d->iconView);

    ZFUISize labelSize = ZFUISizeZero();
    if(labelView != zfnull && labelView->viewVisible() && !d->labelView->text().isEmpty())
    {
        labelSize = labelView->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    }

    ZFUISize iconSize = ZFUISizeZero();
    if(iconView != zfnull && iconView->viewVisible() && d->iconView->image() != zfnull)
    {
        iconSize = iconView->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    }

    ZFUIRect contentFrame = ZFUIRectZero();
    zffloat contentSpace = 0;
    switch(this->iconPosition())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0)
            {
                contentSpace = this->contentSpace();
            }
            contentFrame = ZFUIAlignApply(
                this->contentAlign(),
                bounds,
                ZFUISizeMake(iconSize.width + contentSpace + labelSize.width, zfmMax(iconSize.height, labelSize.height)),
                this->contentMargin());
            if(this->iconPosition() == ZFUIOrientation::e_Left)
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Left, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Right, contentFrame, labelSize));
                }
            }
            else
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Right, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Left, contentFrame, labelSize));
                }
            }
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0)
            {
                contentSpace = this->contentSpace();
            }
            contentFrame = ZFUIAlignApply(
                this->contentAlign(),
                bounds,
                ZFUISizeMake(zfmMax(iconSize.width, labelSize.width), iconSize.height + contentSpace + labelSize.height),
                this->contentMargin());
            if(this->iconPosition() == ZFUIOrientation::e_Top)
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Top, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Bottom, contentFrame, labelSize));
                }
            }
            else
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Bottom, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_Top, contentFrame, labelSize));
                }
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

zfbool ZFUIButtonBasic::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if((d->labelView != zfnull && d->labelView->toObject() == internalView)
        || (d->iconView != zfnull && d->iconView->toObject() == internalView)
        || (d->backgroundView != zfnull && d->backgroundView->toObject() == internalView))
    {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}

void ZFUIButtonBasic::buttonStateOnUpdate(void)
{
    zfsuper::buttonStateOnUpdate();

    #define _ZFP_ZFUIButtonBasic_buttonStateOn(T_State) \
        if(d->labelView != zfnull) \
        { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, label##T_State), this)) \
            { \
                ZFCastZFObjectUnchecked(ZFStyleable *, d->labelView)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->label##T_State())); \
            } \
            d->labelViewUpdate(); \
        } \
        if(d->iconView != zfnull) \
        { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, icon##T_State), this)) \
            { \
                ZFCastZFObjectUnchecked(ZFStyleable *, d->iconView)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->icon##T_State())); \
            } \
            d->iconViewUpdate(); \
        } \
        if(d->backgroundView != zfnull) \
        { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, background##T_State), this)) \
            { \
                ZFCastZFObjectUnchecked(ZFStyleable *, d->backgroundView)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->background##T_State())); \
            } \
            d->backgroundViewUpdate(); \
        }
    switch(this->buttonState())
    {
        case ZFUIButtonState::e_Normal:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Normal)
            break;
        case ZFUIButtonState::e_Highlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Highlighted)
            break;
        case ZFUIButtonState::e_Checked:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Checked)
            break;
        case ZFUIButtonState::e_CheckedHighlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(CheckedHighlighted)
            break;
        case ZFUIButtonState::e_Disabled:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Disabled)
            break;
        default:
            break;
    }
}

const ZFClass *ZFUIButtonBasic::buttonLabelClass(void)
{
    return ZFUITextView::ClassData();
}
const ZFClass *ZFUIButtonBasic::buttonIconClass(void)
{
    return ZFUIImageView::ClassData();
}
const ZFClass *ZFUIButtonBasic::buttonBackgroundClass(void)
{
    return ZFUIImageView::ClassData();
}

ZFMETHOD_DEFINE_0(ZFUIButtonBasic, ZFUITextView *, labelView)
{
    d->labelViewPrepare();
    return d->labelView;
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, ZFUIImageView *, iconView)
{
    d->iconViewPrepare();
    return d->iconView;
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, ZFUIImageView *, backgroundView)
{
    d->backgroundViewPrepare();
    return d->backgroundView;
}

ZF_NAMESPACE_GLOBAL_END

