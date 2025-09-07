/**
 * @file ZFProtocolZFPath.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFPath_h_
#define _ZFI_ZFProtocolZFPath_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFIO_file.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFPath)
public:
    /**
     * @brief see #ZFPathForModule
     */
    virtual const zfstring &pathForModule(void) zfpurevirtual;
    /**
     * @brief see #ZFPathForModuleFile
     */
    virtual const zfstring &pathForModuleFile(void) zfpurevirtual;

    /**
     * @brief see #ZFPathForSetting
     */
    virtual const zfstring &pathForSetting(void) zfpurevirtual;
    /**
     * @brief see #ZFPathForSetting
     */
    virtual void pathForSetting(ZF_IN const zfstring &path) zfpurevirtual;

    /**
     * @brief see #ZFPathForStorage
     */
    virtual const zfstring &pathForStorage(void) zfpurevirtual;
    /**
     * @brief see #ZFPathForStorage
     */
    virtual void pathForStorage(ZF_IN const zfstring &path) zfpurevirtual;

    /**
     * @brief see #ZFPathForStorageShared
     */
    virtual const zfstring &pathForStorageShared(void) zfpurevirtual;
    /**
     * @brief see #ZFPathForStorageShared
     */
    virtual void pathForStorageShared(ZF_IN const zfstring &path) zfpurevirtual;

    /**
     * @brief see #ZFPathForCache
     */
    virtual const zfstring &pathForCache(void) zfpurevirtual;
    /**
     * @brief see #ZFPathForCache
     */
    virtual void pathForCache(ZF_IN const zfstring &path) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFPath)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFPath_h_

