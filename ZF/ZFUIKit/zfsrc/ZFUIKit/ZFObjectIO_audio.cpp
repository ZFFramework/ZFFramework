#include "ZFObjectIO_audio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef ZFCoreSet<zfstring> _ZFP_ZFObjectIO_audio_audioExtMapType;
static _ZFP_ZFObjectIO_audio_audioExtMapType &_ZFP_ZFObjectIO_audio_audioExtMap(void) {
    static _ZFP_ZFObjectIO_audio_audioExtMapType m;
    return m;
}

void ZFObjectIO_audio_audioExtAdd(ZF_IN const zfstring &audioExt) {
    ZFCoreAssert(audioExt);
    _ZFP_ZFObjectIO_audio_audioExtMap().add(audioExt);
}
void ZFObjectIO_audio_audioExtRemove(ZF_IN const zfstring &audioExt) {
    _ZFP_ZFObjectIO_audio_audioExtMap().remove(audioExt);
}
zfbool ZFObjectIO_audio_audioExtCheck(ZF_IN const zfstring &audioExt) {
    _ZFP_ZFObjectIO_audio_audioExtMapType &m = _ZFP_ZFObjectIO_audio_audioExtMap();
    return m.isContain(audioExt);
}
void ZFObjectIO_audio_audioExtGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) {
    _ZFP_ZFObjectIO_audio_audioExtMapType &m = _ZFP_ZFObjectIO_audio_audioExtMap();
    for(zfiter it = m.iter(); it; ++it) {
        ret.add(m.iterValue(it));
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectIO_audio_audioExtDefault, ZFLevelZFFrameworkStatic) {
    ZFObjectIO_audio_audioExtAdd("mp3");
}
ZF_GLOBAL_INITIALIZER_END(ZFObjectIO_audio_audioExtDefault)

// ============================================================
ZFOBJECTIO_DEFINE(audio, ZFLevelZFFrameworkPostNormal, ZFM_EXPAND({
        return ZFObjectIO_audio_audioExtCheck(fileExt);
    }), {
        ret = ZFAudioLoad(input);
        if(ret == zfnull) {
            zfstringAppend(outErrorHint,
                "unable to load audio from %s",
                input.pathInfo());
            return zffalse;
        }
        else {
            return zftrue;
        }
    }, {
        ZFAudio *audio = obj;
        if(audio == zfnull) {
            zfstringAppend(outErrorHint,
                "object %s is not type of %s",
                ZFObjectInfoOfInstance(obj),
                ZFAudio::ClassData()->classNameFull());
            return zffalse;
        }
        if(audio->pathInfo().isEmpty()) {
            zfstringAppend(outErrorHint,
                "object %s contains no audio data",
                audio);
            return zffalse;
        }
        if(ZFInputRead(output, ZFInputForPathInfo(audio->pathInfo())) == zfindexMax()) {
            zfstringAppend(outErrorHint,
                "unable to convert audio %s to audio file",
                audio);
            return zffalse;
        }
        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

