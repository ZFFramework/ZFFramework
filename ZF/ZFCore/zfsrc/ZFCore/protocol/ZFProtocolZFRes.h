/**
 * @file ZFProtocolZFRes.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFRes_h_
#define _ZFI_ZFProtocolZFRes_h_

#include "ZFProtocolZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFRes)

public:
    /**
     * @brief util method to access root path for file res impl
     *
     * for most file res impl, res processing is the same as normal file processing,
     * except the root path differs,
     * so we supply an default impl that process like normal files,
     * and let different impl to specify different root path\n
     * by default, this method would return "./zfres/"
     */
    virtual const zfstring &resRootPath(void);

public:
    /**
     * @brief see #ZFResIsExist
     */
    virtual zfbool resIsExist(ZF_IN const zfchar *resPath);
    /**
     * @brief see #ZFResIsDir
     */
    virtual zfbool resIsDir(ZF_IN const zfchar *resPath);

    /**
     * @brief see #ZFResOpen
     */
    virtual void *resOpen(ZF_IN const zfchar *resPath);
    /**
     * @brief see #ZFResClose
     */
    virtual zfbool resClose(ZF_IN void *token);

    /**
     * @brief see #ZFResTell
     */
    virtual zfindex resTell(ZF_IN void *token);
    /**
     * @brief see #ZFResSeek
     */
    virtual zfbool resSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            );

    /**
     * @brief see #ZFResRead
     */
    virtual zfindex resRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            );

    /**
     * @brief see #ZFResFindFirst, #ZFFileFindFirst
     */
    virtual zfbool resFindFirst(
            ZF_IN_OUT ZFIOFindData::Impl &fd
            , ZF_IN const zfchar *resPath
            );
    /**
     * @brief see #ZFResFindNext, #ZFFileFindNext
     */
    virtual zfbool resFindNext(ZF_IN_OUT ZFIOFindData::Impl &fd);
    /**
     * @brief see #ZFResFindClose, #ZFFileFindClose
     */
    virtual void resFindClose(ZF_IN_OUT ZFIOFindData::Impl &fd);
ZFPROTOCOL_INTERFACE_END(ZFRes)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFRes_h_

