#include "ZFUIImage.h"
#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef zfstlmap<zfstring, _ZFP_ZFUIImageSerializeFromCallback> _ZFP_ZFUIImageSerializeDataMapType;
static _ZFP_ZFUIImageSerializeDataMapType &_ZFP_ZFUIImageSerializeDataMap(void) {
    static _ZFP_ZFUIImageSerializeDataMapType d;
    return d;
}
void _ZFP_ZFUIImageSerializeTypeRegister(
        ZF_IN const zfstring &name
        , ZF_IN _ZFP_ZFUIImageSerializeFromCallback fromCallback
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFUIImageSerializeDataMapType &m = _ZFP_ZFUIImageSerializeDataMap();
    ZFCoreAssert(name != zfnull && fromCallback != zfnull);
    ZFCoreAssertWithMessageTrim(m.find(name) == m.end(),
        "[ZFUIIMAGE_SERIALIZE_TYPE_DEFINE] %s already registered",
        name);
    m[name] = fromCallback;
}
void _ZFP_ZFUIImageSerializeTypeUnregister(ZF_IN const zfstring &name) {
    ZFCoreMutexLocker();
    _ZFP_ZFUIImageSerializeDataMapType &m = _ZFP_ZFUIImageSerializeDataMap();
    m.erase(name);
}

void ZFUIImageSerializeTypeGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) {
    ZFCoreMutexLocker();
    _ZFP_ZFUIImageSerializeDataMapType &m = _ZFP_ZFUIImageSerializeDataMap();
    for(_ZFP_ZFUIImageSerializeDataMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
}

ZFENUM_DEFINE(ZFUIImageStateImplAction)

// ============================================================
// _ZFP_ZFUIImagePrivate
zfclassNotPOD _ZFP_ZFUIImagePrivate {
public:
    ZFUIImage *pimplOwner;
    void *nativeImage;
    zffloat imageScaleFixed;
    ZFUISize imageSizeFixed;
    ZFUISize imageSize;
    ZFListener globalImageScaleOnUpdateListener;
    ZFListener imageStateImpl;
    zfautoT<ZFUIImage> imageState;
    ZFCoreArray<ZFListener> imageStateObservers;

public:
    zfstring imageSerializeType;
    ZFSerializableData imageSerializeData;
    ZFListener imageSerializeDataGetter;

public:
    void imageSizeUpdate(void) {
        if(ZFUIGlobalStyle::DefaultStyle()) {
            this->imageScaleFixed = this->pimplOwner->imageScale() * ZFUIGlobalStyle::DefaultStyle()->imageScale();
        }
        else {
            this->imageScaleFixed = this->pimplOwner->imageScale();
        }
        if(this->nativeImage != zfnull) {
            this->imageSizeFixed = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageSize(this->nativeImage);
            ZFUISizeApplyScaleReverselyT(this->imageSize, this->imageSizeFixed, this->imageScaleFixed);
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

public:
    _ZFP_ZFUIImagePrivate(void)
    : pimplOwner(zfnull)
    , nativeImage(zfnull)
    , imageScaleFixed(ZFUIGlobalStyle::DefaultStyle() ? ZFUIGlobalStyle::DefaultStyle()->imageScale() : (zffloat)1)
    , imageSizeFixed(ZFUISizeZero())
    , imageSize(ZFUISizeZero())
    , imageStateImpl()
    , imageState()
    , imageStateObservers()
    , imageSerializeType(zfnull)
    , imageSerializeData(zfnull)
    , imageSerializeDataGetter(zfnull)
    , globalImageScaleOnUpdateListener()
    {
    }
};

// ============================================================
// ZFUIImage
ZFOBJECT_REGISTER(ZFUIImage)

ZFEVENT_REGISTER(ZFUIImage, ImageScaleOnUpdate)
ZFEVENT_REGISTER(ZFUIImage, ImageStateOnUpdate)

zfbool ZFUIImage::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    // style
    if(this->styleKey() != zfnull) {
        if(d->nativeImage == zfnull && d->imageStateImpl == zfnull) {
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
    zfstring imageBin = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFUIImage_imageBin);
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
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImage_imageType, zfstring, typeName, {
                return zffalse;
            });
    _ZFP_ZFUIImageSerializeFromCallback fromCallback = zfnull;
    {
        ZFCoreMutexLocker();
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
    if(d->nativeImage == zfnull && d->imageStateImpl == zfnull) {
        d->imageSizeFixed = ZFUISizeZero();
        d->imageSize = ZFUISizeZero();
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "nativeImage or imageStateImpl not set");
        return zffalse;
    }

    this->imageSerializeType(typeName);
    this->imageSerializeData(imageData);
    this->imageSerializeDataGetter(zfnull);

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
    if(d->nativeImage == zfnull && d->imageStateImpl == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "serialize an image whose nativeImage and imageStateImpl not set");
        return zffalse;
    }

    if(this->imageSerializeType() || this->imageSerializeDataGetter()) { // custom serialize
        if(ref != zfnull
                && zfstringIsEqual(this->imageSerializeType(), ref->imageSerializeType())
                && this->imageSerializeData().objectCompare(ref->imageSerializeData()) == ZFCompareEqual
                && this->imageSerializeDataGetter() == ref->imageSerializeDataGetter()
                ) {
            // all match, skip
        }
        else {
            // imageType
            ZFSerializableUtilSerializeAttrToData(serializableData, outErrorHint, ref,
                    ZFSerializableKeyword_ZFUIImage_imageType, zfstring, this->imageSerializeType(), ref->imageSerializeType(), "", {
                        return zffalse;
                    });

            { // data
                ZFSerializableData imageData;
                if(this->imageSerializeDataGetter()) {
                    ZFArgs zfargsDataGetter;
                    zfargsDataGetter.sender(this);
                    this->imageSerializeDataGetter().execute(zfargsDataGetter);
                    v_ZFSerializableData *tmp = zfargsDataGetter.result();
                    if(tmp != zfnull) {
                        imageData = tmp->zfv;
                    }
                    else {
                        v_zfstring *errorHint = zfargsDataGetter.result();
                        if(errorHint != zfnull) {
                            ZFSerializableUtilErrorOccurred(outErrorHint,
                                "unable to serialize as type \"%s\" : %s"
                                , this->imageSerializeType()
                                , errorHint->zfv
                                );
                            return zffalse;
                        }
                    }
                }
                else {
                    imageData = this->imageSerializeData();
                }

                if(imageData == zfnull) {
                    ZFSerializableUtilErrorOccurred(outErrorHint,
                        "missing image data for type: \"%s\"", this->imageSerializeType());
                    return zffalse;
                }
                for(zfiter it = imageData.attrIter(); it; ++it) {
                    serializableData.attr(imageData.attrIterKey(it), imageData.attrIterValue(it));
                }
                for(zfindex i = 0; i < imageData.childCount(); ++i) {
                    serializableData.child(imageData.childAt(i).copy());
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

void ZFUIImage::styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    zfsuperI(ZFStyleable)::styleableOnCopyFrom(anotherStyleable);
    ZFUIImage *ref = zfcast(zfself *, anotherStyleable);
    if(ref == zfnull || ref == this) {
        return;
    }
    if(d->nativeImage != zfnull) {
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(d->nativeImage);
        d->nativeImage = zfnull;
    }
    d->imageSizeFixed = ZFUISizeZero();
    d->imageSize = ZFUISizeZero();
    d->imageSerializeType = zfnull;
    d->imageSerializeData = zfnull;
    d->imageSerializeDataGetter = zfnull;

    if(ref->d->nativeImage != zfnull) {
        d->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(ref->d->nativeImage);
    }
    d->imageSizeUpdate();
    d->imageSerializeType = ref->d->imageSerializeType;
    d->imageSerializeData = ref->d->imageSerializeData;
    d->imageSerializeDataGetter = ref->d->imageSerializeDataGetter;
    d->imageState = zfnull;
    d->imageStateObservers.removeAll();

    d->imageStateImpl = zfnull;
    if(ref->d->imageStateImpl) {
        ref->d->imageStateImpl.execute(ZFArgs()
                .sender(ref)
                .param0(zfobj<v_ZFUIImageStateImplAction>(v_ZFUIImageStateImplAction::e_Copy))
                .param1(this)
                );
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImage, zffloat, imageScale) {
    d->imageSizeUpdate();
    this->imageScaleOnUpdate();
}
void ZFUIImage::_ZFP_ZFUIImage_imageScaleOnUpdate(void) {
    d->imageSizeUpdate();
    this->imageScaleOnUpdate();
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

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIImage, zfautoT<ZFUIImage>, imageState) {
    if(d->imageStateImpl) {
        return d->imageState;
    }
    else {
        return this;
    }
}
ZFMETHOD_DEFINE_0(ZFUIImage, zfautoT<ZFUIImage>, imageStateUpdate) {
    if(d->imageStateImpl) {
        d->imageStateImpl.execute(ZFArgs()
                .sender(this)
                .param0(zfobj<v_ZFUIImageStateImplAction>(v_ZFUIImageStateImplAction::e_Attach))
                );
        zfautoT<ZFUIImage> ret = this->imageState();
        if(d->imageStateObservers.isEmpty()) {
            d->imageStateImpl.execute(ZFArgs()
                    .sender(this)
                    );
        }
        return ret;
    }
    else {
        return this;
    }
}
ZFMETHOD_DEFINE_1(ZFUIImage, void, imageStateAttach
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    d->imageStateObservers.add(callback);
    if(d->imageStateObservers.count() == 1) {
        d->imageStateImpl.execute(ZFArgs()
                .sender(this)
                .param0(zfobj<v_ZFUIImageStateImplAction>(v_ZFUIImageStateImplAction::e_Attach))
                );
    }
    else {
        callback.execute(ZFArgs()
                .sender(this)
                );
    }
}
ZFMETHOD_DEFINE_1(ZFUIImage, void, imageStateDetach
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    if(!d->imageStateObservers.isEmpty()) {
        d->imageStateObservers.removeElement(callback);
        if(d->imageStateObservers.isEmpty()) {
            d->imageStateImpl.execute(ZFArgs()
                    .sender(this)
                    .param0(zfobj<v_ZFUIImageStateImplAction>(v_ZFUIImageStateImplAction::e_Detach))
                    );
            d->imageState = zfnull;
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIImage, const ZFListener &, imageStateImpl) {
    return d->imageStateImpl;
}
ZFMETHOD_DEFINE_1(ZFUIImage, void, imageStateImpl
        , ZFMP_IN(const ZFListener &, impl)
        ) {
    if(d->imageStateImpl == impl) {
        return;
    }
    if(d->imageStateImpl) {
        if(!d->imageStateObservers.isEmpty()) {
            d->imageStateImpl.execute(ZFArgs()
                    .sender(this)
                    .param0(zfobj<v_ZFUIImageStateImplAction>(v_ZFUIImageStateImplAction::e_Detach))
                    );
        }
        d->imageStateImpl = zfnull;
        d->imageState = zfnull;
    }
    if(impl) {
        d->imageStateImpl = impl;
        if(!d->imageStateObservers.isEmpty()) {
            d->imageStateImpl.execute(ZFArgs()
                    .sender(this)
                    .param0(zfobj<v_ZFUIImageStateImplAction>(v_ZFUIImageStateImplAction::e_Attach))
                    );
        }
    }
}
ZFMETHOD_DEFINE_1(ZFUIImage, void, imageStateImplNotifyUpdate
        , ZFMP_IN(ZFUIImage *, imageState)
        ) {
    if(d->imageStateImpl) {
        d->imageState = imageState;
        ZFArgs zfargsHolder;
        zfargsHolder.sender(this);
        for(zfindex i = 0; i < d->imageStateObservers.count(); ++i) {
            d->imageStateObservers[i].execute(zfargsHolder);
        }
        this->imageStateOnUpdate();
    }
}

// ============================================================
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
    ZFLISTENER_1(globalImageScaleOnUpdate
            , ZFUIImage *, owner
            ) {
        const ZFProperty *property = zfargs.param0().to<v_ZFProperty *>()->zfv;
        if(property == ZFPropertyAccess(ZFUIGlobalStyle, imageScale)) {
            owner->_ZFP_ZFUIImage_imageScaleOnUpdate();
        }
    } ZFLISTENER_END()
    d->globalImageScaleOnUpdateListener = globalImageScaleOnUpdate;
    ZFUIGlobalStyle::DefaultStyle()->observerAdd(
        ZFObject::E_ObjectPropertyValueOnUpdate(),
        d->globalImageScaleOnUpdateListener);
}
void ZFUIImage::objectOnDeallocPrepare(void) {
    ZFCoreAssertWithMessageTrim(d->imageStateObservers.isEmpty()
            , "%s dealloc while imageState observer still exists, have you forgot imageStateDetach?"
            , this
            );
    if(ZFUIGlobalStyle::DefaultStyle()) {
        ZFUIGlobalStyle::DefaultStyle()->observerRemove(
            ZFObject::E_ObjectPropertyValueOnUpdate(),
            d->globalImageScaleOnUpdateListener);
    }
    zfsuper::objectOnDeallocPrepare();
}

ZFCompareResult ZFUIImage::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(d->nativeImage == another->d->nativeImage
            && d->imageStateImpl == another->d->imageStateImpl
            && this->imageNinePatch() == another->imageNinePatch()
            && zfstringIsEqual(d->imageSerializeType, another->d->imageSerializeType)
            && d->imageSerializeData.objectCompare(another->d->imageSerializeData) == ZFCompareEqual
            && d->imageSerializeDataGetter == another->d->imageSerializeDataGetter
            ) {
        return ZFCompareEqual;
    }
    return ZFCompareUncomparable;
}
void ZFUIImage::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ret += " ";
    ZFUISizeToStringT(ret, this->imageSize());
}

void ZFUIImage::objectPropertyValueOnUpdate(
        ZF_IN const ZFProperty *property
        , ZF_IN void *oldValue
        ) {
    zfsuper::objectPropertyValueOnUpdate(property, oldValue);
    if(property->ownerClass() == ZFUIImage::ClassData()) {
        this->imageStateImplNotifyUpdate(this->imageState());
    }
}

ZFMETHOD_DEFINE_0(ZFUIImage, void *, nativeImage) {
    return d->nativeImage;
}

ZFMETHOD_DEFINE_2(ZFUIImage, void, nativeImage
        , ZFMP_IN(void *, nativeImage)
        , ZFMP_IN_OPT(zfbool, retainNativeImage, zftrue)
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

ZFMETHOD_DEFINE_1(ZFUIImage, void, imageSerializeType
        , ZFMP_IN(const zfstring &, typeName)
        ) {
    d->imageSerializeType = typeName;
}
ZFMETHOD_DEFINE_0(ZFUIImage, const zfstring &, imageSerializeType) {
    return d->imageSerializeType;
}
ZFMETHOD_DEFINE_1(ZFUIImage, void, imageSerializeData
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        ) {
    d->imageSerializeData = serializableData;
}
ZFMETHOD_DEFINE_0(ZFUIImage, const ZFSerializableData &, imageSerializeData) {
    return d->imageSerializeData;
}
ZFMETHOD_DEFINE_1(ZFUIImage, void, imageSerializeDataGetter
        , ZFMP_IN(const ZFListener &, impl)
        ) {
    d->imageSerializeDataGetter = impl;
}
ZFMETHOD_DEFINE_0(ZFUIImage, const ZFListener &, imageSerializeDataGetter) {
    return d->imageSerializeDataGetter;
}

ZFMETHOD_DEFINE_1(ZFUIImage, void, imageSerializeDisable
        , ZFMP_IN(zfbool, disable)
        ) {
    this->imageSerializeType(ZFSerializeDisable);
}
ZFMETHOD_DEFINE_0(ZFUIImage, zfbool, imageSerializeDisable) {
    return zfstringIsEqual(this->imageSerializeType(), ZFSerializeDisable);
}

ZF_NAMESPACE_GLOBAL_END

