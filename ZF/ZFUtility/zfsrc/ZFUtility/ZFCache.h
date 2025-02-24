/**
 * @file ZFCache.h
 * @brief util object to hold #ZFObject as cache
 */

#ifndef _ZFI_ZFCache_h_
#define _ZFI_ZFCache_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFCachePrivate;
/**
 * @brief util object to hold #ZFObject as cache
 */
zfclass ZFLIB_ZFUtility ZFCache : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFCache, ZFObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when cache added,
     * param0 is the added cache
     */
    ZFEVENT(CacheOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when cache removed,
     * param0 is the removed cache
     */
    ZFEVENT(CacheOnRemove)

public:
    /**
     * @brief max cache size, 10 by default
     */
    ZFPROPERTY_ASSIGN(zfindex, cacheMaxSize, 10)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfindex, cacheMaxSize)

    /**
     * @brief whether invoke #cacheTrim when receive #ZFGlobalEvent::E_AppOnMemoryLow, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, cacheTrimWhenReceiveMemoryWarning, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, cacheTrimWhenReceiveMemoryWarning)

    /**
     * @brief leave how many cache alive while #cacheTrimWhenReceiveMemoryWarning, 0.2 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, cacheTrimThreshold, 0.2f)

public:
    /**
     * @brief add cache
     *
     * when cache exceeds #cacheMaxSize,
     * the oldest cache would be removed
     */
    ZFMETHOD_DECLARE_2(void, cacheAdd
            , ZFMP_IN(const zfstring &, cacheKey)
            , ZFMP_IN(ZFObject *, cacheValue)
            )
    /**
     * @brief access and remove cache, or return null if not exist, see #cacheCheck
     */
    ZFMETHOD_DECLARE_1(zfauto, cacheGet
            , ZFMP_IN(const zfstring &, cacheKey)
            )
    /**
     * @brief check cache, or return null if not exist, see #cacheGet
     */
    ZFMETHOD_DECLARE_1(zfauto, cacheCheck
            , ZFMP_IN(const zfstring &, cacheKey)
            )

    /**
     * @brief remove all cache with cacheKey
     */
    ZFMETHOD_DECLARE_1(void, cacheRemove
            , ZFMP_IN(const zfstring &, cacheKey)
            )
    /**
     * @brief remove all cache
     */
    ZFMETHOD_DECLARE_0(void, cacheRemoveAll)

    /**
     * @brief util method to trim the cache
     *   so that the cached data won't exceeds specified size
     */
    ZFMETHOD_DECLARE_1(void, cacheTrim
            , ZFMP_IN(zfindex , size)
            )

public:
    /**
     * @brief access all cache data, used for debug only
     */
    ZFMETHOD_DECLARE_2(void, cacheGetAllT
            , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, cacheKeyList)
            , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, cacheValueList)
            )

protected:
    /** @brief see #E_CacheOnAdd */
    virtual void cacheOnAdd(ZF_IN ZFObject *cache) {
        this->observerNotify(zfself::E_CacheOnAdd(), cache);
    }
    /** @brief see #E_CacheOnRemove */
    virtual void cacheOnRemove(ZF_IN ZFObject *cache) {
        this->observerNotify(zfself::E_CacheOnRemove(), cache);
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFCachePrivate *d;
    friend zfclassFwd _ZFP_ZFCachePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCache_h_

