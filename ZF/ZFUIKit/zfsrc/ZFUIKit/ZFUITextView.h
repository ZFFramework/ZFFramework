/**
 * @file ZFUITextView.h
 * @brief view to display plain text
 */

#ifndef _ZFI_ZFUITextView_h_
#define _ZFI_ZFUITextView_h_

#include "ZFUIText.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextView
/**
 * @brief view to display plain text
 *
 * for editable or selectable text view, you should use #ZFUITextView,
 * for rich text, you should supply you own native view,
 * or use web view with html strings
 */
zfclass ZFLIB_ZFUIKit ZFUITextView : zfextend ZFUIView, zfimplement ZFUIText {
    ZFOBJECT_DECLARE(ZFUITextView, ZFUIView)
    ZFIMPLEMENT_DECLARE(ZFUIText)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextView)

public:
    // ============================================================
    // properties
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, viewUIEnable)
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, viewUIEnableTree)

public:
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, text)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextAppearance, textAppearance)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIAlignFlags, textAlign)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIColor, textColor)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, textSize)

public:
    /**
     * @brief if not 0, auto decrease text size
     *   when the view isn't big enough to hold current text,
     *   #ZFUIGlobalStyle::textSizeTiny by default
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeAutoMin, ZFUIGlobalStyle::DefaultStyle()->textSizeTiny())
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, textSizeAutoMin)
    /**
     * @brief if not 0 and larger than #textSizeAutoMin,
     *   auto increase text size when the view is bigger than current text need,
     *   0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeAutoMax, 0)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, textSizeAutoMax)

    /**
     * @brief single line or not, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, singleLine, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, singleLine)

    /**
     * @brief text truncate mode, #v_ZFUITextTruncateMode::e_Disable by default
     */
    ZFPROPERTY_ASSIGN(ZFUITextTruncateMode, textTruncateMode, ZFUIGlobalStyle::DefaultStyle()->textTruncateMode())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextTruncateMode, textTruncateMode)

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
     * sizeHint shows max size the text view may have, or 0 if no limit
     */
    ZFMETHOD_DECLARE_2(void, measureTextView
            , ZFMP_OUT(ZFUISize &, ret)
            , ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero())
            )

    /**
     * @brief get current text size
     *
     * if the view hasn't been layouted, the size may be invalid
     */
    ZFMETHOD_DECLARE_0(zffloat, textSizeCurrent)

    // ============================================================
    // override
protected:
    /**
     * @brief we would update text settings when scale changed
     */
    zfoverride
    virtual void UIScaleOnUpdate(void);
    /**
     * @brief we would measure according text size
     */
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    zfoverride
    virtual void internalViewOnLayout(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextView_h_

