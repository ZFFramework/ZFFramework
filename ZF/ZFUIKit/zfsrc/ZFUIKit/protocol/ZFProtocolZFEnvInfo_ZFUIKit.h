/**
 * @file ZFProtocolZFEnvInfo_ZFUIKit.h
 * @brief protocol for ZFEnvInfo
 */

#ifndef _ZFI_ZFProtocolZFEnvInfo_ZFUIKit_h_
#define _ZFI_ZFProtocolZFEnvInfo_ZFUIKit_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFEnvInfo_ZFUIKit.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFEnvInfo_deviceUIInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief device type
     */
    virtual ZFEnvDeviceUIType deviceUIType(ZF_IN_OPT ZFEnvDeviceUIType defaultValue = v_ZFEnvDeviceUIType::e_Desktop) {
        return defaultValue;
    }
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_deviceUIInfo)

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFEnvInfo_localeInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief device type
     */
    virtual zfstring localeInfo(void) {return zfnull;}
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_localeInfo)

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFEnvInfo_localeLangInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief device type
     */
    virtual zfstring localeLangInfo(void) {return zfnull;}
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_localeLangInfo)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFEnvInfo_ZFUIKit_h_

