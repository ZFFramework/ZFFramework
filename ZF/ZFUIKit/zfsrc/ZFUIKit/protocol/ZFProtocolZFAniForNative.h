/**
 * @file ZFProtocolZFAniForNative.h
 * @brief protocol for ZFAniForNative
 */

#ifndef _ZFI_ZFProtocolZFAniForNative_h_
#define _ZFI_ZFProtocolZFAniForNative_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFAniForNative.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFAniForNative
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFAniForNative)
public:
    /**
     * @brief create native animation
     */
    virtual void *nativeAniCreate(ZF_IN ZFAniForNative *ani) zfpurevirtual;
    /**
     * @brief destroy native animation
     */
    virtual void nativeAniDestroy(
            ZF_IN ZFAniForNative *ani
            , ZF_IN void *nativeAni
            ) zfpurevirtual;

    /**
     * @brief start native animation
     *
     * @note you should copy all settings from the ani
     * @note this method have no auto scale logic,
     *   size unit are scaled size under ZFFramework,
     *   you should apply scale manually by nativeAniScale
     *
     * here's a list of things you need to implement:
     * -  curve (ZFAniForNativeCurve)
     * -  aniAlpha (zffloat)
     * -  scaleX/Y/Z (zffloat)
     * -  translateX/Y/Z (zffloat)
     * -  rotateX/Y/Z (zffloat)
     *
     * @note more than one ani may added to same view,
     *   you must also implement this logic
     */
    virtual void nativeAniStart(
            ZF_IN ZFAniForNative *ani
            , ZF_IN zffloat nativeAniScale
            ) zfpurevirtual;
    /**
     * @brief stop native animation
     *
     * animation must be stopped immediately
     */
    virtual void nativeAniStop(ZF_IN ZFAniForNative *ani) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must call this to notify animation stopped
     */
    zffinal void notifyAniStop(ZF_IN ZFAniForNative *ani) {
        ani->_ZFP_ZFAniForNative_notifyStop();
    }
ZFPROTOCOL_INTERFACE_END(ZFAniForNative)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFAniForNative_h_

