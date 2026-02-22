/**
 * @file ZFArray.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFArray_h_
#define _ZFI_ZFArray_h_

#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFArrayPrivate;
/**
 * @brief container of ZFObject, see #ZFContainer
 *
 * for the comparer of #isContain, #removeElement, etc:
 * -  sender is the owner container
 * -  param0 and param1 is the item being compared
 * -  the comparer must set result to #v_ZFCompareResult
 */
zfclass ZFLIB_ZFCore ZFArray : zfextend ZFContainer {
    ZFOBJECT_DECLARE(ZFArray, ZFContainer)

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
     *   compared by instance compare by default
     */
    virtual zfbool isContain(
            ZF_IN ZFObject *obj
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief return true if contains the object,
     *   compared by instance compare by default
     */
    ZFMETHOD_DECLARE_2(zfbool, isContain
            , ZFMP_IN(ZFObject *, obj)
            , ZFMP_IN(const ZFListener &, comparer)
            )

    /**
     * @brief return object at index, assert failure if out of range
     */
    ZFMETHOD_DECLARE_1(zfany, get
            , ZFMP_IN(zfindex, index)
            )

    /**
     * @brief return first object, assert failure if out of range
     */
    ZFMETHOD_DECLARE_0(zfany, getFirst)

    /**
     * @brief return last object, assert failure if out of range
     */
    ZFMETHOD_DECLARE_0(zfany, getLast)

    /**
     * @brief find element, compared by instance compare by default
     */
    virtual zfindex find(
            ZF_IN ZFObject *obj
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief find element, compared by instance compare by default
     */
    ZFMETHOD_DECLARE_2(zfindex, find
            , ZFMP_IN(ZFObject *, obj)
            , ZFMP_IN(const ZFListener &, comparer)
            )

    /**
     * @brief find element, compared by instance compare by default
     */
    virtual zfindex findReversely(
            ZF_IN ZFObject *obj
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief find element, compared by instance compare by default
     */
    ZFMETHOD_DECLARE_2(zfindex, findReversely
            , ZFMP_IN(ZFObject *, obj)
            , ZFMP_IN(const ZFListener &, comparer)
            )

public:
    /**
     * @brief insert object at index, assert failure if obj = zfnull
     *   or index out of range [0, count]
     * @note null object is not allowed, use #ZFNull if necessary
     */
    ZFMETHOD_DECLARE_2(void, add
            , ZFMP_IN(ZFObject *, obj)
            , ZFMP_IN(zfindex, indexAddTo)
            )
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
     * @brief replace object at index, assert fail if index out of range
     */
    ZFMETHOD_DECLARE_2(void, set
            , ZFMP_IN(zfindex, index)
            , ZFMP_IN(ZFObject *, obj)
            )

    /**
     * @brief remove first matched object, return whether the element removed
     */
    virtual zfbool removeElement(
            ZF_IN ZFObject *obj
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief remove first matched object, return whether the element removed
     */
    ZFMETHOD_DECLARE_2(zfbool, removeElement
            , ZFMP_IN(ZFObject *, obj)
            , ZFMP_IN(const ZFListener &, comparer)
            )
    /**
     * @brief remove last matched object, return whether the element removed
     */
    virtual zfbool removeElementReversely(
            ZF_IN ZFObject *obj
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief remove last matched object, return whether the element removed
     */
    ZFMETHOD_DECLARE_2(zfbool, removeElementReversely
            , ZFMP_IN(ZFObject *, obj)
            , ZFMP_IN(const ZFListener &, comparer)
            )
    /**
     * @brief remove all matched object, return number of element removed
     */
    virtual zfindex removeElementAll(
            ZF_IN ZFObject *obj
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief remove all matched object, return number of element removed
     */
    ZFMETHOD_DECLARE_2(zfindex, removeElementAll
            , ZFMP_IN(ZFObject *, obj)
            , ZFMP_IN(const ZFListener &, comparer)
            )

    /**
     * @brief remove object at index, assert failure if out of range
     */
    ZFMETHOD_DECLARE_2(void, remove
            , ZFMP_IN(zfindex, index)
            , ZFMP_IN_OPT(zfindex, count, 1)
            )
    /**
     * @brief remove object at index and return removed item, assert failure if out of range
     */
    ZFMETHOD_DECLARE_1(zfauto, removeAndGet
            , ZFMP_IN(zfindex, index)
            )
    /**
     * @brief remove first object, assert fail if out of range
     */
    ZFMETHOD_DECLARE_0(void, removeFirst)
    /**
     * @brief remove first object and return removed item, assert fail if out of range
     */
    ZFMETHOD_DECLARE_0(zfauto, removeFirstAndGet)
    /**
     * @brief remove last object, assert fail if out of range
     */
    ZFMETHOD_DECLARE_0(void, removeLast)
    /**
     * @brief remove last object and return removed item, assert fail if out of range
     */
    ZFMETHOD_DECLARE_0(zfauto, removeLastAndGet)
    /**
     * @brief remove all object
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

    /**
     * @brief move element
     */
    ZFMETHOD_DECLARE_2(void, move
            , ZFMP_IN(zfindex, fromIndex)
            , ZFMP_IN(zfindex, toIndexOrIndexMax)
            )

    /**
     * @brief sort content in range [start, start + count), all of content must be comparable
     */
    virtual void sort(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief sort content in range [start, start + count), all of content must be comparable
     */
    ZFMETHOD_DECLARE_3(void, sort
            , ZFMP_IN(zfindex, start)
            , ZFMP_IN(zfindex, count)
            , ZFMP_IN(const ZFListener &, comparer)
            )
    /**
     * @brief sort content in range [start, start + count), all of content must be comparable
     */
    virtual void sortReversely(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            , ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerDefault
            );
    /**
     * @brief sort content in range [start, start + count), all of content must be comparable
     */
    ZFMETHOD_DECLARE_3(void, sortReversely
            , ZFMP_IN(zfindex, start)
            , ZFMP_IN(zfindex, count)
            , ZFMP_IN(const ZFListener &, comparer)
            )

    // ============================================================
    // extra iterable
public:
    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_1(zfiter, iterForIndex
            , ZFMP_IN(zfindex, index)
            )

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_0(zfiter, iter)

    /** @brief see #zfiter */
    ZFMETHOD_DECLARE_1(zfiter, iterFind
            , ZFMP_IN(ZFObject *, element)
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
    _ZFP_ZFArrayPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFArray_h_

