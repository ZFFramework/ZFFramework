/**
 * @file ZFProtocolZFApp.h
 * @brief protocol for ZFApp
 */

#ifndef _ZFI_ZFProtocolZFApp_h_
#define _ZFI_ZFProtocolZFApp_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFApp.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFApp
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFApp_appExit)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief see #ZFApp::appExit
     */
    virtual void appExit(ZF_IN zfint appExitCode);
ZFPROTOCOL_INTERFACE_END(ZFApp_appExit)

/**
 * @brief protocol for ZFApp
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFApp_appRestart)
ZFPROTOCOL_INTERFACE_OPTIONAL
    /**
     * @brief see #ZFApp::appRestart
     */
    virtual void appRestart(void);
ZFPROTOCOL_INTERFACE_END(ZFApp_appRestart)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFApp_h_

