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
    // ============================================================
    /** @brief see #ZFCompress */
    virtual void *compressBegin(
            ZF_IN_OUT const ZFOutput &outputZip
            , ZF_IN ZFCompressLevel compressLevel
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool compressEnd(ZF_IN_OUT void *compressToken) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool compressContent(
            ZF_IN_OUT void *compressToken
            , ZF_IN_OUT const ZFInput &inputRaw
            , ZF_IN const zfstring &filePathInZip
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool compressContentDir(
            ZF_IN_OUT void *compressToken
            , ZF_IN const zfstring &filePathInZip
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool compressContentRemove(
            ZF_IN_OUT void *compressToken
            , ZF_IN const zfstring &filePathInZip
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool compressContentMove(
            ZF_IN_OUT void *compressToken
            , ZF_IN const zfstring &filePathInZipFrom
            , ZF_IN const zfstring &filePathInZipTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) zfpurevirtual;

    // ============================================================
    /** @brief see #ZFCompress */
    virtual void *decompressBegin(ZF_IN_OUT const ZFInput &inputZip) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool decompressEnd(ZF_IN_OUT void *decompressToken) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool decompressContent(
            ZF_IN_OUT void *decompressToken
            , ZF_IN_OUT const ZFOutput &outputRaw
            , ZF_IN zfindex fileIndexInZip
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfindex decompressContentCount(ZF_IN void *decompressToken) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfindex decompressContentIndex(
            ZF_IN void *decompressToken
            , ZF_IN const zfstring &filePathInZip
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool decompressContentPath(
            ZF_IN void *decompressToken
            , ZF_IN_OUT zfstring &filePathInZip
            , ZF_IN zfindex fileIndexInZip
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool decompressContentIsDir(
            ZF_IN void *decompressToken
            , ZF_IN zfindex fileIndexInZip
            ) zfpurevirtual;
    /** @brief see #ZFCompress */
    virtual zfbool decompressContentIsDir(
            ZF_IN void *decompressToken
            , ZF_IN const zfstring &filePathInZip
            ) {
        zfindex fileIndexInZip = this->decompressContentIndex(decompressToken, filePathInZip);
        return fileIndexInZip != zfindexMax() && this->decompressContentIsDir(decompressToken, fileIndexInZip);
    }

    /** @brief see #ZFCompress */
    virtual zfbool decompressContentFindFirst(
            ZF_IN_OUT ZFFileFindData &fd
            , ZF_IN void *decompressToken
            , ZF_IN const zfstring &filePathInZip
            );
    /** @brief see #ZFCompress */
    virtual zfbool decompressContentFindNext(ZF_IN_OUT ZFFileFindData &fd);
    /** @brief see #ZFCompress */
    virtual void decompressContentFindClose(ZF_IN_OUT ZFFileFindData &fd);
ZFPROTOCOL_INTERFACE_END(ZFCompress)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFCompress_h_

