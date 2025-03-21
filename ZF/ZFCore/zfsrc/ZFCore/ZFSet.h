/**
 * @file ZFSet.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFSet_h_
#define _ZFI_ZFSet_h_

#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFMap;
/**
 * @brief container of ZFObject, see #ZFContainer
 */
zfclass ZFLIB_ZFCore ZFSet : zfextend ZFContainer {
    ZFOBJECT_DECLARE(ZFSet, ZFContainer)

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
     * @brief return true if contains the object,
     *   compared by instance compare
     */
    ZFMETHOD_DECLARE_1(zfbool, isContain
            , ZFMP_IN(ZFObject *, obj)
            )

public:
    /**
     * @brief insert object to last, assert failure if obj = zfnull, see #add
     */
    ZFMETHOD_DECLARE_1(void, add
            , ZFMP_IN(ZFObject *, obj)
            )
    /**
     * @brief add objects from another container
     */
    ZFMETHOD_DECLARE_1(void, addFrom
            , ZFMP_IN(ZFContainer *, another)
            )

    /**
     * @brief remove object, do nothing if not exist
     */
    ZFMETHOD_DECLARE_1(void, remove
            , ZFMP_IN(ZFObject *, obj)
            )
    /**
     * @brief remove object, return removed object or null
     */
    ZFMETHOD_DECLARE_1(zfauto, removeAndGet
            , ZFMP_IN(ZFObject *, obj)
            )
    /**
     * @brief remove all object
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
    ZFMETHOD_DECLARE_1(void, iterAdd
            , ZFMP_IN(ZFObject *, value)
            )
    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_2(void, iterAdd
            , ZFMP_IN(ZFObject *, value)
            , ZFMP_IN_OUT(zfiter &, it)
            )

protected:
    /** @brief see #ZFObject::objectOnInit */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFContainer *, another))
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    ZFMap *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSet_h_

