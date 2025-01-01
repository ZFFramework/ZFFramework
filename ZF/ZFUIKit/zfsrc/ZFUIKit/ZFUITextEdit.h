/**
 * @file ZFUITextEdit.h
 * @brief view to supply simple edit logic
 */

#ifndef _ZFI_ZFUITextEdit_h_
#define _ZFI_ZFUITextEdit_h_

#include "ZFUITextView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextEditKeyboardType
/**
 * @brief keyboard type for text input
 *
 * note this is only a hint for native keyboard to show proper input type,
 * the actual behavior depends on native keyboard\n
 * for advanced keyboard management, you should supply your own native code if necessary
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUITextEditKeyboardType)
    ZFENUM_VALUE(Normal) /**< @brief can input any word */
    ZFENUM_VALUE(CharBased) /**< @brief input char only */
    ZFENUM_VALUE(PhonePad) /**< @brief input phone number only */
    ZFENUM_VALUE(NumberPad) /**< @brief input number only */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(CharBased)
    ZFENUM_VALUE_REGISTER(PhonePad)
    ZFENUM_VALUE_REGISTER(NumberPad)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUITextEditKeyboardType)

// ============================================================
// ZFUITextEditKeyboardReturnType
/**
 * @brief keyboard's return button's type for text input
 *
 * note this is only a hint for native keyboard to show proper input type,
 * the actual behavior depends on native keyboard\n
 * for advanced keyboard management, you should supply your own native code if necessary
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUITextEditKeyboardReturnType)
    ZFENUM_VALUE(Normal) /**< @brief normal return key */
    ZFENUM_VALUE(Next) /**< @brief go to next */
    ZFENUM_VALUE(Search) /**< @brief search */
    ZFENUM_VALUE(Done) /**< @brief done */
    ZFENUM_VALUE(Go) /**< @brief go */
    ZFENUM_VALUE(Send) /**< @brief send */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Next)
    ZFENUM_VALUE_REGISTER(Search)
    ZFENUM_VALUE_REGISTER(Done)
    ZFENUM_VALUE_REGISTER(Go)
    ZFENUM_VALUE_REGISTER(Send)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUITextEditKeyboardReturnType)

// ============================================================
// ZFUITextEditKeyboardReturnAction
/**
 * @brief action to perform when click return
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUITextEditKeyboardReturnAction)
    ZFENUM_VALUE(None) /**< @brief do nothing */
    ZFENUM_VALUE(Confirm) /**< @brief confirm only, keep focus and keyboard state */
    ZFENUM_VALUE(FocusNext) /**< @brief confirm and move focus to next */
    ZFENUM_VALUE(HideKeyboard) /**< @brief confirm and hide on screen keyboard if showing */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(None)
    ZFENUM_VALUE_REGISTER(Confirm)
    ZFENUM_VALUE_REGISTER(FocusNext)
    ZFENUM_VALUE_REGISTER(HideKeyboard)
ZFENUM_END_WITH_DEFAULT(ZFLIB_ZFUIKit, ZFUITextEditKeyboardReturnAction, FocusNext)

zfclassFwd _ZFP_ZFUITextEditPrivate;
// ============================================================
// ZFUITextEdit
/**
 * @brief view to supply simple edit logic
 *
 * @note this view is used for simple text editing only,
 *   for simple text display, use #ZFUITextView,
 *   for rich text edit, you should supply your own implementation
 * @note this view supply quite simple text edit logic,
 *   designed to be lightweighted and able to be embeded to other complex views
 */
