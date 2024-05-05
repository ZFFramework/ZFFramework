/**
 * @file ZFHashMap.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFHashMap_h_
#define _ZFI_ZFHashMap_h_

#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFHashMapPrivate;
/**
 * @brief container of ZFObject, see #ZFKeyValueContainer
 */
zfclass ZFLIB_ZFCore ZFHashMap: zfextend ZFKeyValueContainer {
    ZFOBJECT_DECLARE(ZFHashMap, ZFKeyValueContainer)

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
     * @brief return true if contain pKey
     */
    ZFMETHOD_DECLARE_1(zfbool, isContain
            , ZFMP_IN(ZFObject *, pKey)
            )

    /**
     * @brief get element by key or null if no such key
     */
    ZFMETHOD_DECLARE_1(zfany, get
            , ZFMP_IN(ZFObject *, pKey)
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
     * @brief util method to get and cast to desired type,
     *   no type safe check and type must be valid
     */
    template<typename T_ZFObject>
    T_ZFObject get(ZF_IN ZFObject *pKey) {
        return zfunsafe_zfcast(T_ZFObject, this->get(pKey));
    }

    /** @brief see #allKey */
    template<typename T_ZFObject>
    void allKeyT(ZF_IN_OUT ZFCoreArray<T_ZFObject> &ret) {
        ret.capacity(ret.count() + this->count());
        for(zfiterator it = this->iter(); this->iterValid(it); this->iterNext(it)) {
            ret.add(zfcast(T_ZFObject *, this->iterKey(it)));
        }
    }
    /** @brief see #allKey */
    template<typename T_ZFObject>
    ZFCoreArray<T_ZFObject> allKey(void) {
        ZFCoreArray<T_ZFObject> ret;
        this->allKeyT(ret);
        return ret;
    }

    /** @brief see #allValue */
    template<typename T_ZFObject>
    void allValueT(ZF_IN_OUT ZFCoreArray<T_ZFObject> &ret) {
        ret.capacity(ret.count() + this->count());
        for(zfiterator it = this->iter(); this->iterValid(it); this->iterNext(it)) {
            ret.add(zfcast(T_ZFObject *, this->iterValue(it)));
        }
    }
    /** @brief see #allValue */
    template<typename T_ZFObject>
    ZFCoreArray<T_ZFObject> allValue(void) {
        ZFCoreArray<T_ZFObject> ret;
        this->allValueT(ret);
        return ret;
    }

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
            , ZFMP_IN(ZFObject *, pKey)
            , ZFMP_IN(ZFObject *, pValue)
            )

    /**
     * @brief remove value associated with pKey
     */
    ZFMETHOD_DECLARE_1(void, remove
            , ZFMP_IN(ZFObject *, pKey)
            )
    /**
     * @brief remove and return removed value or null if not exist
     *
     * use this method for performance, instead of "get then remove",
     * since the latter one have two search step
     */
    ZFMETHOD_DECLARE_1(zfauto, removeAndGet
            , ZFMP_IN(ZFObject *, pKey)
            )
    /**
     * @brief remove all content
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

public:
    /**
     * @brief util method to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject removeAndGet(ZF_IN ZFObject *pKey) {
        return zfunsafe_zfcast(T_ZFObject, this->removeAndGet(pKey));
    }

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_0(zfiterator, iter)

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfiterator, iterFind
            , ZFMP_IN(ZFObject *, key)
            )

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfbool, iterValid
            , ZFMP_IN(const zfiterator &, it)
            )

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(void, iterNext
            , ZFMP_IN_OUT(zfiterator &, it)
            )

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfany, iterKey
            , ZFMP_IN(const zfiterator &, it)
            )

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfany, iterValue
            , ZFMP_IN(const zfiterator &, it)
            )

public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(void, iterValue
            , ZFMP_IN_OUT(zfiterator &, it)
            , ZFMP_IN(ZFObject *, value)
            )
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(void, iterRemove
            , ZFMP_IN_OUT(zfiterator &, it)
            )

    /** @brief see #zfiterator */
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
    _ZFP_ZFHashMapPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFHashMap_h_

