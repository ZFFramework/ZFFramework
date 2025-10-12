/**
 * @file ZFObjectIO_audio.h
 * @brief IO type for #ZFObjectIOLoad
 */

#ifndef _ZFI_ZFObjectIO_audio_h_
#define _ZFI_ZFObjectIO_audio_h_

#include "ZFAudioCache.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFObjectIOLoad
 *
 * for "*.mp3" file, raw audio files\n
 * additional audio extension can be added by #ZFObjectIO_audio_audioExtAdd
 */
#define ZFObjectIO_audio "audio"

// ============================================================
/**
 * @brief see #ZFObjectIO_audio
 *
 * add file ext that should be treated as raw audio IO type\n
 * default value:
 * -  "mp3"
 */
extern ZFLIB_ZFUIKit void ZFObjectIO_audio_audioExtAdd(ZF_IN const zfstring &audioExt);
/** @brief see #ZFObjectIO_audio_audioExtAdd */
extern ZFLIB_ZFUIKit void ZFObjectIO_audio_audioExtRemove(ZF_IN const zfstring &audioExt);
/** @brief see #ZFObjectIO_audio_audioExtAdd */
extern ZFLIB_ZFUIKit zfbool ZFObjectIO_audio_audioExtCheck(ZF_IN const zfstring &audioExt);
/** @brief see #ZFObjectIO_audio_audioExtAdd */
extern ZFLIB_ZFUIKit void ZFObjectIO_audio_audioExtGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret);
/** @brief see #ZFObjectIO_audio_audioExtAdd */
inline ZFCoreArray<zfstring> ZFObjectIO_audio_audioExtGetAll(void) {
    ZFCoreArray<zfstring> ret;
    ZFObjectIO_audio_audioExtGetAllT(ret);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectIO_audio_h_

