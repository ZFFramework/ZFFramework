/**
 * @file ZFProtocolZFUITextView.h
 * @brief protocol for #ZFUITextView
 */

#ifndef _ZFI_ZFProtocolZFUITextView_h_
#define _ZFI_ZFProtocolZFUITextView_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUITextView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUITextView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUITextView)
public:
    /**
     * @brief create native text view
     */
    virtual void *nativeTextViewCreate(
            ZF_IN ZFUITextView *textView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) zfpurevirtual;
    /**
     * @brief destroy native text view
     */
    virtual void nativeTextViewDestroy(
            ZF_IN ZFUITextView *textView
            , ZF_IN void *nativeTextView
            ) zfpurevirtual;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUITextView */
    virtual void text(
            ZF_IN ZFUITextView *textView
            , ZF_IN const zfstring &text
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textAppearance(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textAlign(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textColor(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIColor const &textColor
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textSize(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSize
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textSizeAutoMin(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMin
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textSizeAutoMax(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMax
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void singleLine(
            ZF_IN ZFUITextView *textView
            , ZF_IN zfbool singleLine
            ) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textTruncateMode(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextTruncateMode const &textTruncateMode
            ) zfpurevirtual;

    // ============================================================
    // layout
public:
    /**
     * @brief measure text view using size hint
     *
     * implementation should truncate or wrap new line according to text style previously set by setTextStyle\n
     * sizeHint shows max size the text view may have, or 0 if no limit
     */
    virtual ZFUISize measureNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) zfpurevirtual;

    /**
     * @brief see #ZFUITextView::textSizeCurrent
     */
    virtual zffloat textSizeCurrent(ZF_IN ZFUITextView *textView) zfpurevirtual;

    /**
     * @brief layout text view after ZFUIView's layout step,
     *   so that the view may shrink text size if necessary
     */
    virtual void layoutNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &viewSize
            ) zfpurevirtual;

    // ============================================================
    // util method
public:
    /**
     * @brief util method to calculate text size according auto change setting for this text view
     *
     * usually used by implementation that doesn't support text auto resizing,
     * loop to measure and calculate proper text size,
     * may have performance issues
     */
    zffloat calcTextSizeAuto(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &sizeHint
            ) {
        zffloat curTextSize = textView->textSize();
        ZFUISize curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->UIScaleFixed()));

        if(textView->textSizeAutoMax() > 0) {
            while(curTextSize < textView->textSizeAutoMax()
                    && ((sizeHint.width <= 0 || curSize.width < sizeHint.width)
                        && (sizeHint.height <= 0 || curSize.height < sizeHint.height))
                        ) {
                ++curTextSize;
                curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->UIScaleFixed()));
            }
        }

        if(textView->textSizeAutoMin() > 0) {
            while(curTextSize > textView->textSizeAutoMin()
                    && ((sizeHint.width > 0 && curSize.width > sizeHint.width)
                        || (sizeHint.height > 0 && curSize.height > sizeHint.height))
                        ) {
                --curTextSize;
                curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->UIScaleFixed()));
            }
        }

        return ZFUISizeApplyScale(curTextSize, textView->UIScaleFixed());
    }
ZFPROTOCOL_INTERFACE_END(ZFUITextView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUITextView_h_

