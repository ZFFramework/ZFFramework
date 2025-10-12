#include "ZFObjectIO_audio.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef zfstlhashmap<zfstring, zfbool> _ZFP_ZFObjectIO_audio_audioExtMapType;
static _ZFP_ZFObjectIO_audio_audioExtMapType &_ZFP_ZFObjectIO_audio_audioExtMap(void) {
    static _ZFP_ZFObjectIO_audio_audioExtMapType m;
    return m;
}

void ZFObjectIO_audio_audioExtAdd(ZF_IN const zfstring &audioExt) {
    ZFCoreAssert(audioExt);
    _ZFP_ZFObjectIO_audio_audioExtMap()[audioExt] = zftrue;
}
void ZFObjectIO_audio_audioExtRemove(ZF_IN const zfstring &audioExt) {
    _ZFP_ZFObjectIO_audio_audioExtMap().erase(audioExt);
}
zfbool ZFObjectIO_audio_audioExtCheck(ZF_IN const zfstring &audioExt) {
    _ZFP_ZFObjectIO_audio_audioExtMapType &m = _ZFP_ZFObjectIO_audio_audioExtMap();
    return m.find(audioExt) != m.end();
}
void ZFObjectIO_audio_audioExtGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) {
    _ZFP_ZFObjectIO_audio_audioExtMapType &m = _ZFP_ZFObjectIO_audio_audioExtMap();
    for(_ZFP_ZFObjectIO_audio_audioExtMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
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

