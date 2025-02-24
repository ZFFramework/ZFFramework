/**
 * @file ZFPathType_common.h
 * @brief common #ZFPathInfo
 */

#ifndef _ZFI_ZFPathType_common_h_
#define _ZFI_ZFPathType_common_h_

#include "ZFPathType_file.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForModule
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, modulePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForSetting
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, settingPath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForStorage
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, storagePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForStorageShared
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, storageSharedPath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFPathForCache
 *
 * note, the cache path's content would be removed according to #ZFPathForCacheClear
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, cachePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is plain text
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, text)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_common_h_

