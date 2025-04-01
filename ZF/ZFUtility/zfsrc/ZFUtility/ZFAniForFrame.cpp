#include "ZFAniForFrame.h"

// #define _ZFP_ZFAniForFrame_DEBUG_noGlobalTimer 1

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAniForFramePrivate
zfclassNotPOD _ZFP_ZFAniForFramePrivate {
public:
    ZFCoreArray<zftimet> frames;
    zftimet durationFixed;
    zfuint frameIndex;

    zfbool useGlobalTimer;

    ZFListener globalTimerTask;
    zfuint globalTimerFrameCount;
    zfuint globalTimerFrameIndex;

    zfautoT<ZFTimer> builtinTimer;
public:
    _ZFP_ZFAniForFramePrivate(void)
    : frames()
    , durationFixed(-1)
    , frameIndex((zfuint)(-1))
    , useGlobalTimer(zffalse)
    , globalTimerTask()
    , globalTimerFrameCount(0)
    , globalTimerFrameIndex(0)
    , builtinTimer()
    {
    }

public:
    static void doStart(ZF_IN ZFAniForFrame *owner) {
        if(owner->d->frames.isEmpty()) {
            owner->d->frameIndex = (zfuint)-1;
            owner->aniImplNotifyStop();
            return;
        }

        zftimet frameDuration = owner->d->frames[0];

#if !_ZFP_ZFAniForFrame_DEBUG_noGlobalTimer
        if(owner->useGlobalTimer()) {
            owner->d->useGlobalTimer = zftrue;
            owner->d->frameIndex = 0;
            owner->d->globalTimerFrameCount = (zfuint)zfmRound(frameDuration / ZFGlobalTimerIntervalDefault());
            owner->d->globalTimerFrameIndex = 0;

            ZFLISTENER_1(globalTimerOnActivate
                    , ZFAniForFrame *, owner
                    ) {
                _ZFP_ZFAniForFramePrivate::globalTimerOnActivate(owner);
            } ZFLISTENER_END()
            owner->d->globalTimerTask = globalTimerOnActivate;
            ZFGlobalTimerAttach(owner->d->globalTimerTask);
        }
        else
#endif
        {
            owner->d->useGlobalTimer = zffalse;
            owner->d->frameIndex = 0;
            builtinTimerNext(owner);
        }
        owner->aniFrameOnUpdate(0);
    }
    static void doStop(ZF_IN ZFAniForFrame *owner) {
        if(owner->d->useGlobalTimer) {
            if(owner->d->globalTimerTask) {
                ZFGlobalTimerDetach(owner->d->globalTimerTask);
                owner->d->globalTimerTask = zfnull;
            }
        }
        else {
            if(owner->d->builtinTimer) {
                owner->d->builtinTimer->stop();
                owner->d->builtinTimer = zfnull;
            }
        }
    }

private:
    static void globalTimerOnActivate(ZF_IN ZFAniForFrame *owner) {
        ++(owner->d->globalTimerFrameIndex);
        if(owner->d->globalTimerFrameIndex < owner->d->globalTimerFrameCount) {
            return;
        }
        ++(owner->d->frameIndex);
        if(owner->d->frameIndex < owner->d->frames.count()) {
            owner->d->globalTimerFrameCount = (zfuint)zfmRound(owner->d->frames[owner->d->frameIndex] / ZFGlobalTimerIntervalDefault());
            owner->d->globalTimerFrameIndex = 0;
            owner->aniFrameOnUpdate(owner->d->frameIndex);
        }
        else {
            owner->aniImplNotifyStop();
        }
    }
    static void builtinTimerNext(ZF_IN ZFAniForFrame *owner) {
        ZFLISTENER_1(builtinTimerOnActivate
                , ZFAniForFrame *, owner
                ) {
            _ZFP_ZFAniForFramePrivate::builtinTimerOnActivate(owner);
        } ZFLISTENER_END()
        owner->d->builtinTimer = ZFTimerOnce(owner->d->frames[owner->d->frameIndex], builtinTimerOnActivate);
    }
    static void builtinTimerOnActivate(ZF_IN ZFAniForFrame *owner) {
        ++(owner->d->frameIndex);
        if(owner->d->frameIndex < owner->d->frames.count()) {
            owner->aniFrameOnUpdate(owner->d->frameIndex);
            builtinTimerNext(owner);
        }
        else {
            owner->aniImplNotifyStop();
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniForFrame)

ZFEVENT_REGISTER(ZFAniForFrame, AniFrameOnUpdate)

// ============================================================
// object
void ZFAniForFrame::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAniForFramePrivate);
}
void ZFAniForFrame::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfbool ZFAniForFrame::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}
    this->frameRemoveAll();
    zfstring framesStr = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFAniForFrame_frames);
    ZFCoreArray<zftimet> frames;
    if(!ZFCoreDataPairSplitInt(
                frames
                , zfindexMax()
                , framesStr
                , framesStr.length()
                , ","
                , zfnull
                , zfnull
                )) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "invalid frames value: %s", framesStr);
        return zffalse;
    }
    return zftrue;
}
zfbool ZFAniForFrame::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);
    if(ref != zfnull
            && d->frames.objectCompare(ref->d->frames) == ZFCompareEqual
            ) {
        return zftrue;
    }
    zfstring framesStr;
    for(zfindex i = 0; i < d->frames.count(); ++i) {
        if(!framesStr.isEmpty()) {
            framesStr += ",";
        }
        zftimetToStringT(framesStr, d->frames[i]);
    }
    serializableData.attr(ZFSerializableKeyword_ZFAniForFrame_frames, framesStr);
    return zftrue;
}

