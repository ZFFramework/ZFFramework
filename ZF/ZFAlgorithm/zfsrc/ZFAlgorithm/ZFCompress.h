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
 *   zfobj<ZFCompress> zip(ZFOutputForFile(xxx));
 *   zfbool success = zip->valid();
 *   success &= zip->output(filePathInZip0, inputRaw0);
 *   success &= zip->output(filePathInZip1, inputRaw1);
 *   success &= zip->close();
 *
 *   // decompress
 *   zfobj<ZFDecompress> unzip(ZFInputForFile(xxx));
 *   zfbool success = unzip->valid();
 *   success &= unzip->output(outputRaw0, filePathInZip0);
 *   success &= unzip->output(outputRaw1, filePathInZip1);
 *   success &= unzip->outputAt(outputRaw0, fileIndexInZip0);
 *   success &= unzip->outputAt(outputRaw1, fileIndexInZip1);
 *   success &= unzip->close();
 *
 *   // get zip file content info
 *   zfindex fileCountInZip = unzip->contentCount();
 *   zfindex fileIndexInZip = unzip->contentIndex("filePathInZip");
 *   zfstring filePathInZip = unzip->contentPath(fileIndexInZip);
 * @endcode
 *
 * or, use the util methods:
 * @code
 *   // compress buffer
 *   ZFCompressContent(outputZip, inputRaw, filePathInZip);
 *   // decompress buffer
 *   ZFDecompressContent(outputZip, inputRaw, filePathInZip);
 *
 *   // compress dir
 *   ZFCompressDir(outputZip, inputPathInfo);
 *   // decompress dir
 *   ZFDecompressDir(outputPathInfo, inputZip);
 * @endcode
 *
 * note: which compress algorithm would be used, depends on impl
 */
zfclass ZFLIB_ZFAlgorithm ZFCompress : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFCompress, ZFObject)

public:
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_2(zfbool, open
            , ZFMP_IN(const ZFOutput &, outputZip)
            , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_0(zfbool, close)
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_0(zfbool, valid)

    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_2(zfbool, output
            , ZFMP_IN(const zfstring &, filePathInZip)
            , ZFMP_IN_OUT(const ZFInput &, inputRaw)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_1(zfbool, createDir
            , ZFMP_IN(const zfstring &, filePathInZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_1(zfbool, remove
            , ZFMP_IN(const zfstring &, filePathInZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_3(zfbool, move
            , ZFMP_IN(const zfstring &, filePathInZipFrom)
            , ZFMP_IN(const zfstring &, filePathInZipTo)
            , ZFMP_IN_OPT(zfbool, isForce, zftrue)
            )

protected:
    /** @brief see #ZFCompress */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const ZFOutput &, outputZip)
            , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
            )
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
private:
    void *d;
};

/** @brief see #ZFCompress */
zfclass ZFLIB_ZFAlgorithm ZFDecompress : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFDecompress, ZFObject)

public:
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_1(zfbool, open
            , ZFMP_IN_OUT(const ZFInput &, inputZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_0(zfbool, close)
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_0(zfbool, valid)

    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_2(zfbool, outputAt
            , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
            , ZFMP_IN(zfindex, fileIndexInZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_2(zfbool, output
            , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
            , ZFMP_IN(const zfstring &, filePathInZip)
            )

    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_0(zfindex, contentCount)
    /**
     * @brief see #ZFCompress
     *
     * path must not start with '/'\n
     * end with '/' to find dir,
     * and without '/' to find file
     */
    ZFMETHOD_DECLARE_1(zfindex, contentIndex
            , ZFMP_IN(const zfstring &, filePathInZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_1(zfstring, contentPath
            , ZFMP_IN(zfindex, fileIndexInZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_2(zfbool, contentPathT
            , ZFMP_IN_OUT(zfstring &, ret)
            , ZFMP_IN(zfindex, fileIndexInZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_1(zfbool, isDir
            , ZFMP_IN(zfindex, fileIndexInZip)
            )

    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_2(zfbool, findFirst
            , ZFMP_IN_OUT(ZFFileFindData &, fd)
            , ZFMP_IN(const zfstring &, filePathInZip)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_1(zfbool, findNext
            , ZFMP_IN_OUT(ZFFileFindData &, fd)
            )
    /** @brief see #ZFCompress */
    ZFMETHOD_DECLARE_1(void, findClose
            , ZFMP_IN_OUT(ZFFileFindData &, fd)
            )

protected:
    /** @brief see #ZFCompress */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN_OUT(const ZFInput &, inputZip)
            )
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
private:
    void *d;
};

// ============================================================
// util
/** @brief see #ZFCompress */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFCompressContent
        , ZFMP_IN_OUT(const ZFOutput &, outputZip)
        , ZFMP_IN_OUT(const ZFInput &, inputRaw)
        , ZFMP_IN_OPT(const zfstring &, filePathInZip, zftext("content"))
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        )
/** @brief see #ZFCompress */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressContent
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        , ZFMP_IN_OPT(const zfstring &, filePathInZip, zftext("content"))
        )

/** @brief see #ZFCompress */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFCompressDir
        , ZFMP_IN_OUT(const ZFOutput &, outputZip)
        , ZFMP_IN(const ZFPathInfo &, inputPathInfo)
        , ZFMP_IN_OPT(const zfstring &, filePathInZip, zftext("content"))
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        )
/** @brief see #ZFCompress */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressDir
        , ZFMP_IN(const ZFPathInfo &, outputPathInfo)
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCompress_h_

