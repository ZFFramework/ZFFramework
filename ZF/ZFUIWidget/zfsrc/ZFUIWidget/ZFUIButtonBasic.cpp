#include "ZFUIButtonBasic.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#include "ZFCore/ZFSTLWrapper/zfstlset.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFUIButtonBasicPrivate
zfclassNotPOD _ZFP_ZFUIButtonBasicPrivate {
public:
    // <label|icon|bg, <prop, setterCalled> >
    typedef zfstlmap<ZFObject *, zfstlset<const ZFProperty *> > PropSetterCalledMap;
public:
    ZFUIButtonBasic *pimplOwner;
    ZFUITextView *labelView;
    ZFUIImageView *iconView;
    ZFUIImageView *bgView;
    PropSetterCalledMap modMap;

    ZFListener labelOnUpdateListener;
    ZFListener iconOnUpdateListener;
    ZFListener bgOnUpdateListener;

public:
    _ZFP_ZFUIButtonBasicPrivate(void)
    : pimplOwner(zfnull)
    , labelView(zfnull)
    , iconView(zfnull)
    , bgView(zfnull)
    , modMap()

    , labelOnUpdateListener()
    , iconOnUpdateListener()
    , bgOnUpdateListener()
    {
    }

public:
    void labelViewPrepare(void) {
        if(this->labelView == zfnull) {
            this->labelView = zfRetain(this->pimplOwner->labelViewClass()->newInstance());
            ZFCoreAssert(this->labelView != zfnull);
            this->pimplOwner->internalBgViewAdd(this->labelView);

            this->labelStateUpdate();
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

            this->iconStateUpdate();
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

            this->bgStateUpdate();
        }
    }

private:
    inline zfbool _propNeedCopy(
            ZF_IN const ZFProperty *prop
            , ZF_IN ZFObject *owner
            ) {
        PropSetterCalledMap::iterator it = this->modMap.find(owner);
        if(it == this->modMap.end()) {
            return zffalse;
        }
        zfstlset<const ZFProperty *>::iterator propIt = it->second.find(prop);
        if(propIt == it->second.end()) {
            return zffalse;
        }
        return ZFPropertyIsValueAccessed(prop, owner);
    }

public:
    void labelStateUpdate(ZF_IN_OPT const ZFProperty *changedProp = zfnull) {
        if(this->labelView) {
            ZFUITextView *stateNormal = pimplOwner->label(v_ZFUIButtonState::e_Normal);
            ZFUITextView *stateCk0 = zfnull;
            ZFUITextView *stateCk1 = zfnull;
            ZFUITextView *stateCk2 = zfnull;
            switch(pimplOwner->buttonState()) {
                case v_ZFUIButtonState::e_Normal:
                    break;
                case v_ZFUIButtonState::e_Highlighted:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, labelHighlighted), pimplOwner)) {
                        stateCk0 = pimplOwner->label(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_Checked:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, labelChecked), pimplOwner)) {
                        stateCk0 = pimplOwner->label(v_ZFUIButtonState::e_Checked);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, labelHighlighted), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : stateCk1) = pimplOwner->label(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_CheckedHighlighted:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, labelCheckedHighlighted), pimplOwner)) {
                        stateCk0 = pimplOwner->label(v_ZFUIButtonState::e_CheckedHighlighted);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, labelChecked), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : stateCk1) = pimplOwner->label(v_ZFUIButtonState::e_Checked);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, labelHighlighted), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : (!stateCk1 ? stateCk1 : stateCk2)) = pimplOwner->label(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_Disabled:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, labelDisabled), pimplOwner)) {
                        stateCk0 = pimplOwner->label(v_ZFUIButtonState::e_Disabled);
                    }
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }
            if(changedProp) {
                const ZFProperty *prop = changedProp;
                if(stateCk0 && _propNeedCopy(prop, stateCk0)) {
                    ZFPropertyCopy(prop, this->labelView, stateCk0);
                }
                else if(stateCk1 && _propNeedCopy(prop, stateCk1)) {
                    ZFPropertyCopy(prop, this->labelView, stateCk1);
                }
                else if(stateCk2 && _propNeedCopy(prop, stateCk2)) {
                    ZFPropertyCopy(prop, this->labelView, stateCk2);
                }
                else {
                    ZFPropertyCopy(prop, this->labelView, stateNormal);
                }
            }
            else {
                const ZFCoreArray<const ZFProperty *> allProp = stateNormal->styleablePropertyGetAll();
                for(zfindex i = 0; i < allProp.count(); ++i) {
                    const ZFProperty *prop = allProp[i];
                    if(stateCk0 && _propNeedCopy(prop, stateCk0)) {
                        ZFPropertyCopy(prop, this->labelView, stateCk0);
                    }
                    else if(stateCk1 && _propNeedCopy(prop, stateCk1)) {
                        ZFPropertyCopy(prop, this->labelView, stateCk1);
                    }
                    else if(stateCk2 && _propNeedCopy(prop, stateCk2)) {
                        ZFPropertyCopy(prop, this->labelView, stateCk2);
                    }
                    else {
                        ZFPropertyCopy(prop, this->labelView, stateNormal);
                    }
                }
            }
        }
    }
    void iconStateUpdate(ZF_IN_OPT const ZFProperty *changedProp = zfnull) {
        if(this->iconView) {
            ZFUIImageView *stateNormal = pimplOwner->icon(v_ZFUIButtonState::e_Normal);
            ZFUIImageView *stateCk0 = zfnull;
            ZFUIImageView *stateCk1 = zfnull;
            ZFUIImageView *stateCk2 = zfnull;
            switch(pimplOwner->buttonState()) {
                case v_ZFUIButtonState::e_Normal:
                    break;
                case v_ZFUIButtonState::e_Highlighted:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, iconHighlighted), pimplOwner)) {
                        stateCk0 = pimplOwner->icon(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_Checked:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, iconChecked), pimplOwner)) {
                        stateCk0 = pimplOwner->icon(v_ZFUIButtonState::e_Checked);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, iconHighlighted), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : stateCk1) = pimplOwner->icon(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_CheckedHighlighted:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, iconCheckedHighlighted), pimplOwner)) {
                        stateCk0 = pimplOwner->icon(v_ZFUIButtonState::e_CheckedHighlighted);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, iconChecked), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : stateCk1) = pimplOwner->icon(v_ZFUIButtonState::e_Checked);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, iconHighlighted), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : (!stateCk1 ? stateCk1 : stateCk2)) = pimplOwner->icon(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_Disabled:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, iconDisabled), pimplOwner)) {
                        stateCk0 = pimplOwner->icon(v_ZFUIButtonState::e_Disabled);
                    }
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }
            if(changedProp) {
                const ZFProperty *prop = changedProp;
                if(stateCk0 && _propNeedCopy(prop, stateCk0)) {
                    ZFPropertyCopy(prop, this->iconView, stateCk0);
                }
                else if(stateCk1 && _propNeedCopy(prop, stateCk1)) {
                    ZFPropertyCopy(prop, this->iconView, stateCk1);
                }
                else if(stateCk2 && _propNeedCopy(prop, stateCk2)) {
                    ZFPropertyCopy(prop, this->iconView, stateCk2);
                }
                else {
                    ZFPropertyCopy(prop, this->iconView, stateNormal);
                }
            }
            else {
                const ZFCoreArray<const ZFProperty *> allProp = stateNormal->styleablePropertyGetAll();
                for(zfindex i = 0; i < allProp.count(); ++i) {
                    const ZFProperty *prop = allProp[i];
                    if(stateCk0 && _propNeedCopy(prop, stateCk0)) {
                        ZFPropertyCopy(prop, this->iconView, stateCk0);
                    }
                    else if(stateCk1 && _propNeedCopy(prop, stateCk1)) {
                        ZFPropertyCopy(prop, this->iconView, stateCk1);
                    }
                    else if(stateCk2 && _propNeedCopy(prop, stateCk2)) {
                        ZFPropertyCopy(prop, this->iconView, stateCk2);
                    }
                    else {
                        ZFPropertyCopy(prop, this->iconView, stateNormal);
                    }
                }
            }
        }
    }
    void bgStateUpdate(ZF_IN_OPT const ZFProperty *changedProp = zfnull) {
        if(this->bgView) {
            ZFUIImageView *stateNormal = pimplOwner->bg(v_ZFUIButtonState::e_Normal);
            ZFUIImageView *stateCk0 = zfnull;
            ZFUIImageView *stateCk1 = zfnull;
            ZFUIImageView *stateCk2 = zfnull;
            switch(pimplOwner->buttonState()) {
                case v_ZFUIButtonState::e_Normal:
                    break;
                case v_ZFUIButtonState::e_Highlighted:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bgHighlighted), pimplOwner)) {
                        stateCk0 = pimplOwner->bg(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_Checked:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bgChecked), pimplOwner)) {
                        stateCk0 = pimplOwner->bg(v_ZFUIButtonState::e_Checked);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bgHighlighted), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : stateCk1) = pimplOwner->bg(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_CheckedHighlighted:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bgCheckedHighlighted), pimplOwner)) {
                        stateCk0 = pimplOwner->bg(v_ZFUIButtonState::e_CheckedHighlighted);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bgChecked), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : stateCk1) = pimplOwner->bg(v_ZFUIButtonState::e_Checked);
                    }
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bgHighlighted), pimplOwner)) {
                        (!stateCk0 ? stateCk0 : (!stateCk1 ? stateCk1 : stateCk2)) = pimplOwner->bg(v_ZFUIButtonState::e_Highlighted);
                    }
                    break;
                case v_ZFUIButtonState::e_Disabled:
                    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, bgDisabled), pimplOwner)) {
                        stateCk0 = pimplOwner->bg(v_ZFUIButtonState::e_Disabled);
                    }
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }
            if(changedProp) {
                const ZFProperty *prop = changedProp;
                if(stateCk0 && _propNeedCopy(prop, stateCk0)) {
                    ZFPropertyCopy(prop, this->bgView, stateCk0);
                }
                else if(stateCk1 && _propNeedCopy(prop, stateCk1)) {
                    ZFPropertyCopy(prop, this->bgView, stateCk1);
                }
                else if(stateCk2 && _propNeedCopy(prop, stateCk2)) {
                    ZFPropertyCopy(prop, this->bgView, stateCk2);
                }
                else {
                    ZFPropertyCopy(prop, this->bgView, stateNormal);
                }
            }
            else {
                const ZFCoreArray<const ZFProperty *> allProp = stateNormal->styleablePropertyGetAll();
                for(zfindex i = 0; i < allProp.count(); ++i) {
                    const ZFProperty *prop = allProp[i];
                    if(stateCk0 && _propNeedCopy(prop, stateCk0)) {
                        ZFPropertyCopy(prop, this->bgView, stateCk0);
                    }
                    else if(stateCk1 && _propNeedCopy(prop, stateCk1)) {
                        ZFPropertyCopy(prop, this->bgView, stateCk1);
                    }
                    else if(stateCk2 && _propNeedCopy(prop, stateCk2)) {
                        ZFPropertyCopy(prop, this->bgView, stateCk2);
                    }
                    else {
                        ZFPropertyCopy(prop, this->bgView, stateNormal);
                    }
                }
            }
        }
    }
};

