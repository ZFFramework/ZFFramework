#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"
#include "protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// raw image io
ZFMETHOD_FUNC_DEFINE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromBase64,
                       ZFMP_IN(const ZFInput &, inputCallback))
{
    zfblockedAlloc(ZFIOBufferByCacheFile, io);
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(image != zfnull && ZFBase64Decode(io->output(), inputCallback))
    {
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(io->input());
        if(nativeImage != zfnull)
        {
            image->nativeImage(nativeImage, zffalse);
            return ret;
        }
    }
    return zfnull;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFUIImageSaveToBase64,
                       ZFMP_OUT(const ZFOutput &, outputCallback),
                       ZFMP_IN(ZFUIImage *, image))
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback)
    {
        zfblockedAlloc(ZFIOBufferByCacheFile, io);
        if(!ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), io->output()))
        {
            return zffalse;
        }
        return ZFBase64Encode(outputCallback, io->input());
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromFile,
                       ZFMP_IN(const ZFInput &, inputCallback))
{
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(image == zfnull || !inputCallback)
    {
        return zfnull;
    }
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(inputCallback);
    if(nativeImage == zfnull)
    {
        return zfnull;
    }
    image->nativeImage(nativeImage, zffalse);

    if(!inputCallback.callbackSerializeCustomDisabled())
    {
        ZFSerializableData inputData;
        if(ZFCallbackToData(inputData, inputCallback))
        {
            image->imageSerializableType(ZFUIImageSerializeType_input);
            image->imageSerializableData(&inputData);
        }
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFUIImageSaveToFile,
                       ZFMP_OUT(const ZFOutput &, outputCallback),
                       ZFMP_IN(ZFUIImage *, image))
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback)
    {
        return ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), outputCallback);
    }
    return zffalse;
}

ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(input, ZFUIImageSerializeType_input)
{
    ZFCallback input;
    if(!ZFCallbackFromData(input, serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    if(!input)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "invalid callback");
        return zffalse;
    }
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(input);
    if(nativeImage == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "load image failed");
        return zffalse;
    }
    ret->nativeImage(nativeImage, zffalse);
    return zftrue;
}

// ============================================================
// ZFUIImageScale
ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageScale,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN(const ZFUISize &, newSize))
{
    if(image == zfnull || image->nativeImage() == zfnull)
    {
        return zfnull;
    }
    zffloat scale = image->imageScaleFixed();
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageApplyScale(
        scale,
        image->nativeImage(),
        ZFUISizeApplyScale(newSize, scale),
        ZFUIMarginApplyScale(image->imageNinePatch(), scale));
    zfautoObjectT<ZFUIImage *> ret = ZFUIImageLoadFromNativeImage(nativeImage, zffalse);
    return ret;
}

// ============================================================
// ZFUIImageLoadInFrame
static zfbool _ZFP_ZFUIImageLoadInFrame(ZF_IN_OUT ZFUIImage *ret,
                                        ZF_IN ZFUIImage *image,
                                        ZF_IN const ZFUIRect &framePixel,
                                        ZF_IN zfbool storeSerializableData)
{
    if(ret == zfnull || image == zfnull)
    {
        return zffalse;
    }
    const ZFUISize &imageSizeFixed = image->imageSizeFixed();
    if(framePixel.x < 0 || framePixel.x >= imageSizeFixed.width
        || framePixel.y < 0 || framePixel.y >= imageSizeFixed.height)
    {
        return zffalse;
    }
    ZFUIRect frame = framePixel;
    if(frame.width <= 0 || frame.x + frame.width > imageSizeFixed.width)
    {
        frame.width = imageSizeFixed.width - frame.x;
    }
    if(frame.height <= 0 || frame.y + frame.height > imageSizeFixed.height)
    {
        frame.height = imageSizeFixed.height - frame.y;
    }
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadInFrame(
        image->imageScaleFixed(),
        image->nativeImage(),
        framePixel);
    ret->nativeImage(nativeImage, zffalse);

    // only store custom type for performance
    if(storeSerializableData && image->imageSerializableType() != zfnull)
    {
        ZFSerializableData data;
        data.itemClass(ZFSerializableKeyword_node);
        ZFSerializableData refData;
        zfstring frameString;
        if(image->serializeToData(refData) && ZFUIRectToString(frameString, frame))
        {
            data.attr(ZFSerializableKeyword_ZFUIImageIO_ref_frame, frameString);
            refData.category(ZFSerializableKeyword_ZFUIImageIO_ref);
            data.childAdd(refData);
            ret->imageSerializableType(ZFUIImageSerializeType_ref);
            ret->imageSerializableData(&data);
        }
    }

    return zftrue;
}
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ref, ZFUIImageSerializeType_ref)
{
    serializableData.resolveMark();

    zfautoObjectT<ZFUIImage *> ref;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
        require, ZFSerializableKeyword_ZFUIImageIO_ref, ZFObject, ref);
    if(ref == zfnull)
    {
        return zffalse;
    }

    ZFUIRect frame = ZFUIRectMake(ZFUIPointZero(), ref->imageSizeFixed());
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFUIImageIO_ref_frame, ZFUIRect, frame);

    return _ZFP_ZFUIImageLoadInFrame(ret, ref, frame, zffalse);;
}

ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadInFrame,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN(const ZFUIRect &, framePixel))
{
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    if(_ZFP_ZFUIImageLoadInFrame(ret, image, framePixel, zftrue))
    {
        return ret;
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
// ZFUIImageLoadFromNativeImage
ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromNativeImage,
                       ZFMP_IN(void *, nativeImage),
                       ZFMP_IN_OPT(zfbool, retainNativeImage, zftrue))
{
    if(nativeImage == zfnull)
    {
        return zfnull;
    }
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    ret.to<ZFUIImage *>()->nativeImage(nativeImage);
    return ret;
}

// ============================================================
// color
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(color, ZFUIImageSerializeType_color)
{
    ZFUIColor color = ZFUIColorZero();
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFUIImageIO_color, ZFUIColor, color);

    ZFUISize size = ZFUISizeMake(1);
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFUIImageIO_color_size, ZFUISize, size);

    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(size, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    ret->nativeImage(nativeImage, zffalse);
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromColor,
                       ZFMP_IN(const ZFUIColor &, color),
                       ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero()))
{
    ZFUISize sizeTmp = ZFUISizeMake(zfmMax((zffloat)1, size.width), zfmMax((zffloat)1, size.height));
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(sizeTmp, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    zfautoObjectT<ZFUIImage *> ret = ZFUIImageLoadFromNativeImage(nativeImage, zffalse);
    ZFUIImage *image = ret;
    if(image == zfnull)
    {
        return zfnull;
    }

    ZFSerializableData imageData;
    imageData.itemClass(ZFSerializableKeyword_node);
    {
        ZFSerializableUtilSerializeAttributeToDataNoRef(imageData, zfnull,
            ZFSerializableKeyword_ZFUIImageIO_color, ZFUIColor, color, ZFUIColorZero());

        ZFSerializableUtilSerializeAttributeToDataNoRef(imageData, zfnull,
            ZFSerializableKeyword_ZFUIImageIO_color_size, ZFUISize, sizeTmp, ZFUISizeMake(1, 1));
    }
    image->imageSerializableType(ZFUIImageSerializeType_color);
    image->imageSerializableData(&imageData);

    return ret;
}

ZF_NAMESPACE_GLOBAL_END

