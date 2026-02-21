#include "ZFAudio.h"
#include "protocol/ZFProtocolZFAudio.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFAudioPrivate {
public:
    typedef enum {
        LoadFlag = 1 << 1,
        StartFlag = 1 << 2,
        PauseFlag = 1 << 3,
        ImplLoaded = 1 << 4,
        ImplPlaying = 1 << 5,
    } StateFlag;

public:
    void *nativeAudio;
    zftimet positionToUpdate;
    zfuint state; // StateFlag
    zfuint loopCur;
    ZFPathInfo pathInfo;
    zfidentity audioTaskId;

public:
    _ZFP_ZFAudioPrivate(void)
    : nativeAudio(zfnull)
    , positionToUpdate(-1)
    , state(0)
    , loopCur(0)
    , pathInfo()
    , audioTaskId((zfidentity)zfmRand())
    {
    }
};

static zfstlmap<ZFAudio *, zfbool> &_ZFP_ZFAudio_playing(void) {
    static zfstlmap<ZFAudio *, zfbool> d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAudioAutoStop, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFAudioAutoStop) {
    if(!_ZFP_ZFAudio_playing().empty()) {
        zfstlmap<ZFAudio *, zfbool> m;
        m.swap(_ZFP_ZFAudio_playing());
        for(zfstlmap<ZFAudio *, zfbool>::iterator it = m.begin(); it != m.end(); ++it) {
            it->first->stop();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFAudioAutoStop)

// ============================================================
ZFOBJECT_REGISTER(ZFAudio)

ZFEVENT_REGISTER(ZFAudio, AudioOnLoad)
ZFEVENT_REGISTER(ZFAudio, AudioOnStart)
ZFEVENT_REGISTER(ZFAudio, AudioOnStop)
ZFEVENT_REGISTER(ZFAudio, AudioOnResume)
ZFEVENT_REGISTER(ZFAudio, AudioOnPause)
ZFEVENT_REGISTER(ZFAudio, AudioOnLoop)

ZFMETHOD_DEFINE_0(ZFAudio, zfbool, implAvailable) {
    return ZFPROTOCOL_IS_AVAILABLE(ZFAudio);
}

ZFMETHOD_DEFINE_0(ZFAudio, void *, nativeAudio) {
    return d->nativeAudio;
}

ZFMETHOD_DEFINE_0(ZFAudio, zfbool, startable) {
    return ZFBitTest(d->state, 0
            | _ZFP_ZFAudioPrivate::LoadFlag
            | _ZFP_ZFAudioPrivate::ImplLoaded
            );
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, loading) {
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, loaded) {
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, started) {
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, playing) {
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
}
ZFMETHOD_DEFINE_0(ZFAudio, zfbool, paused) {
    return ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
}

ZFMETHOD_DEFINE_0(ZFAudio, zfindex, loopCur) {
    return (zfindex)d->loopCur;
}

ZFMETHOD_DEFINE_1(ZFAudio, void, load
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    this->load(ZFInputForPathInfo(pathInfo));
}

ZFMETHOD_DEFINE_1(ZFAudio, void, load
        , ZFMP_IN(const ZFInput &, input)
        ) {
    if(input.pathInfo() != d->pathInfo) {
        zfobjRetain(this); // release when OnLoad
        this->stop();
        d->pathInfo = input.pathInfo();

        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplLoaded);
        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioLoad(this, input);
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, void, start) {
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplPlaying)) {
        this->stop();
    }
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)) {
        return;
    }
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag)) {
        zfobjRetain(this); // release when OnStop

        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        d->loopCur = 0;
        this->audioOnStart();
    }
    else if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded)) {
        zfobjRetain(this); // release when OnStop

        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        d->loopCur = 0;
        _ZFP_ZFAudio_playing()[this] = zftrue;
        this->audioOnStart();
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStart(this);
    }
    else {
        ZFLogTrim() << this << " start called before load";
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, void, stop) {
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag) && !ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag)) {
        return;
    }
    zfobjReleaseInScope(zfobjRetain(this));
    d->positionToUpdate = -1;
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag)) {
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioLoadCancel(this);
        this->_ZFP_ZFAudio_OnLoad(v_ZFResultType::e_Cancel, zfnull);
    }
    else if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)) {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStop(this);
        this->_ZFP_ZFAudio_OnStop(v_ZFResultType::e_Cancel, zfnull);
    }
    else {
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
        this->audioOnStop(v_ZFResultType::e_Cancel, zfnull);
    }
    ++(d->audioTaskId);
}

ZFMETHOD_DEFINE_0(ZFAudio, void, resume) {
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag) && !ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag)) {
        return;
    }
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag)) {
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioResume(this);
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, void, pause) {
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)) {
        return;
    }
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag)) {
        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPause(this);
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, zftimet, duration) {
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded)) {
        return ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioDuration(this);
    }
    else {
        return 0;
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, zftimet, position) {
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded)) {
        return ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPosition(this);
    }
    else {
        return 0;
    }
}
ZFMETHOD_DEFINE_1(ZFAudio, void, position
        , ZFMP_IN(zftimet, position)
        ) {
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)) {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPosition(this, position);
    }
    else if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)) {
        d->positionToUpdate = position;
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFAudio, zffloat, volume) {
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded)) {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioVolume(this, propertyValue);
    }
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAudio
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    this->load(pathInfo);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFAudio
        , ZFMP_IN(const ZFInput &, input)
        ) {
    this->load(input);
}

