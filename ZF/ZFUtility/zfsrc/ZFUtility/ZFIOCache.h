/**
 * @file ZFIOCache.h
 * @brief utility to load with cache logic
 */

#ifndef _ZFI_ZFIOCache_h_
#define _ZFI_ZFIOCache_h_

#include "ZFCache.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief cache config and holder for #ZFIOCacheLoad
 */
zfclass ZFLIB_ZFUtility ZFIOCache : zfextend ZFCache {
    ZFOBJECT_DECLARE(ZFIOCache, ZFCache)
    ZFOBJECT_SINGLETON_DECLARE(ZFIOCache, instance)

    /**
     * @brief a list of path type which should not stored as local cache
     *
     * by default, temp file would be stored as local cache file to speed up next time load,
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
            )

    /**
     * @brief max local cache count to keep
     */
    ZFPROPERTY_ASSIGN(zfindex, localCacheMaxSize, 100)
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
    /**
     * @brief util method to get a cache file under #localCachePathInfo
     */
    ZFMETHOD_DECLARE_1(ZFPathInfo, localCachePathInfoForChild
            , ZFMP_IN(const zfchar *, childPath)
            )

    ZFPROPERTY_ON_INIT_DECLARE_NO_AUTO_INIT(zfindex, cacheMaxSize)
};

/**
 * @brief load contents with cache logic
 *
 * useful to load contents from slow src input (http for example),
 * load src to local temp file and load it in background thread,
 * use #ZFIOCache to config the cache logic\n
 * \n
 * callback's param0 would be the result object if success,
 * param1 would be a #ZFResultType indicates load result\n
 * \n
 * the loadImpl's param0 would be a ZFInput points to local cache file or original src,
 * param1 would be the original src input,
 * and should set #ZFArgs::result to non null object as result,
 * which would passed to callback
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUtility, zfautoT<ZFTaskId>, ZFIOCacheLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, loadImpl, zfnull)
        )

// ============================================================
/**
 * @brief io load task
 */
zfclass ZFLIB_ZFUtility ZFIOCacheLoadTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFIOCacheLoadTask, ZFTask)

public:
    /** @brief the src input */
    ZFPROPERTY_ASSIGN(ZFInput, input)
    /** @brief the load impl */
    ZFPROPERTY_ASSIGN(ZFListener, loadImpl)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const ZFInput &, input)
            , ZFMP_IN_OPT(const ZFListener &, loadImpl, zfnull)
            )

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCache_h_

