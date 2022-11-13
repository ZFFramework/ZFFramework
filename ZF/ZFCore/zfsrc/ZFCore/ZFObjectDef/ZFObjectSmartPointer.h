/**
 * @file ZFObjectSmartPointer.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_ZFObjectSmartPointer_h_
#define _ZFI_ZFObjectSmartPointer_h_

#include "ZFObjectRetain.h"
#include "zfautoObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFCOREPOINTER_DECLARE(ZFCorePointerForPoolObject, {}, {
        zfCoreMutexLock();
        zfpoolDelete(p);
        zfCoreMutexUnlock();
    })

// ============================================================
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObject, {
        zfRetain(p);
    }, {
        zfRelease(p);
    })

// ============================================================
zffinal zfclassNotPOD ZFLIB_ZFCore _ZFP_zflineRelease
{
public:
    template<typename T_ZFObject>
    T_ZFObject set(T_ZFObject obj)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        return obj;
    }
public:
    _ZFP_zflineRelease(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zflineRelease(void)
    {
        zfRelease(obj);
    }
public:
    ZFObject *obj;
};
zffinal zfclassNotPOD ZFLIB_ZFCore _ZFP_zfunsafe_zflineRelease
{
public:
    template<typename T_ZFObject>
    T_ZFObject set(T_ZFObject obj)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        return obj;
    }
public:
    _ZFP_zfunsafe_zflineRelease(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zfunsafe_zflineRelease(void)
    {
        zfunsafe_zfRelease(obj);
    }
public:
    ZFObject *obj;
};
/**
 * @brief convenient method to release the object after code line end
 *
 * usage:
 * @code
 *   ZFObject *allocObject(void)
 *   {
 *       return zfAlloc(ZFObject);
 *   }
 *
 *   ZFArray *array = ...;
 *   array->add( // step 2, array auto retain it, objectRetainCount 2
 *       zflineRelease(
 *               allocObject() // step 1, objectRetainCount 1
 *           )
 *       ); // step 3, zflineRelease auto release it, objectRetainCount 1
 *
 *   // equivalence:
 *   // ZFObject *obj = allocObject();
 *   // array->add(obj);
 *   // zfRelease(obj);
 * @endcode
 * usually, this method is used to save you some code,
 * or designed for chained code
 */
#define zflineRelease(obj) (_ZFP_zflineRelease().set(obj))
/** @brief no lock version of #zflineRelease, use with caution */
#define zfunsafe_zflineRelease(obj) (_ZFP_zfunsafe_zflineRelease().set(obj))

// ============================================================
/**
 * @brief same as zflineRelease(zfAlloc(...)), see #zflineRelease
 */
#define zflineAlloc(T_ZFObject, ...) \
    zflineRelease(zfAlloc(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zflineAlloc, use with caution */
#define zfunsafe_zflineAlloc(T_ZFObject, ...) \
    zfunsafe_zflineRelease(zfunsafe_zfAlloc(T_ZFObject, ##__VA_ARGS__))

// ============================================================
template<typename T_ZFObject>
zffinal zfclassLikePOD _ZFP_zfblockedAllocContainer
{
public:
    _ZFP_zfblockedAllocContainer(ZF_IN T_ZFObject *obj)
    : obj(obj)
    {
    }
    ~_ZFP_zfblockedAllocContainer(void)
    {
        zfRelease(this->obj);
    }
public:
    T_ZFObject *obj;
};
template<typename T_ZFObject>
zffinal zfclassLikePOD _ZFP_zfunsafe_zfblockedAllocContainer
{
public:
    _ZFP_zfunsafe_zfblockedAllocContainer(ZF_IN T_ZFObject *obj)
    : obj(obj)
    {
    }
    ~_ZFP_zfunsafe_zfblockedAllocContainer(void)
    {
        zfRelease(this->obj);
    }
public:
    T_ZFObject *obj;
};
/**
 * @brief alloc a ZFObject looks like on a stack
 *
 * @code
 *   v_zfstring *saved = zfnull;
 *   {
 *       zfblockedAlloc(
 *           v_zfstring, // object's type
 *           s, // object's name
 *           "init value" // params passed to object's objectOnInit
 *       );
 *       s->objectInfo(); // access like normal ZFObject
 *       saved = zfRetain(s); // you may retain it and save it
 *   } // s would be released after this block
 *   saved->objectInfo(); // s is still alive since we retained it in the block
 *   zfRelease(saved); // release it
 * @endcode
 */
#define zfblockedAlloc(T_ZFObject, name, ...) \
    _ZFP_zfblockedAllocContainer<T_ZFObject> \
        ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name) (zfAlloc(T_ZFObject, ##__VA_ARGS__)); \
    T_ZFObject *name = (ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name).obj)
/** @brief no lock version of #zfblockedAlloc, use with caution */
#define zfunsafe_zfblockedAlloc(T_ZFObject, name, ...) \
    _ZFP_zfunsafe_zfblockedAllocContainer<T_ZFObject> \
        ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name) (zfunsafe_zfAlloc(T_ZFObject, ##__VA_ARGS__)); \
    T_ZFObject *name = (ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name).obj)

// ============================================================
zffinal zfclassLikePOD ZFLIB_ZFCore _ZFP_zfblockedReleaseContainer
{
public:
    template<typename T_ZFObject>
    _ZFP_zfblockedReleaseContainer(ZF_IN T_ZFObject obj)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    {
    }
    ~_ZFP_zfblockedReleaseContainer(void)
    {
        zfRelease(this->obj);
    }
private:
    ZFObject *obj;
};
zffinal zfclassLikePOD ZFLIB_ZFCore _ZFP_zfunsafe_zfblockedReleaseContainer
{
public:
    template<typename T_ZFObject>
    _ZFP_zfunsafe_zfblockedReleaseContainer(ZF_IN T_ZFObject obj)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    {
    }
    ~_ZFP_zfunsafe_zfblockedReleaseContainer(void)
    {
        zfRelease(this->obj);
    }
private:
    ZFObject *obj;
};
/**
 * @brief release the object after nearest block
 *
 * @code
 *   { // code block
 *       ZFObject *obj = ...;
 *       zfblockedRelease(obj); // mark release, must be single line
 *       return ; // safe to return, auto released after exiting code block
 *   } // obj would be released after here
 * @endcode
 */
#define zfblockedRelease(obj) \
    _ZFP_zfblockedReleaseContainer ZFUniqueName(zfblockedRelease) (obj)
/** @brief no lock version of #zfblockedRelease, use with caution */
#define zfunsafe_zfblockedRelease(obj) \
    _ZFP_zfunsafe_zfblockedReleaseContainer ZFUniqueName(zfblockedRelease) (obj)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectSmartPointer_h_

