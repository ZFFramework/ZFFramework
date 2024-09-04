#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"
#include "protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_ZFUIImageFromInputCacheData {
public:
    zfstring inputId;
    void *nativeImage;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageFromInputCacheHolder, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIImageFromInputCacheHolder) {
    zfCoreMutexLocker();
    while(!this->cache.isEmpty()) {
        _ZFP_ZFUIImageFromInputCacheData cacheData = this->cache.removeFirstAndGet();
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(cacheData.nativeImage);
    }
}
public:
    ZFCoreArray<_ZFP_ZFUIImageFromInputCacheData> cache;
ZF_GLOBAL_INITIALIZER_END(ZFUIImageFromInputCacheHolder)

static void *_ZFP_ZFUIImageFromInput(ZF_IN const ZFInput &input) {
    if(zfstringIsEmpty(input.callbackId())) {
        return ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(input);
    }

    zfCoreMutexLock();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageFromInputCacheHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageFromInputCacheHolder);
    for(zfindex i = 0; i < d->cache.count(); ++i) {
        if(zfstringIsEqual(d->cache[i].inputId, input.callbackId())) {
            void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(d->cache[i].nativeImage);
            if(i != d->cache.count() - 1) {
                d->cache.add(d->cache.removeAndGet(i));
            }
            zfCoreMutexUnlock();
            return nativeImage;
        }
    }
    zfCoreMutexUnlock();

    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(input);
    if(nativeImage == zfnull) {
        return zfnull;
    }

    zfCoreMutexLocker();
    _ZFP_ZFUIImageFromInputCacheData cacheData;
    cacheData.inputId = input.callbackId();
    cacheData.nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(nativeImage);
    d->cache.add(cacheData);

    while(d->cache.count() > 5) {
        _ZFP_ZFUIImageFromInputCacheData cacheData = d->cache.removeFirstAndGet();
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(cacheData.nativeImage);
    }

    return nativeImage;
}

// ============================================================
// raw image io
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFUIImage>, ZFUIImageFromBase64
        , ZFMP_IN(const ZFInput &, inputCallback)
        ) {
    zfobj<ZFIOBufferByCacheFile> io;
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(image != zfnull && ZFBase64Decode(io->output(), inputCallback)) {
        void *nativeImage = _ZFP_ZFUIImageFromInput(io->input());
        if(nativeImage != zfnull) {
            image->nativeImage(nativeImage, zffalse);
            return ret;
        }
    }
    return zfnull;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFUIImageToBase64
        , ZFMP_OUT(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFUIImage *, image)
        ) {
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback) {
        zfobj<ZFIOBufferByCacheFile> io;
        if(!ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), io->output())) {
            return zffalse;
        }
        return ZFBase64Encode(outputCallback, io->input());
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFUIImage>, ZFUIImageFromInput
        , ZFMP_IN(const ZFInput &, inputCallback)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(image == zfnull || !inputCallback) {
        return zfnull;
    }
    void *nativeImage = _ZFP_ZFUIImageFromInput(inputCallback);
    if(nativeImage == zfnull) {
        return zfnull;
    }
    image->nativeImage(nativeImage, zffalse);

    if(!inputCallback.callbackSerializeDisable()) {
        ZFSerializableData inputData;
        if(ZFCallbackToDataT(inputData, inputCallback)) {
            ZFSerializableData customData;
            customData.itemClass(ZFUIImage::ClassData()->className());
            inputData.category(ZFSerializableKeyword_ZFUIImageIO_input_imageData);
            customData.childAdd(inputData);

            image->imageSerializeType(ZFUIImageSerializeType_input);
            image->imageSerializeData(customData);
        }
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFUIImageToOutput
        , ZFMP_OUT(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFUIImage *, image)
        ) {
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback) {
        return ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), outputCallback);
    }
    return zffalse;
}

ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(input, ZFUIImageSerializeType_input) {
    ZFCallback input;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImageIO_input_imageData, ZFCallback, input, {
                return zffalse;
            });
    if(!input) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "invalid callback");
        return zffalse;
    }
    void *nativeImage = _ZFP_ZFUIImageFromInput(input);
    if(nativeImage == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "load image failed");
        return zffalse;
    }
    ret->nativeImage(nativeImage, zffalse);
    return zftrue;
}

// ============================================================
// ZFUIImageScale
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFUIImage>, ZFUIImageScale
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN(const ZFUISize &, newSize)
        ) {
    if(image == zfnull || image->nativeImage() == zfnull) {
        return zfnull;
    }
    zffloat scale = image->imageScaleFixed();
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageApplyScale(
        scale,
        image->nativeImage(),
        ZFUISizeApplyScale(newSize, scale),
        ZFUIMarginApplyScale(image->imageNinePatch(), scale));
    zfautoT<ZFUIImage> ret = ZFUIImageFromNativeImage(nativeImage, zffalse);
    return ret;
}

