/**
 * @file ZFProtocolZFApp.h
 * @brief protocol for ZFApp
 */

#ifndef _ZFI_ZFProtocolZFApp_h_
#define _ZFI_ZFProtocolZFApp_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFApp.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for ZFApp
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFApp)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief see #ZFApp::appExit
     */
    virtual void appExit(ZF_IN zfint appExitCode);
    /**
     * @brief see #ZFApp::appRestart
     */
    virtual void appRestart(void);
ZFPROTOCOL_INTERFACE_END(ZFApp)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFApp_h_

