#include "ZFUIImageAni.h"
#include "ZFUIImageCache.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFUIImageAniTask : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIImageAniTask, ZFObject)
public:
    zfweakT<ZFUIImage> holder;
    zfobj<ZFArray> images;
    zfobj<ZFAniForFrame> ani;
    zfautoT<ZFTaskId> implTaskId;
    zfbool implLoaded;
    zfbool imageStateAttached;
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->implLoaded = zffalse;
        this->imageStateAttached = zffalse;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        if(this->implTaskId) {
            this->implTaskId->stop();
            this->implTaskId = zfnull;
        }
        zfsuper::objectOnDeallocPrepare();
    }

public:
    void attach(ZF_IN ZFUIImage *holder) {
        this->holder = holder;
        this->ani->loop(zfindexMax());

        zfself *task = this;

        ZFLISTENER_1(aniOnFrame
                , zfautoT<zfself>, task
                ) {
            zfindex frameIndex = zfargs.param0().to<v_zfindex *>()->zfv;
            if(task->images->isEmpty()) {
                task->holder->imageStateImplNotifyUpdate(zfnull);
            }
            else {
                task->holder->imageStateImplNotifyUpdate(task->images->get(frameIndex % task->images->count()));
            }
        } ZFLISTENER_END()
        ani->observerAdd(ZFAniForFrame::EventAniFrameOnUpdate(), aniOnFrame);

        ZFLISTENER_1(imageStateImpl
                , zfautoT<zfself>, task
                ) {
            v_zfbool *state = zfargs.param0();
            if(state->zfv) {
                task->imageStateAttached = zftrue;
                if(!task->implLoaded) {
                    task->holder->imageStateImplNotifyUpdate(zfnull);
                }
                else {
                    task->ani->start();
                }
            }
            else {
                task->imageStateAttached = zffalse;
                task->ani->stop();
            }
        } ZFLISTENER_END()
        holder->imageStateImpl(imageStateImpl);
    }
    zfbool refOnLoad(
            ZF_IN ZFUIImage *ref
            , ZF_IN const ZFUISize &frameSize
            , ZF_IN zfindex frameCount
            , ZF_IN zftimet frameDuration
            ) {
        if(ref == zfnull) {
            return zffalse;
        }
        const ZFUISize &imageSize = ref->imageSize();
        zfindex frameIndex = 0;
        for(zffloat y = 0; frameIndex < frameCount; ) {
            for(zffloat x = 0; frameIndex < frameCount; ) {
                if(x + frameSize.width > imageSize.width) {
                    x = 0;
                    y += frameSize.height;
                }
                if(y + frameSize.height > imageSize.height) {
                    return zffalse;
                }
                zfautoT<ZFUIImage> frame = ZFUIImageInFrame(ref, ZFUIRectCreate(x, y, frameSize.width, frameSize.height));
                if(frame == zfnull) {
                    return zffalse;
                }

                this->images->add(frame);
                this->ani->frame(frameDuration);

                ++frameIndex;
                x += frameSize.width;
            }
        }
        this->implLoaded = zftrue;
        if(this->imageStateAttached) {
            this->ani->start();
        }
        return zftrue;
    }
    zfbool refOnLoad(
            ZF_IN ZFUIImage *ref
            , ZF_IN ZFArray *frameRects
            , ZF_IN zftimet duration
            , ZF_IN ZFArray *frameDurations
            ) {
        if(ref == zfnull
                || frameRects == zfnull || frameRects->isEmpty()
                ) {
            return zffalse;
        }
        const ZFUISize &imageSize = ref->imageSize();
        for(zfindex i = 0; i < frameRects->count(); ++i) {
            v_ZFUIRect *rectHolder = frameRects->get(i);
            if(rectHolder == zfnull) {
                return zffalse;
            }
            const ZFUIRect &rect = rectHolder->zfv;
            if(rect.x >= imageSize.width
                    || rect.y >= imageSize.height
                    || ZFUIRectGetRight(rect) > imageSize.width
                    || ZFUIRectGetBottom(rect) > imageSize.height
                    ) {
                return zffalse;
            }
            zfautoT<ZFUIImage> frame = ZFUIImageInFrame(ref, rect);
            if(frame == zfnull) {
                return zffalse;
            }
            this->images->add(frame);

            if(frameDurations != zfnull && i < frameDurations->count()) {
                v_zftimet *t = frameDurations->get(i);
                if(t == zfnull) {
                    return zffalse;
                }
                this->ani->frame(t->zfv);
            }
            else {
                this->ani->frame(duration);
            }
        }
        this->implLoaded = zftrue;
        if(this->imageStateAttached) {
            this->ani->start();
        }
        return zftrue;
    }
    void serializeImpl(
            ZF_IN ZFUIImage *holder
            , ZF_IN ZFArray *images
            , zftimet duration
            , ZF_IN ZFArray *frameDurations
            ) {
        ZFLISTENER_3(serializeImpl
                , zfautoT<ZFArray>, images
                , zftimet, duration
                , zfautoT<ZFArray>, frameDurations
                ) {
            ZFSerializableData data;
            ZFSerializableData containerData;
            data.child(containerData);
            containerData.itemClass(ZFSerializableKeyword_node);
            containerData.category(ZFSerializableKeyword_ZFUIImageAni_images);
            if(duration != 0) {
                containerData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(duration));
            }

            zfstring outErrorHint;
            for(zfindex i = 0; i < images->count(); ++i) {
                ZFSerializableData imageData;
                containerData.child(imageData);
                if(!ZFObjectToDataT(imageData, images->get(i), &outErrorHint)) {
                    zfargs.result(zfobj<v_zfstring>(outErrorHint));
                    return;
                }
                if(frameDurations && i < frameDurations->count()) {
                    imageData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(frameDurations->get(i)->to<v_zftimet *>()->zfv));
                }
            }
            zfargs.result(zfobj<v_ZFSerializableData>(data));
        } ZFLISTENER_END()
        holder->imageSerializeType(ZFUIImageSerializeType_ZFUIImageAni);
        holder->imageSerializeDataGetter(serializeImpl);
    }
    void serializeImpl(
            ZF_IN ZFUIImage *holder
            , ZF_IN ZFUIImage *ref
            , ZF_IN const ZFUISize &frameSize
            , ZF_IN zfindex frameCount
            , ZF_IN zftimet frameDuration
            ) {
        ZFLISTENER_4(serializeImpl
                , zfautoT<ZFUIImage>, ref
                , ZFUISize, frameSize
                , zfindex, frameCount
                , zftimet, frameDuration
                ) {
            ZFSerializableData data;
            zfstring outErrorHint;

            ZFSerializableData refData;
            data.child(refData);
            if(!ZFObjectToDataT(refData, ref, &outErrorHint)) {
                zfargs.result(zfobj<v_zfstring>(outErrorHint));
                return;
            }
            refData.category(ZFSerializableKeyword_ZFUIImageAni_ref);

            ZFSerializableData nodeData;
            data.child(nodeData);
            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFUIImageAni_split);
            nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_size, ZFUISizeToString(frameSize));
            nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_count, zfindexToString(frameCount));
            if(frameDuration != 0) {
                nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(frameDuration));
            }
            zfargs.result(zfobj<v_ZFSerializableData>(data));
        } ZFLISTENER_END()
        holder->imageSerializeType(ZFUIImageSerializeType_ZFUIImageAni);
        holder->imageSerializeDataGetter(serializeImpl);
    }
    void serializeImpl(
            ZF_IN ZFUIImage *holder
            , ZF_IN const ZFInput &refSrc
            , ZF_IN const ZFUISize &frameSize
            , ZF_IN zfindex frameCount
            , ZF_IN zftimet frameDuration
            ) {
        if(!refSrc.callbackSerializeDisable()) {
            ZFSerializableData refSrcData;
            if(ZFCallbackToDataT(refSrcData, refSrc)) {
                ZFLISTENER_4(serializeImpl
                        , ZFSerializableData, refSrcData
                        , ZFUISize, frameSize
                        , zfindex, frameCount
                        , zftimet, frameDuration
                        ) {
                    ZFSerializableData data;
                    zfstring outErrorHint;

                    data.child(refSrcData);
                    refSrcData.category(ZFSerializableKeyword_ZFUIImageAni_refSrc);

                    ZFSerializableData nodeData;
                    data.child(nodeData);
                    nodeData.itemClass(ZFSerializableKeyword_node);
                    nodeData.category(ZFSerializableKeyword_ZFUIImageAni_split);
                    nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_size, ZFUISizeToString(frameSize));
                    nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_count, zfindexToString(frameCount));
                    if(frameDuration != 0) {
                        nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(frameDuration));
                    }
                    zfargs.result(zfobj<v_ZFSerializableData>(data));
                } ZFLISTENER_END()
                holder->imageSerializeType(ZFUIImageSerializeType_ZFUIImageAni);
                holder->imageSerializeDataGetter(serializeImpl);
            }
        }
    }
    void serializeImpl(
            ZF_IN ZFUIImage *holder
            , ZF_IN ZFUIImage *ref
            , ZF_IN ZFArray *frameRects
            , ZF_IN zftimet duration
            , ZF_IN ZFArray *frameDurations
            ) {
        ZFLISTENER_4(serializeImpl
                , zfautoT<ZFUIImage>, ref
                , zfautoT<ZFArray>, frameRects
                , zftimet, duration
                , zfautoT<ZFArray>, frameDurations
                ) {
            ZFSerializableData data;
            zfstring outErrorHint;

            ZFSerializableData refData;
            data.child(refData);
            if(!ZFObjectToDataT(refData, ref, &outErrorHint)) {
                zfargs.result(zfobj<v_zfstring>(outErrorHint));
                return;
            }
            refData.category(ZFSerializableKeyword_ZFUIImageAni_ref);

            ZFSerializableData nodeData;
            data.child(nodeData);
            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFUIImageAni_frames);
            if(duration != 0) {
                nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(duration));
            }
            for(zfindex i = 0; i < frameRects->count(); ++i) {
                ZFSerializableData frameData;
                frameData.itemClass(ZFSerializableKeyword_node);

                v_ZFUIRect *rect = frameRects->get(i);
                frameData.attr(ZFSerializableKeyword_ZFUIImageAni_rect, ZFUIRectToString(rect->zfv));

                v_zftimet *frameDuration = (frameDurations && i < frameDurations->count() ? frameDurations->get(i) : zfnull);
                if(frameDuration != zfnull && frameDuration->zfv != duration) {
                    frameData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(frameDuration->zfv));
                }
            }
            zfargs.result(zfobj<v_ZFSerializableData>(data));
        } ZFLISTENER_END()
        holder->imageSerializeType(ZFUIImageSerializeType_ZFUIImageAni);
        holder->imageSerializeDataGetter(serializeImpl);
    }
    void serializeImpl(
            ZF_IN ZFUIImage *holder
            , ZF_IN const ZFInput &refSrc
            , ZF_IN ZFArray *frameRects
            , ZF_IN zftimet duration
            , ZF_IN ZFArray *frameDurations
            ) {
        if(!refSrc.callbackSerializeDisable()) {
            // serialize logic
            ZFSerializableData refSrcData;
            if(ZFCallbackToDataT(refSrcData, refSrc)) {
                ZFLISTENER_4(serializeImpl
                        , ZFSerializableData, refSrcData
                        , zfautoT<ZFArray>, frameRects
                        , zftimet, duration
                        , zfautoT<ZFArray>, frameDurations
                        ) {
                    ZFSerializableData data;
                    zfstring outErrorHint;

                    data.child(refSrcData);
                    refSrcData.category(ZFSerializableKeyword_ZFUIImageAni_refSrc);

                    ZFSerializableData nodeData;
                    data.child(nodeData);
                    nodeData.itemClass(ZFSerializableKeyword_node);
                    nodeData.category(ZFSerializableKeyword_ZFUIImageAni_frames);
                    if(duration != 0) {
                        nodeData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(duration));
                    }
                    for(zfindex i = 0; i < frameRects->count(); ++i) {
                        ZFSerializableData frameData;
                        nodeData.child(frameData);
                        frameData.itemClass(ZFSerializableKeyword_node);

                        v_ZFUIRect *rect = frameRects->get(i);
                        frameData.attr(ZFSerializableKeyword_ZFUIImageAni_rect, ZFUIRectToString(rect->zfv));

                        v_zftimet *frameDuration = (frameDurations && i < frameDurations->count() ? frameDurations->get(i) : zfnull);
                        if(frameDuration != zfnull && frameDuration->zfv != duration) {
                            frameData.attr(ZFSerializableKeyword_ZFUIImageAni_duration, zftimetToString(frameDuration->zfv));
                        }
                    }
                    zfargs.result(zfobj<v_ZFSerializableData>(data));
                } ZFLISTENER_END()
                holder->imageSerializeType(ZFUIImageSerializeType_ZFUIImageAni);
                holder->imageSerializeDataGetter(serializeImpl);
            }
        }
    }
};