// ============================================================
// ZFUIButtonBasic
ZFOBJECT_REGISTER(ZFUIButtonBasic)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonBasic)

#define _ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(T_Type, T_Component, T_State, CkClass, CkProp) \
    ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonBasic, T_Type, T_Component##T_State) { \
        if(!d->T_Component##OnUpdateListener) { \
            ZFLISTENER_1(callback \
                    , _ZFP_ZFUIButtonBasicPrivate *, d \
                    ) { \
                const ZFProperty *prop = zfargs.param0().to<v_ZFProperty *>()->zfv; \
                d->modMap[zfargs.sender()].insert(prop); \
                if(d->T_Component##View == zfnull) { \
                    d->T_Component##ViewPrepare(); \
                } \
                else { \
                    d->T_Component##StateUpdate(prop); \
                } \
            } ZFLISTENER_END() \
            d->T_Component##OnUpdateListener = callback; \
        } \
        this->T_Component##T_State()->toObject()->observerAdd( \
            ZFObject::E_ObjectPropertyValueOnUpdate(), \
            d->T_Component##OnUpdateListener); \
    } \
    ZFPROPERTY_ON_DETACH_DEFINE(ZFUIButtonBasic, T_Type, T_Component##T_State) { \
        this->T_Component##T_State()->toObject()->observerRemove( \
            ZFObject::E_ObjectPropertyValueOnUpdate(), \
            d->T_Component##OnUpdateListener); \
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
    d->labelStateUpdate();
    d->iconStateUpdate();
    d->bgStateUpdate();
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

