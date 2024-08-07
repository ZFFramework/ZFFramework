#include "ZFUIImage.h"
#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serializabel data
typedef zfstlmap<zfstring, _ZFP_ZFUIImageSerializeFromCallback> _ZFP_ZFUIImageSerializeDataMapType;
static _ZFP_ZFUIImageSerializeDataMapType &_ZFP_ZFUIImageSerializeDataMap(void) {
    static _ZFP_ZFUIImageSerializeDataMapType d;
    return d;
}
void _ZFP_ZFUIImageSerializeTypeRegister(
        ZF_IN const zfstring &name
        , ZF_IN _ZFP_ZFUIImageSerializeFromCallback fromCallback
        ) {
    zfCoreMutexLocker();
    _ZFP_ZFUIImageSerializeDataMapType &m = _ZFP_ZFUIImageSerializeDataMap();
    zfCoreAssert(name != zfnull && fromCallback != zfnull);
    zfCoreAssertWithMessageTrim(m.find(name) == m.end(),
        "[ZFUIIMAGE_SERIALIZE_TYPE_DEFINE] %s already registered",
        name);
    m[name] = fromCallback;
}
void _ZFP_ZFUIImageSerializeTypeUnregister(ZF_IN const zfstring &name) {
    zfCoreMutexLocker();
    _ZFP_ZFUIImageSerializeDataMapType &m = _ZFP_ZFUIImageSerializeDataMap();
    m.erase(name);
}

void ZFUIImageSerializeTypeGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) {
    zfCoreMutexLocker();
    _ZFP_ZFUIImageSerializeDataMapType &m = _ZFP_ZFUIImageSerializeDataMap();
    for(_ZFP_ZFUIImageSerializeDataMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
}

// ============================================================
// _ZFP_ZFUIImagePrivate
zfclassNotPOD _ZFP_ZFUIImagePrivate {
public:
    ZFUIImage *pimplOwner;
    void *nativeImage;
    zffloat imageScaleFixed;
    ZFUISize imageSizeFixed;
    ZFUISize imageSize;
    ZFListener globalImageScaleOnChangeListener;

public:
    zfstring serializableType;
    const ZFSerializableData *serializableData;

public:
    void imageSizeUpdate(void) {
        this->imageScaleFixed = this->pimplOwner->imageScale() * ZFUIGlobalStyle::DefaultStyle()->imageScale();
        if(this->nativeImage != zfnull) {
            this->imageSizeFixed = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageSize(this->nativeImage);
            this->imageSize = ZFUISizeApplyScaleReversely(this->imageSizeFixed, this->imageScaleFixed);
            if(this->imageSize.width <= 0) {
                this->imageSize.width = 1;
            }
            if(this->imageSize.height <= 0) {
                this->imageSize.height = 1;
            }
        }
        else {
            this->imageSizeFixed = ZFUISizeZero();
            this->imageSize = ZFUISizeZero();
        }
    }
    void copyFrom(ZF_IN _ZFP_ZFUIImagePrivate *another) {
        if(this->nativeImage != zfnull) {
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(this->nativeImage);
            this->nativeImage = zfnull;
        }
        this->imageSizeFixed = ZFUISizeZero();
        this->imageSize = ZFUISizeZero();
        this->serializableType = zfnull;
        if(this->serializableData != zfnull) {
            zfdelete(this->serializableData);
            this->serializableData = zfnull;
        }

        if(another->nativeImage != zfnull) {
            this->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(another->nativeImage);
        }
        this->imageSizeUpdate();
        this->serializableType = another->serializableType;
        if(another->serializableData != zfnull) {
            this->serializableData = zfnew(ZFSerializableData, *(another->serializableData));
        }
    }

public:
    _ZFP_ZFUIImagePrivate(void)
    : pimplOwner(zfnull)
    , nativeImage(zfnull)
    , imageScaleFixed(ZFUIGlobalStyle::DefaultStyle()->imageScale())
    , imageSizeFixed(ZFUISizeZero())
    , imageSize(ZFUISizeZero())
    , serializableType(zfnull)
    , serializableData(zfnull)
    , globalImageScaleOnChangeListener()
    {
    }
    ~_ZFP_ZFUIImagePrivate(void) {
        zfdelete(this->serializableData);
    }
};

// ============================================================
// ZFUIImage
ZFOBJECT_REGISTER(ZFUIImage)

ZFEVENT_REGISTER(ZFUIImage, ImageScaleOnChange)

zfbool ZFUIImage::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    // style
    if(this->styleKey() != zfnull) {
        if(d->nativeImage == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "unable to load image from style \"%s\"",
                this->styleKey());
            return zffalse;
        }
        else {
            return zftrue;
        }
    }

    // imageBin
    zfstring imageBin = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFUIImage_imageBin);
    if(imageBin != zfnull) {
        zfobj<ZFIOBufferByCacheFile> io;
        if(!ZFBase64Decode(io->output(), ZFInputForString(imageBin))) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "invalid base64 data: \"%s\"", imageBin);
            return zffalse;
        }
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(io->input());
        if(nativeImage == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "fail to load image from base64 data: \"%s\"", imageBin);
            return zffalse;
        }
        this->nativeImage(nativeImage, zffalse);
        return zftrue;
    }

    // imageType
    const zfchar *typeName = zfnull;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImage_imageType, zfstring, typeName, {
                return zffalse;
            });
    _ZFP_ZFUIImageSerializeFromCallback fromCallback = zfnull;
    {
        zfCoreMutexLocker();
        _ZFP_ZFUIImageSerializeDataMapType &m = _ZFP_ZFUIImageSerializeDataMap();
        _ZFP_ZFUIImageSerializeDataMapType::iterator it = m.find(typeName);
        if(it != m.end()) {
            fromCallback = it->second;
        }
    }
    if(fromCallback == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "no such image serializable type registered: \"%s\"", typeName);
        return zffalse;
    }

    // imageData
    ZFSerializableData imageData;
    {
        if(!fromCallback(this, serializableData, outErrorHint, outErrorPos)) {
            return zffalse;
        }

        imageData = serializableData.copy();
        imageData.category(zfnull);
        imageData.propertyName(zfnull);
        imageData.attr(ZFSerializableKeyword_ZFUIImage_imageType, zfnull);
        imageData.attr(ZFSerializableKeyword_ZFUIImage_imageBin, zfnull);
    }

    // check
    if(d->nativeImage == zfnull) {
        d->imageSizeFixed = ZFUISizeZero();
        d->imageSize = ZFUISizeZero();
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "nativeImage not set");
        return zffalse;
    }

    this->imageSerializableType(typeName);
    this->imageSerializableData(&imageData);

    return zftrue;
}
zfbool ZFUIImage::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);

    // check
    if(d->nativeImage == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "serialize an image whose nativeImage not set");
        return zffalse;
    }

    if(this->imageSerializableType() != zfnull) { // custom serialize
        if(ref != zfnull
                && zfstringIsEqual(this->imageSerializableType(), ref->imageSerializableType())
                && this->imageSerializableData() != zfnull && ref->imageSerializableData() != zfnull
                && this->imageSerializableData()->objectCompare(*(ref->imageSerializableData()))
                ) {
            // all match, skip
        }
        else {
            // imageType
            ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref,
                    ZFSerializableKeyword_ZFUIImage_imageType, zfstring, this->imageSerializableType(), ref->imageSerializableType(), "", {
                        return zffalse;
                    });

            { // data
                if(this->imageSerializableData() == zfnull) {
                    ZFSerializableUtilErrorOccurred(outErrorHint,
                        "missing image data for type: \"%s\"", this->imageSerializableType());
                    return zffalse;
                }

                const ZFSerializableData &imageData = *(this->imageSerializableData());
                for(zfiter it = imageData.attrIter(); it; ++it) {
                    serializableData.attr(imageData.attrIterKey(it), imageData.attrIterValue(it));
                }
                for(zfindex i = 0; i < imageData.childCount(); ++i) {
                    serializableData.childAdd(imageData.childAt(i).copy());
                }
            }
        }
    }
    else { // imageBin
        zfstring imageBin;
        if(!ZFUIImageToBase64(ZFOutputForString(imageBin), this)) {
            ZFSerializableUtilErrorOccurred(outErrorHint, "save image to base64 failed");
            return zffalse;
        }
        if(ref != zfnull) {
            zfstring imageBinRef;
            ZFUIImageToBase64(ZFOutputForString(imageBinRef), ref);
            if(imageBin.compare(imageBinRef) == 0) {
                return zftrue;
            }
        }
        serializableData.attr(ZFSerializableKeyword_ZFUIImage_imageBin, imageBin);
    }

    return zftrue;
}

