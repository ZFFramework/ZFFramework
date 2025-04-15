/**
 * @file ZFUIButtonGroup.h
 * @brief button group util
 */

#ifndef _ZFI_ZFUIButtonGroup_h_
#define _ZFI_ZFUIButtonGroup_h_

#include "ZFUIButton.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonGroupType
/**
 * @brief button group type
 */
ZFENUM_BEGIN(ZFLIB_ZFUIWidget, ZFUIButtonGroupType)
    /**
     * @brief act like tab management, all buttons are changed to checkable,
     *   and only one button can be checked at one time
     *
     * this type can be used as tab buttons and ratio buttons
     */
    ZFENUM_VALUE(Tab)
    /**
     * @brief normal, all buttons are added without any modification
     */
    ZFENUM_VALUE(Normal)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Tab)
    ZFENUM_VALUE_REGISTER(Normal)
ZFENUM_END(ZFLIB_ZFUIWidget, ZFUIButtonGroupType)
ZFENUM_REG(ZFLIB_ZFUIWidget, ZFUIButtonGroupType)

// ============================================================
// ZFUIButtonGroup
zfclassFwd _ZFP_ZFUIButtonGroupPrivate;
/**
 * @brief button group util
 *
 * a button group is simply a abstract button manager,
 * you need to add buttons to both button group and view tree
 */
zfclass ZFLIB_ZFUIWidget ZFUIButtonGroup : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFUIButtonGroup, ZFStyle)

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
    ZFEVENT(ButtonOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button removed,
     * sender is the button,
     * param0 is this button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFEVENT(ButtonOnRemove)

public:
    /**
     * @brief button group's type, see #v_ZFUIButtonGroupType::e_Normal for more info
     */
    ZFPROPERTY_ASSIGN(ZFUIButtonGroupType, type, v_ZFUIButtonGroupType::EnumDefault())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIButtonGroupType, type)

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
    ZFMETHOD_DECLARE_1(zfanyT<ZFUIButton>, buttonAt
            , ZFMP_IN(zfindex, buttonIndex)
            )
    /**
     * @brief add button
     *
     * newly added button's setting would be changed according #type,
     * before #buttonOnAdd
     */
    ZFMETHOD_DECLARE_2(void, button
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

protected:
    /**
     * @brief called when any of button event are notified
     *   to the buttons managed by this group
     *
     * here's a list of button event that would be fired:
     * -  #ZFUIButton::E_ButtonOnClick
     * -  #ZFUIButton::E_ButtonStateOnUpdate
     * -  #ZFUIButton::E_ButtonMouseOnDown
     * -  #ZFUIButton::E_ButtonMouseOnMoveEnter
     * -  #ZFUIButton::E_ButtonMouseOnMoveExit
     * -  #ZFUIButton::E_ButtonMouseOnMoveInside
     * -  #ZFUIButton::E_ButtonMouseOnMoveOutside
     * -  #ZFUIButton::E_ButtonMouseOnUpInside
     * -  #ZFUIButton::E_ButtonMouseOnUp
     *
     * for example:
     * @code
     *   ZFUIButtonGroup *buttonGroup = ...;
     *   buttonGroup->->observerAdd(ZFUIButton::E_ButtonOnClick(), myObserver);
     * @endcode
     * note:
     * sender is the button that fired the button event,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    virtual inline void buttonOnEvent(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            , ZF_IN zfidentity eventId
            ) {
        this->observerNotifyWithSender(
            button,
            eventId,
            this,
            zfobj<v_zfindex>(buttonIndex));
    }
    /** @brief see #E_ButtonOnAdd */
    virtual inline void buttonOnAdd(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::E_ButtonOnAdd(),
            this,
            zfobj<v_zfindex>(buttonIndex));
    }
    /** @brief see #E_ButtonOnRemove */
    virtual inline void buttonOnRemove(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::E_ButtonOnRemove(),
            this,
            zfobj<v_zfindex>(buttonIndex));
    }

    // ============================================================
    // for v_ZFUIButtonGroupType::e_Tab type
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #v_ZFUIButtonGroupType::e_Tab type only\n
     * called when #tabChecked changed,
     * sender is the newly clicked button,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the previous checked button index
     * (may be #zfindexMax if nothing checked previously)
     */
    ZFEVENT(TabOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #v_ZFUIButtonGroupType::e_Tab type only\n
     * called when checked button clicked,
     * sender is the clicked button,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFEVENT(TabOnClickChecked)

    /**
     * @brief for #v_ZFUIButtonGroupType::e_Tab type only, whether allow uncheck all button, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, tabAllowUnchecked, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, tabAllowUnchecked)

    /**
     * @brief for #v_ZFUIButtonGroupType::e_Tab type only, the checked tab index, zfindexMax() by default
     */
    ZFPROPERTY_ASSIGN(zfindex, tabChecked, zfindexMax())
    ZFPROPERTY_ON_UPDATE_DECLARE(zfindex, tabChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfindex, tabChecked)

public:
    /**
     * @brief util method to attach container to this button group
     *
     * if a container is attached:
     * -  all children of the container would be added as child button of this button group,
     *   if the child is type of #ZFUIButton
     * -  when adding or removing child to container,
     *   the child would also be added or removed to this button group,
     *   if the child is type of #ZFUIButton
     */
    ZFMETHOD_DECLARE_1(void, containerAttach
            , ZFMP_IN(ZFUIView *, container)
            )
    /** @brief detach container, and remove all buttons, see #containerAttach */
    ZFMETHOD_DECLARE_0(void, containerDetach)
    /** @brief the container currently attached, see #containerAttach */
    ZFMETHOD_DECLARE_0(ZFUIView *, container)

    /** @brief construct with #containerAttach */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(ZFUIView *, container)
            )

protected:
    /** @brief see #E_TabOnUpdate */
    virtual inline void tabOnUpdate(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndexPrev
            ) {
        this->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::E_TabOnUpdate(),
            this,
            zfobj<v_zfindex>(buttonIndexPrev));
    }
    /** @brief see #E_TabOnClickChecked */
    virtual inline void tabOnClickChecked(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->observerNotifyWithSender(
            button,
            ZFUIButtonGroup::E_TabOnClickChecked(),
            this,
            zfobj<v_zfindex>(buttonIndex));
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUIButtonGroupPrivate *d;
    friend zfclassFwd _ZFP_ZFUIButtonGroupPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonGroup_h_

