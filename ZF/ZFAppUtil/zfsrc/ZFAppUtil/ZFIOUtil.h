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
 * param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is a #v_zfstring stores the relative path to source pathInfo\n
 * impl can:
 * -  set #ZFArgs::param0 or #ZFArgs::param1 to null, to prevent checking children of the file or dir (if isRecursive)
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

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOUtil_h_

