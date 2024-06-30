#include "ZFUIImageViewExt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCLASS_EXTEND(ZFUIImageView, ZFUIImageViewExt)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageUrl) {
    if(propertyValue) {
        this->imageSrc(zfnull);
        this->imageOnLoad(ZFInputForHttp(this->imageUrl()));
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageViewExt, zfstring, imageSrc) {
    if(propertyValue) {
        this->imageUrl(zfnull);
        this->imageOnLoad(propertyValue);
    }
}

void ZFUIImageViewExt::imageOnLoad(ZF_IN const ZFInput &src) {
    if(_taskId) {
        ZFUIImageLoadCancel(_taskId);
        _taskId = zfnull;
    }
    ZFUIImageView *owner = zfcast(ZFUIImageView *, this);
    if(owner == zfnull) {
        return;
    }
    if(!this->imageSrc()) {
        if(owner->image() == zfnull) {
            owner->image(this->imageFail());
        }
        return;
    }
    if(owner->image() == zfnull) {
        owner->image(this->imageLoading());
    }
    ZFLISTENER_1(loadOnFinish
            , zfweakT<ZFUIImageView>, owner
            ) {
        if(!owner) {
            return;
        }
        zfcast(zfself *, owner)->_taskId = zfnull;
        ZFUIImage *result = zfargs.param0();
        if(result != zfnull) {
            owner->image(result);
        }
        else {
            owner->image(zfcast(zfself *, owner)->imageFail());
        }
    } ZFLISTENER_END()
    _taskId = ZFUIImageLoad(this->imageSrc(), loadOnFinish);
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFUIImageViewExt, ZFSerializablePropertyType, serializableOnCheckPropertyType
        , ZFMP_IN(const ZFProperty *, property)
        ) {
    if(property == ZFPropertyAccess(ZFUIImageView, image)
            && (this->imageUrl() || this->imageSrc())
            ) {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    return ZFSerializablePropertyTypeUnspecified;
}

ZF_NAMESPACE_GLOBAL_END

