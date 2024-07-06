/**
 * @file ZFProtocolZFAudio.h
 * @brief protocol for ZFAudio
 */

#ifndef _ZFI_ZFProtocolZFAudio_h_
#define _ZFI_ZFProtocolZFAudio_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFAudio.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFAudio
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFAudio)
public:
    /** @brief create native audio */
    virtual void *nativeAudioCreate(ZF_IN ZFAudio *audio) zfpurevirtual;
    /** @brief destroy native audio */
    virtual void nativeAudioDestroy(ZF_IN ZFAudio *audio) zfpurevirtual;

    /** @brief load native audio from input */
    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const ZFInput &input
            ) zfpurevirtual;
    /** @brief load native audio from input */
    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const zfstring &url
            ) zfpurevirtual;
    /** @brief cancel load native audio */
    virtual void nativeAudioLoadCancel(ZF_IN ZFAudio *audio) zfpurevirtual;

    /** @brief start native audio, ensured called after loaded success */
    virtual void nativeAudioStart(ZF_IN ZFAudio *audio) zfpurevirtual;
    /** @brief stop native audio, ensured called after loaded success and started */
    virtual void nativeAudioStop(ZF_IN ZFAudio *audio) zfpurevirtual;
    /** @brief resume native audio, ensured called after loaded success and paused */
    virtual void nativeAudioResume(ZF_IN ZFAudio *audio) zfpurevirtual;
    /** @brief pause native audio, ensured called after loaded success and started */
    virtual void nativeAudioPause(ZF_IN ZFAudio *audio) zfpurevirtual;

    /** @brief get native audio duration */
    virtual zftimet nativeAudioDuration(ZF_IN ZFAudio *audio) zfpurevirtual;
    /** @brief get native audio position */
    virtual zftimet nativeAudioPosition(ZF_IN ZFAudio *audio) zfpurevirtual;
    /** @brief set native audio position */
    virtual void nativeAudioPosition(
            ZF_IN ZFAudio *audio
            , ZF_IN zftimet position
            ) zfpurevirtual;

    /** @brief set native audio volume */
    virtual void nativeAudioVolume(
            ZF_IN ZFAudio *audio
            , ZF_IN zffloat volume
            ) zfpurevirtual;

public:
    /**
     * @brief implementation must notify when audio load success or fail
     */
    zffinal void notifyAudioOnLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN zfbool success
            , ZF_IN v_zfstring *errorHint
            );
    /**
     * @brief implementation must notify when audio play success or fail
     */
    zffinal void notifyAudioOnStop(
            ZF_IN ZFAudio *audio
            , ZF_IN zfbool success
            , ZF_IN v_zfstring *errorHint
            );
    /**
     * @brief implementation must notify when audio start success and about to play,
     *   or resume from pause
     */
    zffinal void notifyAudioOnResume(ZF_IN ZFAudio *audio);
    /**
     * @brief implementation must notify when audio paused
     */
    zffinal void notifyAudioOnPause(ZF_IN ZFAudio *audio);
ZFPROTOCOL_INTERFACE_END(ZFAudio)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFAudio_h_

