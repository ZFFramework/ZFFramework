/**
 * @file ZFProtocolZFUdp.h
 * @brief protocol for #ZFUdp
 */

#ifndef _ZFI_ZFProtocolZFUdp_h_
#define _ZFI_ZFProtocolZFUdp_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUdp.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for #ZFUdp
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFNet, ZFUdp)
public:
    /** @brief see #ZFUdp::open */
    virtual void *open(
            ZF_IN ZFUdp *owner
            , ZF_IN zfuint port
            ) zfpurevirtual;
    /** @brief see #ZFUdp::close */
    virtual void close(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            ) zfpurevirtual;

public:
    /** @brief see #ZFUdp::addrResolve */
    virtual void *addrResolve(
            ZF_IN const zfstring &host
            ) zfpurevirtual;
    /** @brief see #ZFUdp::addrResolve */
    virtual void addrRelease(ZF_IN void *nativeAddr) zfpurevirtual;

public:
    /** @brief see #ZFUdpAddr::remoteInfo */
    virtual zfbool remoteInfo(
            ZF_IN void *nativeAddr
            , ZF_IN_OUT zfstring &remoteAddr
            ) zfpurevirtual;

public:
    /** @brief see #ZFUdp::send */
    virtual zfbool send(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN void *nativeAddr
            , ZF_IN zfuint port
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) zfpurevirtual;
    /** @brief see #ZFUdp::recv */
    virtual zfstring recv(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT void *&nativeAddr
            , ZF_OUT zfuint &port
            , ZF_IN_OPT zftimet timeout
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUdp)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUdp_h_

