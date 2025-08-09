/**
 * @file ZFProtocolZFCompress.h
 * @brief protocol for ZFCompress
 */

#ifndef _ZFI_ZFProtocolZFCompress_h_
#define _ZFI_ZFProtocolZFCompress_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFCompress.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFCompress
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFAlgorithm, ZFCompress)
public:
    /** @brief see #ZFCompressOpen */
    virtual zfautoT<ZFCompressToken> compressOpen(
            ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN ZFCompressLevel compressLevel
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFCompress)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFCompress_h_

