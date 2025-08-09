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
    ZFCoreMutexLocker();
    while(!this->cache.isEmpty()) {
        _ZFP_ZFUIImageFromInputCacheData cacheData = this->cache.removeFirstAndGet();
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(cacheData.nativeImage);
    }
}
public:
    ZFCoreArray<_ZFP_ZFUIImageFromInputCacheData> cache;
ZF_GLOBAL_INITIALIZER_END(ZFUIImageFromInputCacheHolder)

static void *_ZFP_ZFUIImageFromInput(ZF_IN const ZFInput &input) {
    if(!input.callbackId()) {
        return ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(input);
    }

    ZFCoreMutexLock();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageFromInputCacheHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageFromInputCacheHolder);
    for(zfindex i = 0; i < d->cache.count(); ++i) {
        if(zfstringIsEqual(d->cache[i].inputId, input.callbackId())) {
            void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(d->cache[i].nativeImage);
            if(i != d->cache.count() - 1) {
                d->cache.add(d->cache.removeAndGet(i));
            }
            ZFCoreMutexUnlock();
            return nativeImage;
        }
    }
    ZFCoreMutexUnlock();

    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(input);
    if(nativeImage == zfnull) {
        return zfnull;
    }

    ZFCoreMutexLocker();
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
    zfobj<ZFIOBuffer> io;
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(image != zfnull && ZFBase64Decode(io->output(), inputCallback)) {
        io->input().ioSeek(0);
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
        zfobj<ZFIOBuffer> io;
        if(!ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), io->output())) {
            return zffalse;
        }
        io->input().ioSeek(0);
        return ZFBase64Encode(outputCallback, io->input());
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFUIImage>, ZFUIImageFromInput
        , ZFMP_IN(const ZFInput &, inputCallback)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageFromInputT(ret, inputCallback)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFUIImageFromInputT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, inputCallback)
        ) {
    if(ret == zfnull || !inputCallback) {
        return zffalse;
    }
    void *nativeImage = _ZFP_ZFUIImageFromInput(inputCallback);
    if(nativeImage == zfnull) {
        return zffalse;
    }
    ret->nativeImage(nativeImage, zffalse);

    if(!inputCallback.callbackSerializeDisable()
            && !ret->imageSerializeDisable()
            ) {
        ZFSerializableData inputData;
        if(ZFCallbackToDataT(inputData, inputCallback)) {
            ZFSerializableData customData;
            customData.itemClass(ZFUIImage::ClassData()->className());
            inputData.category(ZFSerializableKeyword_ZFUIImageFromInput_imageData);
            customData.child(inputData);

            ret->imageSerializeType(ZFUIImageSerializeType_ZFUIImageFromInput);
            ret->imageSerializeData(customData);
        }
    }
    return zftrue;
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

ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(input, ZFUIImageSerializeType_ZFUIImageFromInput) {
    ZFCallback input;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImageFromInput_imageData, ZFCallback, input, {
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
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ref, ZFUIImageSerializeType_ZFUIImageInFrame) {
    serializableData.resolveMark();

    zfautoT<ZFUIImage> ref;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImageInFrame_ref, ZFObject, ref, {
                return zffalse;
            });
    if(ref == zfnull) {
        return zffalse;
    }

    ZFUIRect frame = ZFUIRectCreate(ZFUIPointZero(), ref->imageSize());
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageInFrame_refFrame, ZFUIRect, frame, {
                return zffalse;
            });

    ret->imageSerializeDisable(zftrue);
    return ZFUIImageInFrameT(ret, ref, frame);
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFUIImage>, ZFUIImageInFrame
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN(const ZFUIRect &, frame)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageInFrameT(ret, image, frame)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIImageInFrameT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN(const ZFUIRect &, frame)
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
    if(!ret->imageSerializeDisable()
            && !image->imageSerializeDisable() && (image->imageSerializeType() || image->imageSerializeDataGetter())
            ) {
        ZFSerializableData data;
        data.itemClass(ZFSerializableKeyword_node);
        ZFSerializableData refData;
        zfstring frameString;
        if(image->serializeToData(refData) && ZFUIRectToStringT(frameString, frameFixed)) {
            data.attr(ZFSerializableKeyword_ZFUIImageInFrame_refFrame, frameString);
            refData.category(ZFSerializableKeyword_ZFUIImageInFrame_ref);
            data.child(refData);
            ret->imageSerializeType(ZFUIImageSerializeType_ZFUIImageInFrame);
            ret->imageSerializeData(data);
        }
    }

    return zftrue;
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
    ret->nativeImage(nativeImage, retainNativeImage);
    return ret;
}

// ============================================================
// color
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(color, ZFUIImageSerializeType_ZFUIImageFromColor) {
    ZFUIColor color = ZFUIColorZero();
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageFromColor_color, ZFUIColor, color, {
                return zffalse;
            });

    ZFUISize size = ZFUISizeCreate(1);
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageFromColor_size, ZFUISize, size, {
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
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageFromColorT(ret, color, size)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIImageFromColorT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFUIColor &, color)
        , ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero())
        ) {
    if(ret == zfnull) {
        return zffalse;
    }
    ZFUISize sizeTmp = ZFUISizeCreate(zfmMax((zffloat)1, size.width), zfmMax((zffloat)1, size.height));
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(sizeTmp, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    ret->nativeImage(nativeImage, zffalse);

    if(!ret->imageSerializeDisable()) {
        ZFSerializableData imageData;
        imageData.itemClass(ZFSerializableKeyword_node);
        {
            ZFSerializableUtilSerializeAttrToDataNoRef(imageData, zfnull,
                    ZFSerializableKeyword_ZFUIImageFromColor_color, ZFUIColor, color, ZFUIColorZero(), {
                        return zffalse;
                    });

            ZFSerializableUtilSerializeAttrToDataNoRef(imageData, zfnull,
                    ZFSerializableKeyword_ZFUIImageFromColor_size, ZFUISize, sizeTmp, ZFUISizeCreate(1, 1), {
                        return zffalse;
                    });
        }
        ret->imageSerializeType(ZFUIImageSerializeType_ZFUIImageFromColor);
        ret->imageSerializeData(imageData);
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

