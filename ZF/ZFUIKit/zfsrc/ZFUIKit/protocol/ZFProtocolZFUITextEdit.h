/**
 * @file ZFProtocolZFUITextEdit.h
 * @brief protocol for #ZFUITextEdit
 */

#ifndef _ZFI_ZFProtocolZFUITextEdit_h_
#define _ZFI_ZFProtocolZFUITextEdit_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUITextEdit.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUITextEdit
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUITextEdit)
public:
    /**
     * @brief create native text edit view
     */
    virtual void *nativeTextEditCreate(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) zfpurevirtual;
    /**
     * @brief destroy native text edit view
     */
    virtual void nativeTextEditDestroy(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN void *nativeTextEdit
            ) zfpurevirtual;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUITextEdit */
    virtual void editEnable(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editEnable
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textEditSecure(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editSecured
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void keyboardType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardType keyboardType
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void keyboardReturnType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardReturnType keyboardReturnType
            ) zfpurevirtual;

    /** @brief see #ZFUITextEdit */
    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT ZFIndexRange &selectedRange
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFIndexRange &selectedRange
            ) zfpurevirtual;

public:
    /** @brief see #ZFUITextEdit */
    virtual void text(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const zfstring &text
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textAppearance(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textAlign(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textColor(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIColor const &textColor
            ) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textSize(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zffloat textSize
            ) zfpurevirtual;

    // ============================================================
    // layout
public:
    /**
     * @brief measure text edit view using size hint
     *
     * implementation should truncate or wrap new line according to text style previously set by setTextStyle\n
     * sizeHint shows max size the text edit view may have, or 0 if no limit
     */
    virtual ZFUISize measureNativeTextEdit(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) zfpurevirtual;

    // ============================================================
    // edit
public:
    /** @brief see #ZFUITextEdit::editBegin */
    virtual void editBegin(ZF_IN ZFUITextEdit *textEdit) zfpurevirtual;
    /** @brief see #ZFUITextEdit::editEnd */
    virtual void editEnd(ZF_IN ZFUITextEdit *textEdit) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must notify to check whether text should change
     */
    zffinal zfbool notifyCheckTextShouldUpdate(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const zfstring &newValue
            ) {
        return textEdit->textShouldUpdate(newValue);
    }
    /**
     * @brief implementation must notify when text changed
     */
    zffinal void notifyTextUpdate(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const zfstring &newValue
            ) {
        textEdit->_ZFP_ZFUITextEdit_textNotifyUpdate(newValue);
    }
    /**
     * @brief implementation must notify when text select range changed
     */
    zffinal void notifyTextSelectRangeOnUpdate(ZF_IN ZFUITextEdit *textEdit) {
        textEdit->_ZFP_ZFUITextEdit_selectedRangeNotifyUpdate();
    }
    /**
     * @brief implementation must notify when text changed
     */
    zffinal void notifyTextReturnClicked(ZF_IN ZFUITextEdit *textEdit) {
        textEdit->_ZFP_ZFUITextEdit_textNotifyReturnClicked();
    }
    /**
     * @brief implementation must notify when text begin edit
     */
    zffinal void notifyTextEditBegin(ZF_IN ZFUITextEdit *textEdit) {
        textEdit->_ZFP_ZFUITextEdit_textNotifyBeginEdit();
    }
    /**
     * @brief implementation must notify when text begin edit
     */
    zffinal void notifyTextEditEnd(ZF_IN ZFUITextEdit *textEdit) {
        textEdit->_ZFP_ZFUITextEdit_textNotifyEndEdit();
    }
ZFPROTOCOL_INTERFACE_END(ZFUITextEdit)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUITextEdit_h_

