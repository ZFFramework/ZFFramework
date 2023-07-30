/**
 * @file ZFUIButtonRatio.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonRatio_h_
#define _ZFI_ZFUIButtonRatio_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonRatio
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZFLIB_ZFUIWidget ZFUIButtonRatio : zfextends ZFUIButtonBasic {
    ZFOBJECT_DECLARE(ZFUIButtonRatio, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonRatio)

public:
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, checkable)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIAlignFlags, contentAlign)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconNormal)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconHighlighted)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconChecked)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconCheckedHighlighted)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonRatio_h_

