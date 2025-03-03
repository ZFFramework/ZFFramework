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
    zfclass Data : zfextend ZFObject {
        ZFOBJECT_DECLARE(Data, ZFObject, _ZFP_I_ZFUIImageAsyncTask)
    public:
        ZFCoreArray<zfweakT<ZFUIImage> > ownerList;
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

public:
    zfweakT<ZFUIImage> owner;
    zfautoT<Data> data;
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        if(this->owner && this->data) {
            this->data->ownerList.removeElement(this->owner);
        }
        zfsuper::objectOnDeallocPrepare();
    }

public:
    void imageStateImplSetup(void) {
        zfself *task = this;
        ZFLISTENER_1(imageStateImpl
                , zfautoT<zfself>, task
                ) {
            v_ZFUIImageStateImplAction *action = zfargs.param0();
            switch(action->zfv()) {
                case v_ZFUIImageStateImplAction::e_Attach: {
                    ZFUIImage *owner = zfargs.sender();
                    if(task->data->imageLoaded) {
                        owner->imageStateImplNotifyUpdate(task->data->imageLoaded);
                    }
                    else if(task->data->imageLoadTaskId) {
                        owner->imageStateImplNotifyUpdate(task->data->imageLoading);
                    }
                    else {
                        owner->imageStateImplNotifyUpdate(task->data->imageLoadFail);
                    }
                    break;
                }
                case v_ZFUIImageStateImplAction::e_Detach:
                    break;
                case v_ZFUIImageStateImplAction::e_Copy: {
                    zfobj<_ZFP_I_ZFUIImageAsyncTask> taskNew;
                    taskNew->owner = zfargs.param1();
                    taskNew->data = task->data;
                    taskNew->imageStateImplSetup();
                    break;
                }
                default:
                    break;
            }
        } ZFLISTENER_END()
        owner->imageStateImpl(imageStateImpl);
        data->ownerList.add(owner);
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
    if(!ret) {
        return zffalse;
    }

    _ZFP_ZFUIImageAsync_log("%p load begin: %s %s %s", holder, zftToString(src).cString(), zftToString(imageLoadFail).cString(), zftToString(imageLoading).cString());
    zfobj<_ZFP_I_ZFUIImageAsyncTask> task;
    zfobj<_ZFP_I_ZFUIImageAsyncTask::Data> data;
    data->imageLoadFail = imageLoadFail;
    data->imageLoading = imageLoading;
    task->owner = ret;
    task->data = data;

    ZFLISTENER_1(loadOnFinish
            , zfweakT<_ZFP_I_ZFUIImageAsyncTask::Data>, data
            ) {
        if(data) {
            data->imageLoadTaskId = zfnull;
            data->imageLoaded = zfargs.param0();
            zfautoT<ZFUIImage> state = data->imageLoaded ? data->imageLoaded : data->imageLoadFail;
            for(zfindex i = 0; i < data->ownerList.count(); ++i) {
                data->ownerList[i]->imageStateImplNotifyUpdate(state);
            }
        }
        _ZFP_ZFUIImageAsync_log("%p load end: %s", holder, zftToString(zfargs.param0()).cString());
    } ZFLISTENER_END()
    task->data->imageLoadTaskId = ZFUIImageLoad(src, loadOnFinish);

    task->imageStateImplSetup();

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
                srcData.category(ZFSerializableKeyword_ZFUIImageAsync_imageSrc);
                data.child(srcData);
            }
            if(imageLoadFail) {
                ZFSerializableData imageLoadFailData;
                if(!ZFObjectToDataT(imageLoadFailData, imageLoadFail)) {
                    return;
                }
                imageLoadFailData.category(ZFSerializableKeyword_ZFUIImageAsync_imageLoadFail);
                data.child(imageLoadFailData);
            }
            if(imageLoading) {
                ZFSerializableData imageLoadingData;
                if(!ZFObjectToDataT(imageLoadingData, imageLoading)) {
                    return;
                }
                imageLoadingData.category(ZFSerializableKeyword_ZFUIImageAsync_imageLoading);
                data.child(imageLoadingData);
            }
            zfargs.result(zfobj<v_ZFSerializableData>(data));
        } ZFLISTENER_END()
        ret->imageSerializeType(ZFUIImageSerializeType_ZFUIImageAsync);
        ret->imageSerializeDataGetter(serializeImpl);
    }
    return zftrue;
}

ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(async, ZFUIImageSerializeType_ZFUIImageAsync) {
    ZFCallback input;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIImageAsync_imageSrc, ZFCallback, input, {
                return zffalse;
            });
    if(!input) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "invalid callback");
        return zffalse;
    }
    zfauto imageLoadFail;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageAsync_imageLoadFail, ZFObject, imageLoadFail, {
                return zffalse;
            });
    zfauto imageLoading;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIImageAsync_imageLoading, ZFObject, imageLoading, {
                return zffalse;
            });
    return ZFUIImageAsyncT(ret, input, imageLoadFail, imageLoading);
}

ZF_NAMESPACE_GLOBAL_END

