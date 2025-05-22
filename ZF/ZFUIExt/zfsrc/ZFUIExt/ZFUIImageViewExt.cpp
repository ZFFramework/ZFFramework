#include "ZFUIImageViewExt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCLASS_EXTEND(ZFUIImageView, ZFUIImageViewExt)

// ============================================================
static void _ZFP_ZFUIImageView_imageSrc(
        ZF_IN ZFUIImageViewExt *owner
        , ZF_IN const ZFInput &src
        ) {
    ZFUIImageView *imageView = zfcast(ZFUIImageView *, owner);
    if(imageView == zfnull) {
        return;
    }
    if(!src) {
        if(imageView->image() == zfnull) {
            imageView->image(owner->imageLoadFail());
        }
    }
    else {
        imageView->image(ZFUIImageAsync(src, owner->imageLoadFail(), owner->imageLoading()));
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageUrl) {
    if(propertyValue && propertyValueOld != propertyValue) {
        this->imageSrc(zfnull);
        _ZFP_ZFUIImageView_imageSrc(this, ZFInputForHttp(this->imageUrl()));
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageSrc) {
    if(propertyValue && propertyValueOld != propertyValue) {
        this->imageUrl(zfnull);
        _ZFP_ZFUIImageView_imageSrc(this, propertyValue);
    }
}

// ============================================================
static void _ZFP_ZFUIImageView_imageAniSrc(
        ZF_IN ZFUIImageViewExt *owner
        , ZF_IN const ZFInput &src
        ) {
    ZFUIImageView *imageView = zfcast(ZFUIImageView *, owner);
    if(imageView == zfnull) {
        return;
    }
    if(!src) {
        imageView->image(zfnull);
    }
    else {
        imageView->image(ZFUIImageAniLoad(src));
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageAniUrl) {
    if(propertyValue && propertyValueOld != propertyValue) {
        this->imageAniSrc(zfnull);
        _ZFP_ZFUIImageView_imageAniSrc(this, ZFInputForHttp(this->imageAniUrl()));
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageAniSrc) {
    if(propertyValue && propertyValueOld != propertyValue) {
        this->imageAniUrl(zfnull);
        _ZFP_ZFUIImageView_imageAniSrc(this, propertyValue);
    }
}

ZFMETHOD_DEFINE_4(ZFUIImageViewExt, void, imageAniUrl
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        ) {
    zfautoT<ZFUIImage> holder = ZFUIImageAniLoad(ZFInputForHttp(url));
    if(holder) {
        this->to<ZFUIImageView *>()->image(holder);
    }
    else {
        this->to<ZFUIImageView *>()->image(ZFUIImageAni(ZFInputForHttp(url), frameSize, frameCount, frameDuration));
    }
}
ZFMETHOD_DEFINE_4(ZFUIImageViewExt, void, imageAniSrc
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        ) {
    zfautoT<ZFUIImage> holder = ZFUIImageAniLoad(src);
    if(holder) {
        this->to<ZFUIImageView *>()->image(holder);
    }
    else {
        this->to<ZFUIImageView *>()->image(ZFUIImageAni(src, frameSize, frameCount, frameDuration));
    }
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFUIImageViewExt, ZFSerializablePropertyType, serializableOnCheckPropertyType
        , ZFMP_IN(const ZFProperty *, property)
        ) {
    if(zffalse
            || property == ZFPropertyAccess(ZFUIImageViewExt, imageUrl)
            || property == ZFPropertyAccess(ZFUIImageViewExt, imageSrc)
            || property == ZFPropertyAccess(ZFUIImageViewExt, imageAniUrl)
            || property == ZFPropertyAccess(ZFUIImageViewExt, imageAniSrc)
            ) {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    return ZFSerializablePropertyTypeUnspecified;
}

ZF_NAMESPACE_GLOBAL_END

