/**
 * @file ZFCompress.h
 * @brief compress util
 */

#ifndef _ZFI_ZFCompress_h_
#define _ZFI_ZFCompress_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief compress level for #ZFCompress
 */
ZFENUM_BEGIN(ZFLIB_ZFAlgorithm, ZFCompressLevel)
    ZFENUM_VALUE(NoCompress)
    ZFENUM_VALUE(BestSpeed)
    ZFENUM_VALUE(GoodSpeed)
    ZFENUM_VALUE(Normal)
    ZFENUM_VALUE(GoodCompress)
    ZFENUM_VALUE(BestCompress)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(NoCompress)
    ZFENUM_VALUE_REGISTER(BestSpeed)
    ZFENUM_VALUE_REGISTER(GoodSpeed)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(GoodCompress)
    ZFENUM_VALUE_REGISTER(BestCompress)
ZFENUM_END_WITH_DEFAULT(ZFLIB_ZFAlgorithm, ZFCompressLevel, Normal)
ZFENUM_REG(ZFLIB_ZFAlgorithm, ZFCompressLevel)

// ============================================================
// base api
/**
 * @brief compress util
 *
 * typical usage:
 * @code
 *   // compress
 *   void *compressToken = ZFCompressBegin(outputZip);
 *   zfbool success = (compressToken != zfnull);
 *   success &= ZFCompressContent(compressToken, inputRaw0, filePathInZip0);
 *   success &= ZFCompressContent(compressToken, inputRaw1, filePathInZip1);
 *   success &= ZFCompressEnd(compressToken);
 *
 *   // decompress
 *   void *decompressToken = ZFDecompressBegin(inputZip);
 *   zfbool success = (decompressToken != zfnull);
 *   success &= ZFDecompressContent(decompressToken, outputRaw0, filePathInZip0);
 *   success &= ZFDecompressContent(decompressToken, outputRaw1, filePathInZip1);
 *   success &= ZFDecompressContentAt(decompressToken, outputRaw0, fileIndexInZip0);
 *   success &= ZFDecompressContentAt(decompressToken, outputRaw1, fileIndexInZip1);
 *   success &= ZFDecompressEnd(decompressToken);
 *
 *   // get zip file content info
 *   zfindex fileCountInZip = ZFDecompressContentCount(decompressToken);
 *   zfindex fileIndexInZip = ZFDecompressContentIndex("filePathInZip");
 *   zfstring filePathInZip = ZFDecompressContentPath(fileIndexInZip);
 * @endcode
 *
 * or, use the util methods:
 * @code
 *   // compress buffer
 *   ZFCompress(outputZip, inputRaw, filePathInZip);
 *   // decompress buffer
 *   ZFDecompress(outputZip, inputRaw, filePathInZip);
 *
 *   // compress dir
 *   ZFCompressDir(outputZip, inputPathInfo);
 *   // decompress dir
 *   ZFDecompressDir(outputPathInfo, inputZip);
 * @endcode
 *
 * note: which compress algorithm would be used, depends on impl
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, void *, ZFCompressBegin
        , ZFMP_IN_OUT(const ZFOutput &, outputZip)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zfbool, ZFCompressEnd
        , ZFMP_IN(void *, compressToken)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFCompressContent
        , ZFMP_IN_OUT(void *, compressToken)
        , ZFMP_IN_OUT(const ZFInput &, inputRaw)
        , ZFMP_IN(const zfchar *, filePathInZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfbool, ZFCompressContentDir
        , ZFMP_IN_OUT(void *, compressToken)
        , ZFMP_IN(const zfchar *, filePathInZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfbool, ZFCompressContentRemove
        , ZFMP_IN_OUT(void *, compressToken)
        , ZFMP_IN(const zfchar *, filePathInZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFCompressContentMove
        , ZFMP_IN_OUT(void *, compressToken)
        , ZFMP_IN(const zfchar *, filePathInZipFrom)
        , ZFMP_IN(const zfchar *, filePathInZipTO)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )

/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, void *, ZFDecompressBegin
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressEnd
        , ZFMP_IN(void *, decompressToken)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressContentAt
        , ZFMP_IN_OUT(void *, decompressToken)
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN(zfindex, fileIndexInZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressContent
        , ZFMP_IN_OUT(void *, decompressToken)
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN(const zfchar *, filePathInZip)
        )
/** @brief see #ZFCompressBegin, return #zfindexMax if fail */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zfindex, ZFDecompressContentCount
        , ZFMP_IN(void *, decompressToken)
        )
/**
 * @brief see #ZFCompressBegin, return #zfindexMax if fail
 *
 * path must not start with '/'\n
 * end with '/' to find dir,
 * and without '/' to find file
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfindex, ZFDecompressContentIndex
        , ZFMP_IN(void *, decompressToken)
        , ZFMP_IN(const zfchar *, filePathInZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressContentPathT
        , ZFMP_IN(void *, decompressToken)
        , ZFMP_IN_OUT(zfstring &, filePathInZip)
        , ZFMP_IN(zfindex, fileIndexInZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfstring, ZFDecompressContentPath
        , ZFMP_IN(void *, decompressToken)
        , ZFMP_IN(zfindex, fileIndexInZip)
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressContentIsDir
        , ZFMP_IN(void *, decompressToken)
        , ZFMP_IN(zfindex, fileIndexInZip)
        )

/** @brief find content in the compress file, see #ZFFileFindFirst */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressContentFindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(void *, decompressToken)
        , ZFMP_IN(const zfchar *, filePathInZip)
        )
/** @brief see #ZFDecompressContentFindFirst */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressContentFindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )
/** @brief see #ZFDecompressContentFindFirst */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, void, ZFDecompressContentFindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )

// ============================================================
// util
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFCompress
        , ZFMP_IN_OUT(const ZFOutput &, outputZip)
        , ZFMP_IN_OUT(const ZFInput &, inputRaw)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        , ZFMP_IN_OPT(const zfchar *, filePathInZip, "content")
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFDecompress
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        , ZFMP_IN_OPT(const zfchar *, filePathInZip, "content")
        )

/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFCompressDir
        , ZFMP_IN_OUT(const ZFOutput &, outputZip)
        , ZFMP_IN(const ZFPathInfo &, inputPathInfo)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        )
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressDir
        , ZFMP_IN(const ZFPathInfo &, outputPathInfo)
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCompress_h_

