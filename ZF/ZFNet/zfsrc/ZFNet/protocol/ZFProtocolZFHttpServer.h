/**
 * @file ZFProtocolZFHttpServer.h
 * @brief protocol for #ZFHttpServer
 */

#ifndef _ZFI_ZFProtocolZFHttpServer_h_
#define _ZFI_ZFProtocolZFHttpServer_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFHttpServer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for #ZFHttpServer
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFNet, ZFHttpServer)
public:
    /**
     * @brief start the server
     */
    virtual void *start(ZF_IN ZFHttpServer *owner) zfpurevirtual;
    /**
     * @brief stop the server
     */
    virtual void stop(ZF_IN ZFHttpServer *owner, ZF_IN void *token) zfpurevirtual;

public:
    /**
     * @brief implementation must notify when receive client request,
     *   in thread of #ZFHttpServerThreadPool
     */
    zffinal void notifyOnRequest(
            ZF_IN ZFHttpServer *owner
            , ZF_IN ZFHttpServerTask *task
            ) {
        owner->observerNotify(ZFHttpServer::E_OnRequest(), task);
    }
    /**
     * @brief implementation must notify when server start fail or error occurred
     */
    zffinal void notifyOnError(
            ZF_IN ZFHttpServer *owner
            , ZF_IN const zfstring &errorHint
            ) {
        owner->_ZFP_ZFHttpServer_onError(errorHint);
    }
ZFPROTOCOL_INTERFACE_END(ZFHttpServer)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFHttpServer_h_

