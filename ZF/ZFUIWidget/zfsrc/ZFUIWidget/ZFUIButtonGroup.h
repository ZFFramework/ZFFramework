/**
 * @file ZFUIButtonGroup.h
 * @brief abstract button group management
 */

#ifndef _ZFI_ZFUIButtonGroup_h_
#define _ZFI_ZFUIButtonGroup_h_

#include "ZFUIButton.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIButtonGroup_buttons "buttons"

// ============================================================
// ZFUIButtonGroupType
/**
 * @brief button group type
 */
ZFENUM_BEGIN(ZFLIB_ZFUIWidget, ZFUIButtonGroupType)
    /**
     * @brief normal, all buttons are added without any modification
     */
    ZFENUM_VALUE(Normal)
    /**
     * @brief act like tab management, all buttons are changed to checkable,
     *   and only one button can be checked at one time
     *
     * this type can be used as tab buttons and ratio buttons
     */
    ZFENUM_VALUE(Tab)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Tab)
ZFENUM_END(ZFLIB_ZFUIWidget, ZFUIButtonGroupType)

// ============================================================
// ZFUIButtonGroup
/**
 * @brief abstract button group management
 *
 * you have two basic ways to use a button group to manage button views:
 * -  implement from #ZFUIButtonGroup,
 *   override #ZFUIButtonGroup::buttonOnAdd and #ZFUIButtonGroup::buttonOnRemove
 *   to manage button views
 * -  use #ZFUIButtonGroupArray to manage buttons,
 *   and observe #ZFUIButtonGroup::EventButtonOnAdd and #ZFUIButtonGroup::EventButtonOnRemove
 *   to manage button views
 *
 * after buttons added, you may observe #ZFUIButton's button event to achieve your logic\n
 * see #ZFUIButtonGroup::buttonGroupOnEvent for more info
 *
 * serializable data:
 * @code
 *   <SomeButtonGroup>
 *       <ZFArray category="buttons">
 *           <SomeButton category="element" />
 *           <SomeButton category="element" />
 *       </ZFArray>
 *   </SomeButtonGroup>
 * @endcode
 */
zfinterface ZFLIB_ZFUIWidget ZFUIButtonGroup : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFUIButtonGroup, ZFInterface)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button added,
     * sender is the button,
     * param0 is this button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button removed,
     * sender is the button,
     * param0 is this button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonOnRemove)

public:
    /**
     * @brief button group's type, see #ZFUIButtonGroupType::e_Normal for more info
     */
    ZFPROPERTY_ASSIGN(ZFUIButtonGroupTypeEnum, buttonGroupType, ZFUIButtonGroupType::EnumDefault())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIButtonGroupTypeEnum, buttonGroupType)

public:
    /**
     * @brief button count
     */
    ZFMETHOD_DECLARE_0(zfindex, buttonCount)
    /**
     * @brief find the button's index or return zfindexMax() if not exist
     */
    ZFMETHOD_DECLARE_1(zfindex, buttonFind
            , ZFMP_IN(ZFUIButton *, button)
            )
    /**
     * @brief get button at index
     */
    ZFMETHOD_DECLARE_1(ZFUIButton *, buttonAt
            , ZFMP_IN(zfindex, buttonIndex)
            )
    /**
     * @brief add button
     *
     * newly added button's setting would be changed according #buttonGroupType,
     * before #buttonOnAdd
     */
    ZFMETHOD_DECLARE_2(void, buttonAdd
            , ZFMP_IN(ZFUIButton *, button)
            , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
            )
    /**
     * @brief remove button or do nothing if not in this button group
     *
     * removed button's setting won't be reset by this method
     */
    ZFMETHOD_DECLARE_1(void, buttonRemove
            , ZFMP_IN(ZFUIButton *, button)
            )
    /**
     * @brief remove button at index
     *
     * removed button's setting won't be reset by this method
     */
    ZFMETHOD_DECLARE_1(void, buttonRemoveAt
            , ZFMP_IN(zfindex, buttonIndex)
            )
    /**
     * @brief remove all button
     */
    ZFMETHOD_DECLARE_0(void, buttonRemoveAll)

public:
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonGroupOnEvent(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            , ZF_IN zfidentity eventId
            ) {
        this->buttonGroupOnEvent(button, buttonIndex, eventId);
    }
