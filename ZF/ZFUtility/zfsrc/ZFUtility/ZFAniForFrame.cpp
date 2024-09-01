#include "ZFAniForFrame.h"

// #define _ZFP_ZFAniForFrame_DEBUG_noGlobalTimer 1

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAniForFramePrivate
zfclassNotPOD _ZFP_ZFAniForFramePrivate {
public:
    ZFCoreArray<zftimet> frames;
    zftimet aniDurationFixed;
    zfuint frameIndex;

    zfbool useGlobalTimer;

    ZFListener globalTimerTask;
    zfuint globalTimerFrameCount;
    zfuint globalTimerFrameIndex;

    zfautoT<ZFTimer> builtinTimer;
public:
    _ZFP_ZFAniForFramePrivate(void)
    : frames()
    , aniDurationFixed(-1)
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
            owner->aniFrameOnUpdate(zfindexMax());
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
            ZFGlobalTimerDetach(owner->d->globalTimerTask);
            owner->d->globalTimerTask = zfnull;
        }
        else {
            owner->d->builtinTimer->timerStop();
            owner->d->builtinTimer = zfnull;
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
            owner->aniFrameOnUpdate(zfindexMax());
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
            owner->aniFrameOnUpdate(zfindexMax());
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
    zfstring framesStr = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFAniForFrame_frames);
    ZFCoreArray<zftimet> frames;
    if(!zfCoreDataPairSplitInt(
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
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);
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
    zfCoreAssert(!this->aniRunning());
    d->aniDurationFixed = -1;
    d->frames.add(duration);
}
ZFMETHOD_DEFINE_1(ZFAniForFrame, void, frames
        , ZFMP_IN(const ZFCoreArray<zftimet> &, frames)
        ) {
    zfCoreAssert(!this->aniRunning());
    d->aniDurationFixed = -1;
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
    d->aniDurationFixed = -1;
    d->frames.remove(index);
}
ZFMETHOD_DEFINE_0(ZFAniForFrame, void, frameRemoveAll) {
    d->aniDurationFixed = -1;
    d->frames.removeAll();
}

ZFMETHOD_DEFINE_0(ZFAniForFrame, zfindex, frameIndex) {
    return d->frameIndex < d->frames.count() ? (zfindex)d->frameIndex : zfindexMax();
}

// ============================================================
// start stop
zftimet ZFAniForFrame::aniDurationFixed(void) {
    if(d->aniDurationFixed == -1) {
        d->aniDurationFixed = 0;
        for(zfindex i = 0; i < d->frames.count(); ++i) {
            d->aniDurationFixed += d->frames[i];
        }
    }
    return d->aniDurationFixed;
}

void ZFAniForFrame::aniImplDelay(void) {
#if !_ZFP_ZFAniForFrame_DEBUG_noGlobalTimer
    d->useGlobalTimer = (this->useGlobalTimer());
#else
    d->useGlobalTimer = zffalse;
#endif
    if(!d->useGlobalTimer) {
        zfsuper::aniImplDelay();
        return;
    }
    d->globalTimerFrameCount = (zfuint)zfmRound(this->aniDelay() / ZFGlobalTimerIntervalDefault());
    if(d->globalTimerFrameCount == 0) {
        d->globalTimerTask = zfnull;
        this->aniImplDelayNotifyFinish();
        return;
    }
    d->globalTimerFrameIndex = 0;
    ZFAniForFrame *owner = this;
    ZFLISTENER_1(globalTimerOnActivate
            , ZFAniForFrame *, owner
            ) {
        ++(owner->d->globalTimerFrameIndex);
        if(owner->d->globalTimerFrameIndex >= owner->d->globalTimerFrameCount) {
            ZFGlobalTimerDetach(owner->d->globalTimerTask);
            owner->d->globalTimerTask = zfnull;
            owner->aniImplDelayNotifyFinish();
        }
    } ZFLISTENER_END()
    d->globalTimerTask = globalTimerOnActivate;
    ZFGlobalTimerAttach(owner->d->globalTimerTask);
}
void ZFAniForFrame::aniImplDelayCancel(void) {
    if(!d->useGlobalTimer) {
        zfsuper::aniImplDelayCancel();
        return;
    }
    if(d->globalTimerTask) {
        ZFGlobalTimerDetach(d->globalTimerTask);
        d->globalTimerTask = zfnull;
    }
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
        ZFAniForFrame::EventAniFrameOnUpdate(),
        zfobj<v_zfindex>(frameIndex));
}

ZF_NAMESPACE_GLOBAL_END

