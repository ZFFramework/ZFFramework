/**
 * @file ZFIOUtil.h
 * @brief debug server
 */

#ifndef _ZFI_ZFIOUtil_h_
#define _ZFI_ZFIOUtil_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief remove outdated files
 *
 * filter would be called before removing each file,
 * param0 is #v_ZFPathInfo that describe the child file,
 * and param1 is a #v_zfstring stores the relative path to source pathInfo\n
 * impl can:
 * -  set #ZFArgs::eventFiltered if process done
 * -  set #ZFArgs::result to any valid object, to prevent the file from being removed
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIORemoveOutdate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        , ZFMP_IN_OPT(zftimet, outdate, zftimetOneWeek())
        )

/**
 * @brief remove empty dir, excluding the pathInfo itself
 *
 * filter would be called before removing each dir,
 * param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is a #v_zfstring stores the relative path to source pathInfo\n
 * impl can:
 * -  set #ZFArgs::param0 or #ZFArgs::param1 to null, to prevent checking children of the file or dir (if isRecursive)
 * -  set #ZFArgs::eventFiltered if process done
 * -  set #ZFArgs::result to any valid object, to prevent the dir from being removed
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIORemoveEmptyDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        )

/**
 * @brief async version for #ZFIOForEach
 *   to prevent blocking UI update when performing heavy task
 *
 * fileCallback is same as #ZFIOForEach,
 * but would be called with some delay between each call for each file/dir\n
 * \n
 * fileCallback's sender would be the #v_ZFIOFindData
 * (may be null if pathInfo already points to a file),
 * param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is a #v_zfstring stores the relative path to source pathInfo
 * (may be empty string if pathInfo already points to a file)\n
 * impl can:
 * -  set #ZFArgs::param0 to null, to prevent checking children of the file or dir (if isRecursive)
 * -  set #ZFArgs::param0 to a #ZFTask, to perform custom task
 * -  set #ZFArgs::eventFiltered if process done
 * -  set #ZFArgs::result, which would be passed as param0 to finishCallback,
 *   note: the result would be kept acrossing different calls
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIOForEachAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief see #ZFIOForEachAsync */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIOForEachFileAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief see #ZFIOForEachAsync */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIOForEachDirAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOUtil_h_

