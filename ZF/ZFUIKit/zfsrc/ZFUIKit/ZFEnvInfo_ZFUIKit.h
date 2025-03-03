/**
 * @file ZFEnvInfo_ZFUIKit.h
 * @brief info of native environment
 */

#ifndef _ZFI_ZFEnvInfo_ZFUIKit_h_
#define _ZFI_ZFEnvInfo_ZFUIKit_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief device type for ZFEnvInfo
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFEnvDeviceUIType)
    /**
     * @brief desktop devices such as PC or laptop,
     *   usually have large display and have keyboard and mouse as input
     */
    ZFENUM_VALUE(Desktop)
    /**
     * @brief handheld devices such as mobiles phones,
     *   usually have smaller display and have touch device as input
     */
    ZFENUM_VALUE(Handheld)
    /**
     * @brief wearable devices such as watches,
     *   usually have tiny display and have touch device and little buttons as input
     */
    ZFENUM_VALUE(Wearable)
    /**
     * @brief TV devices,
     *   usually have huge display and have little input devices
     */
    ZFENUM_VALUE(TV)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Desktop)
    ZFENUM_VALUE_REGISTER(Handheld)
    ZFENUM_VALUE_REGISTER(Wearable)
    ZFENUM_VALUE_REGISTER(TV)
ZFENUM_END(ZFLIB_ZFUIKit, ZFEnvDeviceUIType)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFEnvDeviceUIType)

ZF_NAMESPACE_BEGIN(ZFEnvInfo)

// ============================================================
/**
 * @brief device type, #v_ZFEnvDeviceUIType::e_Desktop if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, ZFEnvDeviceUIType, deviceUIType
        , ZFMP_IN_OPT(ZFEnvDeviceUIType, defaultValue, v_ZFEnvDeviceUIType::e_Desktop)
        )

ZF_NAMESPACE_END(ZFEnvInfo)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnvInfo_ZFUIKit_h_

