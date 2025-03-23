/**
 * @file ZFUIText.h
 * @brief base UI text interface
 */

#ifndef _ZFI_ZFUIText_h_
#define _ZFI_ZFUIText_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief base UI text interface
 */
zfinterface ZFLIB_ZFUIKit ZFUIText : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFUIText, ZFInterface)

public:
    /**
     * @brief text, may be null if not set
     */
    ZFPROPERTY_ASSIGN(zfstring, text)

    /**
     * @brief text appearance, #ZFUIGlobalStyle::textAppearance by default
     *
     * note, for some implementations and font settings, italic or bold may or may not be supported
     */
    ZFPROPERTY_ASSIGN(ZFUITextAppearance, textAppearance, ZFUIGlobalStyle::DefaultStyle()->textAppearance())

    /**
     * @brief text alignment, #ZFUIGlobalStyle::textAlign by default
     *
     * @note only LeftInner, Center, RightInner would be supported for sure
     */
    ZFPROPERTY_ASSIGN(ZFUIAlignFlags, textAlign, ZFUIGlobalStyle::DefaultStyle()->textAlign())

    /**
     * @brief text color, #ZFUIGlobalStyle::textColorDefault by default
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, textColor, ZFUIGlobalStyle::DefaultStyle()->textColorDefault())

    /**
     * @brief text size, #ZFUIGlobalStyle::textSizeNormal by default
     */
    ZFPROPERTY_ASSIGN(zffloat, textSize, ZFUIGlobalStyle::DefaultStyle()->textSizeNormal())
};

/**
 * @brief text config
 */
zfclass ZFLIB_ZFUIKit ZFUITextConfig : zfextend ZFStyle, zfimplement ZFUIText {
    ZFOBJECT_DECLARE(ZFUITextConfig, ZFStyle)
    ZFIMPLEMENT_DECLARE(ZFUIText)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextConfig)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIText_h_

