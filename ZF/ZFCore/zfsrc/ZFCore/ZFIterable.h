/**
 * @file ZFIterable.h
 * @brief interface shows a ZFObject is iterable
 */

#ifndef _ZFI_ZFIterable_h_
#define _ZFI_ZFIterable_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief iterable, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterable, ZFInterface)

public:
    /**
     * @brief return total count of this iterable
     */
    virtual zfindex count(void) zfpurevirtual;
    /**
     * @brief return a iter, see #zfiterator
     */
    virtual zfiterator iter(void) zfpurevirtual;

    /**
     * @brief find element
     */
    virtual zfiterator iterFind(ZF_IN ZFObject *element) zfpurevirtual;

    /**
     * @brief true if the iter is valid, see #zfiterator
     *
     * you may access value by #iterValue,
     * or move to next by #iterNext
     */
    virtual zfbool iterValid(ZF_IN const zfiterator &it) zfpurevirtual;

    /**
     * @brief move to next, see #zfiterator
     *
     * iter must be valid
     */
    virtual void iterNext(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief get value by iter, see #zfiterator
     *
     * iter must be valid
     */
    virtual ZFObject *iterValue(ZF_IN const zfiterator &it) zfpurevirtual;

public:
    /**
     * @brief set value at iter, see #zfiterator
     */
    virtual void iterValue(ZF_IN_OUT zfiterator &it,
                           ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief remove value at iter, see #zfiterator
     */
    virtual void iterRemove(ZF_IN_OUT zfiterator &it) zfpurevirtual;
    /**
     * @brief remove all contents of this iterable
     */
    virtual void removeAll(void) zfpurevirtual;

    /**
     * @brief add value to tail
     */
    virtual void iterAdd(ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief add value before iter,
     *   see #zfiterator
     */
    virtual void iterAdd(ZF_IN ZFObject *value,
                         ZF_IN_OUT zfiterator &it) zfpurevirtual;
};

// ============================================================
/**
 * @brief iterable that with key value pair, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterableKeyValue : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterableKeyValue, ZFIterable)

public:
    /**
     * @brief get key value pair with iter, see #zfiterator
     */
    virtual ZFObject *iterKey(ZF_IN const zfiterator &it) zfpurevirtual;

public:
    /**
     * @brief add key value to tail, see #zfiterator
     *
     * for most key value container,
     * this is same as set key with value
     */
    virtual void iterAdd(ZF_IN ZFObject *key,
                         ZF_IN ZFObject *value) zfpurevirtual;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIterable_h_

