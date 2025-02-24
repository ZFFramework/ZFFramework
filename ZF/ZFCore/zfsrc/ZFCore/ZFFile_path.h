/**
 * @file ZFFile_path.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_path_h_
#define _ZFI_ZFFile_path_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cwd
/**
 * @brief get current working directory
 *
 * return null if not available
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, ZFPathForCwd)
/**
 * @brief change current working directory
 *
 * return false if failed
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFPathForCwd
        , ZFMP_IN(const zfstring &, path)
        )
/**
 * @brief util method to access #ZFPathForCwd,
 *   return #ZFPathForModule if not available
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, ZFPathForCwdOrModule)

// ============================================================
// module
/**
 * @brief get module's parent's path, e.g. "/path"
 *
 * path is ensured to use '/' as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, const zfstring &, ZFPathForModule)
/**
 * @brief get module's path, e.g. "/path/module.exe"
 *
 * path is ensured to use '/' as separator\n
 * it's not ensured module path is actually executable's path,
 * use other path types for data storage such as #ZFPathForSetting
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, const zfstring &, ZFPathForModuleFile)

// ============================================================
// setting
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalObserver only
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFPathForSettingOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper data path that app can save settings to
 *
 * path is ensured to use '/' as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, const zfstring &, ZFPathForSetting)
/**
 * @brief change the data path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFPathForSetting
        , ZFMP_IN(const zfstring &, path)
        )

// ============================================================
// storage
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalObserver only
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFPathForStorageOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper storage path that app can write files to
 *
 * path is ensured to use '/' as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, const zfstring &, ZFPathForStorage)
/**
 * @brief change the storage path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFPathForStorage
        , ZFMP_IN(const zfstring &, path)
        )

// ============================================================
// storage shared
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalObserver only
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFPathForStorageSharedOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper shared storage path that app can write files to,
 *   and other app can access
 *
 * note, this path may or may not be writable,
 * maybe external storage path or user's home path,
 * write with caution and only if necessary\n
 * path is ensured to use '/' as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, const zfstring &, ZFPathForStorageShared)
/**
 * @brief change the storage path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFPathForStorageShared
        , ZFMP_IN(const zfstring &, path)
        )

// ============================================================
// cache
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalObserver only
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFPathForCacheOnUpdate)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified to #ZFGlobalObserver only
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFPathForCacheBeforeClear)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified to #ZFGlobalObserver only
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFPathForCacheAfterClear)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper cache path that app can write files to
 *
 * cache is used for temp files only,
 * may or may not be deleted automatically\n
 * path is ensured to use '/' as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, const zfstring &, ZFPathForCache)
/**
 * @brief change the cache path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFPathForCache
        , ZFMP_IN(const zfstring &, path)
        )
/**
 * @brief clear all caches under #ZFPathForCache
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, void, ZFPathForCacheClear)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_path_h_

