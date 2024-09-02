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
    zfautoT<ZFTaskId> imageLoadTaskId;
    zfautoT<ZFUIImage> imageLoadFail;
    zfautoT<ZFUIImage> imageLoading;
    zfautoT<ZFUIImage> imageLoaded;
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        if(this->imageLoadTaskId) {
            this->imageLoadTaskId->stop();
            this->imageLoadTaskId = zfnull;
        }
        zfsuper::objectOnDeallocPrepare();
    }
};

static void _ZFP_ZFUIImageAsyncImpl(
        ZFUIImage *holder
        , const ZFInput &src
        , ZFUIImage *imageLoadFail = zfnull
        , ZFUIImage *imageLoading = zfnull
        ) {
    _ZFP_ZFUIImageAsync_log("%p load begin: %s %s %s", holder, zftToString(src).cString(), zftToString(imageLoadFail).cString(), zftToString(imageLoading).cString());
    zfobj<_ZFP_I_ZFUIImageAsyncTask> task;
    task->imageLoadFail = imageLoadFail;
    task->imageLoading = imageLoading;

    ZFLISTENER_1(imageStateImpl
            , zfautoT<_ZFP_I_ZFUIImageAsyncTask>, task
            ) {
        v_zfbool *state = zfargs.param0();
        if(state->zfv) {
            ZFUIImage *owner = zfargs.sender();
            if(task->imageLoaded) {
                owner->imageStateImplNotifyUpdate(task->imageLoaded);
            }
            else if(task->imageLoadTaskId) {
                owner->imageStateImplNotifyUpdate(task->imageLoading);
            }
            else {
                owner->imageStateImplNotifyUpdate(task->imageLoadFail);
            }
        }
    } ZFLISTENER_END()
    holder->imageStateImpl(imageStateImpl);

    ZFLISTENER_2(loadOnFinish
            , ZFUIImage *, holder
            , zfweakT<_ZFP_I_ZFUIImageAsyncTask>, task
            ) {
        if(task) {
            task->imageLoadTaskId = zfnull;
            task->imageLoaded = zfargs.param0();
            holder->imageStateImplNotifyUpdate(task->imageLoaded ? task->imageLoaded : task->imageLoadFail);
        }
        _ZFP_ZFUIImageAsync_log("%p load end: %s", holder, zftToString(zfargs.param0()).cString());
    } ZFLISTENER_END()
    task->imageLoadTaskId = ZFUIImageLoad(src, loadOnFinish);

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
        srcData.category(ZFSerializableKeyword_ZFUIImageIO_async_imageSrc);
        data.childAdd(srcData);
    }
    if(imageLoadFail) {
        ZFSerializableData imageLoadFailData;
        if(!ZFObjectToDataT(imageLoadFailData, imageLoadFail)) {
            return;
        }
        imageLoadFailData.category(ZFSerializableKeyword_ZFUIImageIO_async_imageLoadFail);
        data.childAdd(imageLoadFailData);
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
    holder->imageSerializableData(data);

    return;
}
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFUIImage>, ZFUIImageAsync
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoadFail, zfnull)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoading, zfnull)
        ) {
    if(!src) {
        return zfnull;
    }
    zfautoT<ZFUIImage> holder = ZFUIImage::ClassData()->newInstance();
    _ZFP_ZFUIImageAsyncImpl(holder, src, imageLoadFail, imageLoading);
    return holder;
}

ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(async, ZFUIImageSerializeType_async) {
    ZFCallback input;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImageIO_async_imageSrc, ZFCallback, input, {
                return zffalse;
            });
    if(!input) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "invalid callback");
        return zffalse;
    }
    zfauto imageLoadFail;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageIO_async_imageLoadFail, ZFObject, imageLoadFail, {
                return zffalse;
            });
    zfauto imageLoading;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageIO_async_imageLoading, ZFObject, imageLoading, {
                return zffalse;
            });
    _ZFP_ZFUIImageAsyncImpl(ret, input, imageLoadFail, imageLoading);
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

