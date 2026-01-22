#include "ZFUIHint_Basic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
ZFSTYLE_DEFAULT_DEFINE(ZFUIHintContentBasic)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, zfbool, viewUIEnableTree) {
    propertyValue = zffalse;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, ZFUISize, viewSizeMin) {
    propertyValue = ZFUISizeCreate(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, zfanyT<ZFUITextView>, labelNormal) {
    ZFUITextView *value = propertyValue;
    value->textColor(ZFUIColorCreateRGB(0xFFFFFF));
    value->singleLine(zffalse);
    value->textSizeAutoMin(0);
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, zfanyT<ZFUIImageView>, bgNormal) {
    ZFUIImageView *value = propertyValue;
    value->image(zfres("ZFUIWidget/ZFUIHintContentBasic_bg.xml"));
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFUIHint>, ZFUIHintCreate
        , ZFMP_IN(const zfstring &, text)
        , ZFMP_IN_OPT(ZFUIImage *, icon, zfnull)
        ) {
    zfobj<ZFUIHint> hint;

    hint->aniShow(ZFAni("alpha", zfobj<v_zffloat>(0), zfobj<v_zffloat>(1)));
    hint->aniHide(ZFAni("alpha", zfobj<v_zffloat>(1), zfobj<v_zffloat>(0)));

    zfobj<ZFUIHintContentBasic> content;
    hint->content(content);
    content->label()->text(text);
    content->icon()->image(icon);

    return hint;
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(zfautoT<ZFUIHint>, ZFUIHintShow
        , ZFMP_IN(const zfstring &, text)
        , ZFMP_IN_OPT(ZFUIImage *, icon, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END

