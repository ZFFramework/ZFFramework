#include "ZFUIButtonCheckBox.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIButtonCheckBox)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonCheckBox)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonCheckBox, zfbool, checkable) {
    propertyValue = zftrue;
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonCheckBox, ZFUIAlignFlags, contentAlign) {
    propertyValue = ZFUIAlign::e_Left;
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonCheckBox, zfanyT<ZFUIImageView>, iconNormal) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonCheckBox_IconNormal.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonCheckBox, zfanyT<ZFUIImageView>, iconHighlighted) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonCheckBox_IconHighlighted.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonCheckBox, zfanyT<ZFUIImageView>, iconChecked) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonCheckBox_IconChecked.png"));
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonCheckBox, zfanyT<ZFUIImageView>, iconCheckedHighlighted) {
    propertyValue.to<ZFUIImageView *>()->image(zfres("ZFUIWidget/ZFUIButtonCheckBox_IconCheckedHighlighted.png"));
}

ZF_NAMESPACE_GLOBAL_END

