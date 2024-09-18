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

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, zfanyT<ZFUIImageView>, iconNormal) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonRatio_IconNormal.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, zfanyT<ZFUIImageView>, iconHighlighted) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonRatio_IconHighlighted.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, zfanyT<ZFUIImageView>, iconChecked) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonRatio_IconChecked.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonRatio, zfanyT<ZFUIImageView>, iconCheckedHighlighted) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonRatio_IconCheckedHighlighted.png"));
}

ZF_NAMESPACE_GLOBAL_END

