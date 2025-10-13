/**
 * @file ZFProtocolZFWebSocket.h
 * @brief protocol for #ZFWebSocket
 */

#ifndef _ZFI_ZFProtocolZFWebSocket_h_
#define _ZFI_ZFProtocolZFWebSocket_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFWebSocket.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for #ZFWebSocket
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFNet, ZFWebSocket)
public:
    /** @brief see #ZFWebSocket::open */
    virtual void *open(
            ZF_IN ZFWebSocket *owner
            , ZF_IN const zfstring &url
            ) zfpurevirtual;
    /** @brief see #ZFWebSocket::close */
    virtual void close(
            ZF_IN ZFWebSocket *owner
            , ZF_IN void *nativeWebSocket
            ) zfpurevirtual;
    /** @brief see #ZFWebSocket::close */
    virtual void send(
            ZF_IN ZFWebSocket *owner
            , ZF_IN void *nativeWebSocket
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) zfpurevirtual;
    /** @brief see #ZFWebSocket::close */
    virtual void sendBin(
            ZF_IN ZFWebSocket *owner
            , ZF_IN void *nativeWebSocket
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) zfpurevirtual;
public:
    /** @brief implementation must notify when connection opened */
    virtual void notifyOnOpen(
            ZF_IN ZFWebSocket *owner
            ) {
        owner->observerNotify(ZFWebSocket::E_OnOpen());
    }
    /** @brief implementation must notify when connection closed */
    virtual void notifyOnClose(
            ZF_IN ZFWebSocket *owner
            , ZF_IN ZFResultType result
            , ZF_IN const zfstring &reasonHint
            ) {
        owner->observerNotify(ZFWebSocket::E_OnClose(), zfobj<v_ZFResultType>(result), zfobj<v_zfstring>(reasonHint));
    }
    /** @brief implementation must notify when recv data */
    virtual void notifyOnRecv(
            ZF_IN ZFWebSocket *owner
            , ZF_IN const zfstring &data
            ) {
        owner->observerNotify(ZFWebSocket::E_OnRecv(), zfobj<v_zfstring>(data));
    }
    /** @brief implementation must notify when recv data */
    virtual void notifyOnRecvBin(
            ZF_IN ZFWebSocket *owner
            , ZF_IN const zfstring &data
            ) {
        owner->observerNotify(ZFWebSocket::E_OnRecvBin(), zfobj<v_zfstring>(data));
    }
ZFPROTOCOL_INTERFACE_END(ZFWebSocket)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFWebSocket_h_

