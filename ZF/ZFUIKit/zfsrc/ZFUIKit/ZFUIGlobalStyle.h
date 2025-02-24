/**
 * @file ZFUIGlobalStyle.h
 * @brief global style for ui elements
 */

#ifndef _ZFI_ZFUIGlobalStyle_h_
#define _ZFI_ZFUIGlobalStyle_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global style for ui elements
 */
zfclass ZFLIB_ZFUIKit ZFUIGlobalStyle : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFUIGlobalStyle, ZFStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIGlobalStyle)

public:
    // ============================================================
    // image scale
    /**
     * @brief resource image's base scale, 2 by default to suit most case,
     *   see #ZFUIImage::imageScale for more info
     *
     * scale 2 means, 80 pixel size image would be treated as 40 size in #ZFUIView tree,
     * which should suit for most physical screen,
     * if you want to support higher screen resolution,
     * supply larger pixel image and increase this scale value
     */
    ZFPROPERTY_ASSIGN(zffloat, imageScale, 2.0f)

    // ============================================================
    // common size hints
    /**
     * @brief space between inner items
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSpace, 8)
    /**
     * @brief space to outer containers
     */
    ZFPROPERTY_ASSIGN(zffloat, itemMargin, 8)

    // ============================================================
    // item size hints
    /**
     * @brief text item size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeText, 21)
    /**
     * @brief small control item size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeControl, 32)
    /**
     * @brief button size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeButton, 44)
    /**
     * @brief button size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeButtonWidth, 68)
    /**
     * @brief icon size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeIcon, 40)
    /**
     * @brief icon size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeIconSmall, 32)
    /**
     * @brief list cell size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeCell, 48)
    /**
     * @brief dialog item size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeDialog, 80)
    /**
     * @brief dialog item size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, itemSizeDialogWidth, 200)

    // ============================================================
    // text size hints
    /**
     * @brief extra tiny text size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeExtraTiny, 9)
    /**
     * @brief tiny text size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeTiny, 11)
    /**
     * @brief small text size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeSmall, 13)
    /**
     * @brief normal text size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeNormal, 16)
    /**
     * @brief big text size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeBig, 19)
    /**
     * @brief large text size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeLarge, 22)
    /**
     * @brief extra large text size hint
     */
    ZFPROPERTY_ASSIGN(zffloat, textSizeExtraLarge, 25)

    // ============================================================
    // text settings
    /**
     * @brief default text appearance
     */
    ZFPROPERTY_ASSIGN(ZFUITextAppearance, textAppearance, v_ZFUITextAppearance::EnumDefault())
    /**
     * @brief default text align
     */
    ZFPROPERTY_ASSIGN(ZFUIAlignFlags, textAlign, ZFUIAlign::e_Left)
    /**
     * @brief default text truncate mode
     */
    ZFPROPERTY_ASSIGN(ZFUITextTruncateMode, textTruncateMode, v_ZFUITextTruncateMode::EnumDefault())

    // ============================================================
    // generanl colors
    /**
     * @brief default background color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, colorBgDefault, ZFUIColorGrayLight())
    /**
     * @brief item background color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, colorBgItem, ZFUIColorWhite())
    /**
     * @brief separator color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, colorSeparator, ZFUIColorGrayLight())
    /**
     * @brief shadow color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, colorShadow, ZFUIColorWithA(ZFUIColorBlack(), 0.5f))
    /**
     * @brief mask color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, colorMask, ZFUIColorWithA(ZFUIColorBlack(), 0.5f))
    /**
     * @brief tint color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, colorTint, ZFUIColorGrayLight())

    // ============================================================
    // text colors
    /**
     * @brief default text color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, textColorDefault, ZFUIColorBlack())
    /**
     * @brief secondary default text color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, textColorSecondary, ZFUIColorGray())
    /**
     * @brief hint text color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, textColorHint, ZFUIColorGrayLight())

    // ============================================================
    // control
    /**
     * @brief normal control color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, controlColorNormal, ZFUIColorBlack())
    /**
     * @brief highlighted control color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, controlColorHighlighted, ZFUIColorBlack())
    /**
     * @brief checked control color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, controlColorChecked, ZFUIColorBlack())
    /**
     * @brief checked highlighted control color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, controlColorCheckedHighlighted, ZFUIColorBlack())
    /**
     * @brief disabled control color
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, controlColorDisabled, ZFUIColorGray())
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIGlobalStyle_h_

