/**
 * @file ZFCompress.h
 * @brief compress util
 */

#ifndef _ZFI_ZFCompress_h_
#define _ZFI_ZFCompress_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief compress level for #ZFCompressOpen
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

/** @brief see #ZFCompressOpen */
zfabstract ZFLIB_ZFAlgorithm ZFCompressToken : zfextend ZFIOToken {
    ZFOBJECT_DECLARE_ABSTRACT(ZFCompressToken, ZFIOToken)

#if 0
public:
    virtual zfstring pathType(void) zfpurevirtual;
    virtual zfstring pathData(void) zfpurevirtual;
    virtual ZFIOOpenOptionFlags ioFlags(void) zfpurevirtual;
public:
    virtual zfbool ioClose(void) zfpurevirtual;
#endif

public:
    /** @brief see #ZFCompressOpen */
    virtual zfbool ioRead(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN const zfstring &itemPath
            ) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfbool ioWrite(
            ZF_IN const zfstring &itemPath
            , ZF_IN const ZFInput &input
            ) zfpurevirtual;

    /** @brief get input of specified item */
    virtual ZFInput input(ZF_IN const zfstring &itemPath);
    /** @brief get output of specified item */
    virtual ZFOutput output(ZF_IN const zfstring &itemPath);

    /** @brief see #ZFCompressOpen */
    virtual zfbool ioPathCreate(ZF_IN const zfstring &itemPath) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfbool ioRemove(ZF_IN const zfstring &itemPath) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfbool ioMove(
            ZF_IN const zfstring &itemPathFrom
            , ZF_IN const zfstring &itemPathTo
            ) zfpurevirtual;

    /** @brief see #ZFCompressOpen */
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &itemPath
            ) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) zfpurevirtual;

    /** @brief see #ZFCompressOpen */
    virtual zfbool ioIsExist(ZF_IN const zfstring &itemPath) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfbool ioIsDir(ZF_IN const zfstring &itemPath) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfbool ioIsDirAt(ZF_IN zfindex itemIndex) zfpurevirtual;

    /** @brief see #ZFCompressOpen */
    virtual zfindex itemCount(void) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfindex itemIndex(ZF_IN const zfstring &itemPath) zfpurevirtual;
    /** @brief see #ZFCompressOpen */
    virtual zfstring itemPath(ZF_IN zfindex itemIndex) {
        zfstring ret;
        if(this->itemPathT(ret, itemIndex)) {
            return ret;
        }
        else {
            return zfnull;
        }
    }
    /** @brief see #ZFCompressOpen */
    virtual zfbool itemPathT(
            ZF_IN_OUT zfstring &itemPath
            , ZF_IN zfindex itemIndex
            ) zfpurevirtual;

private:
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        ZFCoreCriticalNotSupported();
        return 0;
    }
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {
        ZFCoreCriticalNotSupported();
        return 0;
    }
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {
        ZFCoreCriticalNotSupported();
        return zffalse;
    }
    zfoverride
    virtual zfindex ioTell(void) {
        ZFCoreCriticalNotSupported();
        return zfindexMax();
    }
    zfoverride
    virtual zfindex ioSize(void) {
        ZFCoreCriticalNotSupported();
        return zfindexMax();
    }
};

// ============================================================
/**
 * @brief open compress file for read or write
 *
 * typical usage:
 * @code
 *   zfautoT<ZFCompressToken> compress = ZFCompressOpen(pathInfo, v_ZFIOOpenOption::e_Modify);
 *
 *   // read and write file in the compress file
 *   compress->read(ZFOutputForXxx(), "item/path/in/compress_file");
 *   compress->write("item/path/in/compress_file", ZFInputForXxx());
 *
 *   // find files in compress file
 *   ZFIOFindData fd;
 *   if(compress->itemFindFirst(fd, "item/path/in/compress_file")) {
 *       do {
 *           ...
 *       } while(compress->itemFindNext(fd));
 *       compress->itemFindClose(fd);
 *   }
 *
 *   // iterate items (files or dirs)
 *   for(zfindex i = 0; i < compress->itemCount(); ++i) {
 *       zfstring itemPath = compress->itemPath(i);
 *   }
 *
 *   // explicitly close
 *   // would be closed automatically when compress object deallocated
 *   compress->close();
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfautoT<ZFCompressToken>, ZFCompressOpen
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        )

// ============================================================
// util
/** @brief see #ZFCompressOpen */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFCompressFile
        , ZFMP_IN_OUT(const ZFPathInfo &, outputCompress)
        , ZFMP_IN_OUT(const ZFInput &, inputRaw)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        )
/** @brief see #ZFCompressOpen */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressFile
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN_OUT(const ZFPathInfo &, inputCompress)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        )

/**
 * @brief see #ZFCompressOpen
 *
 * example of different conditions:
 * -  itemPath is empty : inputPathInfo's file name would be used as itemPath in compress file
 *   @code
 *     some_path/dir_name/    <= inputPathInfo
 *         some_file
 *
 *     compress_name.zip      <= outputCompress
 *         dir_name/          <= file name specified by inputPathInfo
 *             some_file      <= children of inputPathInfo
 *   @endcode
 * -  itemPath is "." : all children would be added to root of the compress file
 *   @code
 *     some_path/dir_name/    <= inputPathInfo
 *         some_file
 *
 *     compress_name.zip      <= outputCompress
 *         some_file          <= children of inputPathInfo would be added to root
 *   @endcode
 * -  itemPath is valid path : all children would be added to specified itemPath
 *   @code
 *     some_path/dir_name/    <= inputPathInfo
 *         some_file
 *
 *     compress_name.zip      <= outputCompress
 *         some/item/path/
 *             some_file      <= children of inputPathInfo would be added to specified itemPath
 *   @endcode
 */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFAlgorithm, zfbool, ZFCompressDir
        , ZFMP_IN_OUT(const ZFPathInfo &, outputCompress)
        , ZFMP_IN(const ZFPathInfo &, inputPathInfo)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zfnull)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Modify)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        )
/** @brief see #ZFCompressOpen */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfbool, ZFDecompressDir
        , ZFMP_IN(const ZFPathInfo &, outputPathInfo)
        , ZFMP_IN_OUT(const ZFPathInfo &, inputCompress)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCompress_h_

