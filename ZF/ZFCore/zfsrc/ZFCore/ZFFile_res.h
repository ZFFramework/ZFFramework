/**
 * @file ZFFile_res.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_res_h_
#define _ZFI_ZFFile_res_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief add an additional path to search as resource files
 *
 * by default, resource files are placed in a fixed builtin location,
 * you may add additional path to search as resource files\n
 * to make things clear, while accessing resource files using a res path,
 * we would:
 * -# search all additional resource path to find whether the file exists
 * -# if exist, use the additional resource file
 * -# if not exist, use builtin resource file
 * -# if builtin resource file not exist, fail
 *
 *
 * this method would affect all methods that process with resource files,
 * such as #ZFResCopy, #ZFResOpen, #ZFResRead\n
 * typically, additional resource path should be modified during startup only
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFResExtPathAdd
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFResExtPathAdd */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFResExtPathRemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFResExtPathAdd */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, ZFCoreArray<ZFPathInfo>, ZFResExtPathList)
/**
 * @brief see #ZFResExtPathAdd
 *
 * return the registered additional resource path if file exist,
 * or null if not exist\n
 * if return value is not null,
 * the final file path should be "return value + '/' + resPath"
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFResExtPathCheck
        , ZFMP_OUT(ZFPathInfo &, resExtPath)
        , ZFMP_IN(const zfchar *, resPath)
        )

// ============================================================
/**
 * @brief return true if res file specified by path is exist
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsExist
        , ZFMP_IN(const zfchar *, resPath)
        )
/**
 * @brief return true if res file specified by path is a directory
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsDir
        , ZFMP_IN(const zfchar *, resPath)
        )

/**
 * @brief res version of #ZFFileCopy
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFResCopy
        , ZFMP_IN(const zfchar *, resPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )

/**
 * @see see #ZFResOpen, #ZFFileFindFirst
 *
 * use empty string to find resource's root directory
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFResFindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfchar *, resPath)
        )
/**
 * @see see #ZFResOpen, #ZFFileFindNext
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResFindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )
/**
 * @see see #ZFResOpen, #ZFFileFindClose
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFResFindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )

/**
 * @brief open a resource file for read only, see #ZFFileOpen
 *
 * since the resource files may or may not be opened by normal method
 * (i.e. #ZFFileOpen), such as those in Android project which are located in
 * apk/assets, so use #ZFResOpen and #ZFResFindFirst to access it for cross-platform\n
 * also, since the resource files may or may not be compressed into app,
 * accessing them may or may not take much time
 * \n
 * a resource file is a file in "zfres" directory of native project,
 * such as "project_path/zfres/1.png" and "project_path/zfres/subdir/1.png",
 * which should be "1.png" and "subdir/1.png" when ZFResOpen\n
 * file name could be the same if under different directories,
 * and is recommended to keep all lower case for cross-platform\n
 * \n
 * note that the token is defined same type as the one used by #ZFFileOpen,
 * it's your responsibility to make sure not to misuse it
 * @note path must be well formed, use #ZFPathFormat if necessary
 *
 * ADVANCED:\n
 * resource can be "extended" by #ZFResExtPathAdd,
 * use #ZFResRaw series to bypass the logic
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void *, ZFResOpen
        , ZFMP_IN(const zfchar *, resPath)
        )
/**
 * @brief see #ZFResOpen #ZFFileClose
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResClose
        , ZFMP_IN(void *, token)
        )

/**
 * @brief see #ZFResOpen #ZFFileTell
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFResTell
        , ZFMP_IN(void *, token)
        )
/**
 * @brief see #ZFResOpen #ZFFileSeek
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFResSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        )

/**
 * @brief see #ZFResOpen #ZFFileRead
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfindex, ZFResRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )

/**
 * @brief see #ZFResOpen #ZFFileIsEof
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsEof
        , ZFMP_IN(void *, token)
        )
/**
 * @brief see #ZFResOpen #ZFFileIsEof
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsError
        , ZFMP_IN(void *, token)
        )

/**
 * @brief see #ZFResOpen, #ZFFileSize
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_res_h_