protected:
    /**
     * @brief called when any of button event are notified
     *   to the buttons managed by this group
     *
     * here's a list of button event that would be fired:
     * -  #ZFUIButton::EventButtonOnClick
     * -  #ZFUIButton::EventButtonStateOnUpdate
     * -  #ZFUIButton::EventButtonMouseOnDown
     * -  #ZFUIButton::EventButtonMouseOnMoveEnter
     * -  #ZFUIButton::EventButtonMouseOnMoveExit
     * -  #ZFUIButton::EventButtonMouseOnMoveInside
     * -  #ZFUIButton::EventButtonMouseOnMoveOutside
     * -  #ZFUIButton::EventButtonMouseOnUpInside
     * -  #ZFUIButton::EventButtonMouseOnUp
     *
     * for example:
     * @code
     *   ZFUIButtonGroup *buttonGroup = ...;
     *   buttonGroup->toObject()->observerAdd(ZFUIButton::EventButtonOnClick(), myObserver);
     * @endcode
     * note:
     * sender is the button that fired the button event,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    virtual inline void buttonGroupOnEvent(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            , ZF_IN zfidentity eventId
            ) {
        this->toObject()->observerNotifyWithSender(
            button,
            eventId,
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }
    /** @brief see #EventButtonOnAdd */
    virtual inline void buttonOnAdd(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->toObject()->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::EventButtonOnAdd(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }
    /** @brief see #EventButtonOnRemove */
    virtual inline void buttonOnRemove(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->toObject()->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::EventButtonOnRemove(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }

    // ============================================================
    // for ZFUIButtonGroupType::e_Tab type
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #ZFUIButtonGroupType::e_Tab type only\n
     * called when #buttonTabChecked changed,
     * sender is the newly clicked button,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the previous checked button index
     * (may be #zfindexMax if nothing checked previously)
     */
    ZFOBSERVER_EVENT(ButtonTabOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #ZFUIButtonGroupType::e_Tab type only\n
     * called when checked button clicked,
     * sender is the clicked button,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonTabOnClickChecked)

    /**
     * @brief for #ZFUIButtonGroupType::e_Tab type only, whether allow uncheck all button, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, buttonTabAllowUnchecked, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, buttonTabAllowUnchecked)

    /**
     * @brief for #ZFUIButtonGroupType::e_Tab type only, the checked tab index, zfindexMax() by default
     */
    ZFPROPERTY_ASSIGN(zfindex, buttonTabChecked, zfindexMax())
    ZFPROPERTY_ON_VERIFY_DECLARE(zfindex, buttonTabChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfindex, buttonTabChecked)

public:
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonTabOnChange(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndexPrev
            ) {
        this->buttonTabOnChange(button, buttonIndexPrev);
    }
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonTabOnClickChecked(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->buttonTabOnClickChecked(button, buttonIndex);
    }
protected:
    /** @brief see #EventButtonTabOnChange */
    virtual inline void buttonTabOnChange(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndexPrev
            ) {
        this->toObject()->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::EventButtonTabOnChange(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndexPrev));
    }
    /** @brief see #EventButtonTabOnClickChecked */
    virtual inline void buttonTabOnClickChecked(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->toObject()->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::EventButtonTabOnClickChecked(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }

protected:
    ZFINTERFACE_ON_INIT_DECLARE() {
        _ZFP_buttons = zfAlloc(ZFArray);
    }
    ZFINTERFACE_ON_DEALLOC_DECLARE() {
        zfRetainChange(_ZFP_buttons, zfnull);
    }

protected:
    /** @brief util for serialization */
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        zfautoT<ZFArray *> buttons;
        ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFUIButtonGroup_buttons, ZFObject, buttons, {
                    return zffalse;
                });

        this->buttonRemoveAll();
        for(zfindex i = 0; i < buttons->count(); ++i) {
            ZFUIButton *button = ZFCastZFObject(ZFUIButton *, buttons->get(i));
            if(button == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                    "item %s not type of %s", buttons->get(i), ZFUIButton::ClassData()->className());
                return zffalse;
            }
            this->buttonAdd(button);
        }
        return zftrue;
    }
    /** @brief util for serialization */
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        ZFUIButtonGroup *ref = ZFCastZFObject(ZFUIButtonGroup *, referencedOwnerOrNull);
        ZFSerializableUtilSerializeCategoryToData(serializableData, outErrorHint, ref,
                ZFSerializableKeyword_ZFUIButtonGroup_buttons, ZFObject, this->_ZFP_buttons, ref->_ZFP_buttons, (ZFObject *)zfnull, {
                    return zffalse;
                });
        return zftrue;
    }
    /** @brief util for serialization */
    virtual inline zfbool serializableOnCheckNeedSerializeChildren(void) {
        return zffalse;
    }

public:
    ZFArray *_ZFP_buttons;
    ZFListener _ZFP_buttonOnEvent;
    ZFListener _ZFP_buttonOnClick;
};

// ============================================================
// ZFUIButtonGroupArray
/**
 * @brief simply button group that only holds buttons
 */
zfclass ZFLIB_ZFUIWidget ZFUIButtonGroupArray : zfextend ZFStyleableObject, zfimplement ZFUIButtonGroup {
    ZFOBJECT_DECLARE(ZFUIButtonGroupArray, ZFStyleableObject)
    ZFIMPLEMENT_DECLARE(ZFUIButtonGroup)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        return zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)
            && zfsuperI(ZFUIButtonGroup)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos);
    }
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        return zfsuper::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)
            && zfsuperI(ZFUIButtonGroup)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint);
    }
    zfoverride
    virtual inline zfbool serializableOnCheckNeedSerializeChildren(void) {
        return zfsuperI(ZFUIButtonGroup)::serializableOnCheckNeedSerializeChildren();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonGroup_h_

