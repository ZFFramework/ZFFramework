/**
 * @file ZFProtocolZFFileCwd.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFileCwd_h_
#define _ZFI_ZFProtocolZFFileCwd_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFIODef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFFileCwd)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief see #ZFPathForCwd
     */
    virtual zfstring pathForCwd(void) zfpurevirtual;
    /**
     * @brief see #ZFPathForCwd
     */
    virtual zfbool pathForCwd(ZF_IN const zfstring &pathForCwd) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFileCwd)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFileCwd_h_

