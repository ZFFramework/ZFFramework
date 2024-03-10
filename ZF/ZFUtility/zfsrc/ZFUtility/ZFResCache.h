/**
 * @file ZFResCache.h
 * @brief utility to load serializable from resource with cache logic
 */

#ifndef _ZFI_ZFResCache_h_
#define _ZFI_ZFResCache_h_

#include "ZFCache.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief cache for serializable from resources
 *
 * note: res cache would be clean up and disabled during #ZFFrameworkCleanup
 * of level #ZFLevelZFFrameworkPostNormal to prevent dependency broken,
 * by setting #ZFCache::cacheMaxSize to zero
 */
zfclass ZFLIB_ZFUtility ZFResCache : zfextend ZFCache {
    ZFOBJECT_DECLARE(ZFResCache, ZFCache)
    ZFOBJECT_SINGLETON_DECLARE(ZFResCache, instance)

    ZFPROPERTY_ON_INIT_DECLARE(zfindex, cacheMaxSize)
};

/**
 * @brief load resource by #ZFObjectIOLoad
 *
 * automatically cache the resource in #ZFResCache,
 * you should not modified the returned object if cache enabled\n
 * to remove cache, use #ZFCache::cacheRemove\n
 * \n
 * when pathInfo is specified, the resFilePath can be relative path to pathInfo,
 * at this case, the pathInfo must be well formed
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUtility, zfauto, zfRes
        , ZFMP_IN(const zfchar *, resFilePath)
        , ZFMP_IN_OPT(const ZFPathInfo *, pathInfo, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFResCache_h_

