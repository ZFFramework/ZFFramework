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
 * @brief iterable, see #zfiter
 */
zfinterface ZFLIB_ZFCore ZFIterable : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFIterable, ZFInterface)

public:
    /**
     * @brief return total count of this iterable
     */
    virtual zfindex count(void) zfpurevirtual;
    /**
     * @brief return a iter, see #zfiter
     */
    virtual zfiter iter(void) zfpurevirtual;

    /**
     * @brief find element
     */
    virtual zfiter iterFind(ZF_IN ZFObject *element) zfpurevirtual;

    /**
     * @brief get value by iter, see #zfiter
     *
     * iter must be valid
     */
    virtual zfany iterValue(ZF_IN const zfiter &it) zfpurevirtual;

public:
    /**
     * @brief set value at iter, see #zfiter
     */
    virtual void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN ZFObject *value
            ) zfpurevirtual;
    /**
     * @brief remove value at iter, see #zfiter
     */
    virtual void iterRemove(ZF_IN_OUT zfiter &it) zfpurevirtual;
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
     *   see #zfiter
     */
    virtual void iterAdd(
            ZF_IN ZFObject *value
            , ZF_IN_OUT zfiter &it
            ) zfpurevirtual;
};

// ============================================================
/**
 * @brief iterable that with key value pair, see #zfiter
 */
zfinterface ZFLIB_ZFCore ZFIterableKeyValue : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFIterableKeyValue, ZFInterface)

public:
    /**
     * @brief get key value pair with iter, see #zfiter
     */
    virtual zfany iterKey(ZF_IN const zfiter &it) zfpurevirtual;

public:
    /**
     * @brief add key value to tail, see #zfiter
     *
     * for most key value container,
     * this is same as set key with value
     */
    virtual void iterAdd(
            ZF_IN ZFObject *key
            , ZF_IN ZFObject *value
            ) zfpurevirtual;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIterable_h_