zfclass ZFLIB_ZFUIKit ZFUITextEdit : zfextend ZFUIView, zfimplement ZFUIText {
    ZFOBJECT_DECLARE(ZFUITextEdit, ZFUIView)
    ZFIMPLEMENT_DECLARE(ZFUIText)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextEdit)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     */
    ZFEVENT(TextOnEditBegin)
    /**
     * @brief see #ZFObject::observerNotify
     */
    ZFEVENT(TextOnEditEnd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when text is about to change\n
     * param0 is the text (as #v_zfstring) that about to change\n
     * param1 is a #v_zfbool
     * shows whether the text should change,
     * set to false to show the text should not be changed\n
     * @note if #ZFUITextEdit::editFilter has been set,
     *   it would be checked first
     *   and store result to param1,
     *   you may change the value to modify the filter result
     */
    ZFEVENT(TextOnUpdateCheck)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when text changed,
     * param0 is the old text (as #v_zfstring)
     */
    ZFEVENT(TextOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when return clicked
     */
    ZFEVENT(TextOnReturnClick)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * util event fired when return clicked or focus lost,
     * or, you may manually notify it by #editConfirm
     */
    ZFEVENT(TextOnEditConfirm)

public:
    // ============================================================
    // properties
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, focusable)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUISize, viewSizeMin)

public:
    /**
     * @brief whether the text is editable, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, editEnable, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, editEnable)
    /**
     * @brief whether the text is secured, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, editSecured)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, editSecured)
    /**
     * @brief keyboard hint, see #ZFUITextEditKeyboardType, #v_ZFUITextEditKeyboardType::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN(ZFUITextEditKeyboardType, keyboardType, v_ZFUITextEditKeyboardType::EnumDefault())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextEditKeyboardType, keyboardType)
    /**
     * @brief keyboard hint, see #ZFUITextEditKeyboardReturnType, #v_ZFUITextEditKeyboardReturnType::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN(ZFUITextEditKeyboardReturnType, keyboardReturnType, v_ZFUITextEditKeyboardReturnType::EnumDefault())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextEditKeyboardReturnType, keyboardReturnType)
    /**
     * @brief action to perform when click return, see #ZFUITextEditKeyboardReturnAction, #v_ZFUITextEditKeyboardReturnAction::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN(ZFUITextEditKeyboardReturnAction, keyboardReturnAction, v_ZFUITextEditKeyboardReturnAction::EnumDefault())
    /**
     * @brief text place holder
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, placeholder, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, placeholder)

    /**
     * @brief text filter, null by default
     *
     * #text must match exactly the filter,
     * otherwise text would not be changed
     * @note if text doesn't match filter while changing filter,
     *   text would be replaced to null
     * @note null or empty text would always treated as match for safe
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFRegExp>, editFilter)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFRegExp>, editFilter)

    /**
     * @brief text edit's cursor position
     */
    ZFPROPERTY_ASSIGN(ZFIndexRange, selectedRange, ZFIndexRangeZero())
    ZFPROPERTY_ON_VERIFY_DECLARE(ZFIndexRange, selectedRange)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFIndexRange, selectedRange)

    /**
     * @brief whether #editConfirm when lost focus, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, confirmWhenLostFocus, zftrue)

public:
    ZFPROPERTY_ON_VERIFY_DECLARE(zfstring, text)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, text)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextAppearance, textAppearance)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIAlignFlags, textAlign)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIColor, textColor)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, textSize)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief measure text view
     *
     * this is separate from UIView's measure step,
     * usually called by app to measure text's size,
     * and usually have no need to call\n
     * sizeHint shows max size the text view may have, or -1 if no limit
     */
    ZFMETHOD_DECLARE_2(void, measureTextEdit
            , ZFMP_OUT(ZFUISize &, ret)
            , ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero())
            )

public:
    zffinal void _ZFP_ZFUITextEdit_textNotifyBeginEdit(void);
    zffinal void _ZFP_ZFUITextEdit_textNotifyEndEdit(void);
    zffinal void _ZFP_ZFUITextEdit_textNotifyUpdate(ZF_IN const zfstring &newText);
    zffinal void _ZFP_ZFUITextEdit_selectedRangeNotifyUpdate(void);
    zffinal void _ZFP_ZFUITextEdit_textNotifyReturnClicked(void);
    /**
     * @brief whether text should change
     *
     * by default, this method would check according to #ZFUITextEdit::editFilter,
     * and null or empty string would always treated as allowed for safe
     */
    ZFMETHOD_DECLARE_1(zfbool, textShouldUpdate
            , ZFMP_IN(const zfstring &, newText)
            )
    /**
     * @brief manually start edit
     */
    ZFMETHOD_DECLARE_0(void, editBegin)
    /**
     * @brief manually start edit
     */
    ZFMETHOD_DECLARE_0(void, editEnd)
    /**
     * @brief true if editing text, typically keyboard is showing
     */
    ZFMETHOD_DECLARE_0(zfbool, editing)

protected:
    /** @brief see #E_TextOnEditBegin */
    virtual void textOnEditBegin(void);
    /** @brief see #E_TextOnEditEnd */
    virtual void textOnEditEnd(void);
    /** @brief see #E_TextOnUpdateCheck */
    virtual void textOnUpdateCheck(
            ZF_IN const zfstring &newText
            , ZF_IN_OUT zfbool &shouldUpdate
            );
    /** @brief see #E_TextOnUpdate */
    virtual void textOnUpdate(ZF_IN const zfstring &oldText);
    /** @brief see #E_TextOnReturnClick */
    virtual void textOnReturnClick(void);
    /** @brief see #E_TextOnEditConfirm */
    virtual void textOnEditConfirm(void);
public:
    /** @brief see #E_TextOnEditConfirm */
    ZFMETHOD_DECLARE_0(void, editConfirm)

    // ============================================================
    // override
protected:
    /**
     * @brief text edit view would update text settings when scale changed
     */
    zfoverride
    virtual void UIScaleOnUpdate(void);
    /**
     * @brief text edit view would measure according text size
     */
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    /**
     * @brief text edit view would resolve all key event by default,
     *   except tab and shift
     */
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent);
    zfoverride
    virtual void focusOnUpdate(void);

    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);
    zfoverride
    virtual void internalViewOnLayout(ZF_IN const ZFUIRect &bounds);

private:
    _ZFP_ZFUITextEditPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextEdit_h_

