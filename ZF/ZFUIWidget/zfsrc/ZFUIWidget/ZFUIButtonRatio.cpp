#include "ZFUIButtonRatio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIButtonRatio)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonRatio)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, zfbool, checkable) {
    propertyValue = zftrue;
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIAlignFlags, contentAlign) {
    propertyValue = ZFUIAlign::e_Left;
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, iconNormal) {
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconNormal.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, iconHighlighted) {
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconHighlighted.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, iconChecked) {
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconChecked.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, iconCheckedHighlighted) {
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconCheckedHighlighted.png"));
}

ZF_NAMESPACE_GLOBAL_END

