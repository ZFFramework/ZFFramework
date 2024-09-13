/**
 * @file zfimport.h
 * @brief util to import resource or class
 */

#ifndef _ZFI_zfimport_h_
#define _ZFI_zfimport_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified during #zfimport\n
 * param0 is a #v_ZFInput holds the item being load
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFImportBegin)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified during #zfimport\n
 * param0 is a #v_ZFInput holds the item being load,
 * param1 is a #ZFObject holds the load result
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFImportEnd)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief util to import resource or class
 *
 * this method is useful for script to achive "import" or "require"
 * of other script language such as Javascript\n
 * \n
 * what the path and pathInfo means:
 * -  when pathInfo is not specified, the path is used as #ZFInputForRes
 * -  when pathInfo is specified, the path is the relative path to pathInfo,
 *   and would be used as #ZFInputForLocal
 *
 * \n
 * how the import works:
 * -  when pathInfo/path points to a file
 *   -  #ZFObjectIOLoad would be called
 *     to load the content, and return the result
 *   -  when load failed, nothing happen, and null would be returned
 * -  when pathInfo/path points to a directory
 *   -  #ZFObjectIOLoad would be called
 *     for each file under the directory,
 *     and a #ZFMap would be returned,
 *     contains all the result,
 *     key is a #v_zfstring holds the relative path name to pathInfo/path,
 *     and value is the result returned by #ZFObjectIOLoad
 *   -  when any content load failed, nothing happen,
 *     and a #ZFMap would be returned contains all successful loaded item
 *
 * \n
 * about cache logic:
 * -  all item would be cached by key #ZFCallback::callbackId
 *   (won't be cached if no #ZFCallback::callbackId available)
 * -  cache is stored as level #ZFLevelZFFrameworkNormal
 * -  (not recommended) you may use #zfimportCacheRemove/zfimportCacheRemoveAll
 *   to remove the cache
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfauto, zfimport
        , ZFMP_IN(const zfchar *, path)
        , ZFMP_IN_OPT(const ZFPathInfo &, pathInfo, zfnull)
        )

/** @brief remove the cache created by #zfimport */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfauto, zfimportCacheRemove
        , ZFMP_IN(const zfstring &, callbackId)
        )

/** @brief remove all cache created by #zfimport */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, void, zfimportCacheRemoveAll)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfimport_h_

