#include "ZFUIImageAsync.h"
#include "ZFUIImageCache.h"

// #define _ZFP_ZFUIImageAsync_DEBUG 1

#if _ZFP_ZFUIImageAsync_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFUIImageAsync_log(fmt, ...) \
        zfimplLog("[ZFUIImageAsync] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFUIImageAsync_log(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFUIImageAsyncTask : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIImageAsyncTask, ZFObject)
public:
    zfautoT<ZFUIImage> imageFail;
    zfautoT<ZFUIImage> imageLoading;
    zfauto imageLoadTask;
    zfautoT<ZFUIImage> imageLoaded;
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        if(this->imageLoadTask) {
            ZFUIImageLoadCancel(this->imageLoadTask);
            this->imageLoadTask = zfnull;
        }
        zfsuper::objectOnDeallocPrepare();
    }
};

static void _ZFP_ZFUIImageAsyncImpl(
        ZFUIImage *holder
        , const ZFInput &src
        , ZFUIImage *imageFail = zfnull
        , ZFUIImage *imageLoading = zfnull
        ) {
    _ZFP_ZFUIImageAsync_log("%p load begin: %s %s %s", holder, zftToString(src).cString(), zftToString(imageFail).cString(), zftToString(imageLoading).cString());
    zfobj<_ZFP_I_ZFUIImageAsyncTask> task;
    task->imageFail = imageFail;
    task->imageLoading = imageLoading;

    ZFLISTENER_2(imageStateImpl
            , ZFUIImage *, holder
            , zfautoT<_ZFP_I_ZFUIImageAsyncTask>, task
            ) {
        if(task->imageLoaded) {
            _ZFP_ZFUIImageAsync_log("%p state loaded: %s", holder, zftToString(task->imageLoaded).cString());
            zfargs.result(task->imageLoaded);
        }
        else if(task->imageLoadTask) {
            _ZFP_ZFUIImageAsync_log("%p state loading: %s", holder, zftToString(task->imageLoading).cString());
            zfargs.result(task->imageLoading);
        }
        else {
            _ZFP_ZFUIImageAsync_log("%p state fail: %s", holder, zftToString(task->imageFail).cString());
            zfargs.result(task->imageFail);
        }
    } ZFLISTENER_END()
    holder->imageStateImpl(imageStateImpl);

    ZFLISTENER_2(loadOnFinish
            , ZFUIImage *, holder
            , zfweakT<_ZFP_I_ZFUIImageAsyncTask>, task
            ) {
        if(task) {
            task->imageLoadTask = zfnull;
            task->imageLoaded = zfargs.param0();
            holder->imageStateUpdate();
        }
        _ZFP_ZFUIImageAsync_log("%p load end: %s", holder, zftToString(zfargs.param0()).cString());
    } ZFLISTENER_END()
    task->imageLoadTask = ZFUIImageLoad(src, loadOnFinish);

    if(src.callbackSerializeCustomDisabled()) {
        return;
    }

    // serializez logic
    ZFSerializableData data;
    {
        ZFSerializableData srcData;
        if(!ZFCallbackToDataT(srcData, src)) {
            return;
        }
        srcData.category(ZFSerializableKeyword_ZFUIImageIO_async_imageData);
        data.childAdd(srcData);
    }
    if(imageFail) {
        ZFSerializableData imageFailData;
        if(!ZFObjectToDataT(imageFailData, imageFail)) {
            return;
        }
        imageFailData.category(ZFSerializableKeyword_ZFUIImageIO_async_imageFail);
        data.childAdd(imageFailData);
    }
    if(imageLoading) {
        ZFSerializableData imageLoadingData;
        if(!ZFObjectToDataT(imageLoadingData, imageLoading)) {
            return;
        }
        imageLoadingData.category(ZFSerializableKeyword_ZFUIImageIO_async_imageLoading);
        data.childAdd(imageLoadingData);
    }
    holder->imageSerializableType(ZFUIImageSerializeType_async);
    holder->imageSerializableData(&data);

    return;
}
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFUIImage>, ZFUIImageAsync
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(ZFUIImage *, imageFail, zfnull)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoading, zfnull)
        ) {
    if(!src) {
        return zfnull;
    }
    zfautoT<ZFUIImage> holder = ZFUIImage::ClassData()->newInstance();
    _ZFP_ZFUIImageAsyncImpl(holder, src, imageFail, imageLoading);
    return holder;
}

ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(async, ZFUIImageSerializeType_async) {
    ZFCallback input;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImageIO_async_imageData, ZFCallback, input, {
                return zffalse;
            });
    if(!input) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "invalid callback");
        return zffalse;
    }
    zfauto imageFail;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageIO_async_imageFail, ZFObject, imageFail, {
                return zffalse;
            });
    zfauto imageLoading;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageIO_async_imageLoading, ZFObject, imageLoading, {
                return zffalse;
            });
    _ZFP_ZFUIImageAsyncImpl(ret, input, imageFail, imageLoading);
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

