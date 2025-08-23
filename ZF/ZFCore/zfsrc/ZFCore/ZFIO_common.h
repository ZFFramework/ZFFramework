/**
 * @file ZFIO_common.h
 * @brief abstract IO
 */

#ifndef _ZFI_ZFIO_common_h_
#define _ZFI_ZFIO_common_h_

#include "ZFIODef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForModule
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, modulePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForSetting
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, settingPath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForStorage
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, storagePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForStorageShared
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, storageSharedPath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForCache
 *
 * note, the cache path's content would be removed according to #ZFPathForCacheClear
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, cachePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is unmodified plain text (or buffer),
 * may contain non printable chars,
 * use #ZFPathType_bin type if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, text)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is binary data encoded with #ZFCoreDataEncode with default char map\n
 * use #ZFPathType_text for plain text
 */
ZFIO_DECLARE(ZFLIB_ZFCore, bin)

// ============================================================
/**
 * @brief util to open cache file under #ZFPathType_cachePath
 *
 * if autoRemove is true (by default), the cache file would be removed when closed
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfautoT<ZFIOToken>, ZFIOOpenCache
        , ZFMP_IN_OPT(const ZFPathInfo &, pathInfo, zfnull)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Read | v_ZFIOOpenOption::e_Write)
        , ZFMP_IN_OPT(zfbool, autoRemove, zftrue)
        )
/**
 * @brief generate a cache pathInfo, which ensured not same for each call
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, ZFPathInfo, ZFIO_cachePathGen)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIO_common_h_