void ZFUIImage::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable) {
    zfsuperI(ZFCopyable)::styleableOnCopyFrom(anotherStyleable);
    d->copyFrom(zfcast(zfself *, anotherStyleable)->d);
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImage, zffloat, imageScale) {
    d->imageSizeUpdate();
    this->imageScaleOnChange();
}
void ZFUIImage::_ZFP_ZFUIImage_imageScaleOnChange(void) {
    d->imageSizeUpdate();
    this->imageScaleOnChange();
}

ZFMETHOD_DEFINE_0(ZFUIImage, zffloat const &, imageScaleFixed) {
    return d->imageScaleFixed;
}

ZFMETHOD_DEFINE_0(ZFUIImage, const ZFUISize &, imageSize) {
    return d->imageSize;
}
ZFMETHOD_DEFINE_0(ZFUIImage, const ZFUISize &, imageSizeFixed) {
    return d->imageSizeFixed;
}

void ZFUIImage::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIImagePrivate);
    d->pimplOwner = this;
}
void ZFUIImage::objectOnDealloc(void) {
    if(d->nativeImage != zfnull) {
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(d->nativeImage);
        d->nativeImage = zfnull;
    }
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIImage::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();

    ZFUIImage *owner = this;
    ZFLISTENER_1(globalImageScaleOnChange
            , ZFUIImage *, owner
            ) {
        const ZFProperty *const &property = zfargs.param0().zfv();
        if(property == ZFPropertyAccess(ZFUIGlobalStyle, imageScale)) {
            owner->_ZFP_ZFUIImage_imageScaleOnChange();
        }
    } ZFLISTENER_END()
    d->globalImageScaleOnChangeListener = globalImageScaleOnChange;
    ZFUIGlobalStyle::DefaultStyle()->observerAdd(
        ZFObject::EventObjectPropertyValueOnUpdate(),
        d->globalImageScaleOnChangeListener);
}
void ZFUIImage::objectOnDeallocPrepare(void) {
    ZFUIGlobalStyle::DefaultStyle()->observerRemove(
        ZFObject::EventObjectPropertyValueOnUpdate(),
        d->globalImageScaleOnChangeListener);
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFUIImage::objectHash(void) {
    return zfidentityCalcPointer(d->nativeImage);
}
ZFCompareResult ZFUIImage::objectCompare(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(d->nativeImage == another->d->nativeImage
            && this->imageNinePatch() == another->imageNinePatch()
            && zfstringIsEqual(d->serializableType, another->d->serializableType)
            && ((d->serializableData == zfnull && another->d->serializableData == zfnull)
                || (d->serializableData != zfnull && another->d->serializableData != zfnull
                    && d->serializableData->objectCompare(*(another->d->serializableData)) == ZFCompareEqual))
                    ) {
        return ZFCompareEqual;
    }
    return ZFCompareUncomparable;
}
void ZFUIImage::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    ret += " ";
    ZFUISizeToStringT(ret, this->imageSize());
}

ZFMETHOD_DEFINE_0(ZFUIImage, void *, nativeImage) {
    return d->nativeImage;
}

void ZFUIImage::nativeImage(
        ZF_IN void *nativeImage
        , ZF_IN_OPT zfbool retainNativeImage /* = zftrue */
        ) {
    void *toRelease = d->nativeImage;

    if(nativeImage != zfnull) {
        if(retainNativeImage) {
            d->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(nativeImage);
        }
        else {
            d->nativeImage = nativeImage;
        }
    }
    else {
        d->nativeImage = zfnull;
    }
    d->imageSizeUpdate();

    if(toRelease != zfnull) {
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(toRelease);
    }
}

void ZFUIImage::imageSerializableType(ZF_IN const zfstring &typeName) {
    d->serializableType = typeName;
}
const zfstring &ZFUIImage::imageSerializableType(void) {
    return d->serializableType;
}
void ZFUIImage::imageSerializableData(ZF_IN const ZFSerializableData *serializableData) {
    const ZFSerializableData *old = d->serializableData;
    if(serializableData != zfnull) {
        d->serializableData = zfnew(ZFSerializableData, *serializableData);
    }
    else {
        d->serializableData = zfnull;
    }
    if(old != zfnull) {
        zfdelete(old);
    }
}
const ZFSerializableData *ZFUIImage::imageSerializableData(void) {
    return d->serializableData;
}

ZF_NAMESPACE_GLOBAL_END

