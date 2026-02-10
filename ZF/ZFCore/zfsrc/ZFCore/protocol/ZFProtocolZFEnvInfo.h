/**
 * @file ZFProtocolZFEnvInfo.h
 * @brief protocol for ZFEnvInfo
 */

#ifndef _ZFI_ZFProtocolZFEnvInfo_h_
#define _ZFI_ZFProtocolZFEnvInfo_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFEnvInfo.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFEnvInfo_systemInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /** @brief see #ZFEnvInfo::systemName */
    virtual zfstring systemName(void) {return zfnull;}
    /** @brief see #ZFEnvInfo::systemVersion */
    virtual zfstring systemVersion(void) {return zfnull;}
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_systemInfo)

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFEnvInfo_frameworkInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /** @brief see #ZFEnvInfo::frameworkName */
    virtual zfstring frameworkName(void) {return zfnull;}
    /** @brief see #ZFEnvInfo::frameworkVersion */
    virtual zfstring frameworkVersion(void) {return zfnull;}
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_frameworkInfo)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFEnvInfo_h_

