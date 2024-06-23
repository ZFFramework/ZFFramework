/**
 * @file ZFUIImageCache.h
 * @brief load image with cache logic
 */

#ifndef _ZFI_ZFUIImageCache_h_
#define _ZFI_ZFUIImageCache_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load image with cache logic
 */
zfclass ZFLIB_ZFUIWidget ZFUIImageCache : zfextend ZFCache {
    ZFOBJECT_DECLARE(ZFUIImageCache, ZFCache)
    ZFOBJECT_SINGLETON_DECLARE(ZFUIImageCache, instance)

    /**
     * @brief a list of path type which should not stored as local cache
     *
     * by default, image file would be stored as local cache file to speed up next time load,
     * which is not necessary for files already stored in local,
     * you may add them (as #v_zfstring) to this config to prevent extra local file cache logic,
     * see #ZFPATHTYPE_DECLARE
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFSet>, localCacheExclude, zfobj<ZFSet>()
            ->c_add(zfobj<v_zfstring>(ZFPathType_file()))
            ->c_add(zfobj<v_zfstring>(ZFPathType_res()))
            ->c_add(zfobj<v_zfstring>(ZFPathType_modulePath()))
            ->c_add(zfobj<v_zfstring>(ZFPathType_storagePath()))
            ->c_add(zfobj<v_zfstring>(ZFPathType_storageSharedPath()))
            ->c_add(zfobj<v_zfstring>(ZFPathType_cachePath()))
            ->c_add(zfobj<v_zfstring>(ZFPathType_text()))
            ->c_add(zfobj<v_zfstring>(ZFPathType_base64()))
            )

    /**
     * @brief max local cache count to keep
     */
    ZFPROPERTY_ASSIGN(zfindex, localCacheMaxSize, 50)
    /**
     * @brief cache time in miliseconds
     */
    ZFPROPERTY_ASSIGN(zftimet, localCacheTime, 1 * 24 * 60 * 60 * 1000)
    /**
     * @brief override default cache path
     *
     * the path info must support:
     * -  #ZFInputForPathInfo / #ZFOutputForPathInfo
     * -  #ZFPathInfoFindFirst series
     */
    ZFPROPERTY_ASSIGN(ZFPathInfo, localCachePathInfo)
    /**
     * @brief return a fixed path info of #localCachePathInfo
     */
    ZFMETHOD_DECLARE_0(ZFPathInfo, localCachePathInfoFixed)

    ZFPROPERTY_ON_INIT_DECLARE_NO_AUTO_INIT(zfindex, cacheMaxSize)
};

/**
 * @brief load image with cache logic
 *
 * callback's param0 would be the result image if success,
 * param1 would be a #ZFResultType indicates load result
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIWidget, zfauto, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        )
/**
 * @brief cancel load task of #ZFUIImageLoad
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIWidget, void, ZFUIImageLoadCancel
        , ZFMP_IN(ZFObject *, taskId)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageCache_h_

