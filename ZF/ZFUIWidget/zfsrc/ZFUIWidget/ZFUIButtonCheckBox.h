/**
 * @file ZFUIButtonCheckBox.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonCheckBox_h_
#define _ZFI_ZFUIButtonCheckBox_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonCheckBox
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZFLIB_ZFUIWidget ZFUIButtonCheckBox : zfextends ZFUIButtonBasic {
    ZFOBJECT_DECLARE(ZFUIButtonCheckBox, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonCheckBox)

public:
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, checkable)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIAlignFlags, contentAlign)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconNormal)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconHighlighted)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconChecked)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconCheckedHighlighted)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonCheckBox_h_

