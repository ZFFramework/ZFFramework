#include "ZFUIButtonGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIButtonGroupType)

ZFOBJECT_REGISTER(ZFUIButtonGroup)

ZFEVENT_REGISTER(ZFUIButtonGroup, ButtonOnAdd)
ZFEVENT_REGISTER(ZFUIButtonGroup, ButtonOnRemove)
ZFEVENT_REGISTER(ZFUIButtonGroup, ButtonTabOnUpdate)
ZFEVENT_REGISTER(ZFUIButtonGroup, ButtonTabOnClickChecked)

// ============================================================
// common
static void _ZFP_ZFUIButtonGroup_setup_common(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        , ZF_IN zfindex buttonIndex
        ) {
    if(!buttonGroup->_ZFP_buttonOnEvent) {
        ZFLISTENER_1(buttonEvent
                , ZFUIButtonGroup *, buttonGroup
                ) {
            ZFUIButton *button = zfargs.sender();
            zfindex buttonIndex = buttonGroup->buttonFind(button);
            ZFCoreAssert(buttonIndex != zfindexMax());
            buttonGroup->_ZFP_ZFUIButtonGroup_buttonGroupOnEvent(button, buttonIndex, zfargs.eventId());
        } ZFLISTENER_END()
        buttonGroup->_ZFP_buttonOnEvent = buttonEvent;
    }

    button->observerAdd(ZFUIButton::E_ButtonOnClick(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonStateOnUpdate(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonMouseOnDown(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveEnter(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveExit(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveInside(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveOutside(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonMouseOnUpInside(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerAdd(ZFUIButton::E_ButtonMouseOnUp(), buttonGroup->_ZFP_buttonOnEvent);
}
static void _ZFP_ZFUIButtonGroup_cleanup_common(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        ) {
    button->observerRemove(ZFUIButton::E_ButtonOnClick(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonStateOnUpdate(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonMouseOnDown(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveEnter(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveExit(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveInside(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveOutside(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonMouseOnUpInside(), buttonGroup->_ZFP_buttonOnEvent);
    button->observerRemove(ZFUIButton::E_ButtonMouseOnUp(), buttonGroup->_ZFP_buttonOnEvent);
}

// ============================================================
// v_ZFUIButtonGroupType::e_Normal
static void _ZFP_ZFUIButtonGroup_setup_Normal(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        , ZF_IN zfindex buttonIndex
        ) {
    _ZFP_ZFUIButtonGroup_setup_common(buttonGroup, button, buttonIndex);
}
static void _ZFP_ZFUIButtonGroup_cleanup_Normal(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        ) {
    _ZFP_ZFUIButtonGroup_cleanup_common(buttonGroup, button);
}

// ============================================================
// v_ZFUIButtonGroupType::e_Tab
static void _ZFP_ZFUIButtonGroup_setup_Tab(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        , ZF_IN zfindex buttonIndex
        ) {
    _ZFP_ZFUIButtonGroup_setup_common(buttonGroup, button, buttonIndex);

    button->checkable(zftrue);
    button->checked(zffalse);

    if(!buttonGroup->_ZFP_buttonOnClick) {
        ZFLISTENER_1(buttonOnClick
                , ZFUIButtonGroup *, buttonGroup
                ) {
            ZFUIButton *button = zfargs.sender();
            zfindex buttonIndex = buttonGroup->buttonFind(button);
            ZFCoreAssert(buttonIndex != zfindexMax());
            if(buttonIndex == buttonGroup->buttonTabChecked()) {
                if(buttonGroup->buttonTabAllowUnchecked()) {
                    buttonGroup->buttonTabChecked(zfindexMax());
                    buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnUpdate(button, buttonIndex);
                }
                else {
                    button->checked(zftrue);
                }
                buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnClickChecked(button, buttonIndex);
            }
            else {
                zfindex buttonIndexPrev = buttonGroup->buttonTabChecked();
                buttonGroup->buttonTabChecked(buttonIndex);
                buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnUpdate(button, buttonIndexPrev);
            }
        } ZFLISTENER_END()
        buttonGroup->_ZFP_buttonOnClick = buttonOnClick;
    }
    button->observerAdd(
        ZFUIButton::E_ButtonOnClick(),
        buttonGroup->_ZFP_buttonOnClick);

    if(!buttonGroup->buttonTabAllowUnchecked() && buttonGroup->buttonTabChecked() == zfindexMax()) {
        buttonGroup->buttonTabChecked(buttonIndex);
    }
}
static void _ZFP_ZFUIButtonGroup_cleanup_Tab(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        ) {
    _ZFP_ZFUIButtonGroup_cleanup_common(buttonGroup, button);

    button->observerRemove(
        ZFUIButton::E_ButtonOnClick(),
        buttonGroup->_ZFP_buttonOnClick);
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonGroup, zfbool, buttonTabAllowUnchecked) {
    if(!this->buttonTabAllowUnchecked() && this->buttonCount() > 0 && this->buttonTabChecked() == zfindexMax()) {
        this->buttonTabChecked(0);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIButtonGroup, zfindex, buttonTabChecked) {
    propertyValue = ((propertyValue >= this->buttonCount()) ? zfindexMax() : propertyValue);
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonGroup, zfindex, buttonTabChecked) {
    if(propertyValue != propertyValueOld) {
        if(!this->buttonTabAllowUnchecked() && this->buttonTabChecked() >= this->buttonCount()) {
            return;
        }
        if(propertyValueOld != zfindexMax()) {
            this->buttonAt(propertyValueOld)->checked(zffalse);
        }
        if(this->buttonTabChecked() != zfindexMax()) {
            this->buttonAt(this->buttonTabChecked())->checked(zftrue);
        }
    }
}

// ============================================================
static void _ZFP_ZFUIButtonGroup_setup(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        , ZF_IN zfindex buttonIndex
        ) {
    switch(buttonGroup->type()) {
        case v_ZFUIButtonGroupType::e_Normal:
            _ZFP_ZFUIButtonGroup_setup_Normal(buttonGroup, button, buttonIndex);
            break;
        case v_ZFUIButtonGroupType::e_Tab:
            _ZFP_ZFUIButtonGroup_setup_Tab(buttonGroup, button, buttonIndex);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}
static void _ZFP_ZFUIButtonGroup_cleanup(
        ZF_IN ZFUIButtonGroup *buttonGroup
        , ZF_IN ZFUIButton *button
        ) {
    switch(buttonGroup->type()) {
        case v_ZFUIButtonGroupType::e_Normal:
            _ZFP_ZFUIButtonGroup_cleanup_Normal(buttonGroup, button);
            break;
        case v_ZFUIButtonGroupType::e_Tab:
            _ZFP_ZFUIButtonGroup_cleanup_Tab(buttonGroup, button);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonGroup, ZFUIButtonGroupType, type) {
    if(this->type() == propertyValueOld) {
        return;
    }
    for(zfindex i = 0; i < this->buttonCount(); ++i) {
        _ZFP_ZFUIButtonGroup_cleanup(this, this->buttonAt(i));
    }
    for(zfindex i = 0; i < this->buttonCount(); ++i) {
        _ZFP_ZFUIButtonGroup_setup(this, this->buttonAt(i), i);
    }
}

ZFMETHOD_DEFINE_0(ZFUIButtonGroup, zfindex, buttonCount) {
    return this->_ZFP_buttons->count();
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, zfindex, buttonFind
        , ZFMP_IN(ZFUIButton *, button)
        ) {
    return this->_ZFP_buttons->find(button);
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, zfanyT<ZFUIButton>, buttonAt
        , ZFMP_IN(zfindex, buttonIndex)
        ) {
    return this->_ZFP_buttons->get(buttonIndex);
}
ZFMETHOD_DEFINE_2(ZFUIButtonGroup, void, button
        , ZFMP_IN(ZFUIButton *, button)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        ) {
    if(atIndex == zfindexMax()) {
        atIndex = this->buttonCount();
    }
    else {
        ZFCoreAssertIndexRange(atIndex, this->buttonCount());
    }
    this->_ZFP_buttons->add(button, atIndex);
    _ZFP_ZFUIButtonGroup_setup(this, button, atIndex);
    this->buttonOnAdd(button, atIndex);
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, void, buttonRemove
        , ZFMP_IN(ZFUIButton *, button)
        ) {
    this->buttonRemoveAt(this->_ZFP_buttons->find(button));
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, void, buttonRemoveAt
        , ZFMP_IN(zfindex, buttonIndex)
        ) {
    if(buttonIndex == zfindexMax()) {
        return;
    }
    ZFCoreAssertIndexRange(buttonIndex, this->buttonCount());
    ZFUIButton *button = this->buttonAt(buttonIndex);
    zfRetain(button);
    _ZFP_ZFUIButtonGroup_cleanup(this, button);
    this->_ZFP_buttons->remove(buttonIndex);
    this->buttonOnRemove(button, buttonIndex);
    zfRelease(button);
}
ZFMETHOD_DEFINE_0(ZFUIButtonGroup, void, buttonRemoveAll) {
    while(this->buttonCount() > 0) {
        this->buttonRemoveAt(this->buttonCount() - 1);
    }
}

ZF_NAMESPACE_GLOBAL_END

