/**
 * @file ZFUIHint_Basic.h
 * @brief common types of #ZFUIHint
 */

#ifndef _ZFI_ZFUIHint_Basic_h_
#define _ZFI_ZFUIHint_Basic_h_

#include "ZFUIHint.h"
#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
/**
 * @brief #ZFUIHint's content with simple text and background
 */
zfclass ZFLIB_ZFUIWidget ZFUIHintContentBasic : zfextend ZFUIButtonBasic {
    ZFOBJECT_DECLARE(ZFUIHintContentBasic, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIHintContentBasic)

public:
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, viewUIEnableTree)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUISize, viewSizeMin)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, labelNormal)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, bgNormal)
};

/**
 * @brief make a hint with text
 *
 * the hint would:
 * -  #ZFUIHintContentBasic as the #ZFUIHint::content
 * -  have an alpha change animation as the hint's show and hide animation
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIWidget, zfautoT<ZFUIHint>, ZFUIHintCreate
        , ZFMP_IN(const zfstring &, text)
        , ZFMP_IN_OPT(ZFUIImage *, icon, zfnull)
        )
/**
 * @brief #ZFUIHintCreate and show the hint
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIWidget, zfautoT<ZFUIHint>, ZFUIHintShow
        , ZFMP_IN(const zfstring &, text)
        , ZFMP_IN_OPT(ZFUIImage *, icon, zfnull)
        ) {
    zfautoT<ZFUIHint> hint = ZFUIHintCreate(text, icon);
    hint->show();
    return hint;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIHint_Basic_h_