ZFMETHOD_DEFINE_0(ZFAudio, const ZFPathInfo &, pathInfo) {
    return d->pathInfo;
}

ZFMETHOD_DEFINE_0(ZFAudio, const zfchar *, stateHint) {
    if(this->loading()) {
        return "loading";
    }
    else if(this->playing()) {
        return "playing";
    }
    else if(this->paused()) {
        return "paused";
    }
    else if(this->started()) {
        return "waiting";
    }
    else if(this->loaded()) {
        return "loaded";
    }
    else {
        return "idle";
    }
}

ZFMETHOD_DEFINE_0(ZFAudio, zfidentity, audioTaskId) {
    return d->audioTaskId;
}

// ============================================================
void ZFAudio::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAudioPrivate);
    d->nativeAudio = ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioCreate(this);
}
void ZFAudio::objectOnDealloc(void) {
    ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioDestroy(this);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAudio::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += this->classData()->className();
    ret += "(";
    if(this->pathInfo()) {
        ZFPathInfoToStringT(ret, this->pathInfo());
    }
    else {
        zfsFromPointerT(ret, this);
    }
    ret += ") ";
    ret += this->stateHint();
    if(this->started()) {
        ret += " ";
        zftimetToStringT(ret, this->position() / 1000);
        ret += "/";
        zftimetToStringT(ret, this->duration() / 1000);
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

zfbool ZFAudio::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {
        return zffalse;
    }
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos
            , check, "src", ZFPathInfo, d->pathInfo, {
                return zffalse;
            });
    return zftrue;
}
zfbool ZFAudio::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    zfself *ref = zfcast(zfself *, refOwner);
    ZFSerializableUtilSerializeAttrToData(serializableData, outErrorHint, ref
            , "src", ZFPathInfo, this->pathInfo(), ref->pathInfo(), ZFPathInfo(), {
                return zffalse;
            });
    return zfsuper::serializableOnSerializeToData(serializableData, outErrorHint, refOwner);
}

void ZFAudio::_ZFP_ZFAudio_OnLoad(
        ZF_IN ZFResultType result
        , ZF_IN v_zfstring *errorHint
        ) {
    ZFCoreAssertWithMessage(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::LoadFlag)
            , "invalid audio state: %s (%s, %s)"
            , d->state
            , result
            , errorHint
            );
    ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::LoadFlag);
    if(result == v_ZFResultType::e_Success) {
        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::ImplLoaded);

        if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)) {
            _ZFP_ZFAudio_playing()[this] = zftrue;
            ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStart(this);
            ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioVolume(this, this->volume());
        }
        if(d->positionToUpdate != -1) {
            ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPosition(this, d->positionToUpdate);
            d->positionToUpdate = -1;
        }
    }

    this->audioOnLoad(result, errorHint);
    zfobjRelease(this); // retained when load
}
void ZFAudio::_ZFP_ZFAudio_OnStop(
        ZF_IN ZFResultType result
        , ZF_IN v_zfstring *errorHint
        ) {
    ZFCoreAssertWithMessage(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)
            , "invalid audio state: %s (%s, %s)"
            , d->state
            , result
            , errorHint
            );
    if(result != v_ZFResultType::e_Cancel) {
        ++d->loopCur;
    }
    if(result != v_ZFResultType::e_Cancel && (this->loop() == zfindexMax() || d->loopCur <= this->loop())) {
        _ZFP_ZFAudio_playing()[this] = zftrue;
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioStart(this);
        this->audioOnLoop();
    }
    else {
        _ZFP_ZFAudio_playing().erase(this);
        if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplPlaying)) {
            this->_ZFP_ZFAudio_OnPause();
        }
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::StartFlag);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::PauseFlag);
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
        this->audioOnStop(result, errorHint);

        ++(d->audioTaskId);
        zfobjRelease(this); // retained when start
    }
}
void ZFAudio::_ZFP_ZFAudio_OnResume(void) {
    ZFCoreAssertWithMessage(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)
            , "invalid audio state: %s"
            , d->state
            );
    if(!ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplPlaying)) {
        ZFBitSet(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
        this->audioOnResume();
    }

    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::PauseFlag)) {
        ZFPROTOCOL_ACCESS(ZFAudio)->nativeAudioPause(this);
    }
}
void ZFAudio::_ZFP_ZFAudio_OnPause(void) {
    ZFCoreAssertWithMessage(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplLoaded) && ZFBitTest(d->state, _ZFP_ZFAudioPrivate::StartFlag)
            , "invalid audio state: %s"
            , d->state
            );
    if(ZFBitTest(d->state, _ZFP_ZFAudioPrivate::ImplPlaying)) {
        ZFBitUnset(d->state, _ZFP_ZFAudioPrivate::ImplPlaying);
        this->audioOnPause();
    }
}

ZF_NAMESPACE_GLOBAL_END