// ============================================================
// animation setting
ZFMETHOD_DEFINE_1(ZFAniForFrame, void, frame
        , ZFMP_IN(zftimet, duration)
        ) {
    ZFCoreAssert(!this->started());
    d->durationFixed = -1;
    d->frames.add(duration);
}
ZFMETHOD_DEFINE_1(ZFAniForFrame, void, frames
        , ZFMP_IN(const ZFCoreArray<zftimet> &, frames)
        ) {
    ZFCoreAssert(!this->started());
    d->durationFixed = -1;
    d->frames.addFrom(frames);
}

ZFMETHOD_DEFINE_0(ZFAniForFrame, zfindex, frameCount) {
    return d->frames.count();
}
ZFMETHOD_DEFINE_1(ZFAniForFrame, zftimet, frameAt
        , ZFMP_IN(zfindex, index)
        ) {
    return d->frames[index];
}
ZFMETHOD_DEFINE_1(ZFAniForFrame, void, frameRemoveAt
        , ZFMP_IN(zfindex, index)
        ) {
    d->durationFixed = -1;
    d->frames.remove(index);
}
ZFMETHOD_DEFINE_0(ZFAniForFrame, void, frameRemoveAll) {
    d->durationFixed = -1;
    d->frames.removeAll();
}

ZFMETHOD_DEFINE_0(ZFAniForFrame, zfindex, frameIndex) {
    return d->frameIndex < d->frames.count() ? (zfindex)d->frameIndex : zfindexMax();
}

// ============================================================
// start stop
zftimet ZFAniForFrame::durationFixed(void) {
    if(d->durationFixed == -1) {
        d->durationFixed = 0;
        for(zfindex i = 0; i < d->frames.count(); ++i) {
            d->durationFixed += d->frames[i];
        }
    }
    return d->durationFixed;
}

void ZFAniForFrame::aniImplStart(void) {
    zfsuper::aniImplStart();
    _ZFP_ZFAniForFramePrivate::doStart(this);
}
void ZFAniForFrame::aniImplStop(void) {
    _ZFP_ZFAniForFramePrivate::doStop(this);
    zfsuper::aniImplStop();
}

void ZFAniForFrame::aniFrameOnUpdate(ZF_IN zfindex frameIndex) {
    this->observerNotify(
        ZFAniForFrame::E_AniFrameOnUpdate(),
        zfobj<v_zfindex>(frameIndex));
}

ZF_NAMESPACE_GLOBAL_END

