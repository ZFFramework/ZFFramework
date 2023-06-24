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
    virtual zfbool open(ZF_IN ZFUdp *owner,
                        ZF_IN_OUT zfuint &port,
                        ZF_OUT void *&nativeUdp) zfpurevirtual;
    /** @brief see #ZFUdp::close */
    virtual void close(ZF_IN ZFUdp *owner,
                       ZF_IN void *nativeUdp) zfpurevirtual;
    /** @brief see #ZFUdp::send */
    virtual zfbool send(ZF_IN ZFUdp *owner,
                        ZF_IN ZFUdpPacket *packet) zfpurevirtual;
    /** @brief see #ZFUdp::recv */
    virtual zfautoObjectT<ZFUdpPacket *> recv(ZF_IN ZFUdp *owner) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUdp)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUdp_h_

