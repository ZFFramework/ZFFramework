/**
 * @file ZFProtocolZFTcp.h
 * @brief protocol for #ZFTcp
 */

#ifndef _ZFI_ZFProtocolZFTcp_h_
#define _ZFI_ZFProtocolZFTcp_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFNet/ZFTcp.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for #ZFTcp
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFNet, ZFTcp)
public:
    /** @brief see #ZFTcp::open */
    virtual void *open(
            ZF_IN ZFTcp *owner
            , ZF_IN const zfstring &host
            , ZF_IN zfuint port
            , ZF_IN zftimet timeout
            ) zfpurevirtual;
    /** @brief see #ZFTcp::close */
    virtual void close(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            ) zfpurevirtual;

public:
    /** @brief see #ZFTcp::remoteInfo */
    virtual zfbool remoteInfo(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN_OUT zfstring &remoteAddr
            ) zfpurevirtual;
    /** @brief see #ZFTcp::accept */
    virtual void *accept(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN_OPT zftimet timeout = -1
            ) zfpurevirtual;

public:
    /** @brief see #ZFTcp::send */
    virtual zfbool send(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) zfpurevirtual;
    /** @brief see #ZFTcp::recv */
    virtual zfindex recv(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT void *data
            , ZF_IN zfindex maxSize
            , ZF_IN_OPT zftimet timeout
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFTcp)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFTcp_h_

