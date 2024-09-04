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

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFUIImage>, ZFUIImageAsync
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoadFail, zfnull)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoading, zfnull)
        ) {
    if(!src) {
        return zfnull;
    }
    zfautoT<ZFUIImage> holder = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageAsyncT(holder, src, imageLoadFail, imageLoading)) {
        return holder;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFUIImageAsyncT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoadFail, zfnull)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoading, zfnull)
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
    ret->imageStateImpl(imageStateImpl);

    ZFLISTENER_2(loadOnFinish
            , ZFUIImage *, ret
            , zfweakT<_ZFP_I_ZFUIImageAsyncTask>, task
            ) {
        if(task) {
            task->imageLoadTaskId = zfnull;
            task->imageLoaded = zfargs.param0();
            ret->imageStateImplNotifyUpdate(task->imageLoaded ? task->imageLoaded : task->imageLoadFail);
        }
        _ZFP_ZFUIImageAsync_log("%p load end: %s", holder, zftToString(zfargs.param0()).cString());
    } ZFLISTENER_END()
    task->imageLoadTaskId = ZFUIImageLoad(src, loadOnFinish);

    if(!src.callbackSerializeDisable()
            && !ret->imageSerializeDisable()
            ) {
        // serialize logic
        ZFSerializableData srcData;
        if(!ZFCallbackToDataT(srcData, src)) {
            return zftrue;
        }
        ZFLISTENER_3(serializeImpl
                , ZFSerializableData, srcData
                , zfautoT<ZFUIImage>, imageLoadFail
                , zfautoT<ZFUIImage>, imageLoading
                ) {
            ZFSerializableData data;
            {
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
            zfargs.result(zfobj<v_ZFSerializableData>(data));
        } ZFLISTENER_END()
        ret->imageSerializeType(ZFUIImageSerializeType_async);
        ret->imageSerializeDataGetter(serializeImpl);
    }
    return zftrue;
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
    return ZFUIImageAsyncT(ret, input, imageLoadFail, imageLoading);
}

ZF_NAMESPACE_GLOBAL_END

