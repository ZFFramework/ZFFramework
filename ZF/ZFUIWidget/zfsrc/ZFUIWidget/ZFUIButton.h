/**
 * @file ZFUIButton.h
 * @brief abstract button
 */

#ifndef _ZFI_ZFUIButton_h_
#define _ZFI_ZFUIButton_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonState
/**
 * @brief button state
 */
ZFENUM_BEGIN(ZFLIB_ZFUIWidget, ZFUIButtonState)
    ZFENUM_VALUE(Normal) /**< @brief normal state */
    ZFENUM_VALUE(Highlighted) /**< @brief pressed state */
    ZFENUM_VALUE(Checked) /**< @brief checked state */
    ZFENUM_VALUE(CheckedHighlighted) /**< @brief pressed when in checked state */
    ZFENUM_VALUE(Disabled) /**< @brief disabled state */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Highlighted)
    ZFENUM_VALUE_REGISTER(Checked)
    ZFENUM_VALUE_REGISTER(CheckedHighlighted)
    ZFENUM_VALUE_REGISTER(Disabled)
ZFENUM_END(ZFLIB_ZFUIWidget, ZFUIButtonState)

// ============================================================
// ZFUIButton
zfclassFwd _ZFP_ZFUIButtonPrivate;
/**
 * @brief abstract button
 *
 * use #ZFUIButton::E_ButtonOnClick to register button click listener,
 * see #ZFObject::observerNotify for more info\n
 * #ZFUIButton is an abstract button, use its subclass to create
 */
zfclass ZFLIB_ZFUIWidget ZFUIButton : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIButton, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButton)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button clicked
     * (for both checkable and non-checkable button)\n
     * param0 is the #ZFUIMouseEvent or #ZFUIKeyEvent that cause the button click,
     * or null for other clicked reason, such as #simulateClick
     */
    ZFEVENT(ButtonOnClick)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button init finished and each time button state update
     */
    ZFEVENT(ButtonStateOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button mouse down,
     * param0 is the #ZFUIMouseEvent
     */
    ZFEVENT(ButtonMouseOnDown)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move from outside to inside,
     * param0 is the #ZFUIMouseEvent
     */
    ZFEVENT(ButtonMouseOnMoveEnter)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move from inside to outside,
     * param0 is the #ZFUIMouseEvent
     */
    ZFEVENT(ButtonMouseOnMoveExit)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move inside the button,
     * param0 is the #ZFUIMouseEvent
     */
    ZFEVENT(ButtonMouseOnMoveInside)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move outside the button,
     * param0 is the #ZFUIMouseEvent
     */
    ZFEVENT(ButtonMouseOnMoveOutside)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button mouse up inside (clicked),
     * param0 is the #ZFUIMouseEvent
     */
    ZFEVENT(ButtonMouseOnUpInside)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button mouse up (outside or cancel),
     * param0 is the #ZFUIMouseEvent
     */
    ZFEVENT(ButtonMouseOnUp)

public:
    // ============================================================
    // properties
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, focusable)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUISize, viewSizeMin)

    /**
     * @brief true if the button is enabled, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, enable, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, enable)
    /**
     * @brief true if the button is checkable button, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, checkable, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, checkable)
    /**
     * @brief true if the button is checked, valid only if #checkable, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, checked, zffalse)
    ZFPROPERTY_ON_VERIFY_DECLARE(zfbool, checked)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, checked)
    /**
     * @brief mouse up tolerance to detect as click, (0 - #ZFUIGlobalStyle::itemMargin) by default
     *
     * a negative value would increase the button area while mouse up,
     * so that button is easier to be clicked,
     * and a positive value would reduce the button area
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, clickTolerance, ZFUIMarginCreate(- ZFUIGlobalStyle::DefaultStyle()->itemMargin()))
    /**
     * @brief if two click event's interval smaller than this value,
     *   the second one would be ignored,
     *   100 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, clickInterval, 100)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief reset #clickInterval so that next click event would be
     *   ensured to be fired
     */
    ZFMETHOD_DECLARE_0(void, clickIntervalReset)

public:
    /**
     * @brief util method to simulate click button
     *
     * the optional event paran would be passed to #E_ButtonOnClick
     */
    ZFMETHOD_DECLARE_1(void, simulateClick
            , ZFMP_IN_OPT(ZFUIEvent *, event, zfnull)
            )

    /**
     * @brief util to observe #E_ButtonOnClick
     */
    ZFMETHOD_DECLARE_2(void, onClick
            , ZFMP_IN(const ZFListener &, observer)
            , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
            )
    /**
     * @brief util to observe #E_ButtonOnClick
     */
    ZFMETHOD_DECLARE_2(void, onClickForOnce
            , ZFMP_IN(const ZFListener &, observer)
            , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
            )

protected:
    zfoverride
    virtual void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent);
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent);

protected:
    /** @brief see #E_ButtonOnClick */
    virtual inline void buttonOnClick(ZF_IN ZFUIEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonOnClick(), event);
    }

protected:
    /** @brief see #E_ButtonMouseOnDown */
    virtual void buttonMouseOnDown(ZF_IN ZFUIMouseEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonMouseOnDown(), event);
    }
    /** @brief see #E_ButtonMouseOnMoveEnter */
    virtual void buttonMouseOnMoveEnter(ZF_IN ZFUIMouseEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonMouseOnMoveEnter(), event);
    }
    /** @brief see #E_ButtonMouseOnMoveExit */
    virtual void buttonMouseOnMoveExit(ZF_IN ZFUIMouseEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonMouseOnMoveExit(), event);
    }
    /** @brief see #E_ButtonMouseOnMoveInside */
    virtual void buttonMouseOnMoveInside(ZF_IN ZFUIMouseEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonMouseOnMoveInside(), event);
    }
    /** @brief see #E_ButtonMouseOnMoveOutside */
    virtual void buttonMouseOnMoveOutside(ZF_IN ZFUIMouseEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonMouseOnMoveOutside(), event);
    }
    /** @brief see #E_ButtonMouseOnUpInside */
    virtual void buttonMouseOnUpInside(ZF_IN ZFUIMouseEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonMouseOnUpInside(), event);
    }
    /** @brief see #E_ButtonMouseOnUp */
    virtual void buttonMouseOnUp(ZF_IN ZFUIMouseEvent *event) {
        this->observerNotify(ZFUIButton::E_ButtonMouseOnUp(), event);
    }

public:
    /**
     * @brief current button state
     */
    ZFMETHOD_DECLARE_0(ZFUIButtonState, buttonState)

protected:
    /** @brief see #E_ButtonStateOnUpdate */
    virtual inline void buttonStateOnUpdate(void) {
        this->observerNotify(ZFUIButton::E_ButtonStateOnUpdate());
    }

private:
    _ZFP_ZFUIButtonPrivate *d;
    friend zfclassFwd _ZFP_ZFUIButtonPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButton_h_

