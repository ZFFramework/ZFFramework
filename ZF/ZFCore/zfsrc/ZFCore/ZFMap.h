/**
 * @file ZFMap.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFMap_h_
#define _ZFI_ZFMap_h_

#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFMapPrivate;
/**
 * @brief container of ZFObject, see #ZFKeyValueContainer
 */
zfclass ZFLIB_ZFCore ZFMap: zfextend ZFKeyValueContainer {
    ZFOBJECT_DECLARE(ZFMap, ZFKeyValueContainer)

public:
    /**
     * @brief return number of content
     */
    ZFMETHOD_DECLARE_0(zfindex, count)

    /**
     * @brief return true if empty or false if not empty
     */
    ZFMETHOD_DECLARE_0(zfbool, isEmpty)

    /**
     * @brief return true if contain key
     */
    ZFMETHOD_DECLARE_1(zfbool, isContain
            , ZFMP_IN(ZFObject *, key)
            )

    /**
     * @brief get element by key or null if no such key
     */
    ZFMETHOD_DECLARE_1(zfany, get
            , ZFMP_IN(ZFObject *, key)
            )

    /**
     * @brief get a copy of all key
     */
    ZFMETHOD_DECLARE_1(void, allKeyT
            , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
            )
    /** @brief see #allKey */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfauto>, allKey)

    /**
     * @brief get a copy of all value
     */
    ZFMETHOD_DECLARE_1(void, allValueT
            , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, ret)
            )
    /** @brief see #allValue */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfauto>, allValue)

public:
    /**
     * @brief add data from another container
     */
    ZFMETHOD_DECLARE_1(void, addFrom
            , ZFMP_IN(ZFKeyValueContainer *, another)
            )

    /**
     * @brief set a key value pair
     *
     * create new one if not exist,
     * replace old one if already exist\n
     * null key is not allowed,
     * use null value to remove the pair
     */
    ZFMETHOD_DECLARE_2(void, set
            , ZFMP_IN(ZFObject *, key)
            , ZFMP_IN(ZFObject *, value)
            )

    /**
     * @brief remove value associated with key
     */
    ZFMETHOD_DECLARE_1(void, remove
            , ZFMP_IN(ZFObject *, key)
            )
    /**
     * @brief remove and return removed value or null if not exist
     *
     * use this method for performance, instead of "get then remove",
     * since the latter one have two search step
     */
    ZFMETHOD_DECLARE_1(zfauto, removeAndGet
            , ZFMP_IN(ZFObject *, key)
            )
    /**
     * @brief remove all content
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_0(zfiter, iter)

    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_1(zfiter, iterFind
            , ZFMP_IN(ZFObject *, key)
            )

    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_1(zfany, iterKey
            , ZFMP_IN(const zfiter &, it)
            )

    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_1(zfany, iterValue
            , ZFMP_IN(const zfiter &, it)
            )

public:
    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_2(void, iterValue
            , ZFMP_IN_OUT(zfiter &, it)
            , ZFMP_IN(ZFObject *, value)
            )
    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_1(void, iterRemove
            , ZFMP_IN_OUT(zfiter &, it)
            )

    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_2(void, iterAdd
            , ZFMP_IN(ZFObject *, key)
            , ZFMP_IN(ZFObject *, value)
            )

protected:
    /**
     * @brief init from a existing container
     */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFKeyValueContainer *, another))
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFMapPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMap_h_

