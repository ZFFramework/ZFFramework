#include "ZFUIImageViewExt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCLASS_EXTEND(ZFUIImageView, ZFUIImageViewExt)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageUrl) {
    if(propertyValue && propertyValueOld != propertyValue) {
        this->imageSrc(zfnull);
        this->imageOnLoad(ZFInputForHttp(this->imageUrl()));
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageSrc) {
    if(propertyValue && propertyValueOld != propertyValue) {
        this->imageUrl(zfnull);
        this->imageOnLoad(propertyValue);
    }
}

void ZFUIImageViewExt::imageOnLoad(ZF_IN const ZFInput &src) {
    ZFUIImageView *owner = zfcast(ZFUIImageView *, this);
    if(owner == zfnull) {
        return;
    }
    if(!src) {
        if(owner->image() == zfnull) {
            owner->image(this->imageLoadFail());
        }
    }
    else {
        owner->image(ZFUIImageAsync(src, this->imageLoadFail(), this->imageLoading()));
    }
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFUIImageViewExt, ZFSerializablePropertyType, serializableOnCheckPropertyType
        , ZFMP_IN(const ZFProperty *, property)
        ) {
    if(zffalse
            || property == ZFPropertyAccess(ZFUIImageViewExt, imageUrl)
            || property == ZFPropertyAccess(ZFUIImageViewExt, imageSrc)
            ) {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    return ZFSerializablePropertyTypeUnspecified;
}

ZF_NAMESPACE_GLOBAL_END
