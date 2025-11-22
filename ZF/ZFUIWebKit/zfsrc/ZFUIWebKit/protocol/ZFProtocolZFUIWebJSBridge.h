/**
 * @file ZFProtocolZFUIWebJSBridge.h
 * @brief protocol for ZFUIWebJSBridge
 */

#ifndef _ZFI_ZFProtocolZFUIWebJSBridge_h_
#define _ZFI_ZFProtocolZFUIWebJSBridge_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUIWebJSBridge.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUIWebJSBridge
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIWebKit, ZFUIWebJSBridge)
public:
    /**
     * @brief create native web JS bridge
     */
    virtual void *nativeWebJSBridgeCreate(ZF_IN ZFUIWebJSBridge *webJSBridge) zfpurevirtual;
    /**
     * @brief destroy native web JS bridge
     */
    virtual void nativeWebJSBridgeDestroy(ZF_IN ZFUIWebJSBridge *webJSBridge) zfpurevirtual;

public:
    /** @brief see #ZFUIWebJSBridge::send */
    virtual void send(
            ZF_IN ZFUIWebJSBridge *webJSBridge
            , ZF_IN_OUT ZFJson &send
            ) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when message sent from web
     */
    zffinal void notifyWebMessageRecv(
            ZF_IN ZFUIWebJSBridge *webJSBridge
            , ZF_IN_OUT ZFJson &recv
            ) {
        webJSBridge->_ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(recv);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIWebJSBridge)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIWebJSBridge_h_

