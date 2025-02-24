/**
 * @file ZFProtocolZFUdp.h
 * @brief protocol for #ZFUdp
 */

#ifndef _ZFI_ZFProtocolZFUdp_h_
#define _ZFI_ZFProtocolZFUdp_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFNet/ZFUdp.h"
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
    /** @brief see #ZFUdp::hostResolve */
    virtual void *hostResolve(
            ZF_IN const zfstring &host
            , ZF_IN zfuint port
            ) zfpurevirtual;
    /** @brief see #ZFUdp::hostResolve */
    virtual void hostRelease(ZF_IN void *hostAddr) zfpurevirtual;

public:
    /** @brief see #ZFUdpAddr::remoteInfo */
    virtual zfbool remoteInfo(
            ZF_IN void *hostAddr
            , ZF_OUT zfstring &remoteAddr
            , ZF_OUT zfuint &remotePort
            ) zfpurevirtual;

public:
    /** @brief see #ZFUdp::send */
    virtual zfbool send(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN void *hostAddr
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) zfpurevirtual;
    /** @brief see #ZFUdp::recv */
    virtual zfindex recv(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT void *&hostAddr
            , ZF_OUT void *data
            , ZF_IN zfindex maxSize
            , ZF_IN_OPT zftimet timeout
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUdp)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUdp_h_