// ============================================================
// images
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(ZFArray *, images)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageAniT(ret, images, duration, frameDurations)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFArray *, images)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        ) {
    if(ret == zfnull
            || images == zfnull || images->isEmpty()
            ) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFUIImageAniTask> task;

    task->images->addFrom(images);

    ZFAniForFrame *ani = task->ani;
    if(frameDurations == zfnull || frameDurations->isEmpty()) {
        for(zfindex i = 0; i < images->count(); ++i) {
            ani->frame(duration);
        }
    }
    else {
        zfindex count = images->count();
        if(frameDurations->count() >= count) {
            for(zfindex i = 0; i < count; ++i) {
                ani->frame(frameDurations->get(i)->to<v_zftimet *>()->zfv);
            }
        }
        else {
            for(zfindex i = 0; i < frameDurations->count(); ++i) {
                ani->frame(frameDurations->get(i)->to<v_zftimet *>()->zfv);
            }
            for(zfindex i = frameDurations->count(); i < count; ++i) {
                ani->frame(duration);
            }
        }
    }
    task->implLoaded = zftrue;

    task->attach(ret);
    task->serializeImpl(ret, images, duration, frameDurations);
    return zftrue;
}

// ============================================================
// split
ZFMETHOD_FUNC_DEFINE_4(zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageAniT(ret, ref, frameSize, frameCount, frameDuration)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        ) {
    if(ret == zfnull
            || ref == zfnull
            || frameSize.width == 0 || frameSize.height == 0
            || frameSize.width > ref->imageSize().width
            || frameCount == 0
            ) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFUIImageAniTask> task;
    if(!task->refOnLoad(ref, frameSize, frameCount, frameDuration)) {
        return zffalse;
    }
    task->attach(ret);
    task->serializeImpl(ret, ref, frameSize, frameCount, frameDuration);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_4(zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageAniT(ret, refSrc, frameSize, frameCount, frameDuration)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        ) {
    if(ret == zfnull
            || refSrc == zfnull
            || frameSize.width == 0 || frameSize.height == 0
            || frameCount == 0
            ) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFUIImageAniTask> task;

    ZFLISTENER_4(refOnLoad
            , zfautoT<_ZFP_I_ZFUIImageAniTask>, task
            , ZFUISize, frameSize
            , zfindex, frameCount
            , zftimet, frameDuration
            ) {
        task->implTaskId = zfnull;
        task->refOnLoad(zfargs.param0(), frameSize, frameCount, frameDuration);
    } ZFLISTENER_END()
    task->implTaskId = ZFUIImageLoad(refSrc, refOnLoad);

    task->attach(ret);
    task->serializeImpl(ret, refSrc, frameSize, frameCount, frameDuration);
    return ret;
}

// ============================================================
// frames
ZFMETHOD_FUNC_DEFINE_4(zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageAniT(ret, ref, frameRects, duration, frameDurations)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        ) {
    if(ret == zfnull
            || ref == zfnull
            || frameRects == zfnull || frameRects->isEmpty()
            ) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFUIImageAniTask> task;
    if(!task->refOnLoad(ref, frameRects, duration, frameDurations)) {
        return zffalse;
    }
    task->attach(ret);
    task->serializeImpl(ret, ref, frameRects, duration, frameDurations);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_4(zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        ) {
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    if(ZFUIImageAniT(ret, refSrc, frameRects, duration, frameDurations)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        ) {
    if(ret == zfnull
            || refSrc == zfnull
            || frameRects == zfnull || frameRects->isEmpty()
            ) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFUIImageAniTask> task;

    ZFLISTENER_4(refOnLoad
            , zfautoT<_ZFP_I_ZFUIImageAniTask>, task
            , zfautoT<ZFArray>, frameRects
            , zftimet, duration
            , zfautoT<ZFArray>, frameDurations
            ) {
        task->implTaskId = zfnull;
        task->refOnLoad(zfargs.param0(), frameRects, duration, frameDurations);
    } ZFLISTENER_END()
    task->implTaskId = ZFUIImageLoad(refSrc, refOnLoad);

    task->attach(ret);
    task->serializeImpl(ret, refSrc, frameRects, duration, frameDurations);
    return ret;
}

// ============================================================
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ani, ZFUIImageSerializeType_ZFUIImageAni) {
    ZFSerializableData refData;

    // images
    refData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageAni_images);
    if(refData != zfnull) {
        refData.resolveMark();

        zfobj<ZFArray> images;
        zftimet duration = 0;
        zfobj<ZFArray> frameDurations;
        zfbool hasSpecDuration = zffalse;

        ZFSerializableUtilSerializeAttrFromData(refData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFUIImageAni_duration, zftimet, duration, {
                    return zffalse;
                });
        for(zfindex i = 0; i < refData.childCount(); ++i) {
            const ZFSerializableData &frameData = refData.childAt(i);

            zftimet frameDuration = duration;
            ZFSerializableUtilSerializeAttrFromData(frameData, outErrorHint, outErrorPos,
                    check, ZFSerializableKeyword_ZFUIImageAni_duration, zftimet, frameDuration, {
                        return zffalse;
                    });

            zfauto frameImage;
            if(!ZFObjectFromDataT(frameImage, frameData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            if(frameImage == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, frameData
                        , "failed to serialize frame image"
                        );
                return zffalse;
            }

            images->add(frameImage);
            frameDurations->add(zfobj<v_zftimet>(frameDuration));
            if(frameDuration != duration) {
                hasSpecDuration = zftrue;
            }
        }
        if(images->isEmpty()) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, refData
                    , "no valid frame image"
                    );
            return zffalse;
        }
        ret->imageSerializeDisable(zftrue);
        return ZFUIImageAniT(ret, images, duration, hasSpecDuration ? frameDurations : zfnull);
    }

    // ref or refSrc
    refData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageAni_ref);
    ZFSerializableData refSrcData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageAni_refSrc);
    if(refData != zfnull || refSrcData != zfnull) {
        ZFSerializableData splitData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageAni_split);
        ZFSerializableData framesData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageAni_frames);
        if(splitData == zfnull && framesData == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, refData
                    , "missing split or frames node"
                    );
            return zffalse;
        }

        zfautoT<ZFUIImage> ref;
        ZFInput refSrc;
        if(refData) {
            if(!ZFObjectFromDataT(ref, refData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
        }
        else {
            if(!ZFCallbackFromDataT(refSrc, refSrcData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
        }

        if(splitData) {
            splitData.resolveMark();
            ZFUISize frameSize = ZFUISizeZero();
            zfindex frameCount = 0;
            zftimet frameDuration = 0;

            ZFSerializableUtilSerializeAttrFromData(splitData, outErrorHint, outErrorPos,
                    require, ZFSerializableKeyword_ZFUIImageAni_size, ZFUISize, frameSize, {
                        return zffalse;
                    });
            ZFSerializableUtilSerializeAttrFromData(splitData, outErrorHint, outErrorPos,
                    require, ZFSerializableKeyword_ZFUIImageAni_count, zfindex, frameCount, {
                        return zffalse;
                    });
            ZFSerializableUtilSerializeAttrFromData(splitData, outErrorHint, outErrorPos,
                    check, ZFSerializableKeyword_ZFUIImageAni_duration, zftimet, frameDuration, {
                        return zffalse;
                    });
            ret->imageSerializeDisable(zftrue);
            if(refData) {
                return ZFUIImageAniT(ret, ref, frameSize, frameCount, frameDuration);
            }
            else {
                return ZFUIImageAniT(ret, refSrc, frameSize, frameCount, frameDuration);
            }
        }
        else {
            framesData.resolveMark();

            zfobj<ZFArray> frameRects;
            zftimet duration = 0;
            zfobj<ZFArray> frameDurations;
            zfbool hasSpecDuration = zffalse;

            ZFSerializableUtilSerializeAttrFromData(framesData, outErrorHint, outErrorPos,
                    check, ZFSerializableKeyword_ZFUIImageAni_duration, zftimet, duration, {
                        return zffalse;
                    });

            for(zfindex i = 0; i < framesData.childCount(); ++i) {
                const ZFSerializableData &nodeData = framesData.childAt(i);
                nodeData.resolveMark();
                ZFUIRect frameRect = ZFUIRectZero();
                zftimet frameDuration = duration;
                ZFSerializableUtilSerializeAttrFromData(nodeData, outErrorHint, outErrorPos,
                        require, ZFSerializableKeyword_ZFUIImageAni_rect, ZFUIRect, frameRect, {
                            return zffalse;
                        });
                ZFSerializableUtilSerializeAttrFromData(nodeData, outErrorHint, outErrorPos,
                        check, ZFSerializableKeyword_ZFUIImageAni_duration, zftimet, frameDuration, {
                            return zffalse;
                        });
                frameRects->add(zfobj<v_ZFUIRect>(frameRect));
                frameDurations->add(zfobj<v_zftimet>(frameDuration));
                if(frameDuration != duration) {
                    hasSpecDuration = zftrue;
                }
            }
            ret->imageSerializeDisable(zftrue);
            if(refData) {
                return ZFUIImageAniT(ret, ref, frameRects, duration, hasSpecDuration ? frameDurations : zfnull);
            }
            else {
                return ZFUIImageAniT(ret, refSrc, frameRects, duration, hasSpecDuration ? frameDurations : zfnull);
            }
        }
    }

    ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, refData
            , "no valid image ref"
            );
    return zffalse;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFUIImage>, ZFUIImageAniLoad
        , ZFMP_IN(const ZFInput &, input)
        ) {
    if(!input || !input.callbackId() || !input.pathInfo()) {
        return zfnull;
    }

    zfstring fileName = (input.pathInfo()
            ? ZFPathInfoToFileName(input.pathInfo())
            : input.callbackId()
            );

    // path/test-(40x30-10).png
    // path/test-(40x30-10-33).png
    zfobj<ZFRegExp> pattern("\\-\\(([0-9]+)x([0-9]+)\\-([0-9]+)(\\-([0-9]+))?\\)");
    ZFRegExpResult match;
    pattern->find(match, fileName);
    if(!match.matched
            || (match.namedGroups.count() != 3 && match.namedGroups.count() != 5)
            ) {
        return zfnull;
    }

    ZFUISize frameSize = ZFUISizeZero();
    zfindex frameCount = 0;
    zftimet frameDuration = 0;

    if(zffalse
            || !zffloatFromStringT(frameSize.width, fileName + match.namedGroups[0].start, match.namedGroups[0].count)
            || !zffloatFromStringT(frameSize.height, fileName + match.namedGroups[1].start, match.namedGroups[1].count)
            || !zfindexFromStringT(frameCount, fileName + match.namedGroups[2].start, match.namedGroups[2].count)
            ) {
        return zfnull;
    }
    if(match.namedGroups.count() == 5) {
        if(!zftimetFromStringT(frameDuration, fileName + match.namedGroups[4].start, match.namedGroups[4].count)) {
            return zfnull;
        }
    }
    return ZFUIImageAni(input, frameSize, frameCount, frameDuration);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIImageAniSave
        , ZFMP_IN(const ZFPathInfo &, dst)
        , ZFMP_IN(ZFArray *, images)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        ) {
    if(images == zfnull || images->isEmpty()) {
        return zffalse;
    }
    const ZFPathInfoImpl *pathInfoImpl = ZFPathInfoImplForPathType(dst.pathType());
    if(pathInfoImpl == zfnull) {
        return zffalse;
    }

    zfuint frameCount = (zfuint)images->count();
    ZFUISize frameSize = ZFUISizeZero();
    for(zfuint i = 0; i < frameCount; ++i) {
        ZFUIImage *image = images->get(i);
        if(image == zfnull || image->imageSize().width == 0 || image->imageSize().height == 0) {
            return zffalse;
        }
        if(image->imageSize().width > frameSize.width) {
            frameSize.width = image->imageSize().width;
        }
        if(image->imageSize().height > frameSize.height) {
            frameSize.height = image->imageSize().height;
        }
    }

    zfuint x = 1;
    zffloat diffMin = -1;
    for(zfuint t = 1; t <= frameCount; ++t) {
        zffloat diff = zfmAbs(frameSize.width * t - frameSize.height * (zfuint)((frameCount + t - 1) / t));
        if(diffMin < 0 || diff <= diffMin) {
            x = t;
        }
    }

    ZFUISize imageSize = ZFUISizeCreate(frameSize.width * x, frameSize.height * (zfuint)((frameCount + x - 1) / x));
    void *token = ZFUIDraw::beginForImage(imageSize);
    zfuint frameIndex = 0;
    for(zffloat y = 0; frameIndex < frameCount; ) {
        for(zffloat x = 0; frameIndex < frameCount; ) {
            if(x + frameSize.width > imageSize.width) {
                x = 0;
                y += frameSize.height;
            }
            if(y + frameSize.height > imageSize.height) {
                return zffalse;
            }

            ZFUIImage *image = images->get(frameIndex);
            if(image->imageSize() == frameSize) {
                ZFUIDraw::drawImage(token, image, ZFUIRectZero(), ZFUIRectCreate(
                            x, y, frameSize.width, frameSize.height
                            ));
            }
            else {
                const ZFUISize &t = image->imageSize();
                ZFUIDraw::drawImage(token, image, ZFUIRectZero(), ZFUIRectCreate(
                            x + (frameSize.width - t.width) / 2
                            , y + (frameSize.height - t.height) / 2
                            , t.width
                            , t.height
                            ));
            }


            ++frameIndex;
            x += frameSize.width;
        }
    }
    zfautoT<ZFUIImage> holder = ZFUIDraw::endForImage(token);
    if(!holder) {
        return zffalse;
    }

    zfstring fileName = ZFFileNameOfWithoutExt(ZFPathInfoToFileName(dst));
    if(!fileName) {
        return zffalse;
    }
    if(frameDuration != 0) {
        zfstringAppend(fileName, "-(%sx%s-%s-%s)"
                , frameSize.width
                , frameSize.height
                , frameCount
                , frameDuration
                );
    }
    else {
        zfstringAppend(fileName, "-(%sx%s-%s)"
                , frameSize.width
                , frameSize.height
                , frameCount
                );
    }
    zfstring fileExt = ZFFileExtOf(pathInfoImpl->implToFileName(dst.pathData()));
    if(!fileExt) {
        return zffalse;
    }
    if(!fileExt) {
        fileName += ".";
        fileName += fileExt;
    }

    zfstring pathData = pathInfoImpl->implToParent(dst.pathData());
    pathData = pathInfoImpl->implToChild(pathData, fileName);
    if(!pathData) {
        return zffalse;
    }
    ZFOutput output;
    output.callbackSerializeDisable();
    if(!ZFOutputForPathInfoT(output, ZFPathInfo(dst.pathType(), pathData))) {
        return zffalse;
    }
    if(!ZFUIImageToOutput(output, holder)) {
        return zffalse;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END
