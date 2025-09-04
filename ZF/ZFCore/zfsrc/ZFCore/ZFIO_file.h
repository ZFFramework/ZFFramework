/**
 * @file ZFIO_file.h
 * @brief abstract IO
 */

#ifndef _ZFI_ZFIO_file_h_
#define _ZFI_ZFIO_file_h_

#include "ZFIODef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the file path
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, file)

// ============================================================
// ZFInputForFile
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const zfstring &)filePath: file path to use
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * auto setup callback cache id with res file path
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFCore, ZFInput, ZFInputForFile
        , ZFMP_IN(const zfstring &, filePath)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_file(), filePath));
    return ret;
}

// ============================================================
// ZFOutputForFile
/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const zfstring &)filePath: file path to use
 *
 * auto open and auto close files, may return a null callback if open file error
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFCore, ZFOutput, ZFOutputForFile
        , ZFMP_IN(const zfstring &, filePath)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, ZFPathInfo(ZFPathType_file(), filePath));
    return ret;
}

// ============================================================
/**
 * @brief return true if file specified by path is exist
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileIsExist
        , ZFMP_IN(const zfstring &, path)
        )
/**
 * @brief return true if file specified by path is a directory
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileIsDir
        , ZFMP_IN(const zfstring &, path)
        )

/**
 * @brief make directory
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFilePathCreate
        , ZFMP_IN(const zfstring &, path)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )

/**
 * @brief delete a file or directory from srcPath to dstPath
 *
 * fail if isRecursive is zffalse and dst is a dir
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFFileRemove
        , ZFMP_IN(const zfstring &, path)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
/**
 * @brief move a file or directory from srcPath to dstPath
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFFileMove
        , ZFMP_IN(const zfstring &, srcPath)
        , ZFMP_IN(const zfstring &, dstPath)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )

/**
 * @brief find file or directory, similar to FindFirstFile under Windows
 *
 * path supports path only, without wildcard support,
 * e.g. "/path/" or "/path",
 * "." and ".." won't be included\n
 * typical usage:
 * @code
 *   ZFIOFindData fd;
 *   if(ZFFileFindFirst(fd, path)) {
 *     do {
 *       // do something
 *     } while(ZFFileFindNext(fd));
 *     ZFFileFindClose(fd);
 *   }
 * @endcode
 * @return false if error or nothing found
 * @warning strings in #ZFIOFindData would be deleted
 *   after each findXXX call,
 *   you must save it if need future use
 * @see ZFIOFindData, ZFFileFindFirst, ZFFileFindNext, ZFFileFindClose
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, path)
        )

/**
 * @see ZFIOFindData, ZFFileFindFirst, ZFFileFindClose
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileFindNext
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )

/**
 * @see ZFIOFindData, ZFFileFindFirst, ZFFileFindNext
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFFileFindClose
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )

/**
 * @brief open a file for read or write
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, void *, ZFFileOpen
        , ZFMP_IN(const zfstring &, filePath)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
/**
 * @brief close and save the file if need, return false if save failed
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileClose
        , ZFMP_IN(void *, token)
        )

/**
 * @brief read file
 *
 * return size read, or zfindexMax if error occurred, typical usage:
 * @code
 *   zfindex sizeRead = 0;
 *   while((sizeRead = ZFFileRead(token, buf)) > 0) {
 *       // do something with buf
 *   }
 * @endcode
 *
 * note that the tail '\0' won't be written to buf
 * @warning ZFFile always read files in binary mode,
 *   for text files,
 *   you must make sure the file is UTF8 encoded without BOM
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfindex, ZFFileRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )

/**
 * @brief write file, see #ZFFileRead
 *
 * return size written, even if error occurred, typical usage:
 * @code
 *   zfindex sizeWritten = ZFFileWrite(token, src, size);
 *   if(sizeWritten != size) { // you can also check by sizeWritten
 *       // write fail
 *   }
 * @endcode
 * \n
 * maxByteSize could be zfindexMax(),
 * which means #ZFFileWrite should be stopped when reached 0x00 in src,
 * usually to output a UTF8 string
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfindex, ZFFileWrite
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const void *, src)
        , ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax())
        )

/**
 * @brief similar to fseek, return false if seek out of range
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFFileSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, seekPos, ZFSeekPosBegin)
        )
/**
 * @brief get current file's position or zfindexMax() if error
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFFileTell
        , ZFMP_IN(void *, token)
        )
/**
 * @brief util method to get file's total size (not left size)
 *
 * #ZFFileSeek to end, #ZFFileTell, then #ZFFileSeek to restore,
 * return zfindexMax() if error\n
 * note that result is not ensured if file is opened in append mode
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFFileSize
        , ZFMP_IN(void *, token)
        )

// ============================================================
zfclass ZFLIB_ZFCore _ZFP_I_ZFIOImpl_file : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_file, ZFIOImpl)
protected:
    virtual zfstring pathConvert(ZF_IN const zfstring &pathData) {
        return pathData;
    }
public:
    zfoverride
    virtual zfstring pathType(void) {return ZFPathType_file();}

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {return ZFFileIsExist(pathConvert(pathData));}
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {return ZFFileIsDir(pathConvert(pathData));}
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return ioToFileNameDefault(ret, pathData);}
    zfoverride
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) {return ioToChildDefault(ret, pathData, childName);}
    zfoverride
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return ioToParentDefault(ret, pathData);}
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {return ZFFilePathCreate(pathConvert(pathData), autoCreateParent);}
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return ZFFileRemove(pathConvert(pathData), isRecursive, isForce);}
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return ZFFileMove(pathConvert(pathDataFrom), pathConvert(pathDataTo), isForce);}
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {return ZFFileFindFirst(fd, pathConvert(pathData));}
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {return ZFFileFindNext(fd);}
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {ZFFileFindClose(fd);}
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            );
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIO_file_h_

