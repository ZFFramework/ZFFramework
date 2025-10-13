/**
 * @file ZFProtocolZFFile.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFile_h_
#define _ZFI_ZFProtocolZFFile_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFIODef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFFile)
public:
    /**
     * @brief see #ZFFileIsExist
     */
    virtual zfbool fileIsExist(ZF_IN const zfstring &path) zfpurevirtual;
    /**
     * @brief see #ZFFileIsDir
     */
    virtual zfbool isDir(ZF_IN const zfstring &path) zfpurevirtual;

    /**
     * @brief see #ZFFilePathCreate
     */
    virtual zfbool filePathCreate(
            ZF_IN const zfstring &path
            , ZF_IN_OPT zfbool autoCreateParent = zffalse
            ) zfpurevirtual;
    /**
     * @brief see #ZFFileMove
     */
    virtual zfbool fileMove(
            ZF_IN const zfstring &dstPath
            , ZF_IN const zfstring &srcPath
            , ZF_IN_OPT zfbool isForce = zftrue
            ) zfpurevirtual;
    /**
     * @brief see #ZFFileRemove
     */
    virtual zfbool fileRemove(
            ZF_IN const zfstring &path
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) zfpurevirtual;

    /**
     * @brief see #ZFFileFindFirst
     *
     * use ZFIOFindData::Impl to store find result\n
     * remember that you must remove the dir "." and "..",
     * and if there is only two dir named "." and "..",
     * you should return false for fileFindFirst\n
     * if returned zffalse, no fileFindClose would be called anymore
     * @warning it's the implementation's responsibility
     *   to allocate and deallocate the nativeFd,
     *   and, if fileFindFirst would return zffalse,
     *   you must deallocate the nativeFd (if allocated) before return
     */
    virtual zfbool fileFindFirst(
            ZF_IN_OUT ZFIOFindData::Impl &fd
            , ZF_IN const zfstring &path
            ) zfpurevirtual;
    /**
     * @brief see #ZFFileFindNext, fileFindFirst
     */
    virtual zfbool fileFindNext(ZF_IN_OUT ZFIOFindData::Impl &fd) zfpurevirtual;
    /**
     * @brief see #ZFFileFindClose, fileFindFirst
     */
    virtual void fileFindClose(ZF_IN_OUT ZFIOFindData::Impl &fd) zfpurevirtual;

    /** @brief see #ZFFileOpen */
    virtual void *fileOpen(
            ZF_IN const zfstring &filePath
            , ZF_IN ZFIOOpenOptionFlags flags
            ) zfpurevirtual;
    /** @brief see #ZFFileClose */
    virtual zfbool fileClose(ZF_IN void *token) zfpurevirtual;

    /** @brief see #ZFFileTell */
    virtual zfindex fileTell(ZF_IN void *token) zfpurevirtual;
    /** @brief see #ZFFileSeek */
    virtual zfbool fileSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) zfpurevirtual;

    /** @brief see #ZFFileRead */
    virtual zfindex fileRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) zfpurevirtual;

    /** @brief see #ZFFileWrite */
    virtual zfindex fileWrite(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN zfindex maxByteSize
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFile)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFile_h_

