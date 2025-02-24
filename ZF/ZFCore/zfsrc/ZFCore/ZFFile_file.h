/**
 * @file ZFFile_file.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_file_h_
#define _ZFI_ZFFile_file_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief return true if file specified by path is exist
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileIsExist
        , ZFMP_IN(const zfchar *, path)
        )
/**
 * @brief return true if file specified by path is a directory
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileIsDir
        , ZFMP_IN(const zfchar *, path)
        )

/**
 * @brief make directory
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathCreate
        , ZFMP_IN(const zfchar *, path)
        , ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue)
        , ZFMP_OUT_OPT(zfstring *, errPos, zfnull)
        )

/**
 * @brief copy a file or directory from srcPath to dstPath
 *
 * if both src and dst exist, but one is file and another is dir,
 * return zffalse\n
 * merge directory if dst is an existing dir
 * (if isForce not set,
 * would return false if dst has a child file with the same path in src)\n
 * override file if dst is an existing file and isForce is zftrue
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFCore, zfbool, ZFFileCopy
        , ZFMP_IN(const zfchar *, srcPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        , ZFMP_IN_OPT(zfstring *, errPos, zfnull)
        )
/**
 * @brief move a file or directory from srcPath to dstPath
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFFileMove
        , ZFMP_IN(const zfchar *, srcPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
/**
 * @brief delete a file or directory from srcPath to dstPath
 *
 * fail if isRecursive is zffalse and dst is a dir
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFFileRemove
        , ZFMP_IN(const zfchar *, path)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        , ZFMP_IN_OPT(zfstring *, errPos, zfnull)
        )

/**
 * @brief find file or directory, similar to FindFirstFile under Windows
 *
 * path supports path only, without wildcard support,
 * e.g. "/path/" or "/path",
 * "." and ".." won't be included\n
 * typical usage:
 * @code
 *   ZFFileFindData fd;
 *   if(ZFFileFindFirst(fd, path)) {
 *     do {
 *       // do something
 *     } while(ZFFileFindNext(fd));
 *     ZFFileFindClose(fd);
 *   }
 * @endcode
 * @return false if error or nothing found
 * @warning strings in #ZFFileFindData would be deleted
 *   after each findXXX call,
 *   you must save it if need future use
 * @see ZFFileFindData, ZFFileFindFirst, ZFFileFindNext, ZFFileFindClose
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileFindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfchar *, path)
        )

/**
 * @see ZFFileFindData, ZFFileFindFirst, ZFFileFindClose
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileFindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )

/**
 * @see ZFFileFindData, ZFFileFindFirst, ZFFileFindNext
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFFileFindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )

/**
 * @brief open a file for read or write
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, void *, ZFFileOpen
        , ZFMP_IN(const zfchar *, filePath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
/**
 * @brief close and save the file if need, return false if save failed
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileClose
        , ZFMP_IN(void *, token)
        )

/**
 * @brief get current file's position or zfindexMax() if error
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFFileTell
        , ZFMP_IN(void *, token)
        )
/**
 * @brief similar to fseek, return false if seek out of range
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFFileSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        )

/**
 * @brief read file
 *
 * return size read, even if error occurred
 * (whether error occurred or eof,
 * could be accessed by #ZFFileIsError and #ZFFileIsEof)\n
 * typical usage:
 * @code
 *   zfindex sizeRead = 0;
 *   while((sizeRead = ZFFileRead(token, buf)) > 0
 *           && !ZFFileIsError(token)
 *           ) {
 *       // do something with buf
 *   }
 *   if(ZFFileIsError(token)) {
 *       // exit because of error occurred, read fail
 *   }
 *   else if(ZFFileIsEof(token)) {
 *       // exit because of eof, read success
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
 * return size written, even if error occurred
 * (whether error occurred or not,
 * could be accessed by #ZFFileIsError)\n
 * typical usage:
 * @code
 *   zfindex sizeWritten = ZFFileWrite(token, src, size);
 *   if(ZFFileIsError(token)) {
 *       // write fail
 *   }
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
 * @brief flush the file, useful only for files opened for write
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFFileFlush
        , ZFMP_IN(void *, token)
        )
/**
 * @brief see #ZFFileRead
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileIsEof
        , ZFMP_IN(void *, token)
        )
/**
 * @brief see #ZFFileRead
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFFileIsError
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

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_file_h_