// ============================================================
// ZFUIImageInFrame
static zfbool _ZFP_ZFUIImageInFrame(
        ZF_IN_OUT ZFUIImage *ret
        , ZF_IN ZFUIImage *image
        , ZF_IN const ZFUIRect &frame
        , ZF_IN zfbool storeSerializableData
        ) {
    if(ret == zfnull || image == zfnull) {
        return zffalse;
    }
    const ZFUISize &imageSize = image->imageSize();
    if(frame.x < 0 || frame.x >= imageSize.width
            || frame.y < 0 || frame.y >= imageSize.height
            ) {
        return zffalse;
    }
    ZFUIRect frameFixed = frame;
    if(frameFixed.width <= 0 || frameFixed.x + frameFixed.width > imageSize.width) {
        frameFixed.width = imageSize.width - frameFixed.x;
    }
    if(frameFixed.height <= 0 || frameFixed.y + frameFixed.height > imageSize.height) {
        frameFixed.height = imageSize.height - frameFixed.y;
    }
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadInFrame(
        image->imageScaleFixed(),
        image->nativeImage(),
        ZFUIRectApplyScale(frameFixed, image->imageScaleFixed()));
    ret->nativeImage(nativeImage, zffalse);

    // only store custom type for performance
    if(storeSerializableData && (image->imageSerializeType() || image->imageSerializeDataGetter())) {
        ZFSerializableData data;
        data.itemClass(ZFSerializableKeyword_node);
        ZFSerializableData refData;
        zfstring frameString;
        if(image->serializeToData(refData) && ZFUIRectToStringT(frameString, frameFixed)) {
            data.attr(ZFSerializableKeyword_ZFUIImageIO_ref_refFrame, frameString);
            refData.category(ZFSerializableKeyword_ZFUIImageIO_ref_ref);
            data.childAdd(refData);
            ret->imageSerializeType(ZFUIImageSerializeType_ref);
            ret->imageSerializeData(data);
        }
    }

    return zftrue;
}
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ref, ZFUIImageSerializeType_ref) {
    serializableData.resolveMark();

    zfautoT<ZFUIImage> ref;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImageIO_ref_ref, ZFObject, ref, {
                return zffalse;
            });
    if(ref == zfnull) {
        return zffalse;
    }

    ZFUIRect frame = ZFUIRectCreate(ZFUIPointZero(), ref->imageSize());
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageIO_ref_refFrame, ZFUIRect, frame, {
                return zffalse;
            });

    return _ZFP_ZFUIImageInFrame(ret, ref, frame, zffalse);;
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFUIImage>, ZFUIImageInFrame
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN(const ZFUIRect &, frame)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(_ZFP_ZFUIImageInFrame(ret, image, frame, zftrue)) {
        return ret;
    }
    else {
        return zfnull;
    }
}

// ============================================================
// ZFUIImageFromNativeImage
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFUIImage>, ZFUIImageFromNativeImage
        , ZFMP_IN(void *, nativeImage)
        , ZFMP_IN_OPT(zfbool, retainNativeImage, zftrue)
        ) {
    if(nativeImage == zfnull) {
        return zfnull;
    }
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    ret->nativeImage(nativeImage);
    return ret;
}

// ============================================================
// color
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(color, ZFUIImageSerializeType_color) {
    ZFUIColor color = ZFUIColorZero();
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageIO_color_color, ZFUIColor, color, {
                return zffalse;
            });

    ZFUISize size = ZFUISizeCreate(1);
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageIO_color_size, ZFUISize, size, {
                return zffalse;
            });

    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(size, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    ret->nativeImage(nativeImage, zffalse);
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFUIImage>, ZFUIImageFromColor
        , ZFMP_IN(const ZFUIColor &, color)
        , ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero())
        ) {
    ZFUISize sizeTmp = ZFUISizeCreate(zfmMax((zffloat)1, size.width), zfmMax((zffloat)1, size.height));
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(sizeTmp, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    zfautoT<ZFUIImage> ret = ZFUIImageFromNativeImage(nativeImage, zffalse);
    ZFUIImage *image = ret;
    if(image == zfnull) {
        return zfnull;
    }

    ZFSerializableData imageData;
    imageData.itemClass(ZFSerializableKeyword_node);
    {
        ZFSerializableUtilSerializeAttributeToDataNoRef(imageData, zfnull,
                ZFSerializableKeyword_ZFUIImageIO_color_color, ZFUIColor, color, ZFUIColorZero(), {
                    return zfnull;
                });

        ZFSerializableUtilSerializeAttributeToDataNoRef(imageData, zfnull,
                ZFSerializableKeyword_ZFUIImageIO_color_size, ZFUISize, sizeTmp, ZFUISizeCreate(1, 1), {
                    return zfnull;
                });
    }
    image->imageSerializeType(ZFUIImageSerializeType_color);
    image->imageSerializeData(imageData);

    return ret;
}

ZF_NAMESPACE_GLOBAL_END

